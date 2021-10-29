# averysumner - traveller
# CMakeLists.txt
# third party CMake file
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

# Detours
set(DETOURS_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/Detours/src/creatwth.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Detours/src/detours.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Detours/src/disasm.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Detours/src/disolarm.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Detours/src/disolarm64.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Detours/src/disolia64.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Detours/src/disolx64.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Detours/src/disolx86.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Detours/src/image.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Detours/src/modules.cpp")

add_library(Detours STATIC ${DETOURS_SOURCE_FILES})

set(property SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/Detours/src/uimports.cpp" APPEND PROPERTY HEADER_FILE_ONLY TRUE)

#target_compile_options(Detours PRIVATE /W4 /WX /Zi /MT /Gy /Gm- /Zl /Od)

target_include_directories(Detours PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Detours/src")

# enet
set(ENET_STATIC ON)
add_subdirectory("enet")