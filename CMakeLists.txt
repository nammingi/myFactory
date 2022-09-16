CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(MyFactoryTest)

SET(TARGET_APP MyFactoryTest)
SET(LIBRARY My_Message)
SET(THREADS_PREFER_PTRHEAD_FLAG ON)

#Below for Debug
SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_BUILD_TYPE RelWithDebInfo)

SET(SRC_FILES
    ./LinkedList.c
    ./MessageLib.c
    ./Signal.c
    ./Common.c
)

ADD_LIBRARY( ${LIBRARY} SHARED ${SRC_FILES})
ADD_EXECUTABLE(${TARGET_APP} MessageLibTest.c)

find_package(Threads REQUIRED)

#target_include_directories(${LIBRARY} PUBLIC ${CMAKE_CURRENT_DIR}/include)
target_link_libraries(${TARGET_APP} PRIVATE Threads::Threads)
target_link_libraries(${TARGET_APP} PUBLIC ${LIBRARY})
