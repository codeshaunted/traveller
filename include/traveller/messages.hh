// averysumner - traveller
// include/traveller/messages.hh
// contains messages declarations
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

#ifndef TRAVELLER_MESSAGES_HH
#define TRAVELLER_MESSAGES_HH

#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"

#include "raw_types.hh"
#include "object.hh"

namespace traveller {

enum class MessageID : uint8_t {
    SET_LEVEL = ID_USER_PACKET_ENUM + 1,
    LEVEL_SET,
    CONSTRUCT_OBJECT,
    DESTRUCT_OBJECT,
    UPDATE_OBJECT,
    POSITION_UPDATE_TEST
};

class Message {
    public:
        Message(MessageID __message_id) : _message_id(__message_id) {}
        virtual void serialize(RakNet::BitStream& __bitstream) const {}
        virtual void deserialize(RakNet::BitStream& __bitstream) {}
    protected:
        MessageID _message_id;
};

class MessageSetLevel : public Message {
    public:
        MessageSetLevel(uint32_t __level_id = 0) : Message(MessageID::SET_LEVEL), level_id(__level_id) {}
        void serialize(RakNet::BitStream& __bitstream) const override {
            __bitstream.Write((RakNet::MessageID)_message_id);

            if (level_id != 0) {
                __bitstream.Write1();
                __bitstream.Write(level_id);
            }
            else {
                __bitstream.Write0();
            }
        }
        void deserialize(RakNet::BitStream& __bitstream) override {
            if (__bitstream.ReadBit()) {
                __bitstream.Read(level_id);
            }
        }
        uint32_t level_id;
};

class MessageLevelSet : public Message {
public:
    MessageLevelSet(uint32_t __level_id = 0) : Message(MessageID::LEVEL_SET), level_id(__level_id) {}
    void serialize(RakNet::BitStream& __bitstream) const override {
        __bitstream.Write((RakNet::MessageID)_message_id);

        if (level_id != 0) {
            __bitstream.Write1();
            __bitstream.Write(level_id);
        }
        else {
            __bitstream.Write0();
        }
    }
    void deserialize(RakNet::BitStream& __bitstream) override {
        if (__bitstream.ReadBit()) {
            __bitstream.Read(level_id);
        }
    }
    uint32_t level_id;
};

class MessageConstructObject : public Message {
    public:
        MessageConstructObject(t_objid __object_id = 0, uint16_t __character_id = 0, nuvec_s __position = nuvec_s()) : Message(MessageID::CONSTRUCT_OBJECT), object_id(__object_id), character_id(__character_id), position(__position) {}
        void serialize(RakNet::BitStream& __bitstream) const override {
            __bitstream.Write((RakNet::MessageID)_message_id);

            if (object_id != 0) {
                __bitstream.Write1();
                __bitstream.Write(object_id);
            }
            else {
                __bitstream.Write0();
            }

            if (character_id != 0) {
                __bitstream.Write1();
                __bitstream.Write(character_id);
            }
            else {
                __bitstream.Write0();
            }

            if (position.x != 0 && position.y != 0 && position.z != 0) {
                __bitstream.Write1();
                __bitstream.Write(position);
            }
            else {
                __bitstream.Write0();
            }
        }
        void deserialize(RakNet::BitStream& __bitstream) override {
            if (__bitstream.ReadBit()) {
                __bitstream.Read(object_id);
            }

            if (__bitstream.ReadBit()) {
                __bitstream.Read(character_id);
            }

            if (__bitstream.ReadBit()) {
                __bitstream.Read(position);
            }
        }
        t_objid object_id;
        uint16_t character_id;
        nuvec_s position;
};

class Messages {
    public:
        static void handle(RakNet::BitStream& __bitstream);
        static void handleSetLevel(const MessageSetLevel& __message);
        static void handleConstructObject(const MessageConstructObject& __message);

};

} // namespace traveller

#endif // TRAVELLER_MESSAGES_HH