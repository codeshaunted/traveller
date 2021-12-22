// averysumner - traveller
// source/traveller/server.cc
// contains server class definitions
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

#include "server.hh"

#include "MessageIdentifiers.h"

#include "logger.hh"
#include "messages.hh"
#include "raw_api.hh"

namespace traveller {

void Server::start() {
    _interface = RakNet::RakPeerInterface::GetInstance();

    RakNet::SocketDescriptor socket_descriptor(_port, _host.c_str());
    _interface->Startup(_max_clients, &socket_descriptor, 1);
    _interface->SetMaximumIncomingConnections(_max_clients);

    TRAVELLER_LOG("Started server at %s|%u.", _host.c_str(), _port);
}

void Server::update() {
    for (RakNet::Packet* packet = _interface->Receive(); packet; _interface->DeallocatePacket(packet), packet = _interface->Receive()) {
        uint8_t message_id = packet->data[0];

        switch (message_id) {
            case ID_NEW_INCOMING_CONNECTION: {
                TRAVELLER_LOG("A new client has connected from %s.", packet->systemAddress.ToString());
                MessageSetLevel message = MessageSetLevel(*RawAPI::Level);
                send(message, packet->systemAddress, true);
                break;
            }
            case ID_CONNECTION_LOST:
                TRAVELLER_LOG("Lost connection to client at %s.", packet->systemAddress.ToString());
                break;
            default:
                RakNet::BitStream bitstream(packet->data, packet->length, false);
                Messages::handle(bitstream);
                break;
        }
    }
}

void Server::stop() {
    RakNet::RakPeerInterface::DestroyInstance(_interface);

    TRAVELLER_LOG("Stopping server.");
}

} // namespace traveller