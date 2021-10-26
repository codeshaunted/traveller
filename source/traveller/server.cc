// averysumner - traveller
// server.cc
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

#include "logger.hh"

namespace traveller {

void Server::start() {
  if (enet_initialize() != 0) {
    TRAVELLER_LOG_ERROR("Failed to initialize ENet!");
    return;
  }

  ENetAddress address = {0};

  enet_address_set_host(&address, _host.c_str());
  address.port = _port;

  _server = enet_host_create(&address, _max_clients, 2, 0, 0);

  if (!_server) {
    TRAVELLER_LOG_ERROR("A server could not be started!");
    return;
  }

  TRAVELLER_LOG("Server successfully started at %s:%i.", _host, _port);
}

void Server::update() {
  ENetEvent event;

  // todo: maybe make server timeout configurable
  while (enet_host_service(_server, &event, 1000) > 0) {
    char host_name[64];
    enet_address_get_host_ip(&event.peer->address, host_name, sizeof(host_name));

    switch (event.type) {
    case ENET_EVENT_TYPE_CONNECT: {
      TRAVELLER_LOG("New connection from %s.", host_name);
      //packets::GoToLevel go_to_level{ mod_api_->GetCurrentLevelID() };
      //SendPacket(event.peer, &go_to_level);
      break;
    }
    case ENET_EVENT_TYPE_RECEIVE:
      enet_packet_destroy(event.packet);
      break;
    case ENET_EVENT_TYPE_DISCONNECT:
      TRAVELLER_LOG("Lost connection to client at %s.", host_name);
      break;
    case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
      TRAVELLER_LOG("Lost connection to client at %s via timeout.", host_name);
      break;
    case ENET_EVENT_TYPE_NONE:
      break;
    }
  }
}

void Server::stop() {
  enet_host_destroy(_server);
  enet_deinitialize();
}
/*
void Server::sendPacket(ENetPeer* __peer, packets::Base* __data) {
  DataStream data_stream;
  __data->Serialize(data_stream);
  ENetPacket* packet = enet_packet_create(data_stream.GetBytes(), data_stream.Length() , ENET_PACKET_FLAG_RELIABLE); // todo: maybe not always use reliable transmission?
  if (__peer == PEER_BROADCAST) {
    enet_host_broadcast(_server, 0, packet); // pass PEER_BROADCAST for peer to broadcast
  }
  else {
    enet_peer_send(__peer, 0, packet);
  }
}*/

} // namespace traveller