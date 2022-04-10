// averysumner - traveller
// source/traveller/peer.cc
// contains peer class definitions
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

#include "peer.hh"

namespace traveller {

void Peer::start() {}

void Peer::update() {}

void Peer::stop() {}

bool Peer::getIsServer() { return _is_server; }

void Peer::send(const RakNet::BitStream& __bitstream, const RakNet::SystemAddress& __system_address, bool __broadcast) {
    TRAVELLER_LOG_DEBUG("BITSTREAM LENGTH: %u", __bitstream.GetNumberOfBytesUsed());
    _interface->Send(&__bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, __system_address, __broadcast);
}

void Peer::send(const RakNet::BitStream& __bitstream) { // broadcasts
    send(__bitstream, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Peer::send(const Message& message, const RakNet::SystemAddress& __system_address, bool __broadcast) {
    RakNet::BitStream bitstream;
    message.serialize(bitstream);

    send(bitstream, __system_address, __broadcast);
}

void Peer::send(const Message& message) { // broadcasts
    send(message, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Peer::_handleMessage(RakNet::BitStream& __bitstream) {}

void Peer::_addUpdateCallback(std::function<void(uint32_t)> __callback_function) {
    _update_callbacks.insert({ _next_callback_id, __callback_function });
    _next_callback_id++;
}

void Peer::_removeUpdateCallback(uint32_t __callback_id) {
    _update_callbacks.erase(__callback_id);
}

void Peer::_callCallbacks() {
    for (auto& callback : _update_callbacks) {
        callback.second(callback.first);
    }
}

} // namespace traveller