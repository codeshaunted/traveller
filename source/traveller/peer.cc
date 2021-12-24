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
    TRAVELLER_LOG("BITSTREAM LENGTH: %u", __bitstream.GetNumberOfBytesUsed());
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

} // namespace traveller