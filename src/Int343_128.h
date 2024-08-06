
/**********************************\
 ==================================
 
          Bit manipulations
 
 ==================================
\**********************************/

#define U64 unsigned long long
#define U128 unsigned __int128

#define GetHigh(value128) ((U64) (value128 >> 64))
#define GetLow(value128) ((U64) value128)

struct Int343 {

public:
    U128 Bits[3];

    Int343() {
        Bits[0] = 0;
        Bits[1] = 0;
        Bits[2] = 0;
    }

    Int343(U64 zero, U64 one, U64 two, U64 three, U64 four, U64 five) {
        Bits[0] = zero; Bits[0] <<= 64; Bits[0] |= one;
        Bits[1] = two; Bits[1] <<= 64; Bits[1] |= three;
        Bits[2] = four; Bits[2] <<= 64; Bits[2] |= five;
    }

    void Print() {
        printf("%lluULL, %lluULL, %lluULL, %lluULL, %lluULL, %lluULL\n", 
            GetHigh(Bits[0]), GetLow(Bits[0]), GetHigh(Bits[1]), GetLow(Bits[1]), GetHigh(Bits[2]), GetLow(Bits[2]));
    }

    //TODO - improve
    inline Int343 operator<<(int count) {
        Int343 result;
        int start = count / 128;
        int shift = count % 128;
        //start == 0
        //start == 1
        //start == 2
        for (int i = 0; i < 3; i++) {
            if (start < 2)
                result.Bits[i] = (Bits[start] << shift) | ((shift > 0) ? Bits[start + 1] >> (128 - shift) : (U128) 0);
            else if (start < 3)
                result.Bits[i] = Bits[start] << shift;
            else
                result.Bits[i] =  (U128) 0;
            start++;
        }
        result.Bits[0] <<= 41;
        result.Bits[0] >>= 41;
        return result;
    }

    //TODO - improve
    inline Int343 operator>>(int count) {
        Int343 result;
        int start = 2 - (count / 128);
        int shift = count % 128;
        for (int i = 2; i >= 0; i--) {
            if (start > 0)
                result.Bits[i] = (Bits[start] >> shift) | ((shift > 0) ? Bits[start - 1] << (128 - shift) : (U128) 0);
            else if (start == 0)
                result.Bits[i] = (Bits[start] >> shift);
            else
                result.Bits[i] = (U128) 0;
            start--;
        }
        return result;
    }

    inline void operator&=(Int343 other) {
        Bits[0] &= other.Bits[0];
        Bits[1] &= other.Bits[1];
        Bits[2] &= other.Bits[2];
    }

    inline Int343 operator&(Int343 other) {
        Int343 result;
        result.Bits[0] = Bits[0] & other.Bits[0];
        result.Bits[1] = Bits[1] & other.Bits[1];
        result.Bits[2] = Bits[2] & other.Bits[2];
        return result;
    }

    inline void operator|=(Int343 other) {
        Bits[0] |= other.Bits[0];
        Bits[1] |= other.Bits[1];
        Bits[2] |= other.Bits[2];
    }

    inline Int343 operator|(Int343 other) {
        Int343 result;
        result.Bits[0] = Bits[0] | other.Bits[0];
        result.Bits[1] = Bits[1] | other.Bits[1];
        result.Bits[2] = Bits[2] | other.Bits[2];
        return result;
    }

    inline void operator^=(Int343 other) {
        Bits[0] ^= other.Bits[0];
        Bits[1] ^= other.Bits[1];
        Bits[2] ^= other.Bits[2];
    }
    
    inline Int343 operator^(Int343 other) {
        Int343 result;
        result.Bits[0] = Bits[0] ^ other.Bits[0];
        result.Bits[1] = Bits[1] ^ other.Bits[1];
        result.Bits[2] = Bits[2] ^ other.Bits[2];
        return result;
    }
    
    inline Int343 operator~() {
        Int343 result;
        result.Bits[0] = ~Bits[0];
        result.Bits[1] = ~Bits[1];
        result.Bits[2] = ~Bits[2];
        result.Bits[0] <<= 41;
        result.Bits[0] >>= 41;
        return result;
    }

    inline bool operator==(Int343 other) {
        return Bits[0] == other.Bits[0] &&
            Bits[1] == other.Bits[1] &&
            Bits[2] == other.Bits[2];
    }

