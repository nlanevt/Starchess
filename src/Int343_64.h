/**********************************\
 ==================================
 
          Bit manipulations
 
 ==================================
\**********************************/

#define U64 unsigned long long
 #define U128 unsigned __int128

struct Int343 {

public:
    U64 Bits[6];

    Int343() {
        Bits[0] = 0;
        Bits[1] = 0;
        Bits[2] = 0;
        Bits[3] = 0;
        Bits[4] = 0;
        Bits[5] = 0;
    }

    Int343(U64 zero, U64 one, U64 two, U64 three, U64 four, U64 five) {
        Bits[0] = zero;
        Bits[1] = one;
        Bits[2] = two;
        Bits[3] = three;
        Bits[4] = four;
        Bits[5] = five;
    }

    void Print() {
        printf("%lluULL, %lluULL, %lluULL, %lluULL, %lluULL, %lluULL\n", Bits[0], Bits[1], Bits[2], Bits[3], Bits[4], Bits[5]);
    }

    inline U64 &operator[](int i) {
         return Bits[i];
    }

    inline Int343 operator<<(int count) {
        Int343 result;
        int start = count / 64;
        int shift = count % 64;
        for (int i = 0; i < 6; i++) {
            if (start < 5)
                result.Bits[i] = (Bits[start] << shift) | ((shift > 0) ? Bits[start + 1] >> (64 - shift) : 0ULL);
            else if (start < 6)
                result.Bits[i] = Bits[start] << shift;
            else
                result.Bits[i] = 0ULL;
            start++;
        }
        result.Bits[0] &= 0x7FFFFF;
        return result;
    }

    inline Int343 operator>>(int count) {
        Int343 result;
        int start = 5 - (count / 64);
        int shift = count % 64;
        for (int i = 5; i >= 0; i--) {
            if (start > 0)
                result[i] = (Bits[start] >> shift) | ((shift > 0) ? Bits[start - 1] << (64 - shift) : 0ULL);
            else if (start == 0)
                result.Bits[i] = (Bits[start] >> shift);
            else
                result.Bits[i] = 0ULL;
            start--;
        }
        return result;
    }

    inline void operator&=(Int343 other) {
        Bits[0] &= other.Bits[0];
        Bits[1] &= other.Bits[1];
        Bits[2] &= other.Bits[2];
        Bits[3] &= other.Bits[3];
        Bits[4] &= other.Bits[4];
        Bits[5] &= other.Bits[5];
    }

    inline Int343 operator&(Int343 other) {
        Int343 result;
        result.Bits[0] = Bits[0] & other.Bits[0];
        result.Bits[1] = Bits[1] & other.Bits[1];
        result.Bits[2] = Bits[2] & other.Bits[2];
        result.Bits[3] = Bits[3] & other.Bits[3];
        result.Bits[4] = Bits[4] & other.Bits[4];
        result.Bits[5] = Bits[5] & other.Bits[5];
        return result;
    }

    inline void operator|=(Int343 other) {
        Bits[0] |= other.Bits[0];
        Bits[1] |= other.Bits[1];
        Bits[2] |= other.Bits[2];
        Bits[3] |= other.Bits[3];
        Bits[4] |= other.Bits[4];
        Bits[5] |= other.Bits[5];
    }

    inline Int343 operator|(Int343 other) {
        Int343 result;
        result.Bits[0] = Bits[0] | other.Bits[0];
        result.Bits[1] = Bits[1] | other.Bits[1];
        result.Bits[2] = Bits[2] | other.Bits[2];
        result.Bits[3] = Bits[3] | other.Bits[3];
        result.Bits[4] = Bits[4] | other.Bits[4];
        result.Bits[5] = Bits[5] | other.Bits[5];
        return result;
    }

    inline void operator^=(Int343 other) {
        Bits[0] ^= other.Bits[0];
        Bits[1] ^= other.Bits[1];
        Bits[2] ^= other.Bits[2];
        Bits[3] ^= other.Bits[3];
        Bits[4] ^= other.Bits[4];
        Bits[5] ^= other.Bits[5];
    }
    
    inline Int343 operator^(Int343 other) {
        Int343 result;
        result.Bits[0] = Bits[0] ^ other.Bits[0];
        result.Bits[1] = Bits[1] ^ other.Bits[1];
        result.Bits[2] = Bits[2] ^ other.Bits[2];
        result.Bits[3] = Bits[3] ^ other.Bits[3];
        result.Bits[4] = Bits[4] ^ other.Bits[4];
        result.Bits[5] = Bits[5] ^ other.Bits[5];
        return result;
    }
    
    inline Int343 operator~() {
        Int343 result;
        result.Bits[0] = ~Bits[0];
        result.Bits[1] = ~Bits[1];
        result.Bits[2] = ~Bits[2];
        result.Bits[3] = ~Bits[3];
        result.Bits[4] = ~Bits[4];
        result.Bits[5] = ~Bits[5];
        result.Bits[0] &= 0x7FFFFF;
        return result;
    }

    inline bool operator==(Int343 other) {
        return Bits[0] == other.Bits[0] &&
            Bits[1] == other.Bits[1] &&
            Bits[2] == other.Bits[2] &&
            Bits[3] == other.Bits[3] &&
            Bits[4] == other.Bits[4] &&
            Bits[5] == other.Bits[5];
    }

