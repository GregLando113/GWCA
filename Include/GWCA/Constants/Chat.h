#pragma once
#include <GWCA/Constants/Types.h>

namespace GW {
    namespace Constants {
        enum class TextLanguage {
            English,
            Korean,
            French,
            German,
            Italian,
            Spanish,
            TraditionalChinese,
            Japanese = 8,
            Polish,
            Russian,
            BorkBorkBork = 17,

            None = -1
        };
        ENUM_CLASS_OPERATORS(TextLanguage, -1)
    }
    namespace Chat {
        enum class Channel {
            CHANNEL_ALLIANCE = 0,
            CHANNEL_ALLIES = 1, // coop with two groups for instance.
            CHANNEL_GWCA1 = 2,
            CHANNEL_ALL = 3,
            CHANNEL_GWCA2 = 4,
            CHANNEL_MODERATOR = 5,
            CHANNEL_EMOTE = 6,
            CHANNEL_WARNING = 7, // shows in the middle of the screen and does not parse <c> tags
            CHANNEL_GWCA3 = 8,
            CHANNEL_GUILD = 9,
            CHANNEL_GLOBAL = 10,
            CHANNEL_GROUP = 11,
            CHANNEL_TRADE = 12,
            CHANNEL_ADVISORY = 13,
            CHANNEL_WHISPER = 14,
            CHANNEL_COUNT,

            // non-standard channel, but usefull.
            CHANNEL_COMMAND,
            CHANNEL_UNKNOW = -1
        };
        ENUM_CLASS_OPERATORS(Channel, -1)
    }
}