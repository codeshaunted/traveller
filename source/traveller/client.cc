// averysumner - traveller
// source/traveller/client.cc
// contains client class definitions
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

#include "client.hh"

#include "MessageIdentifiers.h"

#include "logger.hh"

namespace traveller {

void Client::start() {
    _interface = RakNet::RakPeerInterface::GetInstance();

    RakNet::SocketDescriptor socket_descriptor;
    _interface->Startup(1, &socket_descriptor, 1);

    _interface->Connect(_host.c_str(), _port, 0, 0);

    TRAVELLER_LOG("Started client and attempted connection to %s|%u.", _host.c_str(), _port);
}

void Client::update() {
    for (RakNet::Packet* packet = _interface->Receive(); packet; _interface->DeallocatePacket(packet), packet = _interface->Receive()) {
        uint8_t message_id = packet->data[0];

        switch (message_id) {
            case ID_CONNECTION_REQUEST_ACCEPTED:
                TRAVELLER_LOG("Sucessfully connected to %s.", packet->systemAddress.ToString());
                break;
            case ID_CONNECTION_LOST:
                TRAVELLER_LOG("Lost connection to server.", packet->systemAddress.ToString());
                break;
            default:
                RakNet::BitStream bitstream(packet->data, packet->length, false);
                Messages::handle(bitstream);
                break;
        }
    }
}

void Client::stop() {
    RakNet::RakPeerInterface::DestroyInstance(_interface);

    TRAVELLER_LOG("Stopping client.");
}

} // namespace traveller