#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <cstdint>
#include <bit>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>

using namespace std;

//NOTE: GLOBAL = values that must be cleared or changed with a new turn or a new game.

#define START_POSITION  "0000000" \
                        "0cdodc0" \
                        "0dioid0" \
                        "0oosoo0" \
                        "0dioid0" \
                        "0cdodc0" \
                        "0000000" \
                                  \
                        "0000000" \
                        "0ttttt0" \
                        "0ttttt0" \
                        "0ttttt0" \
                        "0ttttt0" \
                        "0ttttt0" \
                        "0000000" \
                                  \
                        "0000000" \
                        "0000000" \
                        "0000000" \
                        "0000000" \
                        "0000000" \
                        "0000000" \
                        "0000000" \
                                  \
                        "0000000" \
                        "0000000" \
                        "0000000" \
                        "0000000" \
                        "0000000" \
                        "0000000" \
                        "0000000" \
                                  \
                        "0000000" \
                        "0000000" \
                        "0000000" \
                        "0000000" \
                        "0000000" \
                        "0000000" \
                        "0000000" \
                                  \
                        "0000000" \
                        "0TTTTT0" \
                        "0TTTTT0" \
                        "0TTTTT0" \
                        "0TTTTT0" \
                        "0TTTTT0" \
                        "0000000" \
                                  \
                        "0000000" \
                        "0CDODC0" \
                        "0DIOID0" \
                        "0OOSOO0" \
                        "0DIOID0" \
                        "0CDODC0" \
                        "0000000 w"

// unicode pieces
char *unicode_pieces[12] = {(char *)"♙", (char *)"♘", (char *)"♗", (char *)"♖", (char *)"♕", (char *)"♔",
                            (char *)"♟︎", (char *)"♞", (char *)"♝", (char *)"♜", (char *)"♛", (char *)"♚"};
// ASCII pieces
char ascii_pieces[] = "TDOCIStdocis";

// encode pieces
enum { T, D, O, C, I, S, t, d, o, c, i, s };
enum { white, black, both }; //Piece colors
enum {NO_MOVE, FOUND_MOVE};

#define INF 50000
#define MATE_VALUE 49000
#define DRAW_VALUE 0
#define CHECKMATE 100
#define STALEMATE 101
#define DRAW_GAME 102
#define NO_CAPTURE 6
#define NO_DIR 19
#define NO_MOVE 0

// convert ASCII character pieces to encoded constants
int char_pieces[] = {
    ['T'] = T,
    ['D'] = D,
    ['O'] = O,
    ['C'] = C,
    ['I'] = I,
    ['S'] = S,
    ['t'] = t,
    ['d'] = d,
    ['o'] = o,
    ['c'] = c,
    ['i'] = i,
    ['s'] = s
};

char XSymbols[] = "abcdefg";
char YSymbols[] = "1234567";
char ZSymbols[] = "ABCDEFG";

enum {
    NORTH,      //LSB
    SOUTH,      //MSB
    EAST,       //LSB
    WEST,       //MSB
    UP,         //LSB
    DOWN,       //MSB
    NORTH_EAST, //LSB
    SOUTH_EAST, //MSB
    NORTH_WEST, //LSB
    SOUTH_WEST, //MSB
    UP_NORTH,   //LSB
    UP_SOUTH,   //MSB
    UP_EAST,    //LSB
    DOWN_EAST,  //MSB
    UP_WEST,    //LSB
    DOWN_WEST,  //MSB
    DOWN_NORTH, //LSB
    DOWN_SOUTH  //MSB
};

//board blocks (z, y, x);
// blacks are in A, B depths;
// Whites start in F and G depths;
enum {
    A1a, A1b, A1c, A1d, A1e, A1f, A1g,
    A2a, A2b, A2c, A2d, A2e, A2f, A2g,
    A3a, A3b, A3c, A3d, A3e, A3f, A3g,
    A4a, A4b, A4c, A4d, A4e, A4f, A4g,
    A5a, A5b, A5c, A5d, A5e, A5f, A5g,
    A6a, A6b, A6c, A6d, A6e, A6f, A6g,
    A7a, A7b, A7c, A7d, A7e, A7f, A7g,

    B1a, B1b, B1c, B1d, B1e, B1f, B1g,
    B2a, B2b, B2c, B2d, B2e, B2f, B2g,
    B3a, B3b, B3c, B3d, B3e, B3f, B3g,
    B4a, B4b, B4c, B4d, B4e, B4f, B4g,
    B5a, B5b, B5c, B5d, B5e, B5f, B5g,
    B6a, B6b, B6c, B6d, B6e, B6f, B6g,
    B7a, B7b, B7c, B7d, B7e, B7f, B7g,

    C1a, C1b, C1c, C1d, C1e, C1f, C1g,
    C2a, C2b, C2c, C2d, C2e, C2f, C2g,
    C3a, C3b, C3c, C3d, C3e, C3f, C3g,
    C4a, C4b, C4c, C4d, C4e, C4f, C4g,
    C5a, C5b, C5c, C5d, C5e, C5f, C5g,
    C6a, C6b, C6c, C6d, C6e, C6f, C6g,
    C7a, C7b, C7c, C7d, C7e, C7f, C7g,

    D1a, D1b, D1c, D1d, D1e, D1f, D1g,
    D2a, D2b, D2c, D2d, D2e, D2f, D2g,
    D3a, D3b, D3c, D3d, D3e, D3f, D3g,
    D4a, D4b, D4c, D4d, D4e, D4f, D4g,
    D5a, D5b, D5c, D5d, D5e, D5f, D5g,
    D6a, D6b, D6c, D6d, D6e, D6f, D6g,
    D7a, D7b, D7c, D7d, D7e, D7f, D7g,

    E1a, E1b, E1c, E1d, E1e, E1f, E1g,
    E2a, E2b, E2c, E2d, E2e, E2f, E2g,
    E3a, E3b, E3c, E3d, E3e, E3f, E3g,
    E4a, E4b, E4c, E4d, E4e, E4f, E4g,
    E5a, E5b, E5c, E5d, E5e, E5f, E5g,
    E6a, E6b, E6c, E6d, E6e, E6f, E6g,
    E7a, E7b, E7c, E7d, E7e, E7f, E7g,

    F1a, F1b, F1c, F1d, F1e, F1f, F1g,
    F2a, F2b, F2c, F2d, F2e, F2f, F2g,
    F3a, F3b, F3c, F3d, F3e, F3f, F3g,
    F4a, F4b, F4c, F4d, F4e, F4f, F4g,
    F5a, F5b, F5c, F5d, F5e, F5f, F5g,
    F6a, F6b, F6c, F6d, F6e, F6f, F6g,
    F7a, F7b, F7c, F7d, F7e, F7f, F7g,

    G1a, G1b, G1c, G1d, G1e, G1f, G1g,
    G2a, G2b, G2c, G2d, G2e, G2f, G2g,
    G3a, G3b, G3c, G3d, G3e, G3f, G3g,
    G4a, G4b, G4c, G4d, G4e, G4f, G4g,
    G5a, G5b, G5c, G5d, G5e, G5f, G5g,
    G6a, G6b, G6c, G6d, G6e, G6f, G6g,
    G7a, G7b, G7c, G7d, G7e, G7f, G7g
};


/**********************************\
 ==================================
 
          Bit manipulations
 
 ==================================
\**********************************/
#define U64 unsigned long long

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

#define BitScan(side, bitboard) (side == white ? BitScanReverse(bitboard) : BitScanForward(bitboard))

#define MaxValue(a, b) (a >= b ? a : b)

/**********************************\
 ==================================
 
            Chess Board
 
 ==================================
\**********************************/

Int343 Bitboards[12]; //piece bitboards //GLOBAL
Int343 Occupancies[3]; //occupancy bitboards //GLOBAL
char SIDE = 0; //side to move //GLOBAL
int TURN = 0; //The current turn tracker //GLOBAL

#define MAX_DEPTH 20 //20
#define MAX_QDEPTH 24 //24
int DEPTH = MAX_DEPTH; //8
int QDEPTH = MAX_QDEPTH; //12

#define MAX_TURNS 1000 //Cap on number of turns

struct Turn {
    U64 key;
    long move;
};

Turn TurnList[MAX_TURNS]; 

static inline void AddToTurnList(Turn turn) {
    if (TURN < MAX_TURNS) {
        TurnList[TURN] = turn;
        TURN++;
    }
}
/**********************************\
 ==================================
 
           Input & Output
 
 ==================================
\**********************************/

void print_bitboard(Int343 bitboard) {
    printf("\n");

    for (int depth = 6; depth >= 0; depth--) {
        printf("%c", ZSymbols[depth]);

        for (int rank = 6; rank >= 0; rank--) {

            for (int file = 0; file < 7; file++) {

                int block = (depth * 49) + (rank * 7) + file;

                
                if (rank == 6 && !file)
                    printf(" %d", GetBit(bitboard, block) ? 1 : 0);
                else if (!file)
                    printf("  %d", GetBit(bitboard, block) ? 1 : 0);
                else
                    printf(" %d", GetBit(bitboard, block) ? 1 : 0);

                if (file == 6)
                    printf("  %d ", rank + 1);
            }
            printf("\n");
        }
        printf("\n");
    }

    printf("  a b c d e f g\n\n"); //bitboard files

    // print bitboard as unsigned decimal number
    printf("  Bitboard: ");
    bitboard.Print();

}

