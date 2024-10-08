// averysumner - traveller
// source/traveller/object.cc
// contains object definitions
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

#include "object.hh"

namespace traveller {

GameObject_s* Object::getGameObject() {
    return _game_object;
}

nuvec_s Object::getPosition() {
    return _game_object->position;
}

nuvec_s Object::getVelocity() {
    return _game_object->velocity;
}

void Object::setPosition(nuvec_s __position) {
    _game_object->position = __position;
}

void Object::setVelocity(nuvec_s __velocity) {
    _game_object->velocity = __velocity;
}

} // namespace traveller