cmake_minimum_required(VERSION 3.7)
project(BoolPgia)

set(CMAKE_CXX_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
set(SDK_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../P2PGamePlatform)

set(SOURCE_FILES
        BoolPgia.cpp
        BPCLientHandler.cpp BPCLientHandler.h
        BPDispatcher.cpp BPDispatcher.h
        BPServer.cpp BPServer.h
        GameProtocol.h
        test/RunTest.cpp
        test/RunTest.h BPLoginService.cpp BPLoginService.h ScoreSaverService.cpp ScoreSaverService.h test/TestMessageHandler.cpp test/TestMessageHandler.h)

include_directories(${SDK_LIB})
add_executable(BoolPgia ${SOURCE_FILES})

target_link_libraries(BoolPgia Threads::Threads)
target_link_libraries(BoolPgia ${SDK_LIB}/cmake-build-debug/libP2PGamePlatform.a)