void print_board_enums() {
    printf("\n");
    for (int depth = 0; depth < 7; depth++) {
        for (int rank = 0; rank < 7; rank++) {
            for (int file = 0; file < 7; file++) {
                printf("%c%d%c, ", ZSymbols[depth], rank + 1, XSymbols[file]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void init_bitboards(char * fen) {
    // reset board position (bitboards)
    memset(Bitboards, 0ULL, sizeof(Bitboards));
    
    // reset occupancies (bitboards)
    memset(Occupancies, 0ULL, sizeof(Occupancies));
    
    // reset game state variables
    SIDE = 0;

    for (int depth = 0; depth < 7; depth++) {

        for (int rank = 0; rank < 7; rank++) {

            for (int file = 0; file < 7; file++) {
                int block = (depth * 49) + (rank * 7) + file;

                // match ascii pieces within FEN string
                if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z'))
                {
                    // init piece type
                    int piece = char_pieces[*fen];
                    
                    // set piece on corresponding bitboard
                    SetBit(Bitboards[piece], block);
                    
                    //printf("%d", piece);
                    // increment pointer to FEN string
                    
                }

                fen++;
            }
        }
    }

    fen++;

    SIDE = (*fen == 'w') ? white : black;

    //loop over white pieces to form occupancy boards
    for (int piece = T; piece <= S; piece++)
        Occupancies[white] |= Bitboards[piece];

    //loop over black pieces to form occupancy boards
    for (int piece = t; piece <= s; piece++)
        Occupancies[black] |= Bitboards[piece];

    Occupancies[both] |= Occupancies[white];
    Occupancies[both] |= Occupancies[black];
}

void print_full_board() {
    string piece = "0";
    int block = 0;
    bool piece_found = false;
    printf("\n");
    
    printf("Turn %d = %s\n", TURN, SIDE ? "Black" : "White");
    for (int depth = 6; depth >= 0; depth--) {
        printf("%c", ZSymbols[depth]);

        for (int rank = 6; rank >= 0; rank--) {

            for (int file = 0; file < 7; file++) {

                block = (depth * 49) + (rank * 7) + file;
                piece = ".";
                piece_found = false;
                for (int type = 0; type < 12 && !piece_found; type++) {
                    if (GetBit(Bitboards[type], block)) {
                        piece = unicode_pieces[type];
                        piece_found = true;
                    }
                }

                if (rank == 6 && !file)
                    printf(" %s", piece.c_str());
                else if (!file)
                    printf("  %s", piece.c_str());
                else
                    printf(" %s", piece.c_str());

                if (file == 6)
                    printf("  %d ", rank + 1);
            }
            printf("\n");
        }
        printf("\n");
    }

    printf("  a b c d e f g\n\n"); //bitboard files
}

void print_fen_string() {
    string piece = "0";
    for (int block = 0; block < 343; block++) {
        piece = "0";
        for (int type = 0; type < 12 && piece == "0"; type++) {
            if (GetBit(Bitboards[type], block)) {
                piece = ascii_pieces[type];
            }
        }

        printf("%s", piece.c_str());
    }

    printf(" %c", (SIDE) ? 'b' : 'w');
}

void load_bar() {
    cout << "|" ;
    fflush(stdout);
}

/**********************************\
 ==================================
 
           Random numbers
 
 ==================================
\**********************************/

 // pseudo random number state
unsigned int random_state = 1804289383;

// generate 32-bit pseudo legal numbers
unsigned int get_random_U32_number()
{
    // get current state
    unsigned int number = random_state;
    
    // XOR shift algorithm
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;
    
    // update random number state
    random_state = number;
    
    // return random number
    return number;
}

// generate 64-bit pseudo legal numbers
U64 get_random_U64_number()
{
    // define 4 random numbers
    U64 n1, n2, n3, n4;
    
    // init random numbers slicing 16 bits from MS1B side
    n1 = (U64)(get_random_U32_number()) & 0xFFFF;
    n2 = (U64)(get_random_U32_number()) & 0xFFFF;
    n3 = (U64)(get_random_U32_number()) & 0xFFFF;
    n4 = (U64)(get_random_U32_number()) & 0xFFFF;
    
    // return random number
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

/**********************************\
 ==================================
 
              Attacks
 
 ==================================
\**********************************/

Int343 not_1_file = {8388607ULL, 18446709164215369727ULL, 17302829768357443599ULL, 18446744073642967039ULL, 18446741891866165247ULL, 18375249429625044864ULL};
Int343 not_7_file = {65535ULL, 18446743800979128319ULL, 18437807243198988272ULL, 2305843009213173759ULL, 18446744056663900159ULL, 18446185521802641407ULL};
Int343 not_a_file = {8322555ULL, 17865744260364368887ULL, 17284744447019186159ULL, 16122744820328820703ULL, 13798745566948089791ULL, 9150747060186627966ULL};
Int343 not_g_file = {4161277ULL, 18156244167036960251ULL, 17865744260364368887ULL, 17284744447019186159ULL, 16122744820328820703ULL, 13798745566948089791ULL};
Int343 not_ab_file = {8190451ULL, 16703744633674003431ULL, 14960745193638455247ULL, 11474746313567358879ULL, 4502748553425166142ULL, 9005497106850332284ULL};
Int343 not_fg_file = {2047612ULL, 18010994213700664569ULL, 17575244353691777523ULL, 16703744633674003431ULL, 14960745193638455247ULL, 11474746313567358879ULL};
Int343 not_12_file = {8388607ULL, 18442240748960088056ULL, 9007199254478863ULL, 18446744065120141311ULL, 18446462615912710143ULL, 9223934986808180736ULL};
Int343 not_67_file = {511ULL, 18446743798848421887ULL, 18437737424210624496ULL, 18014398508957727ULL, 18446744056530731007ULL, 18446181158115868671ULL};

Int343 tetra_attacks[2][343]; // tetra attacks table [side][block]
Int343 tetra_moves[2][343]; // tetra forward move table [side][block]
Int343 dodeca_attacks[343]; //dodeca attacks table [block];
Int343 sphere_attacks[343]; // Sphere attacks table [square]

Int343 RAYS[343][18];
char octa_directions[343][12];
char cube_directions[343][6];
Int343 octa_edge_masks[343];
Int343 cube_edge_masks[343];
Int343 tetra_isolation_masks[49];


// Generate Tetra Attacks
Int343 build_tetra_attacks(char side, int block) {
    Int343 attacks; //result attacks board
    Int343 bitboard; // piece bitboard

    SetBit(bitboard, block);

    if (!side) {
        if (Count((bitboard >> 50) & not_g_file) > 0) attacks |= (bitboard >> 50); // left
        if (Count((bitboard >> 48) & not_a_file) > 0) attacks |= (bitboard >> 48); // right
        if (Count((bitboard >> 42) & not_1_file) > 0) attacks |= (bitboard >> 42); // up
        if (Count((bitboard >> 56) & not_7_file) > 0) attacks |= (bitboard >> 56); // down
    }
    else {
        if (Count((bitboard << 50) & not_a_file) > 0) attacks |= (bitboard << 50);
        if (Count((bitboard << 48) & not_g_file) > 0) attacks |= (bitboard << 48);
        if (Count((bitboard << 42) & not_7_file) > 0) attacks |= (bitboard << 42);
        if (Count((bitboard << 56) & not_1_file) > 0) attacks |= (bitboard << 56);
    }

    return attacks;
}

// Generate Tetra Attacks
Int343 build_tetra_moves(char side, int block) {
    Int343 moves; //result attacks board
    Int343 bitboard; // piece bitboard

    SetBit(bitboard, block);

    if (!side) {
        moves |= (bitboard >> 49);
    }
    else {
        moves |= (bitboard << 49);
    }

    return moves;
}

Int343 build_tetra_isolation_mask(int rf) {
    Int343 mask; Int343 bit;
    SetBit(bit, 0);
    for (int d = 0; d < 7; d++) {
        if ((rf % 7) > 0) mask |= (bit << ((d * 49) + rf - 1)); //Left File
        if ((rf % 7) < 6) mask |= (bit << ((d * 49) + rf + 1)); //Right file
        if (rf > 6) mask |= (bit << ((d * 49) + rf - 7)); //Below file
        if (rf < 42) mask |= (bit << ((d * 49) + rf + 7)); //Below file
    }
    return mask;
}


//>>97 forward left
//>>99 forward right
//>>105 forward down
//>>91 forward up
//>>13 down left
//>>15 down right
//>>63 down forward
//<35 down back
Int343 build_dodeca_attacks(int block) {
    Int343 attacks; //result attacks board
    Int343 bitboard; // piece bitboard

    SetBit(bitboard, block);

    if (Count((bitboard >> 97) & not_a_file) > 0) attacks |= (bitboard >> 97); //Forward
    if (Count((bitboard >> 99) & not_g_file) > 0) attacks |= (bitboard >> 99);
    if (Count((bitboard >> 105) & not_7_file) > 0) attacks |= (bitboard >> 105);
    if (Count((bitboard >> 91) & not_1_file) > 0) attacks |= (bitboard >> 91);

    if (Count((bitboard >> 13) & not_67_file & not_a_file) > 0) attacks |= (bitboard >> 13); //Down
    if (Count((bitboard >> 15) & not_67_file & not_g_file) > 0) attacks |= (bitboard >> 15);
    if (Count((bitboard >> 63) & not_67_file) > 0) attacks |= (bitboard >> 63);
    if (Count((bitboard << 35) & not_67_file) > 0) attacks |= (bitboard << 35);

    if (Count((bitboard << 13) & not_12_file & not_g_file) > 0) attacks |= (bitboard << 13); //Up
    if (Count((bitboard << 15) & not_12_file & not_a_file) > 0) attacks |= (bitboard << 15);
    if (Count((bitboard << 63) & not_12_file) > 0) attacks |= (bitboard << 63);
    if (Count((bitboard >> 35) & not_12_file) > 0) attacks |= (bitboard >> 35);

    if (Count((bitboard << 97 & not_g_file)) > 0) attacks |= (bitboard << 97); //Backward
    if (Count((bitboard << 99) & not_a_file) > 0) attacks |= (bitboard << 99);
    if (Count((bitboard << 105) & not_1_file) > 0) attacks |= (bitboard << 105);
    if (Count((bitboard << 91) & not_7_file) > 0) attacks |= (bitboard << 91);

    if (Count((bitboard >> 9) & not_fg_file & not_7_file) > 0) attacks |= (bitboard >> 9); //Left
    if (Count((bitboard << 5) & not_fg_file & not_1_file) > 0) attacks |= (bitboard << 5);
    if (Count((bitboard >> 51) & not_fg_file) > 0) attacks |= (bitboard >> 51);
    if (Count((bitboard << 47) & not_fg_file) > 0) attacks |= (bitboard << 47);

    if (Count((bitboard << 9) & not_ab_file & not_1_file) > 0) attacks |= (bitboard << 9); //Right
    if (Count((bitboard >> 5 & not_ab_file) & not_7_file) > 0) attacks |= (bitboard >> 5);
    if (Count((bitboard >> 47 & not_ab_file)) > 0) attacks |= (bitboard >> 47);
    if (Count((bitboard << 51) & not_ab_file) > 0) attacks |= (bitboard << 51);

    return attacks;
}

//TODO: Not used, just here for historical purposes
Int343 build_octa_attacks(int block, int is_mask) {
    Int343 attacks; //result attacks board
    Int343 bit; // piece bitboard
    
    SetBit(bit, 0);

    int d, r, f;
    int td = block / 49;
    int tf = block % 7;
    int tr = (block - (td * 49) - tf) / 7;

    int max = is_mask ? 5 : 6;
    int min = is_mask ? 1 : 0;

    for (d = td + 1, r = tr, f = tf + 1; d <= max && f <= max; d++, f++) attacks |= (bit << ((d * 49) + (r * 7) + f)); //NORTH EAST
    for (d = td + 1, r = tr, f = tf - 1; d <= max && f >= min; d++, f--) attacks |= (bit << ((d * 49) + (r * 7) + f)); //NORTH WEST
    for (d = td - 1, r = tr, f = tf - 1; d >= min && f >= min; d--, f--) attacks |= (bit << ((d * 49) + (r * 7) + f)); //SOUTH EAST
    for (d = td - 1, r = tr, f = tf + 1; d >= min && f <= max; d--, f++) attacks |= (bit << ((d * 49) + (r * 7) + f)); //SOUTH WEST
    for (d = td + 1, r = tr + 1, f = tf; d <= max && r <= max; d++, r++) attacks |= (bit << ((d * 49) + (r * 7) + f)); //UP NORTH
    for (d = td - 1, r = tr + 1, f = tf; d >= min && r <= max; d--, r++) attacks |= (bit << ((d * 49) + (r * 7) + f)); //UP SOUTH
    for (d = td, r = tr + 1, f = tf + 1; r <= max && f <= max; r++, f++) attacks |= (bit << ((d * 49) + (r * 7) + f)); //UP EAST
    for (d = td, r = tr + 1, f = tf - 1; r <= max && f >= min; r++, f--) attacks |= (bit << ((d * 49) + (r * 7) + f)); //UP WEST*/
    for (d = td + 1, r = tr - 1, f = tf; d <= max && r >= min; d++, r--) attacks |= (bit << ((d * 49) + (r * 7) + f));//DOWN NORTH
    for (d = td - 1, r = tr - 1, f = tf; d >= min && r >= min; d--, r--) attacks |= (bit << ((d * 49) + (r * 7) + f)); //DOWN SOUTH
    for (d = td, r = tr - 1, f = tf + 1; r >= min && f <= max; r--, f++) attacks |= (bit << ((d * 49) + (r * 7) + f)); //DOWN EAST
    for (d = td, r = tr - 1, f = tf - 1; r >= min && f >= min; r--, f--) attacks |= (bit << ((d * 49) + (r * 7) + f)); //DOWN WEST*/

    return attacks;
}

//TODO: Not used, just here for historical purposes
Int343 build_cube_attacks(int block, int is_mask) {
    Int343 attacks; //result attacks board
    Int343 bit; // piece bitboard

    SetBit(bit, 0);

    int d, r, f;
    int td = block / 49;
    int tf = block % 7;
    int tr = (block - (td * 49) - tf) / 7;

    int max = is_mask ? 5 : 6;
    int min = is_mask ? 1 : 0;

    for (d = td + 1, r = tr, f = tf; d <= max; d++) attacks |= (bit << ((d * 49) + (r * 7) + f)); //NORTH
    for (d = td - 1, r = tr, f = tf; d >= min; d--) attacks |= (bit << ((d * 49) + (r * 7) + f)); //SOUTH
    for (d = td, r = tr, f = tf + 1; f <= max; f++) attacks |= (bit << ((d * 49) + (r * 7) + f)); //EAST
    for (d = td, r = tr, f = tf - 1; f >= min; f--) attacks |= (bit << ((d * 49) + (r * 7) + f)); //WEST
    for (d = td, r = tr + 1, f = tf; r <= max; r++) attacks |= (bit << ((d * 49) + (r * 7) + f)); //UP
    for (d = td, r = tr - 1, f = tf; r >= min; r--) attacks |= (bit << ((d * 49) + (r * 7) + f)); //DOWN

    return attacks;
}

Int343 build_sphere_attacks(int block) {
    Int343 attacks; //result attacks board
    Int343 bitboard; // piece bitboard

    SetBit(bitboard, block);

    if (Count((bitboard >> 49)) > 0) attacks |= (bitboard >> 49); //Forward
    if (Count((bitboard >> 42) & not_1_file) > 0) attacks |= (bitboard >> 42);
    if (Count((bitboard >> 48) & not_a_file) > 0) attacks |= (bitboard >> 48);
    if (Count((bitboard >> 50) & not_g_file) > 0) attacks |= (bitboard >> 50);
    if (Count((bitboard >> 56) & not_7_file) > 0) attacks |= (bitboard >> 56);

    if (Count((bitboard << 49)) > 0) attacks |= (bitboard << 49); //Backward
    if (Count((bitboard << 42) & not_7_file) > 0) attacks |= (bitboard << 42);
    if (Count((bitboard << 48) & not_g_file) > 0) attacks |= (bitboard << 48);
    if (Count((bitboard << 50) & not_a_file) > 0) attacks |= (bitboard << 50);
    if (Count((bitboard << 56) & not_1_file) > 0) attacks |= (bitboard << 56);

    if (Count((bitboard << 6) & not_1_file & not_g_file) > 0) attacks |= (bitboard << 6); //Up
    if (Count((bitboard << 7) & not_1_file) > 0) attacks |= (bitboard << 7);
    if (Count((bitboard << 8) & not_1_file & not_a_file) > 0) attacks |= (bitboard << 8);

    if (Count((bitboard >> 6) & not_7_file & not_a_file) > 0) attacks |= (bitboard >> 6); //Down
    if (Count((bitboard >> 7) & not_7_file) > 0) attacks |= (bitboard >> 7);
    if (Count((bitboard >> 8) & not_7_file & not_g_file) > 0) attacks |= (bitboard >> 8);

    if (Count((bitboard >> 1) & not_g_file) > 0) attacks |= (bitboard >> 1); //Left
    if (Count((bitboard << 1) & not_a_file) > 0) attacks |= (bitboard << 1); //Right

    return attacks;
}

Int343 build_ray(int block, int direction) {
    Int343 attacks; //result attack
    Int343 bit; // piece bitboard

    SetBit(bit, 0);

    int d, r, f;
    int td = block / 49;
    int tf = block % 7;
    int tr = (block - (td * 49) - tf) / 7;

    int max = 6;
    int min = 0;

    if (direction == NORTH) for (d = td + 1, r = tr, f = tf; d <= max; d++) attacks |= (bit << ((d * 49) + (r * 7) + f)); //NORTH
    else if (direction == SOUTH) for (d = td - 1, r = tr, f = tf; d >= min; d--) attacks |= (bit << ((d * 49) + (r * 7) + f)); //SOUTH
    else if (direction == EAST) for (d = td, r = tr, f = tf + 1; f <= max; f++) attacks |= (bit << ((d * 49) + (r * 7) + f)); //EAST
    else if (direction == WEST) for (d = td, r = tr, f = tf - 1; f >= min; f--) attacks |= (bit << ((d * 49) + (r * 7) + f)); //WEST
    else if (direction == UP) for (d = td, r = tr + 1, f = tf; r <= max; r++) attacks |= (bit << ((d * 49) + (r * 7) + f)); //UP
    else if (direction == DOWN) for (d = td, r = tr - 1, f = tf; r >= min; r--) attacks |= (bit << ((d * 49) + (r * 7) + f)); //DOWN
    else if (direction == NORTH_EAST) for (d = td + 1, r = tr, f = tf + 1; d <= max && f <= max; d++, f++) attacks |= (bit << ((d * 49) + (r * 7) + f)); //NORTH EAST
    else if (direction == NORTH_WEST) for (d = td + 1, r = tr, f = tf - 1; d <= max && f >= min; d++, f--) attacks |= (bit << ((d * 49) + (r * 7) + f)); //NORTH WEST
    else if (direction == SOUTH_EAST) for (d = td - 1, r = tr, f = tf - 1; d >= min && f >= min; d--, f--) attacks |= (bit << ((d * 49) + (r * 7) + f)); //SOUTH EAST
    else if (direction == SOUTH_WEST) for (d = td - 1, r = tr, f = tf + 1; d >= min && f <= max; d--, f++) attacks |= (bit << ((d * 49) + (r * 7) + f)); //SOUTH WEST
    else if (direction == UP_NORTH) for (d = td + 1, r = tr + 1, f = tf; d <= max && r <= max; d++, r++) attacks |= (bit << ((d * 49) + (r * 7) + f)); //UP NORTH
    else if (direction == UP_SOUTH) for (d = td - 1, r = tr + 1, f = tf; d >= min && r <= max; d--, r++) attacks |= (bit << ((d * 49) + (r * 7) + f)); //UP SOUTH
    else if (direction == UP_EAST) for (d = td, r = tr + 1, f = tf + 1; r <= max && f <= max; r++, f++) attacks |= (bit << ((d * 49) + (r * 7) + f)); //UP EAST
    else if (direction == UP_WEST) for (d = td, r = tr + 1, f = tf - 1; r <= max && f >= min; r++, f--) attacks |= (bit << ((d * 49) + (r * 7) + f)); //UP WEST*/
    else if (direction == DOWN_NORTH) for (d = td + 1, r = tr - 1, f = tf; d <= max && r >= min; d++, r--) attacks |= (bit << ((d * 49) + (r * 7) + f));//DOWN NORTH
    else if (direction == DOWN_SOUTH) for (d = td - 1, r = tr - 1, f = tf; d >= min && r >= min; d--, r--) attacks |= (bit << ((d * 49) + (r * 7) + f)); //DOWN SOUTH
    else if (direction == DOWN_EAST) for (d = td, r = tr - 1, f = tf + 1; r >= min && f <= max; r--, f++) attacks |= (bit << ((d * 49) + (r * 7) + f)); //DOWN EAST
    else if (direction == DOWN_WEST) for (d = td, r = tr - 1, f = tf - 1; r >= min && f >= min; r--, f--) attacks |= (bit << ((d * 49) + (r * 7) + f)); //DOWN WEST*/

    return attacks;

}

void build_directions_and_edge_mask(int block) {
    Int343 cube_mask;
    Int343 octa_mask;
    Int343 ray;

    //Set up Cube directions and mask
    int i = 0;
    for (int dir = 0; dir < 6; dir++) {
        ray = RAYS[block][dir];
        if (IsSet(ray)) {
            if (Count(ray) > 1)
                cube_directions[block][i++] = dir;
            else
                cube_mask |= ray;
        }
    }

    if (i < 6)
        cube_directions[block][i] = NO_DIR;

    cube_edge_masks[block] = cube_mask;

    //Set up Octa directions and mask
    i = 0;
    for (int dir = 6; dir < 18; dir++) {
        ray = RAYS[block][dir];
        if (IsSet(ray)) {
            if (Count(ray) > 1)
                octa_directions[block][i++] = dir;
            else
                octa_mask |= ray;
        }
    }

    if (i < 12)
        octa_directions[block][i] = NO_DIR;

    octa_edge_masks[block] = octa_mask;
}

//Helper function for printing files. Not used at in runtime code.
Int343 SetNotXFile() {
    Int343 result;
    for (int depth = 0; depth < 7; depth++) {

        for (int rank = 0; rank < 7; rank++) {

            for (int file = 0; file < 7; file++) {
                int block = (depth * 49) + (rank * 7) + file;

                if ((block % 49) > 13)
                    SetBit(result, block);
            }
        }
    }

    return result;
}

//Initializes all attacks
void init_attacks() {
    for (int block = 0; block < 343; block++) {
        tetra_attacks[white][block] = build_tetra_attacks(white, block);
        tetra_attacks[black][block] = build_tetra_attacks(black, block);
        tetra_moves[white][block] = build_tetra_moves(white, block);
        tetra_moves[black][block] = build_tetra_moves(black, block);
        dodeca_attacks[block] = build_dodeca_attacks(block);
        sphere_attacks[block] = build_sphere_attacks(block);
    }
}

void init_tetra_isolation_masks() {
    for (int rf = 0; rf < 49; rf++) {
        tetra_isolation_masks[rf] = build_tetra_isolation_mask(rf);
    }
}

void init_rays_directions_edges() {
    for (int block = 0; block < 343; block++) {
        for (int dir = 0; dir < 18; dir++) {
            RAYS[block][dir] = build_ray(block, dir);
        }

        build_directions_and_edge_mask(block);
    }
}

/**********************************\
 ==================================
 
           Move generator
 
 ==================================
\**********************************/

#define MAX_MOVE_COUNT_SIZE 400

struct Move {
    long encoding;
    int score;

    void Clear() {
        encoding = 0;
        score = 0;
    }
};

struct MoveStack {
    int count = 0;
    long move = 0;
    int static_eval = 0;
    Move moves[MAX_MOVE_COUNT_SIZE];

    void Add(Move move) {
        if (count >= MAX_MOVE_COUNT_SIZE) return;

        int i;
        for (i = count-1; (i >= 0 && moves[i].score < move.score); i--)
            moves[i+1] = moves[i];

        moves[i+1] = move;
        count++;
    }

    void Clear() {
        count = 0;
        move = 0;
        static_eval = 0;
    }
};

struct SearchResult {
    long final_move = 0;
    U64 final_hash_key = 0;
    int final_score = 0;
    char thread_id = 0;
    char flag = 0;
    int search_depth = 0;
    
    void Clear() {
        final_move = 0;
        final_hash_key = 0;
        final_score = 0;
        thread_id = 0;
        flag = NO_MOVE;
        search_depth = 0;
    }
};

 struct Global {
    Int343 bitboards[12];
    Int343 occupancies[3];
    long nodes_searched = 0;
    long killer_moves[2][MAX_DEPTH];
    int history_moves[12][343]; //history moves [piece][square]
    MoveStack mstack[MAX_DEPTH];
    MoveStack qstack[MAX_QDEPTH];
    SearchResult search_result;
    int pv_length[MAX_DEPTH]; // PV length [ply]
    long pv_table[MAX_DEPTH][MAX_DEPTH]; // PV table [ply][ply]
    int total_depth = 0;
    bool IsEndGame = false;

    void CleanAndBuild(int id, bool EndGame) {
        for (int i = 0; i < 12; i++) bitboards[i] = Bitboards[i];
        for (int i = 0; i < 3; i++) occupancies[i] = Occupancies[i];
        for (int depth = 0; depth < DEPTH; depth++) mstack[depth].Clear();
        for (int depth = 0; depth < QDEPTH; depth++) qstack[depth].Clear();
        nodes_searched = 0;
        memset(killer_moves, 0, sizeof(killer_moves));
        memset(history_moves, 0, sizeof(history_moves));
        memset(pv_table, 0, sizeof(pv_table));
        memset(pv_length, 0, sizeof(pv_length));
        total_depth = 0;
        IsEndGame = EndGame;
        search_result.Clear();
        search_result.thread_id = id;
    }
};

#define MAX_THREADS 8 //8
size_t THREAD_COUNT = MAX_THREADS;

Global globals[MAX_THREADS]; //GLOBAL

// encode move
#define encode_move(source, target, piece, capture, promotion, side) \
    (source) |          \
    (target << 9) |     \
    (piece << 18) |     \
    (capture << 21) |   \
    (promotion << 24) |  \
    (side << 27)        \
    
// extract source block
//00000000000000000000000111111111
#define get_move_source(move) (move & 0x1FF)

// extract target block
//00000000000000111111111000000000
#define get_move_target(move) ((move & 0x3FE00) >> 9)

// extract piece bits
//00000000000111000000000000000000
#define get_move_piece(move) ((move & 0x1C0000) >> 18)

// extract capture bits
//00000000111000000000000000000000
#define get_move_capture(move) ((move & 0xE00000) >> 21)

// extract promotion flag
//00000111000000000000000000000000
#define get_move_promotion(move) ((move & 0x7000000) >> 24)

// extract side bit
//00001000000000000000000000000000
#define get_move_side(move) ((move & 0x8000000) >> 27)


#define GetValidTetraMoves(id, side, block) ((tetra_attacks[side][block] & globals[id].occupancies[!side]) | (tetra_moves[side][block] ^ (tetra_moves[side][block] & globals[id].occupancies[both])))
#define GetValidDodecaMoves(id, side, block) (dodeca_attacks[block] ^ (dodeca_attacks[block] & globals[id].occupancies[side]))
#define GetValidSphereMoves(id, side, block) (sphere_attacks[block] ^ (sphere_attacks[block] & globals[id].occupancies[side]))
#define EncodeNullMove(side) (encode_move(0, 0, 0, 0, 0, side))
#define IsNullMove(move) ((move << 5) == 0)
/*
* Process:
* 1.) attacks = octa/cube edges if they exist
* 2.) LOOP through octa/cube directions, ORing using the RAY classical method
* 3.) Return attacks
*/
inline Int343 GetOctaMoves(char id, int block) {
    Int343 attacks = octa_edge_masks[block];

    Int343 ray;
    char dir;
    for (int i = 0; i < 12 && (dir = octa_directions[block][i]) != NO_DIR; i++) {
        attacks |= RAYS[block][dir]; ray = RAYS[block][dir] & globals[id].occupancies[both];
        if (IsSet(ray)) attacks &= (dir % 2) ? ~RAYS[BitScanReverse(ray)][dir] : ~RAYS[BitScanForward(ray)][dir];
    }

    return attacks;
}

inline Int343 GetCubeMoves(char id, int block) {
    Int343 attacks = cube_edge_masks[block];

    Int343 ray;
    char dir;
    for (int i = 0; i < 6 && (dir = cube_directions[block][i]) != NO_DIR; i++) {
        attacks |= RAYS[block][dir]; ray = RAYS[block][dir] & globals[id].occupancies[both];
        if (IsSet(ray)) attacks &= (dir % 2) ? ~RAYS[BitScanReverse(ray)][dir] : ~RAYS[BitScanForward(ray)][dir];
    }

    return attacks;
}

#define GetTetraCaptures(id, side, block) (tetra_attacks[side][block] & globals[id].occupancies[!side])
#define GetDodecaCaptures(id, side, block) (dodeca_attacks[block] & globals[id].occupancies[!side])
#define GetSphereCaptures(id, side, block) (sphere_attacks[block] & globals[id].occupancies[!side])

inline Int343 GetPossibleTetraMoves(char side, int block) {
    return tetra_attacks[side][block] | tetra_moves[side][block];
}

inline Int343 GetPossibleDodecaMoves(int block) {
    return dodeca_attacks[block];
}

inline Int343 GetPossibleSphereMoves(int block) {
    return sphere_attacks[block];
}

inline Int343 GetPossibleOctaMoves(int block) {
    Int343 attacks;
    for (int dir = 6; dir < 18; dir++) attacks |= RAYS[block][dir];
    return attacks;
}

inline Int343 GetPossibleCubeMoves(int block) {
    Int343 attacks;
    for (int dir = 0; dir < 6; dir++) attacks |= RAYS[block][dir];
    return attacks;
}

#define IsPromotion(side, block) ((side == white && (block < 49)) || (side == black && (block >= 294)))
#define IsCapture(capture) (capture != NO_CAPTURE)

static inline int GetCapture(char id, char side, int target) {
    if (GetBit(globals[id].bitboards[((!side)*6) + T], target))
        return T;
    else if (GetBit(globals[id].bitboards[((!side)*6) + D], target))
        return D;
    else if (GetBit(globals[id].bitboards[((!side)*6) + O], target))
        return O;
    else if (GetBit(globals[id].bitboards[((!side)*6) + C], target))
        return C;
    else if (GetBit(globals[id].bitboards[((!side)*6) + I], target))
        return I;
    else if (GetBit(globals[id].bitboards[((!side)*6) + S], target))
        return S;

    return NO_CAPTURE;
}

//side = the attacking side
inline bool is_block_attacked(char id, int block, char side) {
    Int343 octa_moves = GetOctaMoves(id, block);
    Int343 check = octa_moves & ((side == white) ? globals[id].bitboards[O] : globals[id].bitboards[o]);
    if (IsSet(check)) return true;

    // attacked by Cube
    Int343 cube_moves = GetCubeMoves(id, block);
    check = cube_moves & ((side == white) ? globals[id].bitboards[C] : globals[id].bitboards[c]);
    if (IsSet(check)) return true;

    // attacked by Icosa
    check = (cube_moves | octa_moves) & ((side == white) ? globals[id].bitboards[I] : globals[id].bitboards[i]);
    if (IsSet(check)) return true;

    // attacked by dodecas
    check = dodeca_attacks[block] & ((side == white) ? globals[id].bitboards[D] : globals[id].bitboards[d]);
    if (IsSet(check)) return true;

    // attacked by tetras
    check = (side == white) ? (tetra_attacks[black][block] & globals[id].bitboards[T]) 
                                   : (tetra_attacks[white][block] & globals[id].bitboards[t]);
    if (IsSet(check)) return true;

    // attacked by Sphere
    check = sphere_attacks[block] & ((side == white) ? globals[id].bitboards[S] : globals[id].bitboards[s]);
    if (IsSet(check)) return true;

    return false;
}

#define IsInCheck(id, side, type, target, sphere_source)  ((type != S) ? is_block_attacked(id, sphere_source, !side) : is_block_attacked(id, target, !side))
#define GetSphereSource(id, side) ((side == white) ? BitScan(side, globals[id].bitboards[S]) : BitScan(side, globals[id].bitboards[s]))

void print_move(long move) {
    char type = get_move_piece(move);
    int source = get_move_source(move);
    int target = get_move_target(move);
    printf("%c-%d-%d", ascii_pieces[type], source, target);
}

static inline void HandlePVMove(char id, int ply, long move) {
    if (ply >= DEPTH - 1) return; //This check prevents a bus error 10
    // write PV move
    globals[id].pv_table[ply][ply] = move;
    
    // loop over the next ply
    for (int next_ply = ply + 1; next_ply < globals[id].pv_length[ply + 1]; ++next_ply)
        // copy move from deeper ply into a current ply's line
        globals[id].pv_table[ply][next_ply] = globals[id].pv_table[ply + 1][next_ply];
    
    // adjust PV length
    globals[id].pv_length[ply] = globals[id].pv_length[ply + 1];
}

/**********************************\
 ==================================
 
        Transposition Table
 
 ==================================
\**********************************/
//#define HASH_ALPHA 60000
//#define HASH_BETA 70000
 enum {HASH_ALPHA, HASH_BETA, HASH_EXACT};
#define NO_HASH 80000

U64 piece_keys[12][343]; //GLOBAL

struct TTEntry {
    U64 key;
    uint16_t turn;
    uint8_t depth;
    int score;
    char flag;
};

// hash table size
#define HASH_SIZE 0x800000 //0x800000
TTEntry TransTable[HASH_SIZE][2];

// init random hash keys
void init_random_keys()
{
    // update pseudo random number state
    random_state = 1804289383;

    // loop over piece codes
    for (int piece = T; piece <= s; piece++)
    {
        // loop over board squares
        for (int block = 0; block < 343; block++)
            // init random piece keys
            piece_keys[piece][block] = get_random_U64_number();
    }
}

U64 BuildTranspositionKey() {
    U64 key = 0;
    Int343 bitboard;
    int block;
    for (int piece = T; piece <= s; piece++) {
        bitboard = Bitboards[piece];
        while((block = ForwardScanPop(&bitboard)) >= 0) {
            key ^= piece_keys[piece][block];
        }
    }

    return key;
}

static inline U64 MakeTTKey(U64 initial_key, long move) {
    if (move == NO_MOVE) return initial_key;

    char side = get_move_side(move);
    U64 trans_key = initial_key ^ side;
    if (IsNullMove(move)) return trans_key;

    char exact_type = (side * 6) + get_move_piece(move);
    char capture = get_move_capture(move);
    int source = get_move_source(move);
    int target = get_move_target(move);

    trans_key = trans_key ^ (piece_keys[exact_type][source]) ^ (piece_keys[exact_type][target]);
    return (IsCapture(capture)) ? trans_key ^ (piece_keys[(side * 6) + capture][target]) : trans_key;
}

static inline TTEntry* GetTTEntry(U64 key) {
    TTEntry *tt_entry = &TransTable[key % HASH_SIZE][0];
    if (tt_entry->turn == TURN && tt_entry->key != key) tt_entry = &TransTable[key % HASH_SIZE][1];
    return tt_entry;
}

//hash_key = prev_key ^ (piece_key + dst);
void ClearTranspositionTable() {
    for (int i = 0; i < HASH_SIZE; i++) {
        TransTable[i][0] = {0, 0, 0, 0, 0};
        TransTable[i][1] = {0, 0, 0, 0, 0};
    }
}

//We add by one so the TT is checked as not emptry, i.e. non-zero
#define TT_TURN() (TURN+1)
#define IsSameTurn(tt_entry) (tt_entry->turn == TT_TURN())

static inline void WriteToHashTable(U64 hash_key, int ply_depth, int score, int alpha, int beta) {
    //Replace tt entry if its an untouched entry or if the current depth is greater than whats in the entry
    TTEntry * tt_entry = GetTTEntry(hash_key);
    if (!IsSameTurn(tt_entry)) {
        tt_entry->key = hash_key;
        tt_entry->turn = TT_TURN(); //Add by one so its stored as non-zero, hence not empty
        tt_entry->depth = ply_depth;

        if (score >= beta) {
            tt_entry->score = beta;
            tt_entry->flag = HASH_BETA;
        }
        else if (score > alpha) {
            tt_entry->score = score;
            tt_entry->flag = HASH_EXACT;
        }
        else {
            tt_entry->score = alpha;
            tt_entry->score = HASH_ALPHA;
        }
    }
}

#define GetTableValue(tt_entry, hash_key) ((tt_entry->key == hash_key && IsSameTurn(tt_entry)) ? tt_entry->score : NO_HASH)

/**********************************\
 ==================================
 
             Evaluation
 
 ==================================
\**********************************/
//const int material_score[12] = { 100, 320, 325, 500, 975, 32767, -100, -320, -325, -500, -975, -32767};
//const int material_score[12] = { 100, 305, 350, 548, 994, 32767, -100, -305, -350, -548, -994, -32767};
const int material_score[12] = { 100, 320, 325, 500, 1000, 32767, -100, -320, -325, -500, -1000, -32767};

//Positional score board
const int position_scores[6][343] =
{{  // Tetra position scores
    90,  90,  90,  90,  90,  90,  90,
    90,  90,  90,  90,  90,  90,  90,
    90,  90,  90,  90,  90,  90,  90,
    90,  90,  90,  90,  90,  90,  90,
    90,  90,  90,  90,  90,  90,  90,
    90,  90,  90,  90,  90,  90,  90,
    90,  90,  90,  90,  90,  90,  90,

    30,  30,  30,  30,  30,  30,  30,
    30,  40,  40,  40,  40,  40,  30,
    30,  40,  50,  50,  50,  40,  30,
    30,  40,  50,  50,  50,  40,  30,
    30,  40,  50,  50,  50,  40,  30,
    30,  40,  40,  40,  40,  40,  30,
    30,  40,  40,  40,  40,  40,  30,

    20,  20,  20,  20,  20,  20,  20,
    20,  30,  30,  30,  30,  30,  20,
    20,  30,  30,  30,  30,  30,  20,
    20,  30,  30,  30,  30,  30,  20,
    20,  30,  30,  30,  30,  30,  20,
    20,  30,  30,  30,  30,  30,  20,
    20,  20,  20,  20,  20,  20,  20,

    10,  10,  10,  10,  10,  10,  10,
    10,  20,  20,  20,  20,  20,  10,
    10,  20,  20,  20,  20,  20,  10,
    10,  20,  20,  20,  20,  20,  10,
    10,  20,  20,  20,  20,  20,  10,
    10,  20,  20,  20,  20,  20,  10,
    10,  10,  10,  10,  10,  10,  10,

    10,  10,  10,  10,  10,  10,  10,
    10,  15,  15,  15,  15,  15,  10,
    10,  15,  15,  15,  15,  15,  10,
    10,  15,  15,  15,  15,  15,  10,
    10,  15,  15,  15,  15,  15,  10,
    10,  15,  15,  15,  15,  15,  10,
    10,  10,  10,  10,  10,  10,  10,

     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0, -10,   0, -10,   0,   0,
     0,   0,   0, -10,   0,   0,   0,
     0,   0, -10,   0, -10,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,

     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0
}, { // Dodeca position scores
    -5,  -5,  -5,  -5,  -5,  -5,  -5,
    -5,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,  -5,
    -5,  -5,  -5,  -5,  -5,  -5,  -5,

    -5,  -5,  -5,  -5,  -5,  -5,  -5,
    -5,   0,   0,   0,   0,   0,  -5,
    -5,   0,  10,  10,  10,   0,  -5,
    -5,   0,  10,  10,  10,   0,  -5,
    -5,   0,  10,  10,  10,   0,  -5,
    -5,   0,   0,   0,   0,   0,  -5,
    -5,  -5,  -5,  -5,  -5,  -5,  -5,

    -5,  -5,  -5,  -5,  -5,  -5,  -5,
    -5,   5,   5,   5,   5,   5,  -5,
    -5,   5,  10,  10,  10,   5,  -5,
    -5,   5,  10,  10,  10,   5,  -5,
    -5,   5,  10,  10,  10,   5,  -5,
    -5,   5,   5,   5,   5,   5,  -5,
    -5,  -5,  -5,  -5,  -5,  -5,  -5,

    -5,  -5,  -5,  -5,  -5,  -5,  -5,
    -5,   5,  10,  10,  10,   5,  -5,
    -5,  10,  10,  10,  10,  10,  -5,
    -5,  10,  10,  10,  10,  10,  -5,
    -5,  10,  10,  10,  10,  10,  -5,
    -5,   5,  10,  10,  10,   5,  -5,
    -5,  -5,  -5,  -5,  -5,  -5,  -5,

    -5,  -5,  -5,  -5,  -5,  -5,  -5,
    -5,   5,   5,   5,   5,   5,  -5,
    -5,   5,   5,   5,   5,   5,  -5,
    -5,   5,   5,   5,   5,   5,  -5,
    -5,   5,   5,   5,   5,   5,  -5,
    -5,   5,   5,   5,   5,   5,  -5,
    -5,  -5,  -5,  -5,  -5,  -5,  -5,

    -5,  -5,  -5,  -5,  -5,  -5,  -5,
    -5,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,  -5,
    -5,  -5,  -5,  -5,  -5,  -5,  -5,

    -5,  -5,  -5,  -5,  -5,  -5,  -5,
    -5,   0, -10,   0, -10,   0,  -5,
    -5, -10,   0,   0,   0, -10,  -5,
    -5,   0,   0,   0,   0,   0,  -5,
    -5, -10,   0,   0,   0, -10,  -5,
    -5,   0, -10,   0, -10,   0,  -5,
    -5,  -5,  -5,  -5,  -5,  -5,  -5
}, { // Octa Position Scores
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,

     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,

     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,  10,  10,  10,   0,   0,
     0,   0,  10,  10,  10,   0,   0,
     0,   0,  10,  10,  10,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,

     0,   0,   0,   0,   0,   0,   0,
     0,   5,   5,   5,   5,   5,   0,
     0,   5,  10,  10,  10,   5,   0,
     0,   5,  10,  10,  10,   5,   0,
     0,   5,  10,  10,  10,   5,   0,
     0,   5,   5,   5,   5,   5,   0,
     0,   0,   0,   0,   0,   0,   0,

     0,   0,   0,   0,   0,   0,   0,
     0,   5,   5,   5,   5,   5,   0,
     0,   5,   0,   0,   0,   5,   0,
     0,   5,   0,   0,   0,   5,   0,
     0,   5,   0,   0,   0,   5,   0,
     0,   5,   5,   5,   5,   5,   0,
     0,   0,   0,   0,   0,   0,   0,

     0,   0,   0,  10,   0,   0,   0,
     0,   0,   0,  10,   0,   0,   0,
     0,  10,  10,   0,  10,  10,   0,
     10,   0,   0,   0,   0,   0,  10,
     0,  10,  10,   0,  10,  10,   0,
     0,   0,   0,  10,   0,   0,   0,
     0,   0,   0,  10,   0,   0,   0,

     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0
}, { // Cube Position Scores
    20,  20,  20,  20,  20,  20,  20,
    20,  20,  20,  20,  20,  20,  20,
    20,  20,  20,  20,  20,  20,  20,
    20,  20,  20,  20,  20,  20,  20,
    20,  20,  20,  20,  20,  20,  20,
    20,  20,  20,  20,  20,  20,  20,
    20,  20,  20,  20,  20,  20,  20,

    20,  20,  20,  20,  20,  20,  20,
    20,  20,  20,  20,  20,  20,  20,
    20,  20,  20,  20,  20,  20,  20,
    20,  20,  20,  20,  20,  20,  20,
    20,  20,  20,  20,  20,  20,  20,
    20,  20,  20,  20,  20,  20,  20,
    20,  20,  20,  20,  20,  20,  20,

     0,   0,   0,   0,   0,   0,   0,
     0,  10,  10,  10,  10,  10,   0,
     0,  10,  20,  20,  20,  10,   0,
     0,  10,  20,  20,  20,  10,   0,
     0,  10,  20,  20,  20,  10,   0,
     0,  10,  10,  10,  10,  10,   0,
     0,   0,   0,   0,   0,   0,   0,

     3,   5,   5,  10,   5,   5,   3,
     5,  10,  10,  10,  10,  10,   5,
     5,  10,  20,  20,  20,  10,   5,
    10,  10,  20,  20,  20,  10,  10,
     5,  10,  20,  20,  20,  10,   5,
     5,  10,  10,  10,  10,  10,   5,
     3,   5,   5,  10,   5,   5,   3,

     0,   0,   0,   0,   0,   0,   0,
     0,  10,  10,  10,  10,  10,   0,
     0,  10,  20,  20,  20,  10,   0,
     0,  10,  20,  20,  20,  10,   0,
     0,  10,  20,  20,  20,  10,   0,
     0,  10,  10,  10,  10,  10,   0,
     0,   0,   0,   0,   0,   0,   0,

     0,   0,   0,   0,   0,   0,   0,
     0,  10,  10,  10,  10,  10,   0,
     0,  10,  20,  20,  20,  10,   0,
     0,  10,  20,  20,  20,  10,   0,
     0,  10,  20,  20,  20,  10,   0,
     0,  10,  10,  10,  10,  10,   0,
     0,   0,   0,   0,   0,   0,   0,

     0,   5,   0,   0,   0,   5,   0,
     5,  -5,   0,   0,   0,  -5,   5,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     5,  -5,   0,   0,   0,  -5,   5,
     0,   5,   0,   0,   0,   5,   0,
}, { // Icosa Position Scores
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,

     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,

     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,

     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,

     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,

     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,

     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0
}, { // Sphere position scores
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,

     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   5,   5,   5,   0,   0,
     0,   0,   5,   5,   5,   0,   0,
     0,   0,   5,   5,   5,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,

     0,   0,   0,   0,   0,   0,   0,
     0,   5,   5,   5,   5,   5,   0,
     0,   5,   5,   5,   5,   5,   0,
     0,   5,   5,  10,   5,   5,   0,
     0,   5,   5,   5,   5,   5,   0,
     0,   5,   5,   5,   5,   5,   0,
     0,   0,   0,   0,   0,   0,   0,

     0,   0,   0,   0,   0,   0,   0,
     0,   5,   5,   5,   5,   5,   0,
     0,   5,  10,  10,  10,   5,   0,
     0,   5,  10,  20,  10,   5,   0,
     0,   5,  10,  10,  10,   5,   0,
     0,   5,   5,   5,   5,   5,   0,
     0,   0,   0,   0,   0,   0,   0,

     0,   0,   0,   0,   0,   0,   0,
     0,   5,   5,   5,   5,   5,   0,
     0,   5,   5,   5,   5,   5,   0,
     0,   5,   5,  10,   5,   5,   0,
     0,   5,   5,   5,   5,   5,   0,
     0,   5,   5,   5,   5,   5,   0,
     0,   0,   0,   0,   0,   0,   0,

     0,   0,   0,   0,   0,   0,   0,
     0,   5,   5,   5,   5,   5,   0,
     0,   5,  -5,  -5,  -5,   5,   0,
     0,   5,  -5,  -5,  -5,   5,   0,
     0,   5,  -5,  -5,  -5,   5,   0,
     0,   5,   5,   5,   5,   5,   0,
     0,   0,   0,   0,   0,   0,   0,

     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0, -15,   0, -15,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0, -15,   0, -15,   0,   0,
     0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0
}};
// mirror positional score tables for opposite side
const int mirror_score[343] =
{
    G1a, G1b, G1c, G1d, G1e, G1f, G1g,
    G2a, G2b, G2c, G2d, G2e, G2f, G2g,
    G3a, G3b, G3c, G3d, G3e, G3f, G3g,
    G4a, G4b, G4c, G4d, G4e, G4f, G4g,
    G5a, G5b, G5c, G5d, G5e, G5f, G5g,
    G6a, G6b, G6c, G6d, G6e, G6f, G6g,
    G7a, G7b, G7c, G7d, G7e, G7f, G7g,

    F1a, F1b, F1c, F1d, F1e, F1f, F1g,
    F2a, F2b, F2c, F2d, F2e, F2f, F2g,
    F3a, F3b, F3c, F3d, F3e, F3f, F3g,
    F4a, F4b, F4c, F4d, F4e, F4f, F4g,
    F5a, F5b, F5c, F5d, F5e, F5f, F5g,
    F6a, F6b, F6c, F6d, F6e, F6f, F6g,
    F7a, F7b, F7c, F7d, F7e, F7f, F7g,

    E1a, E1b, E1c, E1d, E1e, E1f, E1g,
    E2a, E2b, E2c, E2d, E2e, E2f, E2g,
    E3a, E3b, E3c, E3d, E3e, E3f, E3g,
    E4a, E4b, E4c, E4d, E4e, E4f, E4g,
    E5a, E5b, E5c, E5d, E5e, E5f, E5g,
    E6a, E6b, E6c, E6d, E6e, E6f, E6g,
    E7a, E7b, E7c, E7d, E7e, E7f, E7g,

    D1a, D1b, D1c, D1d, D1e, D1f, D1g,
    D2a, D2b, D2c, D2d, D2e, D2f, D2g,
    D3a, D3b, D3c, D3d, D3e, D3f, D3g,
    D4a, D4b, D4c, D4d, D4e, D4f, D4g,
    D5a, D5b, D5c, D5d, D5e, D5f, D5g,
    D6a, D6b, D6c, D6d, D6e, D6f, D6g,
    D7a, D7b, D7c, D7d, D7e, D7f, D7g,

    C1a, C1b, C1c, C1d, C1e, C1f, C1g,
    C2a, C2b, C2c, C2d, C2e, C2f, C2g,
    C3a, C3b, C3c, C3d, C3e, C3f, C3g,
    C4a, C4b, C4c, C4d, C4e, C4f, C4g,
    C5a, C5b, C5c, C5d, C5e, C5f, C5g,
    C6a, C6b, C6c, C6d, C6e, C6f, C6g,
    C7a, C7b, C7c, C7d, C7e, C7f, C7g,

    B1a, B1b, B1c, B1d, B1e, B1f, B1g,
    B2a, B2b, B2c, B2d, B2e, B2f, B2g,
    B3a, B3b, B3c, B3d, B3e, B3f, B3g,
    B4a, B4b, B4c, B4d, B4e, B4f, B4g,
    B5a, B5b, B5c, B5d, B5e, B5f, B5g,
    B6a, B6b, B6c, B6d, B6e, B6f, B6g,
    B7a, B7b, B7c, B7d, B7e, B7f, B7g,

    A1a, A1b, A1c, A1d, A1e, A1f, A1g,
    A2a, A2b, A2c, A2d, A2e, A2f, A2g,
    A3a, A3b, A3c, A3d, A3e, A3f, A3g,
    A4a, A4b, A4c, A4d, A4e, A4f, A4g,
    A5a, A5b, A5c, A5d, A5e, A5f, A5g,
    A6a, A6b, A6c, A6d, A6e, A6f, A6g,
    A7a, A7b, A7c, A7d, A7e, A7f, A7g
};

// MVV LVA [attacker][victim]
static int mvv_lva[12][12] = {
    105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605,
    104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604,
    103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603,
    102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602,
    101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601,
    100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600,

    105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605,
    104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604,
    103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603,
    102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602,
    101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601,
    100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600
};

//Used by engine
//Alters the threaded bitboards
inline void MakeKnownMove(char id, char side, int attacker, char capture, char promotion, int source, int target) {
    if (!promotion) {
        PopBit(globals[id].bitboards[(side*6) + attacker], source);
        SetBit(globals[id].bitboards[(side*6) + attacker], target);
    }
    else {
        PopBit(globals[id].bitboards[(side*6) + T], source); //Remove tetra from side at source
        SetBit(globals[id].bitboards[(side*6) + I], target); //Add icosa to side at target
    }
    
    PopBit(globals[id].occupancies[side], source);
    SetBit(globals[id].occupancies[side], target);
    PopBit(globals[id].occupancies[both], source);
    SetBit(globals[id].occupancies[both], target);

    if (IsCapture(capture)) {
        PopBit(globals[id].bitboards[((!side)*6) + capture], target);
        PopBit(globals[id].occupancies[!side], target);
    }
}

//Used for SEE and other applications where capture is guaranteed
inline void MakeCapture(char id, char side, int attacker, char capture, int source, int target) {
    PopBit(globals[id].bitboards[(side*6) + attacker], source);
    SetBit(globals[id].bitboards[(side*6) + attacker], target);

    PopBit(globals[id].occupancies[side], source);
    SetBit(globals[id].occupancies[side], target);
    PopBit(globals[id].occupancies[both], source);
    SetBit(globals[id].occupancies[both], target);

    PopBit(globals[id].bitboards[((!side)*6) + capture], target);
    PopBit(globals[id].occupancies[!side], target);
}

//Used for reversing a SEE known capture
inline void ReverseCapture(char id, char side, int attacker, char capture, int source, int target) {
    PopBit(globals[id].bitboards[(side*6) + attacker], target);
    SetBit(globals[id].bitboards[(side*6) + attacker], source);
    
    PopBit(globals[id].occupancies[side], target);
    SetBit(globals[id].occupancies[side], source);
    PopBit(globals[id].occupancies[both], target);
    SetBit(globals[id].occupancies[both], source);

    SetBit(globals[id].bitboards[((!side)*6) + capture], target);
    SetBit(globals[id].occupancies[!side], target);
    SetBit(globals[id].occupancies[both], target);
}

inline void ReverseMove(char id, char side, int attacker, char capture, char promotion, int source, int target) {
    if (!promotion) {
        PopBit(globals[id].bitboards[(side*6) + attacker], target);
        SetBit(globals[id].bitboards[(side*6) + attacker], source);
    }
    else {
        PopBit(globals[id].bitboards[(side*6) + I], target); //Remove icosa from target
        SetBit(globals[id].bitboards[(side*6) + T], source); //Add tetra to source
    }
    
    PopBit(globals[id].occupancies[side], target);
    SetBit(globals[id].occupancies[side], source);
    PopBit(globals[id].occupancies[both], target);
    SetBit(globals[id].occupancies[both], source);
    if (IsCapture(capture)) {
        SetBit(globals[id].bitboards[((!side)*6) + capture], target);
        SetBit(globals[id].occupancies[!side], target);
        SetBit(globals[id].occupancies[both], target);
    }
}

inline Int343 GetAttacksTo(char id, char attacking_side, int block) {
    Int343 attacks = (attacking_side == white) ? ((tetra_attacks[black][block] & globals[id].bitboards[T]) | 
                                                  (dodeca_attacks[block] & globals[id].bitboards[D]) |
                                                  (GetOctaMoves(id, block) & (globals[id].bitboards[O] | globals[id].bitboards[I])) |
                                                  (GetCubeMoves(id, block) & (globals[id].bitboards[C] | globals[id].bitboards[I])) |
                                                  (sphere_attacks[block] & globals[id].bitboards[S])) :
                                                 ((tetra_attacks[white][block] & globals[id].bitboards[t]) | 
                                                  (dodeca_attacks[block] & globals[id].bitboards[d]) |
                                                  (GetOctaMoves(id, block) & (globals[id].bitboards[o] | globals[id].bitboards[i])) |
                                                  (GetCubeMoves(id, block) & (globals[id].bitboards[c] | globals[id].bitboards[i])) |
                                                  (sphere_attacks[block] & globals[id].bitboards[s]));


    return attacks;
}

inline bool HasSafeCheck(char id, char attacking_side, int sphere_block) {
    int block; Int343 attackers;
    
    //Attacked by Octas
    Int343 octa_moves = GetOctaMoves(id, sphere_block);
    octa_moves ^= (octa_moves & sphere_attacks[sphere_block]);
    attackers = octa_moves & ((attacking_side == white) ? globals[id].bitboards[O] : globals[id].bitboards[o]);
    while((block = ForwardScanPop(&attackers)) >= 0) {
        if (!is_block_attacked(id, block, !attacking_side)) return true;
    }

    //attacked by Cube
    Int343 cube_moves = GetCubeMoves(id, sphere_block);
    cube_moves ^= (cube_moves & sphere_attacks[sphere_block]);
    attackers = cube_moves & ((attacking_side == white) ? globals[id].bitboards[C] : globals[id].bitboards[c]);
    while((block = ForwardScanPop(&attackers)) >= 0) {
        if (!is_block_attacked(id, block, !attacking_side)) return true;
    }

    //attacked by Icosa  
    attackers = (cube_moves | octa_moves) & ((attacking_side == white) ? globals[id].bitboards[I] : globals[id].bitboards[i]);
    while((block = ForwardScanPop(&attackers)) >= 0) {
        if (!is_block_attacked(id, block, !attacking_side)) return true;
    }

    // attacked by dodecas
    attackers = dodeca_attacks[sphere_block] & ((attacking_side == white) ? globals[id].bitboards[D] : globals[id].bitboards[d]);
    while((block = ForwardScanPop(&attackers)) >= 0) {
        if (!is_block_attacked(id, block, !attacking_side)) return true;
    }

    return false;
}

//Calculates percentage of safe attacks in the region of the sphere at sphere_block.
//This region includes the sphere itself plus locations it can move to.
//The percentage returns as a positive integer greater than 1.
inline int GetSafeSphereRegionAttacksPercent(char id, char attacking_side, int sphere_block) {
    Int343 sphere_region = sphere_attacks[sphere_block];
    SetBit(sphere_region, sphere_block);

    Int343 attackers;
    int block, attack, safe_attacks = 0, sphere_attacks_count = Count(sphere_region);

    while ((block = ForwardScanPop(&sphere_region)) >= 0) {
        attackers = GetAttacksTo(id, attacking_side,  block);
        if ((attack = ForwardScanPop(&attackers)) >= 0) {
            if (!is_block_attacked(id, attack, !attacking_side)) safe_attacks++;
        } 
    }

    return (safe_attacks * 100) / sphere_attacks_count;
}

inline int Evaluation(char id, char side, bool full_eval) {
    int block, score = 0; Int343 bitboard; Int343 moves;

    if (globals[id].IsEndGame) score += (side == white) ? 3 : -3; //Add points for tempo
    else score += (side == white) ? 10 : -10; //Add points for tempo

    //Add white scores --------------------------------------------------------------------------------------------------------------------
    bitboard = globals[id].bitboards[T]; while((block = ForwardScanPop(&bitboard)) >= 0) {
        score += material_score[T]; score += position_scores[T][block]; 
        if (full_eval) {
            score -= (!IsSet((tetra_isolation_masks[block % 49] & globals[id].bitboards[T]))) ? 10 : 0; 
        }
    }
    bitboard = globals[id].bitboards[D]; while((block = ForwardScanPop(&bitboard)) >= 0) { score += material_score[D]; score += position_scores[D][block];}
    bitboard = globals[id].bitboards[O]; while((block = ForwardScanPop(&bitboard)) >= 0) { score += material_score[O]; score += position_scores[O][block]; }
    bitboard = globals[id].bitboards[C]; while((block = ForwardScanPop(&bitboard)) >= 0) { score += material_score[C]; score += position_scores[C][block]; }
    score += (Count(globals[id].bitboards[I]) * material_score[I]); //Add the White Icosa material scores
    bitboard = globals[id].bitboards[S]; while((block = ForwardScanPop(&bitboard)) >= 0) { 
        score += material_score[S]; score += position_scores[S][block]; 
        if (full_eval && side == white) {
            score -= (globals[id].IsEndGame) ? GetSafeSphereRegionAttacksPercent(id, black, block) : HasSafeCheck(id, black, block) * 7;
        }
    }

    //Subtract black scores (material_score contains negative values) ----------------------------------------------------------------------
    bitboard = globals[id].bitboards[t]; while((block = ForwardScanPop(&bitboard)) >= 0) {score += material_score[t]; 
        score -= position_scores[T][mirror_score[block]]; 
        if (full_eval) {
            score += (!IsSet((tetra_isolation_masks[block % 49] & globals[id].bitboards[t]))) ? 10 : 0;
        }
    }
    bitboard = globals[id].bitboards[d]; while((block = ForwardScanPop(&bitboard)) >= 0) { score += material_score[d]; score -= position_scores[D][mirror_score[block]]; }
    bitboard = globals[id].bitboards[o]; while((block = ForwardScanPop(&bitboard)) >= 0) { score += material_score[o]; score -= position_scores[O][mirror_score[block]]; }
    bitboard = globals[id].bitboards[c]; while((block = ForwardScanPop(&bitboard)) >= 0) { score += material_score[c]; score -= position_scores[C][mirror_score[block]]; }
    score += (Count(globals[id].bitboards[i]) * material_score[i]); //Add the Black Icosa material scores
    bitboard = globals[id].bitboards[s]; while((block = ForwardScanPop(&bitboard)) >= 0) { 
        score += material_score[s]; score -= position_scores[S][mirror_score[block]]; 
        if (full_eval && side == black) {
            score += (globals[id].IsEndGame) ? GetSafeSphereRegionAttacksPercent(id, white, block) : HasSafeCheck(id, white, block) * 7;
        }
    }

    return score * ((side == white) ? 1 : -1);
}

//Side is the attacking side.
//attacks is changed to contain the set of attacks that could occur for the smallest attacker.
static inline char get_smallest_attacker(char id, Int343 &attacks, char side,  int block) {
    
    // attacked by tetras
    attacks = (side == white) ? (tetra_attacks[black][block] & globals[id].bitboards[T]) : (tetra_attacks[white][block] & globals[id].bitboards[t]);
    if (IsSet(attacks)) return T;

    // attacked by dodecas
    attacks = dodeca_attacks[block] & ((side == white) ? globals[id].bitboards[D] : globals[id].bitboards[d]);
    if (IsSet(attacks)) return D;

    // attacked by octas
    Int343 octa_moves = GetOctaMoves(id, block);
    attacks = octa_moves & ((side == white) ? globals[id].bitboards[O] : globals[id].bitboards[o]);
    if (IsSet(attacks)) return O;

    // attacked by Cube
    Int343 cube_moves = GetCubeMoves(id, block);
    attacks = cube_moves & ((side == white) ? globals[id].bitboards[C] : globals[id].bitboards[c]);
    if (IsSet(attacks)) return C;

    // attacked by Icosa
    attacks = (cube_moves | octa_moves) & ((side == white) ? globals[id].bitboards[I] : globals[id].bitboards[i]);
    if (IsSet(attacks)) return I;

    // attacked by Sphere
    attacks = sphere_attacks[block] & ((side == white) ? globals[id].bitboards[S] : globals[id].bitboards[s]);
    if (IsSet(attacks)) return S;

    return -1;
}

static inline int SeeHelper(char id, char side, char piece, int target) {
    int value, source;
    
    Int343 attacks;
    char attacker = get_smallest_attacker(id, attacks, side, target);
    value = 0;

    if (attacker >= 0) {
        source = ForwardScanPop(&attacks);
        MakeCapture(id, side, attacker, piece, source, target);
        value = material_score[piece] - SeeHelper(id, !side, attacker, target);
        ReverseCapture(id, side, attacker, piece, source, target);
    }

    return value;
}

//Static Exchange Evaluation
//Needed for scoring captures. Returns a positive, negative or zero value.
//Positive value is good, zero value is draw, and negative is bad
static inline int See(char id, char side, char piece, char capture, int source, int target) {
    int value = 0;
    MakeKnownMove(id, side, piece, capture, 0, source, target);
    value = SeeHelper(id, !side, piece, target);
    ReverseMove(id, side, piece, capture, 0, source, target);
    return value;
}


static inline int ScoreMove(char id, int list_index, long move, char side, char type, char capture, char promotion, int target) {
    //If PV Move
    if (globals[id].pv_table[0][list_index] == move && move != 0) {
        return 20000;
    }
    else if (IsCapture(capture)) {
        int seeValue = See(id, side, type, capture, get_move_source(move), target);
        if (seeValue >= 0)
            return seeValue + ((promotion) ? 9000 : 8000);
        else
            return seeValue;
    }
    else if (promotion) { // Note: code possibly improve ordering through having a promotion + capture > promotion
        return 9000; //Made up number for promotion relative to the mvv_lva lookup table;
    }
    else {
        if (globals[id].killer_moves[0][list_index] == move) {
            return 8000;
        }
        else if (globals[id].killer_moves[1][list_index] == move) {
            return 7000;
        }
        else {
            //return globals[id].history_moves[(side * 6) + type][target]; //This is possibly buggy
            if (globals[id].history_moves[(side * 6) + type][target] > 0) {
                return 1000 + (globals[id].history_moves[(side * 6) + type][target] / 100);
            }
            else
                return 0;
        } 
    }
    
    return 0;
}

#define ScoreCapture(type, capture) (IsCapture(capture) ? mvv_lva[type][capture] : 0);

/**********************************\
 ==================================
 
           Move Generation
 
 ==================================
\**********************************/
#define EMPTY_MOVE_THRESHOLD 50

static inline bool IsRepeated(U64 new_position) {
    if (TURN >= 8 && (new_position == TurnList[TURN-4].key || new_position == TurnList[TURN-6].key || new_position == TurnList[TURN-8].key)) {
        return true;
    }

    return false;
}

bool IsThreefoldRepetition(U64 move_key) {
    if (TURN >= MAX_TURNS) return true;

    //threefold repetition
    int repetition_counter = 0;
    for (int i = (TURN < MAX_TURNS) ? TURN-2 : (MAX_TURNS / 4); i >= 0; i--) {
        if (move_key == TurnList[i].key) repetition_counter++;
        if (repetition_counter >= 3) {
            return true;
        }
    }

    return false;
}

//Doesn't necessarily use 50 as the threshold
bool IsEmptyMoveRule(long move) {
    if (TURN >= MAX_TURNS) return true;

    bool fifty_rule = false;
    if (TURN >= EMPTY_MOVE_THRESHOLD && get_move_piece(move) != T && !IsCapture(get_move_capture(move))) {
        fifty_rule = true;
        int cap = 0;
        char type, capture;
        for (int i = (TURN < MAX_TURNS) ? TURN-1 : MAX_TURNS-1; cap < EMPTY_MOVE_THRESHOLD && i >= 0; i--, cap++) {
            type = get_move_piece(TurnList[i].move);
            capture = get_move_capture(TurnList[i].move);
            if (type == T || IsCapture(capture)) {
                fifty_rule = false;
                break;
            }
        }
    }

    return fifty_rule;
}

bool IsInsufficientMaterial() {
    //If both sides only have one king
    //if one side has only one minor piece (dodeca, octa) and opponent is bare
    //if both sides have a king and an octa, with the octas being the same color
    if (Count(Bitboards[t]) > 0 || Count(Bitboards[T]) > 0 ||
        Count(Bitboards[c]) > 0 || Count(Bitboards[C]) > 0 ||
        Count(Bitboards[i]) > 0 || Count(Bitboards[I]) > 0) return false;

    int black_octas = Count(Bitboards[o]);
    int white_octas = Count(Bitboards[O]);
    int black_dodecas = Count(Bitboards[d]);
    int white_dodecas = Count(Bitboards[D]);

    // If both sides only have one octa but on different colored squares, no draw.
    if (black_octas == 1 && white_octas == 1 && black_dodecas + white_dodecas == 0 &&
        (BitScanForward(Bitboards[o]) % 2) != (BitScanForward(Bitboards[O]) % 2))
        return false;

    //If either side has more than one minor piece, no draw
    if (black_octas + black_dodecas > 1 ||
        white_octas + white_dodecas > 1) return false;

    //If both sides have at least one minor piece, no draw
    if (black_octas + black_dodecas == 1 && white_octas + white_dodecas == 1) return false;

    return true;
}

bool IsDrawnGame(Turn turn) {
    if (TURN >= MAX_TURNS) return true;

    //threefold repetition
    if (IsThreefoldRepetition(turn.key)) return true;

    //fifty move rule: if after fifty moves no pawn was moved and no captures occurred.
    if (IsEmptyMoveRule(turn.move)) return true;
    
    //Insufficient Material
    if (IsInsufficientMaterial()) return true;

    return false;
}

static inline void GenerateMoves(char id, int list_index, char side) {
    int source, target, score;
    char capture;
    Int343 piece_set; Int343 moves;
    long move;
    
    //Tetras
    piece_set = globals[id].bitboards[(side * 6) + T];
    while((source = ForwardScanPop(&piece_set)) >= 0) {
        moves = GetValidTetraMoves(id, side, source);
        while ((target = ForwardScanPop(&moves)) >= 0) {
            capture = GetCapture(id, side, target);
            move = encode_move(source, target, T, capture, IsPromotion(side, target), side);
            score = ScoreMove(id, list_index, move, side, T, capture, IsPromotion(side, target), target);
            globals[id].mstack[list_index].Add({move, score});
        }
    }

    //Dodecas
    piece_set = globals[id].bitboards[(side * 6) + D];
    while((source = ForwardScanPop(&piece_set)) >= 0) {
        moves = GetValidDodecaMoves(id, side, source);
        while ((target = ForwardScanPop(&moves)) >= 0) {
            capture = GetCapture(id, side, target);
            move = encode_move(source, target, D, capture, 0, side);
            score = ScoreMove(id, list_index, move, side, D, capture, 0, target);
            globals[id].mstack[list_index].Add({move, score});
        }
    }

    //Octas
    piece_set = globals[id].bitboards[(side * 6) + O];
    while((source = ForwardScanPop(&piece_set)) >= 0) {
        moves = GetOctaMoves(id, source);
        moves ^= (moves & globals[id].occupancies[side]);
        while ((target = ForwardScanPop(&moves)) >= 0) {
            capture = GetCapture(id, side, target);
            move = encode_move(source, target, O, capture, 0, side);
            score = ScoreMove(id, list_index, move, side, O, capture, 0, target);
            globals[id].mstack[list_index].Add({move, score});
        }
    }

    //Cubes
    piece_set = globals[id].bitboards[(side * 6) + C];
    while((source = ForwardScanPop(&piece_set)) >= 0) {
        moves = GetCubeMoves(id, source);
        moves ^= (moves & globals[id].occupancies[side]);
        while ((target = ForwardScanPop(&moves)) >= 0) {
            capture = GetCapture(id, side, target);
            move = encode_move(source, target, C, capture, 0, side);
            score = ScoreMove(id, list_index, move, side, C, capture, 0, target);
            globals[id].mstack[list_index].Add({move, score});
        }
    }

    //Icosas
    piece_set = globals[id].bitboards[(side * 6) + I];
    while((source = ForwardScanPop(&piece_set)) >= 0) {
        moves = GetOctaMoves(id, source) | GetCubeMoves(id, source);
        moves ^= (moves & globals[id].occupancies[side]);
        while ((target = ForwardScanPop(&moves)) >= 0) {
            capture = GetCapture(id, side, target);
            move = encode_move(source, target, I, capture, 0, side);
            score = ScoreMove(id, list_index, move, side, I, capture, 0, target);
            globals[id].mstack[list_index].Add({move, score});
        }
    }

    //Sphere
    source = BitScan(side, globals[id].bitboards[(side * 6) + S]);
    moves = GetValidSphereMoves(id, side, source);
    while ((target = ForwardScanPop(&moves)) >= 0) {
        capture = GetCapture(id, side, target);
        move = encode_move(source, target, S, capture, 0, side);
        score = ScoreMove(id, list_index, move, side, S, capture, 0, target);
        globals[id].mstack[list_index].Add({move, score});
    }
}

static inline void GenerateCaptures(char id, int list_index, char side) {
    int source, target, score;
    char capture;
    Int343 piece_set; Int343 moves;

    //Tetras
    piece_set = globals[id].bitboards[(side * 6) + T];
    while((source = ForwardScanPop(&piece_set)) >= 0) {
        moves = GetTetraCaptures(id, side, source);
        while ((target = ForwardScanPop(&moves)) >= 0) {
            capture = GetCapture(id, side, target);
            score = ScoreCapture(T, capture);
            globals[id].qstack[list_index].Add({encode_move(source, target, T, capture, IsPromotion(side, target), side), score});
        }
    }

    //Dodecas
    piece_set = globals[id].bitboards[(side * 6) + D];
    while((source = ForwardScanPop(&piece_set)) >= 0) {
        moves = GetDodecaCaptures(id, side, source);
        while ((target = ForwardScanPop(&moves)) >= 0) {
            capture = GetCapture(id, side, target);
            score = ScoreCapture(D, capture);
            globals[id].qstack[list_index].Add({encode_move(source, target, D, capture, 0, side), score});
        }
    }

    //Octas
    piece_set = globals[id].bitboards[(side * 6) + O];
    while((source = ForwardScanPop(&piece_set)) >= 0) {
        moves = GetOctaMoves(id, source) & globals[id].occupancies[!side];
        while ((target = ForwardScanPop(&moves)) >= 0) {
            capture = GetCapture(id, side, target);
            score = ScoreCapture(O, capture);
            globals[id].qstack[list_index].Add({encode_move(source, target, O, capture, 0, side), score});
        }
    }

    //Cubes
    piece_set = globals[id].bitboards[(side * 6) + C];
    while((source = ForwardScanPop(&piece_set)) >= 0) {
        moves = GetCubeMoves(id, source) & globals[id].occupancies[!side];
        while ((target = ForwardScanPop(&moves)) >= 0) {
            capture = GetCapture(id, side, target);
            score = ScoreCapture(C, capture);
            globals[id].qstack[list_index].Add({encode_move(source, target, C, capture, 0, side), score});
        }
    }

    //Icosas
    piece_set = globals[id].bitboards[(side * 6) + I];
    while((source = ForwardScanPop(&piece_set)) >= 0) {
        moves = (GetOctaMoves(id, source) | GetCubeMoves(id, source)) & globals[id].occupancies[!side];
        while ((target = ForwardScanPop(&moves)) >= 0) {
            capture = GetCapture(id, side, target);
            score = ScoreCapture(I, capture);
            globals[id].qstack[list_index].Add({encode_move(source, target, I, capture, 0, side), score});
        }
    }

    //Sphere
    source = BitScan(side, globals[id].bitboards[(side * 6) + S]);
    moves = GetSphereCaptures(id, side, source);
    while ((target = ForwardScanPop(&moves)) >= 0) {
        capture = GetCapture(id, side, target);
        score = ScoreCapture(S, capture);
        globals[id].qstack[list_index].Add({encode_move(source, target, S, capture, 0, side), score});
    }
}

/**********************************\
 ==================================
 
           Searching
 
 ==================================
\**********************************/
#define FullDepthMoves 4
#define ReductionLimit 2
#define TIME_LIMIT 4000000 //4 seconds

long NODES_SEARCHED = 0; //GLOBAL
int FINAL_DEPTH = 0;
bool TIMEOUT_STOPPED = false; //GLOBAL
bool ABORT_GAME = false;
chrono::time_point<chrono::steady_clock> START_TIME;

//Testing Variables - used for monitoring performance
long long AVERAGE_SEARCH_TIME = 0; //Should get reset when the whole game ends. //GLOBAL
long long MAX_SEARCH_TIME = 0; //GLOBAL

void StartTimer() {
    START_TIME = chrono::steady_clock::now();
}

//Returns the duration of time in microseconds passed since start time.
inline long long GetTimePassed() {
    auto finish = chrono::steady_clock::now();
    auto microseconds = chrono::duration_cast<chrono::microseconds>(finish - START_TIME);
    return microseconds.count();
}

//Returns true if the game needs to be stopped.
inline bool StopGame(char id) {
    if (TIMEOUT_STOPPED || ABORT_GAME) return true;
    if ((globals[id].nodes_searched & 2047) != 0) return false;

    long long time_passed = GetTimePassed();
    if (time_passed > TIME_LIMIT) {
        TIMEOUT_STOPPED = true;
        return true;
    }

    return false;
}

void ClearTimeNodeValues() {
    NODES_SEARCHED = 0;
    FINAL_DEPTH = 0;
    TIMEOUT_STOPPED = false;
    ABORT_GAME = false;
}

string GetTimeStampString(long long timestamp) {
    long milliseconds = (long) (timestamp / 1000) % 1000;
    long seconds = (((long) (timestamp / 1000) - milliseconds)/1000) % 60;
    long minutes = (((((long) (timestamp / 1000) - milliseconds)/1000) - seconds)/60) % 60;
    return to_string(minutes) + ":" + to_string(seconds) + ":" + to_string(milliseconds);
}

inline int FutilityMoveCount(bool improving, int depth) {
    return improving ? (9 + depth * depth)
                     : (9 + depth * depth) / 2;
}

static inline int QSearch(char id, int ply_depth, int list_index, char side, U64 hash_key, int alpha, int beta) {
    if (ABORT_GAME) return 0;

    const bool PvNode = beta - alpha > 1;

    //Transposition Table Lookup
    const TTEntry * tt_entry = GetTTEntry(hash_key);
    const int ttValue = GetTableValue(tt_entry, hash_key);
    if (!PvNode && tt_entry->depth >= ply_depth && ttValue != NO_HASH) {
        if (tt_entry->flag == HASH_EXACT) return ttValue;
        if (tt_entry->flag == HASH_ALPHA && ttValue <= alpha) return alpha;
        if (tt_entry->flag == HASH_BETA && ttValue >= beta) return beta;
    }

    globals[id].nodes_searched++;

    int static_eval = Evaluation(id, side, true);

    // Note: ttValue at this point is no_hash AND a dpeth less than ply_depth, so not good enough for an
    // early return, but still useful.
    // We use ttValue as the evaluation if it is better.
    if (ttValue != NO_HASH && ttValue > static_eval) static_eval = ttValue; 

    if (ply_depth <= 0 || StopGame(id) || list_index >= QDEPTH) return static_eval;

    int initial_alpha = alpha;
    if (static_eval >= beta) {
        WriteToHashTable(hash_key, ply_depth, static_eval, alpha, beta);
        return beta;
    }
    if (static_eval < alpha - 1000) return alpha;//Delta pruning
    if (static_eval > alpha) alpha = static_eval;

    GenerateCaptures(id, list_index, side);
    
    char type, capture, promotion; int source, target; long move; U64 new_key;
    int sphere_source = GetSphereSource(id, side);
    int legal_moves = 0;
    int score = 0;
    for (int i = 0; i < globals[id].qstack[list_index].count; i++) {
        move = globals[id].qstack[list_index].moves[i].encoding;
        type = get_move_piece(move);
        capture = get_move_capture(move);
        source = get_move_source(move);
        target = get_move_target(move);
        promotion = get_move_promotion(move);

        //Move Count and Futility Pruning
        if (promotion == 0) {
            if (legal_moves > 9) continue; //standard is currently 6
            if (static_eval + material_score[capture] <= alpha) continue; //if static evaluation + material value is too low, then prune
        }

        MakeKnownMove(id, side, type, capture, promotion, source, target);

        if (IsInCheck(id, side, type, target, sphere_source)) {
            ReverseMove(id, side, type, capture, promotion, source, target);
            continue;
        }

        new_key = MakeTTKey(hash_key, move);

        score = -QSearch(id, ply_depth-1, list_index+1, !side, new_key, -beta, -alpha);

        ReverseMove(id, side, type, capture, promotion, source, target);

        legal_moves++;

        if (score >= beta) {
            WriteToHashTable(hash_key, ply_depth, score, alpha, beta);
            globals[id].qstack[list_index].Clear();
            return beta;
        }

        if (score > alpha) {
            alpha = score;
            if (!PvNode) break; //In general, the best move won't go up in later (weaker) moves due to MVVLVA move ordering
        }
    }

    WriteToHashTable(hash_key, ply_depth, alpha, initial_alpha, beta);

    globals[id].qstack[list_index].Clear();
    return alpha;
}

static inline int SubSearch(const char id, const int ply_depth, const int list_index, const char side, const U64 hash_key, const bool CanDoNullMove, int alpha, int beta) {
     if (ABORT_GAME) return 0;
     if (ply_depth <= 0 || StopGame(id) || list_index >= DEPTH) return QSearch(id, QDEPTH, 0, side, hash_key, alpha, beta);

    //Set PV Length. This must occur here at the top.
    int ply = (list_index < DEPTH) ? list_index : DEPTH-1; 
    globals[id].pv_length[ply] = ply;
    const bool PvNode = beta - alpha > 1;

    //Transposition Table Lookup
    const TTEntry * tt_entry = GetTTEntry(hash_key);
    const int ttValue = GetTableValue(tt_entry, hash_key);
    if (!PvNode && tt_entry->depth >= ply_depth && ttValue != NO_HASH) {
        if (tt_entry->flag == HASH_EXACT) return ttValue;
        if (tt_entry->flag == HASH_ALPHA && ttValue <= alpha) return alpha;
        if (tt_entry->flag == HASH_BETA && ttValue >= beta) return beta;
    }

    globals[id].nodes_searched++;

    // Mate Distance Pruning;
    if (alpha < -MATE_VALUE + (globals[id].total_depth - ply_depth)) alpha = -MATE_VALUE + (globals[id].total_depth - ply_depth);
    if (beta > MATE_VALUE - (globals[id].total_depth - ply_depth) - 1) beta = MATE_VALUE - (globals[id].total_depth - ply_depth) - 1;
    if (alpha >= beta) return alpha;
    
    int sphere_source = GetSphereSource(id, side);
    bool in_check = is_block_attacked(id, sphere_source, !side);

    int static_eval = Evaluation(id, side, false);
    if (ttValue != NO_HASH && ttValue > static_eval) static_eval = ttValue;

    //improving = current static eval - static eval two turns ago
    bool improving = (static_eval - (list_index > 1 ? globals[id].mstack[list_index-2].static_eval : 0)) > 0;
    
    int score = 0; U64 new_key;

    if (!in_check && !PvNode) {
        //Razoring
        if (static_eval < alpha - 426 - 252 * ply_depth * ply_depth) { //Use 600 instead
            score = QSearch(id, QDEPTH, 0, side, hash_key, alpha - 1, alpha);
            if (score < alpha)
                return score;
        }

        //Futility Pruning
        if (ply_depth <= 4 &&
            static_eval - (165 * (ply_depth - improving)) - ((list_index > 0 ? globals[id].mstack[list_index-1].static_eval : 0) / 280) >= beta &&
            static_eval < 48000) {
            return static_eval;
        }

        //Null Move Pruning
        if (CanDoNullMove && ply_depth >= 3) {
            long null_move = EncodeNullMove(side);
            new_key = MakeTTKey(hash_key, null_move);
            globals[id].mstack[list_index].move = null_move; //Store null move in stack
            globals[id].mstack[list_index].static_eval = static_eval; //Store static eval in stack
            score = -SubSearch(id, ply_depth - 3, list_index+1, !side, new_key, false, -beta, -beta + 1);
            if (score >= beta) {
                globals[id].mstack[list_index].Clear();
                return beta;
            }
        }
    }

    GenerateMoves(id, list_index, side);

    int initial_alpha = alpha;
    char type, capture, promotion; int source, target, legal_moves = 0; long move; 
    int futility_move_count = FutilityMoveCount(improving, ply_depth);
    int extensions = (in_check) ? 1 : 0;

    for (int i = 0; i < globals[id].mstack[list_index].count; i++) {
        move = globals[id].mstack[list_index].moves[i].encoding;
        capture = get_move_capture(move);
        type = get_move_piece(move);
        source = get_move_source(move);
        target = get_move_target(move);
        promotion = get_move_promotion(move);

        //Early Pruning
        if (!in_check && legal_moves > 0 && promotion == 0) {
            if (IsCapture(capture)) { //Futility pruning for captures
                if (!PvNode && ply_depth < 6 && (static_eval + material_score[capture] + (165 * (ply_depth + improving))) <= alpha) continue;
                if (!PvNode && globals[id].mstack[list_index].moves[i].score < -205 * ply_depth) continue; //prune bad SEE captures; //find a way to store this
            }
            else {
                if (legal_moves >= futility_move_count && type != T) continue; //Move Count Pruning for quiet moves. Not dependent on PvNode
                if (!PvNode && ply_depth < 13 && static_eval + 103 + 138 * ply_depth <= alpha) continue;
            }
        }
        
        MakeKnownMove(id, side, type, capture, promotion, source, target);

        if (IsInCheck(id, side, type, target, sphere_source)) {
            ReverseMove(id, side, type, capture, promotion, source, target);
            continue;
        }

        new_key = MakeTTKey(hash_key, move);
        globals[id].mstack[list_index].move = move; //Store move in stack
        globals[id].mstack[list_index].static_eval = static_eval; //Store eval in stack
            
        if (legal_moves == 0) {//Do normal search on the first one
            score = -SubSearch(id, ply_depth - 1 + extensions, list_index+1, !side, new_key, true, -beta, -alpha);
        }
        else { //Late Move Reduction (LMR)
            if (!in_check &&
                !PvNode && 
                legal_moves >= FullDepthMoves &&
                ply_depth >= ReductionLimit &&
                !IsCapture(capture) &&
                promotion == 0)
                score = -SubSearch(id, ply_depth - 2, list_index+1, !side, new_key, true, -alpha - 1, -alpha); //-beta would be -alpha - 1 in proper LMR
            else
                score = alpha + 1;

            // if found a better move during LMR then do PVS
            if (score > alpha) { // re-search at full depth but with narrowed bandwitdh
                score = -SubSearch(id, ply_depth - 1 + extensions, list_index+1, !side, new_key, true, -alpha - 1, -alpha);
                if ((score > alpha) && (score < beta))
                    score = -SubSearch(id, ply_depth - 1 + extensions, list_index+1, !side, new_key, true, -beta, -alpha);
            }
        }

        ReverseMove(id, side, type, capture, promotion, source, target);

        legal_moves++;

        if (score > alpha) {
            alpha = score;
            HandlePVMove(id, ply, move);

            if (score >= beta) {
                WriteToHashTable(hash_key, ply_depth, score, alpha, beta);

                if (!IsCapture(capture)) {
                    globals[id].killer_moves[1][list_index] = globals[id].killer_moves[0][list_index];
                    globals[id].killer_moves[0][list_index] = move;
                    globals[id].history_moves[(side * 6) + type][target] += ply_depth * ply_depth;
                }

                globals[id].mstack[list_index].Clear();
                return beta;
            }
        }
    }

    globals[id].mstack[list_index].Clear();

    // we don't have any legal moves to make in the current postion, then return checkmate or stalemate
    if (legal_moves == 0)  {
        return (in_check) ? -MATE_VALUE + (globals[id].total_depth - ply_depth) : DRAW_VALUE;
    }

    WriteToHashTable(hash_key, ply_depth, alpha, initial_alpha, beta);

    return alpha;
}

static inline void RootSearch(char id, int ply_depth, char side, int alpha, int beta) {
    globals[id].total_depth = ply_depth;

    int score = 0;
    char type, capture, promotion; int source, target; long move; U64 hash_key; 
    U64 start_key = BuildTranspositionKey();
    int list_index = 0;
    int legal_moves = 0;
    int sphere_source = GetSphereSource(id, side);
    bool in_check = is_block_attacked(id, sphere_source, !side);
    int static_eval = Evaluation(id, side, false);

    globals[id].pv_length[0] = 0;

    GenerateMoves(id, list_index, side); 

    for (int i = 0; i < globals[id].mstack[list_index].count; i++) {
        move = globals[id].mstack[list_index].moves[i].encoding;
        source = get_move_source(move);
        target = get_move_target(move);
        type = get_move_piece(move);
        capture = get_move_capture(move);
        promotion = get_move_promotion(move);

        MakeKnownMove(id, side, type, capture, promotion, source, target);

        if (IsInCheck(id, side, type, target, sphere_source)) {
            ReverseMove(id, side, type, capture, promotion, source, target);
            continue;
        }

        hash_key = MakeTTKey(start_key, move);
        globals[id].mstack[list_index].move = move;
        globals[id].mstack[list_index].static_eval = static_eval;

        if (!IsRepeated(hash_key))
            score = -SubSearch(id, ply_depth-1, list_index+1, !side, hash_key, true, -beta, -alpha);
        else
            score = alpha;

        ReverseMove(id, side, type, capture, promotion, source, target);

        legal_moves++;

        if (score > alpha) {
            alpha = score;

            HandlePVMove(id, 0, move);

            globals[id].search_result.final_move = move;
            globals[id].search_result.final_score = score;
            globals[id].search_result.final_hash_key = hash_key;
            globals[id].search_result.flag = FOUND_MOVE;
        }
    }

    globals[id].mstack[list_index].Clear();

    globals[id].search_result.search_depth = ply_depth; //May be better to have it here, but untested.

    if (legal_moves == 0) // we don't have any legal moves to make in the current postion
        globals[id].search_result.flag = (in_check) ? CHECKMATE : STALEMATE;
    else if (IsDrawnGame({globals[id].search_result.final_hash_key, globals[id].search_result.final_move}))
        globals[id].search_result.flag = DRAW_GAME;
    else
        globals[id].search_result.flag = FOUND_MOVE;
    
}

static inline void Negamax(char id, int ply_depth, int initial_depth, char side) {
    int alpha = -INF;
    int beta = INF;
    int depth_counter = 0;
    
    for (int current_depth = initial_depth; current_depth <= ply_depth; current_depth++) {
        if (StopGame(id))
            break;

        //If a thread is about to run depth=d, first check what others are doing, and if half (or more) of the threads are running at depths >= d, 
        //then skip depth=d (rule is iterative, continue until a suitable depth is found to work on). 
        //it's a synchronized way of saying that we want (approx.) half threads on some depth d, and half on some depth d+1
        if (current_depth > initial_depth && current_depth < ply_depth-1) {
            depth_counter = 0;
            for (int i = 0; i < THREAD_COUNT; i++) {
                if (i != id && current_depth >= globals[id].total_depth) depth_counter++;
            }
            if (depth_counter >= THREAD_COUNT / 2) current_depth++;
        }
        

        RootSearch(id, current_depth, side, alpha, beta);
    }
}

static inline SearchResult FindBestMove(char side) {
    StartTimer();

    SearchResult best_result;

    vector<thread> threads;
    threads.reserve(THREAD_COUNT);
    int ply_depth = DEPTH;

    bool IsEndGame = Count(Occupancies[both]) <= 40;

    //Reset global values
    for (int id = 0; id < THREAD_COUNT; id++) {
        globals[id].CleanAndBuild(id, IsEndGame);
    }

    //Generate Threads
    int initial_depth = 1;
    for (int id = 0; id < THREAD_COUNT; id++) {
        threads.emplace_back([id, ply_depth, initial_depth, side]{
            Negamax(id, ply_depth, initial_depth, side);
        });
        initial_depth++;
    }


    for (int id = 0; id < THREAD_COUNT; id++) {
        threads[id].join();
    }

    //Get the best move in all the threads
    for (int id = 0; id < THREAD_COUNT; id++) {
        NODES_SEARCHED += globals[id].nodes_searched;

        if (globals[id].search_result.flag != NO_MOVE) {
            if (best_result.flag == NO_MOVE ||
                globals[id].search_result.flag != FOUND_MOVE ||
                globals[id].search_result.search_depth >= best_result.search_depth) {
                best_result = globals[id].search_result;
            }
        }

        if (globals[id].total_depth > FINAL_DEPTH)
            FINAL_DEPTH = globals[id].total_depth;
    }

    return best_result;
}

void UpdateTurnValues(Turn turn) {
    //Switch sides and readjust key variables
    AddToTurnList(turn);
    SIDE = !SIDE;
    ClearTimeNodeValues();
}

void ClearMiscTables() {
    for (int i = 0; i < MAX_TURNS; i++) TurnList[i] = {0, 0};
}

// init all variables
void init_variables()
{   //TODO: Separate the static variables from the new_game variables

    //Static variables
    init_attacks();
    init_rays_directions_edges();
    init_tetra_isolation_masks();
    init_random_keys();

    //New Game Variables
    ClearTranspositionTable();
    ClearMiscTables();
    ClearTimeNodeValues();

    //Testing Variables
    AVERAGE_SEARCH_TIME = 0;
    MAX_SEARCH_TIME = 0;

    //In Game Turn counter
    TURN = 0;
}


/**********************************\
 ==================================
 
        User Interaction
 
 ==================================
\**********************************/

void TestBitBoards() {
    string response;
    int block = 0;
    int max = 343;
    Int343 bits;
    while (true) {
        if (block >= max) block = 0;
        cout << ">> " << block;
        getline(cin, response);
        printf("white: %d, black: %d", position_scores[T][block], position_scores[T][mirror_score[block]]);
        block++;
    }
}

void PrintEndGameMetrics() {
    AVERAGE_SEARCH_TIME = (TURN > 0) ? AVERAGE_SEARCH_TIME / TURN : 0;

    printf("TurnCount=%d\n", TURN);
    printf("WhitePieces=%d\n", Count(Occupancies[white]));
    printf("BlackPieces=%d\n", Count(Occupancies[black]));
    printf("TotalPieces=%d\n", Count(Occupancies[both]));
    printf("AverageSearchTime=%s\n", GetTimeStampString(AVERAGE_SEARCH_TIME).c_str());
    printf("MaxSearchTime=%s\n", GetTimeStampString(MAX_SEARCH_TIME).c_str());
}

void PrintTitle() {
    cout << "**********************************\n" <<
            "==================================\n\n" <<
            "\t   STARCHESS\n\n" <<
            "==================================\n" <<
            "**********************************\n";
    cout << "Depth = " << DEPTH << " | QDepth = " << QDEPTH << "\n";
    cout << "Thread Count = " << THREAD_COUNT << "\n";
    cout << "Timeout = " << GetTimeStampString(TIME_LIMIT) << "\n";
}

//Splits the string str and puts it in out
void tokenize(string const &str, const char delim, vector<string> &out)
{
    // construct a stream from the string
    stringstream ss(str);
 
    string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
}

//Used by UI and SelfPlay
//Directly alters the global bitboards
inline void MakeRealMove(Turn turn) {
    char side = get_move_side(turn.move);
    char attacker = get_move_piece(turn.move);
    char capture = get_move_capture(turn.move);
    int source = get_move_source(turn.move);
    int target = get_move_target(turn.move);

    if (attacker != T || !IsPromotion(side, target)) {
        PopBit(Bitboards[(side*6) + attacker], source);
        SetBit(Bitboards[(side*6) + attacker], target);
    }
    else {
        PopBit(Bitboards[(side*6) + T], source); //Remove tetra from side at source
        SetBit(Bitboards[(side*6) + I], target); //Add icosa to side at target
    }
    
    PopBit(Occupancies[side], source);
    SetBit(Occupancies[side], target);
    PopBit(Occupancies[both], source);
    SetBit(Occupancies[both], target);

    if (IsCapture(capture)) {
        PopBit(Bitboards[((!side)*6) + capture], target);
        PopBit(Occupancies[!side], target);
    }

    UpdateTurnValues(turn);
}

void SelfPlay() {
    string response;
    long long search_time;
    bool is_continuous = false;
    bool log_only = false;
    bool log_timed = false;
    int log_counter = 0;
    int log_max = 0;

    SearchResult search_result;
    char type, capture, promotion, side; int source, target;
    bool new_game = false;

    PrintTitle();

    init_variables();
    init_bitboards((char *)START_POSITION);
    
    while (true) {
        if (log_timed) {
            log_counter++;
            if (log_counter >= log_max) {log_timed = false; is_continuous = false; log_max = 0;}
        }

        if (!log_only && !log_timed && !new_game) print_full_board();

        response = "";
        if (!is_continuous) {
            cout << ">> ";
            getline(cin, response);
        }
        
        if (!response.compare("exit")) break;
        if (!response.compare("-l")) log_only = true; //Makes only the log line show
        if (!response.compare("-a")) log_only = false; //Makes only the log line show
        if (!response.compare("continuous")) is_continuous = 1; //Makes the game keep going without my input
        if (!response.compare("log") || !response.compare("continuous -l")) {is_continuous = true; log_only = true; log_timed = false;}
        if (!response.compare("log20")) {
            log_only = false;
            log_timed = true;
            log_max = 20;
            log_counter = 0;
            is_continuous = true;
        }

        if (!response.compare("new") || new_game) {
            init_variables();
            init_bitboards((char *)START_POSITION);
            new_game = false;
            printf("\nNEW GAME\n");
            continue;
        }

        if (!log_only && !log_timed) printf(">> Generating Moves for %s...\n", (SIDE) ? "BLACK" : "WHITE" );

        search_result = FindBestMove(SIDE);

        source = get_move_source(search_result.final_move);
        target = get_move_target(search_result.final_move);
        type = get_move_piece(search_result.final_move);
        capture = get_move_capture(search_result.final_move);
        promotion = get_move_promotion(search_result.final_move);
        side = get_move_side(search_result.final_move);

        //Get length of search, for testing purposes
        search_time = GetTimePassed();
        AVERAGE_SEARCH_TIME += search_time;

        if (search_time > MAX_SEARCH_TIME) MAX_SEARCH_TIME = search_time;

        if (search_result.flag == CHECKMATE || 
            search_result.flag == STALEMATE ||
            search_result.flag == DRAW_GAME) {

            cout << "****************************\n" <<
                    "============================\n";

            if (search_result.flag == CHECKMATE) {
                printf("CHECKMATE!\n");
                if (SIDE == white) printf("BLACK WON!\n");
                else printf("WHITE WON!\n");
            }
            else if (search_result.flag == STALEMATE) {
                printf("STALEMATE!\nGAME OVER!\n");
            }
            else {
                printf("DRAW!\nGAME OVER!\n");
            }

            PrintEndGameMetrics();
            cout << "============================\n" <<
                    "****************************\n";

            is_continuous = 0;
            new_game = true;
        }
        else {
            //Print out testing/logging info
            if (!log_only && !log_timed) printf(">> %s %c %d to %d\n", (SIDE) ? "BLACK" : "WHITE", ascii_pieces[type], source, target);
            printf("[Turn=%d|MaxDepth=%d|MaxQDepth=%d|FinalDepth=%d|SearchDepth=%d|%s|Type=%c|Source=%d|Target=%d|Capture=%c|Score=%d|Thread=%d|PiecesLeft=%d|NodesSearched=%ld|Time=%s]%s\n",
                    TURN, DEPTH, QDEPTH, FINAL_DEPTH, search_result.search_depth, (side) ? "BLACK" : "WHITE", ascii_pieces[type], source, target, 
                    (IsCapture(capture)) ? ascii_pieces[capture] : 'X', search_result.final_score, search_result.thread_id, 
                    Count(Occupancies[both]), NODES_SEARCHED, GetTimeStampString(search_time).c_str(), (TIMEOUT_STOPPED) ? " >> TIMEOUT" : "");

            //Make the move just searched, if not a checkmate/stalemate
            MakeRealMove({search_result.final_hash_key, search_result.final_move}); 
        }
    }
}

/**********************************\
 ==================================
 
        Unity Library
 
 ==================================
\**********************************/

//Helper variable for transfering bits from here to Unity engine.
//We can't port Int343 values directly, so we need a helper to grab the indivdual 64 bit long values
Int343 BB_VALUE; 

/*
** Helper method for the C# Dll Methods
** The global[id] value is set so that methods can be called smoothly
*/
void SetGlobalBitBoards() {
    for (int i = 0; i < 12; i++) globals[0].bitboards[i] = Bitboards[i];
    for (int i = 0; i < 3; i++) globals[0].occupancies[i] = Occupancies[i];
}

bool is_sphere_in_check(char side) {
    int sphere_source = GetSphereSource(0, side);
    return IsInCheck(0, side, S, sphere_source, 0);
}

bool is_legal_move(int side, int type, int source, int target) {
    int capture = GetCapture(0, side, target);
    MakeKnownMove(0, side, type, capture, 0, source, target);
    bool is_legal = !is_sphere_in_check(side);
    ReverseMove(0, side, type, capture, 0, source, target);
    return is_legal;
}

Int343 get_legal_moves(int type, int side, int block) {
    Int343 attacks;

    if (type == T)
        attacks = GetValidTetraMoves(0, side, block);
    else if (type == D)
        attacks = GetValidDodecaMoves(0, side, block);
    else if (type == O) {
        attacks = GetOctaMoves(0, block);
        attacks ^= (attacks & globals[0].occupancies[side]);
    }
    else if (type == C) {
        attacks = GetCubeMoves(0, block);
        attacks ^= (attacks & globals[0].occupancies[side]);
    }
    else if (type == I) {
        attacks = GetOctaMoves(0, block) | GetCubeMoves(0, block);
        attacks ^= (attacks & globals[0].occupancies[side]);
    }
    else if (type == S)
        attacks = GetValidSphereMoves(0, side, block);
    else 
        attacks = GetValidTetraMoves(0, side, block);

    Int343 legal_moves = attacks;

    int target;
    while((target = ForwardScanPop(&attacks)) >= 0) {
        if (!is_legal_move(side, type, block, target)) {
            PopBit(legal_moves, target);
        }
    }

    return legal_moves;
}

#define DllExport __attribute__(( visibility("default")))
extern "C"
{
    DllExport void InitVariables() {
        init_variables();
    }

    DllExport void InitStartPosition(char * fen) {
        init_bitboards(fen);
    }

    DllExport void MakeBBMove(int side, int type, int source, int target) {
        SetGlobalBitBoards();

        char promotion = type == T && IsPromotion(side, target);
        char capture = GetCapture(0, side, target);
        long move = encode_move(source, target, type, capture, promotion, side);
        U64 move_key = MakeTTKey(BuildTranspositionKey(), move);
        Turn turn = {move_key, move};

        MakeRealMove(turn);
    }

    DllExport long Search(int side) {
        ClearTimeNodeValues();
        
        SearchResult search_result = FindBestMove(side);
        if (search_result.flag == CHECKMATE) return CHECKMATE;
        else if (search_result.flag == STALEMATE) return STALEMATE;
        else if (search_result.flag == DRAW_GAME) return DRAW_GAME;
        
        return search_result.final_move;
    }

    DllExport int CountPieces(int side, int type) {
        return Count(Bitboards[(side * 6) + type]);
    }

    DllExport int GetMaxDepth() {
        return MAX_DEPTH;
    }

    DllExport int GetSearchDepth() {
        return DEPTH;
    }

    DllExport int GetMaxQDepth() {
        return MAX_QDEPTH;
    }

    DllExport int GetQDepth() {
        return QDEPTH;
    }

    DllExport void SetDepths(int depth, int qdepth) {
        DEPTH = depth;
        QDEPTH = qdepth;
    }

    DllExport size_t GetThreadCount() {
        return THREAD_COUNT;
    }

    DllExport void SetThreadCount(size_t thread_count) {
        THREAD_COUNT = (thread_count <= MAX_THREADS) ? thread_count : MAX_THREADS;
    }


    DllExport U64 GetBitboard(int side, int type, int index) {
        if (index >= 6 || type >= 6) return 0ULL;
        return Bitboards[(side * 6) + type].Bits[index];
    }

    DllExport U64 GetOccupancies(int side, int index) {
        if (side >= 3 || index >= 6) return 0ULL;
        return Occupancies[side].Bits[index];
    }

    DllExport int GetBBCapture(int side, int target) {
        SetGlobalBitBoards();
        return GetCapture(0, side, target);
    }

    DllExport bool IsSphereInCheck(int side) {
        SetGlobalBitBoards();
        return is_sphere_in_check(side);
    }

    //Gets all possible legal moves
    DllExport int CountLegalMoves(int side) {
        SetGlobalBitBoards();

        Int343 piece_set; Int343 moves;
        int source; int total = 0;

        for (int type = T; type <= S; type++) {
            piece_set = globals[0].bitboards[(side * 6) + type];
            while((source = ForwardScanPop(&piece_set)) >= 0) {
                total += Count(get_legal_moves(type, side, source));
            }
        }

        return total;
    }

    DllExport bool CheckDrawnGame() {
        if (TURN <= 0) return false;

        TURN--; //Hack to get the last turn and allow the IsDrawnGame methods to work.
        Turn last_turn = TurnList[TURN];
        bool is_draw = IsDrawnGame(last_turn);
        TURN++;

        return is_draw;
    }

    DllExport int EvaluateBoard(int side) {
        int result = 0;
        if (CountLegalMoves(side) == 0) {
            if (IsSphereInCheck(side))
                result = CHECKMATE;
            else
                result = STALEMATE;
        }
        else if (CheckDrawnGame()) {
            result = DRAW_GAME;
        }

        return result;
    }

    DllExport void AbortGame() {
        ABORT_GAME = true;
    }

    //Sets the possible moves to BB_VALUE
    DllExport void SetPossibleMoves(int type, int side, int block) {
        if (type == T)
            BB_VALUE = GetPossibleTetraMoves(side, block);
        else if (type == D)
            BB_VALUE = GetPossibleDodecaMoves(block);
        else if (type == O)
            BB_VALUE = GetPossibleOctaMoves(block);
        else if (type == C)
            BB_VALUE = GetPossibleCubeMoves(block);
        else if (type == I)
            BB_VALUE = GetPossibleOctaMoves(block) | GetPossibleCubeMoves(block);
        else if (type == S)
            BB_VALUE = GetPossibleSphereMoves(block);
        else 
            BB_VALUE = GetPossibleTetraMoves(side, block);
    }

    // Sets the legal moves to BB_VALUE
    DllExport void SetLegalMoves(int type, int side, int block) {
        SetGlobalBitBoards();
        BB_VALUE = get_legal_moves(type, side, block);
    }

    //Used for grabbing the info in BB_VALUE
    //We can't return Int343, so we need a helper to access the individual 64bit long values.
    DllExport U64 GetBBValue(int index) {
        return BB_VALUE.Bits[index];
    }

    DllExport long GetMoveAtTurn(int turn) {
        if (turn < 0 || turn >= TURN || turn >= MAX_TURNS) return 0;
        return TurnList[turn].move;
    }

    DllExport void AddToTurnList(long move) {
        U64 key = MakeTTKey(BuildTranspositionKey(), move);
        Turn turn = {key, move};
        AddToTurnList(turn);
    }

    DllExport int GetTurn() {
        return TURN;
    }

    DllExport int GetSide() {
        return SIDE;
    }
}

/*
* CHECK-LIST:
* Y Implement Transposition Table
* Y Implement Late Move Reduction
* Y Implement Null Move
* Y PVS Searching
* Y PV Node Check
* Y PV Move Ordering
* Y History Heuristic
* Y Killer Moves
* Y Razoring
* Y Futility Pruning
* Y Evaluation Pruning
* Y Lazy SMP / Multi-threading
* Y Mate Distance Pruning
* Y Mate Count Pruning
* Y Implement Quisesence + Delta Search
* N Probability Cut
* Y Isolated Pawn Evaluation
* Y Add Code to handle being in_check
* Y Static Exchange Evaluation
* Y SEE Pruning
* N Add Mobility Scoring to Evaluation
* Y Add Check Scoring to Evaluation
* Y Implement Search Timeout
* Y  Implement multi-thread searching during player's turn
* Y Iterative Deepening
*/
int main() {

    int debug = 1; //0

    // if debugging
    if (debug)
    {
        SelfPlay();
    }

    return 0;
}