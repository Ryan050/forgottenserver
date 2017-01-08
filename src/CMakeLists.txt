cmake_minimum_required(VERSION 2.8)

# Find packages.
find_package(PugiXML REQUIRED)
find_package(LuaJIT)
find_package(MySQL)
find_package(Threads)

option(USE_LUAJIT "Use LuaJIT" ${LUAJIT_FOUND})

if(USE_LUAJIT)
	find_package(LuaJIT REQUIRED)
	if(APPLE)
		set(CMAKE_EXE_LINKER_FLAGS "-pagezero_size 10000 -image_base 100000000")
	endif()
else()
	find_package(Lua)
endif()

find_package(Boost 1.53.0 COMPONENTS iostreams system REQUIRED)

set(tfs_SRC
	${CMAKE_CURRENT_LIST_DIR}/otpch.cpp
	${CMAKE_CURRENT_LIST_DIR}/actions.cpp
	${CMAKE_CURRENT_LIST_DIR}/ban.cpp
	${CMAKE_CURRENT_LIST_DIR}/baseevents.cpp
	${CMAKE_CURRENT_LIST_DIR}/bed.cpp
	${CMAKE_CURRENT_LIST_DIR}/chat.cpp
	${CMAKE_CURRENT_LIST_DIR}/combat.cpp
	${CMAKE_CURRENT_LIST_DIR}/commands.cpp
	${CMAKE_CURRENT_LIST_DIR}/condition.cpp
	${CMAKE_CURRENT_LIST_DIR}/configmanager.cpp
	${CMAKE_CURRENT_LIST_DIR}/connection.cpp
	${CMAKE_CURRENT_LIST_DIR}/container.cpp
	${CMAKE_CURRENT_LIST_DIR}/creature.cpp
	${CMAKE_CURRENT_LIST_DIR}/creatureevent.cpp
	${CMAKE_CURRENT_LIST_DIR}/cylinder.cpp
	${CMAKE_CURRENT_LIST_DIR}/database.cpp
	${CMAKE_CURRENT_LIST_DIR}/databasemanager.cpp
	${CMAKE_CURRENT_LIST_DIR}/databasetasks.cpp
	${CMAKE_CURRENT_LIST_DIR}/depotchest.cpp
	${CMAKE_CURRENT_LIST_DIR}/depotlocker.cpp
	${CMAKE_CURRENT_LIST_DIR}/events.cpp
	${CMAKE_CURRENT_LIST_DIR}/fileloader.cpp
	${CMAKE_CURRENT_LIST_DIR}/game.cpp
	${CMAKE_CURRENT_LIST_DIR}/globalevent.cpp
	${CMAKE_CURRENT_LIST_DIR}/guild.cpp
	${CMAKE_CURRENT_LIST_DIR}/groups.cpp
	${CMAKE_CURRENT_LIST_DIR}/house.cpp
	${CMAKE_CURRENT_LIST_DIR}/housetile.cpp
	${CMAKE_CURRENT_LIST_DIR}/inbox.cpp
	${CMAKE_CURRENT_LIST_DIR}/ioguild.cpp
	${CMAKE_CURRENT_LIST_DIR}/iologindata.cpp
	${CMAKE_CURRENT_LIST_DIR}/iomap.cpp
	${CMAKE_CURRENT_LIST_DIR}/iomapserialize.cpp
	${CMAKE_CURRENT_LIST_DIR}/iomarket.cpp
	${CMAKE_CURRENT_LIST_DIR}/item.cpp
	${CMAKE_CURRENT_LIST_DIR}/items.cpp
	${CMAKE_CURRENT_LIST_DIR}/luascript.cpp
	${CMAKE_CURRENT_LIST_DIR}/mailbox.cpp
	${CMAKE_CURRENT_LIST_DIR}/map.cpp
	${CMAKE_CURRENT_LIST_DIR}/monster.cpp
	${CMAKE_CURRENT_LIST_DIR}/monsters.cpp
	${CMAKE_CURRENT_LIST_DIR}/mounts.cpp
	${CMAKE_CURRENT_LIST_DIR}/movement.cpp
	${CMAKE_CURRENT_LIST_DIR}/npc.cpp
	${CMAKE_CURRENT_LIST_DIR}/otserv.cpp
	${CMAKE_CURRENT_LIST_DIR}/outfit.cpp
	${CMAKE_CURRENT_LIST_DIR}/party.cpp
	${CMAKE_CURRENT_LIST_DIR}/player.cpp
	${CMAKE_CURRENT_LIST_DIR}/position.cpp
	${CMAKE_CURRENT_LIST_DIR}/quests.cpp
	${CMAKE_CURRENT_LIST_DIR}/raids.cpp
	${CMAKE_CURRENT_LIST_DIR}/scheduler.cpp
	${CMAKE_CURRENT_LIST_DIR}/scriptmanager.cpp
	${CMAKE_CURRENT_LIST_DIR}/server.cpp
	${CMAKE_CURRENT_LIST_DIR}/signals.cpp
	${CMAKE_CURRENT_LIST_DIR}/spawn.cpp
	${CMAKE_CURRENT_LIST_DIR}/spells.cpp
	${CMAKE_CURRENT_LIST_DIR}/talkaction.cpp
	${CMAKE_CURRENT_LIST_DIR}/tasks.cpp
	${CMAKE_CURRENT_LIST_DIR}/teleport.cpp
	${CMAKE_CURRENT_LIST_DIR}/thing.cpp
	${CMAKE_CURRENT_LIST_DIR}/tile.cpp
	${CMAKE_CURRENT_LIST_DIR}/tools.cpp
	${CMAKE_CURRENT_LIST_DIR}/trashholder.cpp
	${CMAKE_CURRENT_LIST_DIR}/vocation.cpp
	${CMAKE_CURRENT_LIST_DIR}/waitlist.cpp
	${CMAKE_CURRENT_LIST_DIR}/weapons.cpp
	${CMAKE_CURRENT_LIST_DIR}/wildcardtree.cpp
)

set(INCLUDE_DIRS ${MYSQL_INCLUDE_DIR} ${LUA_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${PUGIXML_INCLUDE_DIR})
set(LIBRARIES ${MYSQL_CLIENT_LIBS} ${LUA_LIBRARIES} ${Boost_LIBRARIES} ${PUGIXML_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

add_executable(tfs ${tfs_SRC})
add_subdirectory(networking)

include_directories(${INCLUDE_DIRS})
target_link_libraries(tfs networking ${LIBRARIES})

set_target_properties(tfs PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "otpch.h")
set_target_properties(tfs PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
cotire(tfs)