    inline bool operator!() {
        return !Bits[2] && !Bits[1] && !Bits[0];
    }
};


#define SetBit(bitboard, block) (bitboard.Bits[2 - (block / 128)] |= (U128) 1 << (block % 128))
#define GetBit(bitboard, block) (bitboard.Bits[2 - (block / 128)] & ((U128) 1 << (block % 128)))
#define PopBit(bitboard, block) (bitboard.Bits[2 - (block / 128)] &= ~((U128) 1 << (block % 128)))
#define IsSet(bitboard) (bitboard.Bits[2] > 0 || bitboard.Bits[1] > 0 || bitboard.Bits[0] > 0)
#define GetBitsU64(bitboard, index) (index % 2 == 0 ? GetHigh(bitboard.Bits[index]) : GetLow(bitboard.Bits[index])) //index is from 0 to 5 with U64  - index % 2 == 0 ? hi : low

static inline int ctz128(U128 value) {
    int index = __builtin_ctzll(GetLow(value));
    //printf("index = %d, low value = %llu, high value = %llu\n", index, GetLow(value), GetHigh(value));
    if (index >= 64) index = 64 + __builtin_ctzll(GetHigh(value));
    return index;
}

static inline int clz128(U128 value) {
    int index = __builtin_clzll(GetHigh(value));
    //printf("index = %d, low value = %llu, high value = %llu\n", index, GetLow(value), GetHigh(value));
    if (index >= 64) index = 64 + __builtin_clzll(GetLow(value));
    return index;
}

static inline int Count(Int343 bitboard) {
    return __builtin_popcountll(GetLow(bitboard.Bits[2])) + __builtin_popcountll(GetHigh(bitboard.Bits[2]))
         + __builtin_popcountll(GetLow(bitboard.Bits[1])) + __builtin_popcountll(GetHigh(bitboard.Bits[1]))
         + __builtin_popcountll(GetLow(bitboard.Bits[0])) + __builtin_popcountll(GetHigh(bitboard.Bits[0]));
}

static inline int ForwardScanPop(Int343 *bitboard) {
    int result = 0;
    if (bitboard->Bits[2] != 0) {
        result = ctz128(bitboard->Bits[2]);
        bitboard->Bits[2] &= bitboard->Bits[2] - 1;
        return result;
    }

    if (bitboard->Bits[1] != 0) {
        result = ctz128(bitboard->Bits[1]);
        bitboard->Bits[1] &= bitboard->Bits[1] - 1;
        return 128 + result;
    }

    if (bitboard->Bits[0] != 0) {
        result = ctz128(bitboard->Bits[0]);
        bitboard->Bits[0] &= bitboard->Bits[0] - 1;
        return 256 + result;
    }

    return -1;
}

static inline int BitScanForward(Int343 bitboard) {
    if (bitboard.Bits[2] != 0) return ctz128(bitboard.Bits[2]);
    if (bitboard.Bits[1] != 0) return 128 + ctz128(bitboard.Bits[1]);
    if (bitboard.Bits[0] != 0) return 256 + ctz128(bitboard.Bits[0]);
    return -1;
}

//TODO - replace with 
static inline int BitScanReverse(Int343 bitboard) {
    if (bitboard.Bits[0] != 0) return 256 + (127 - clz128(bitboard.Bits[0]));
    if (bitboard.Bits[1] != 0) return 128 + (127 - clz128(bitboard.Bits[1]));
    if (bitboard.Bits[2] != 0) return 127 - clz128(bitboard.Bits[2]);
    return -1;
}

static inline Int343 GetFirstBit(Int343 bitboard) {
    Int343 result;
    if (bitboard.Bits[2] != 0) result.Bits[2] |= (bitboard.Bits[2] & -bitboard.Bits[2]);
    else if (bitboard.Bits[1] != 0) result.Bits[1] |= (bitboard.Bits[1] & -bitboard.Bits[1]);
    else if (bitboard.Bits[0] != 0) result.Bits[0] |= (bitboard.Bits[0] & -bitboard.Bits[0]);
    return result;
}

#define BitScan(side, bitboard) (side == white ? BitScanReverse(bitboard) : BitScanForward(bitboard))
//#define BitScan(side, bitboard) BitScanReverse(bitboard)
