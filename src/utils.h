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

