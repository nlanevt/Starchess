using namespace std;


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
 
           Timer
 
 ==================================
\**********************************/
chrono::time_point<chrono::steady_clock> START_TIME;

void StartTimer() {
    START_TIME = chrono::steady_clock::now();
}

//Returns the duration of time in microseconds passed since start time.
inline long long GetTimePassed() {
    auto finish = chrono::steady_clock::now();
    auto microseconds = chrono::duration_cast<chrono::microseconds>(finish - START_TIME);
    return microseconds.count();
}

string GetTimeStampString(long long timestamp) {
    long milliseconds = (long) (timestamp / 1000) % 1000;
    long seconds = (((long) (timestamp / 1000) - milliseconds)/1000) % 60;
    long minutes = (((((long) (timestamp / 1000) - milliseconds)/1000) - seconds)/60) % 60;
    return to_string(minutes) + ":" + to_string(seconds) + ":" + to_string(milliseconds);
}

/**********************************\
 ==================================
 
           Input & Output
 
 ==================================
\**********************************/

void print_bb(Int343 bitboard) {
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

void print_full_board_vertical(const int turn, const int side, const Int343 * Bitboards) {
    string piece = "0";
    int block = 0;
    bool piece_found = false;
    
    printf("Turn %d - %s\n", turn, side ? "Black" : "White");
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

//TODO - Complete
void print_full_board_horizontal(const int turn, const int side, const Int343 * Bitboards) {
    string piece = "0";
    int block = 0;
    bool piece_found = false;
    
    printf("Turn %d - %s\n", turn, side ? "Black" : "White");

    printf("*");
    for (int rank = 0; rank < 49; rank++) {
        printf(" %c", YSymbols[rank % 7]);
        if (rank % 7 == 6) printf(" ");
    }

    printf("\n");

    for (int file = 0; file < 7; file++) {
        printf("%c", XSymbols[file]);
        for (int depth = 0; depth < 7; depth++) {

            for (int rank = 0; rank < 7; rank++) {

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
                    printf(" %s", piece.c_str());
                else
                    printf(" %s", piece.c_str());

            }
            printf(" ");
        }
        printf("\n");
    }

    for (int depth = 0; depth < 7; depth++) {
        printf("        %c      ", ZSymbols[depth]);
    }

    printf("\n");
}

void print_fen_string(const int side, const Int343 * Bitboards) {
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

    printf(" %c", (side) ? 'b' : 'w');
}

void load_bar() {
    cout << "|" ;
    fflush(stdout);
}

/**********************************\
 ==================================
 
           OTHER
 
 ==================================
\**********************************/

string ToStringU128(U128 num) {
    string str;
    do {
        int digit = num % 10;
        str = to_string(digit) + str;
        num = (num - digit) / 10;
    } while (num != 0);
    return str;
}

#define MaxValue(a, b) (a >= b ? a : b)

#define tti(side, type) (((side) * 6) + type) //Type to Index conversion, for accessing Bitboards

inline string asciiToString(char type) {
    type = type % 6;
    switch (type) {
        case T:
            return "Tetra";
        case D:
            return "Dodeca";
        case O:
            return "Octa";
        case C:
            return "Cube";
        case I:
            return "Icosa";
        case S:
            return "Sphere";
    }

    return "";

}


