/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2017  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "otpch.h"

#include "configmanager.h"
#include "game.h"
#include "waitlist.h"

extern ConfigManager g_config;
extern Game g_game;


namespace {

struct Wait
{
	constexpr Wait(std::size_t timeout, uint32_t playerGUID) :
			timeout(timeout), playerGUID(playerGUID) {}

	std::size_t timeout;
	uint32_t playerGUID;
};

using WaitList = std::list<Wait>;

std::size_t getTimeout(std::size_t slot)
{
	//timeout is set to 15 seconds longer than expected retry attempt
	return WaitingList::getTime(slot) + 15;
}

} // namespace

struct WaitListInfo
{
	WaitList waitList;
	WaitList::iterator priorityEnd = waitList.end();

	std::pair<WaitList::iterator, WaitList::size_type> findClient(const Player *player) {
		std::size_t slot = 1;
		for (auto it = waitList.begin(), end = waitList.end(); it != end; ++it, ++slot) {
			if (it->playerGUID == player->getGUID()) {
				return {it, slot};
			}
		}
		return {waitList.end(), slot};
	}
};

WaitingList& WaitingList::getInstance()
{
	static WaitingList waitingList;
	return waitingList;
}

std::size_t WaitingList::getTime(std::size_t slot)
{
	if (slot < 5) {
		return 5;
	} else if (slot < 10) {
		return 10;
	} else if (slot < 20) {
		return 20;
	} else if (slot < 50) {
		return 60;
	} else {
		return 120;
	}
}

bool WaitingList::clientLogin(const Player* player)
{
	if (player->hasFlag(PlayerFlag_CanAlwaysLogin) || player->getAccountType() >= ACCOUNT_TYPE_GAMEMASTER) {
		return true;
	}

	uint32_t maxPlayers = static_cast<uint32_t>(g_config.getNumber(ConfigManager::MAX_PLAYERS));
	if (maxPlayers == 0 || (info->waitList.empty() && g_game.getPlayersOnline() < maxPlayers)) {
		return true;
	}

	int64_t time = OTSYS_TIME();

	WaitList::iterator it = info->waitList.begin(), end = info->waitList.end();
	while (it != end) {
		if ((it->timeout - time) <= 0) {
			it = info->waitList.erase(it);
		} else {
			++it;
		}
	}

	WaitList::size_type slot;
	std::tie(it, slot) = info->findClient(player);
	if (it != info->waitList.end()) {
		if ((g_game.getPlayersOnline() + slot) <= maxPlayers) {
			//should be able to login now
			info->waitList.erase(it);
			return true;
		}

		//let them wait a bit longer
		it->timeout = time + (getTimeout(slot) * 1000);
		return false;
	}

	if (player->isPremium()) {
		slot = static_cast<std::size_t>(std::distance(info->waitList.begin(), info->priorityEnd));
		info->priorityEnd = info->waitList.emplace(info->priorityEnd, time + (getTimeout(slot + 1) * 1000), player->getGUID());
	} else {
		info->waitList.emplace_back(time + (getTimeout(info->waitList.size() + 1) * 1000), player->getGUID());
	}
	return false;
}

std::size_t WaitingList::getClientSlot(const Player* player)
{
	WaitList::iterator it;
	WaitList::size_type slot;
	std::tie(it, slot) = info->findClient(player);
	if (it == info->waitList.end()) {
		return 0;
	}
	return slot;
}

WaitingList::WaitingList() : info(new WaitListInfo) {}
