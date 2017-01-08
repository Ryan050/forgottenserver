find_package(GMP REQUIRED)

set(SRC
	${CMAKE_CURRENT_LIST_DIR}/networkmessage.cpp
	${CMAKE_CURRENT_LIST_DIR}/outputmessage.cpp
	${CMAKE_CURRENT_LIST_DIR}/protocol.cpp
	${CMAKE_CURRENT_LIST_DIR}/protocolgame.cpp
	${CMAKE_CURRENT_LIST_DIR}/protocollogin.cpp
	${CMAKE_CURRENT_LIST_DIR}/protocolold.cpp
	${CMAKE_CURRENT_LIST_DIR}/protocolstatus.cpp
	${CMAKE_CURRENT_LIST_DIR}/rsa.cpp
)

add_library(networking ${SRC})
include_directories(${INCLUDE_DIRS} ${GMP_INCLUDE_DIR})
target_link_libraries(networking ${LIBRARIES} ${GMP_LIBRARIES})
