cmake_minimum_required(VERSION 3.19)

set(PROJECT_NAME "es8cli")

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp execute.cpp commandline.cpp es8data.cpp patch.cpp globals.cpp helpers.cpp bitcoder.cpp midimessages.cpp midi.cpp es8parameters.cpp rtmidi-4.0.0/RtMidi.cpp)

if(APPLE)
	target_compile_definitions(${PROJECT_NAME} PRIVATE "-D__MACOSX_CORE__")
	target_link_libraries(${PROJECT_NAME} "-framework CoreServices" "-framework CoreAudio" "-framework CoreMIDI" "-framework CoreFoundation")
endif()
