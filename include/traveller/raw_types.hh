// averysumner - traveller
// include/traveller/raw_types.hh
// contains raw types for nu2
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

#ifndef TRAVELLER_RAW_TYPES_HH
#define TRAVELLER_RAW_TYPES_HH

#include <wtypes.h>

#include <cstdint>

namespace traveller {

struct nuvec_s {
    float x;
    float y;
    float z;
};

struct nugspline_s {};

struct CHARACTERDATA_s {};

struct AIPATHINFO_s {};

struct AIGROUP_s {};

struct LEVELDATA_s {};

struct APIOBJECTSYS_s {};

#pragma pack(1)
struct WORLDINFO_s {
    uint8_t field_0[0x10f];
    BOOL loaded;
    uint8_t field_2[0x14];
    LEVELDATA_s* level_data;
    LEVELDATA_s* alternate_level_data;
    uint8_t field_5[0x285d];
    APIOBJECTSYS_s* api_object_system;
};

#pragma pack(1)
struct GameObject_s {
    uint8_t field_0[0x58];
    CHARACTERDATA_s* character_data;
    nuvec_s position;
    nuvec_s velocity;
    uint8_t field_1[0xff0];
    uint16_t character_id;
};

struct MENU_s {};

class CD3DCore {};

} // namespace traveller

#endif // TRAVELLER_RAW_TYPES_HH