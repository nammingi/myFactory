CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(MyFactoryTest)

SET(TARGET_APP MyFactoryTest)
SET(LIBRARY My_Message)
SET(THREADS_PREFER_PTRHEAD_FLAG ON)

SET(SRC_FILES
    ./LinkedList.c
)

ADD_LIBRARY( ${LIBRARY} SHARED ${SRC_FILES})
ADD_EXECUTABLE(${TARGET_APP} testbed.c)

find_package(Threads REQUIRED)

#target_include_directories(${LIBRARY} PUBLIC ${CMAKE_CURRENT_DIR}/include)
target_link_libraries(${TARGET_APP} PRIVATE Threads::Threads)
target_link_libraries(${TARGET_APP} PUBLIC ${LIBRARY})
