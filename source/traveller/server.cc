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
#include "object_manager.hh"

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
                MessageSetLevel message(*RawAPI::Level);
                send(message); // todo: fix so i don't have to broadcast
                break;
            }
            case ID_CONNECTION_LOST:
                TRAVELLER_LOG("Lost connection to client at %s.", packet->systemAddress.ToString());
                break;
            default:
                RakNet::BitStream bitstream(packet->data, packet->length, false);
                _handleMessage(bitstream);
                break;
        }

        _callCallbacks();
    }
}

void Server::stop() {
    RakNet::RakPeerInterface::DestroyInstance(_interface);

    TRAVELLER_LOG("Stopping server.");
}

void Server::_handleMessage(RakNet::BitStream& __bitstream) {
    MessageID message_id;
    __bitstream.Read(message_id);

    TRAVELLER_LOG_DEBUG("GOT MESSAGE WITH ID: %u", message_id);

    switch (message_id) {
        case MessageID::LEVEL_SET: {
            MessageLevelSet message;
            message.deserialize(__bitstream);

            for (auto& object : ObjectManager::getObjects()) {
                TRAVELLER_LOG_DEBUG("%u", object.second.getGameObject()->character_id);
                MessageConstructObject construct_object_message(object.first, object.second.getGameObject()->character_id, object.second.getPosition());
                send(construct_object_message); // todo: fix so i don't have to broadcast
            }

            break;
        }
        default: {
            TRAVELLER_LOG_ERROR("Unhandled message ID: %u", message_id);
            break;
        }
    }
}

} // namespace traveller