    inline bool operator!() {
        return !Bits[5] && !Bits[4] && !Bits[3] &&
               !Bits[2] && !Bits[1] && !Bits[0];
    }
};


#define SetBit(bitboard, block) (bitboard.Bits[5 - (block / 64)] |= 1ULL << (block % 64))
#define GetBit(bitboard, block) (bitboard.Bits[5 - (block / 64)] & (1ULL << (block % 64)))
#define PopBit(bitboard, block) (bitboard.Bits[5 - (block / 64)] &= ~(1ULL << (block % 64)))
#define IsSet(bitboard) (bitboard.Bits[5] > 0 || bitboard.Bits[4] > 0 || bitboard.Bits[3] > 0 || bitboard.Bits[2] > 0 || bitboard.Bits[1] > 0 || bitboard.Bits[0] > 0)
#define GetBitsU64(bitboard, index) (bitboard.Bits[index]) //index is from 0 to 5 with U64

static inline int Count(Int343 bitboard) {
    return __builtin_popcountll(bitboard.Bits[5]) + __builtin_popcountll(bitboard.Bits[4])
         + __builtin_popcountll(bitboard.Bits[3]) + __builtin_popcountll(bitboard.Bits[2])
         + __builtin_popcountll(bitboard.Bits[1]) + __builtin_popcountll(bitboard.Bits[0]);
}

static inline int ForwardScanPop(Int343 *bitboard) {
    int result = 0;
    if (bitboard->Bits[5] != 0) {result = __builtin_ctzll(bitboard->Bits[5]); bitboard->Bits[5] &= bitboard->Bits[5] - 1; return result;}
    if (bitboard->Bits[4] != 0) {result = 64 + __builtin_ctzll(bitboard->Bits[4]); bitboard->Bits[4] &= bitboard->Bits[4] - 1; return result;}
    if (bitboard->Bits[3] != 0) {result = 128 + __builtin_ctzll(bitboard->Bits[3]); bitboard->Bits[3] &= bitboard->Bits[3] - 1; return result;}
    if (bitboard->Bits[2] != 0) {result = 192 + __builtin_ctzll(bitboard->Bits[2]); bitboard->Bits[2] &= bitboard->Bits[2] - 1; return result;}
    if (bitboard->Bits[1] != 0) {result = 256 + __builtin_ctzll(bitboard->Bits[1]); bitboard->Bits[1] &= bitboard->Bits[1] - 1; return result;}
    if (bitboard->Bits[0] != 0) {result = 320 + __builtin_ctzll(bitboard->Bits[0]); bitboard->Bits[0] &= bitboard->Bits[0] - 1; return result;}

    return -1;
}

static inline int BitScanForward(Int343 bitboard) {
    if (bitboard.Bits[5] != 0) return __builtin_ctzll(bitboard.Bits[5]);
    if (bitboard.Bits[4] != 0) return 64 + __builtin_ctzll(bitboard.Bits[4]);
    if (bitboard.Bits[3] != 0) return 128 + __builtin_ctzll(bitboard.Bits[3]);
    if (bitboard.Bits[2] != 0) return 192 + __builtin_ctzll(bitboard.Bits[2]);
    if (bitboard.Bits[1] != 0) return 256 + __builtin_ctzll(bitboard.Bits[1]);
    if (bitboard.Bits[0] != 0) return 320 + __builtin_ctzll(bitboard.Bits[0]);
    return -1;
}

static inline int BitScanReverse(Int343 bitboard) {
    if (bitboard.Bits[0] != 0) return 320 + (63 - __builtin_clzll(bitboard.Bits[0]));
    if (bitboard.Bits[1] != 0) return 256 + (63 - __builtin_clzll(bitboard.Bits[1]));
    if (bitboard.Bits[2] != 0) return 192 + (63 - __builtin_clzll(bitboard.Bits[2]));
    if (bitboard.Bits[3] != 0) return 128 + (63 - __builtin_clzll(bitboard.Bits[3]));
    if (bitboard.Bits[4] != 0) return 64 + (63 - __builtin_clzll(bitboard.Bits[4]));
    if (bitboard.Bits[5] != 0) return 63 - __builtin_clzll(bitboard.Bits[5]);
    return -1;
}

static inline Int343 GetFirstBit(Int343 bitboard) {
    Int343 result;
    if (bitboard.Bits[5] != 0) result.Bits[5] |= (bitboard.Bits[5] & -bitboard.Bits[5]); 
    else if (bitboard.Bits[4] != 0) result.Bits[4] |= (bitboard.Bits[4] & -bitboard.Bits[4]);
    else if (bitboard.Bits[3] != 0) result.Bits[3] |= (bitboard.Bits[3] & -bitboard.Bits[3]);
    else if (bitboard.Bits[2] != 0) result.Bits[2] |= (bitboard.Bits[2] & -bitboard.Bits[2]);
    else if (bitboard.Bits[1] != 0) result.Bits[1] |= (bitboard.Bits[1] & -bitboard.Bits[1]);
    else if (bitboard.Bits[0] != 0) result.Bits[0] |= (bitboard.Bits[0] & -bitboard.Bits[0]);
    return result;
}

#define BitScan(side, bitboard) (side == white ? BitScanReverse(bitboard) : BitScanForward(bitboard))