# averysumner - traveller
# source/traveller/CMakeLists.txt
# project source directory CMake file
# Copyright 2021 averysumner
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http:#www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set(TRAVELLER_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/dll_main.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/logger.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/hook.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/object.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/object_manager.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/peer.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/server.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/client.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/messages.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/dinput8.def")

set(TRAVELLER_COMPILE_DEFINITIONS
    WIN32_LEAN_AND_MEAN
    WINDOWS_IGNORE_PACKING_MISMATCH)

set(TRAVELLER_INCLUDE_DIRECTORIES
    "${CMAKE_SOURCE_DIR}/include/traveller"
    "${CMAKE_SOURCE_DIR}/third_party/RakNet/Source"
    "${CMAKE_SOURCE_DIR}/third_party/DynoHook/include"
    ${PROJECT_BINARY_DIR})

set(TRAVELLER_LINK_LIBRARIES
    RakNetLibStatic
    dynohook)

add_library(dinput8 SHARED ${TRAVELLER_SOURCE_FILES})

target_compile_definitions(dinput8 PUBLIC ${TRAVELLER_COMPILE_DEFINITIONS})

target_include_directories(dinput8 PUBLIC ${TRAVELLER_INCLUDE_DIRECTORIES})

target_link_libraries(dinput8 PUBLIC ${TRAVELLER_LINK_LIBRARIES})

# todo: add way to configure this
install(TARGETS dinput8 DESTINATION "C:/Program Files (x86)/Steam/steamapps/common/Lego Star Wars Saga")