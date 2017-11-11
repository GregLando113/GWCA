#ifndef _TEXT_PARSER_INC
#define _TEXT_PARSER_INC

namespace GW {
    using wchar = wchar_t;

    struct TextCache {
        /* +h0000 */ DWORD h0000;
    };

    struct SubStruct1 {
        /* +h0000 */ DWORD h0000;
    };

    // Allocated @0078C243
    struct SubStructUnk { // total: 0x54/84
        /* +h0000 */ DWORD AsyncDecodeStr_Callback;
        /* +h0004 */ DWORD AsyncDecodeStr_Param;
        /* +h0008 */ DWORD buffer_used; // if it's 1 then uses s1 & if it's 0 uses s2.
        /* +h000C */ Array<wchar> s1;
        /* +h001C */ Array<wchar> s2;
        /* +h002C */ DWORD h002C;
        /* +h0030 */ DWORD h0030; // tell us how many string will be enqueue before decoding.
        /* +h0034 */ DWORD h0034; // @0078B990
        /* +h0038 */ BYTE h0038[0x1C];
    };

    struct TextParser {
        /* +h0000 */ BYTE h0000[0x20];
        /* +h0020 */ wchar *dec_start;
        /* +h0024 */ wchar *dec_end;
        /* +h0028 */ wchar *h0028; // related to h0020 & h0024
        /* +h002C */ wchar *h002C; // related to h0020 & h0024
        /* +h0030 */ TextCache *cache;
        /* +h0034 */ BYTE h0034[0x12C];
        /* +h0160 */ DWORD h0160; // @0078BEF5
        /* +h0164 */ DWORD h0164;
        /* +h0168 */ DWORD h0168; // set to 0 @0078BF34
        /* +h016C */ BYTE h016C[0x14];
        /* +h0180 */ SubStruct1 *sub_struct;
        /* +h0184 */ BYTE h0184[0x38];
        /* +h01BC */ DWORD h01BC; // @0078B9B2
    };

}

#endif // _TEXT_PARSER_INC
