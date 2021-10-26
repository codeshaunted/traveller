// averysumner - traveller
// object.hh
// contains object declarations
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

#ifndef TRAVELLER_OBJECT_HH
#define TRAVELLER_OBJECT_HH

#include "raw_types.hh"

namespace traveller {

typedef uint32_t t_objid;

class Object {
public:
  Object(GameObject_s* __game_object) : _game_object(__game_object) {}
  GameObject_s* getGameObject();
private:
  GameObject_s* _game_object;
};

} // namespace traveller

#endif // TRAVELLER_OBJECT_HH