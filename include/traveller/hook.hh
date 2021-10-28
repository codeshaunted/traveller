// averysumner - traveller
// hook.hh
// contains hook class declarations
// Copyright 2021 averysumner
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef TRAVELLER_HOOK_HH
#define TRAVELLER_HOOK_HH

#include <cstdint>

namespace traveller {

#pragma pack(1)
struct s_JMPInstruction {
  uint8_t op_code = 0xe9;
  uintptr_t address;
};

class Hook {
  public:
    static bool detour(uintptr_t __source, void* __destination, int length = 5);
    static void* trampoline(uintptr_t __source, void* __destination, int length = 5);
};

} // namespace traveller

#endif // TRAVELLER_HOOK_HH