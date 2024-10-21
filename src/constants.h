/**********************************\
 ==================================
 
             General Constants
 
 ==================================
\**********************************/

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
 
        Evaluation Constants
 
 ==================================
\**********************************/

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
///<<35 down back
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