# averysumner - traveller
# CMakeLists.txt
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
	"${CMAKE_CURRENT_SOURCE_DIR}/dinput8.def")

set(TRAVELLER_INCLUDE_DIRECTORIES
	"${CMAKE_SOURCE_DIR}/include/traveller"
	${PROJECT_BINARY_DIR})

add_library(dinput8 SHARED ${TRAVELLER_SOURCE_FILES})

target_compile_definitions(dinput8 PUBLIC WIN32_LEAN_AND_MEAN)

target_include_directories(dinput8 PUBLIC ${TRAVELLER_INCLUDE_DIRECTORIES})

# todo: add way to configure this
install(TARGETS dinput8 DESTINATION "C:/Program Files (x86)/Steam/steamapps/common/Lego Star Wars Saga")