#include <chrono>
using namespace std;

#define INT343_SIZE 343

void TestSetGetPop() {
	StartTimer();

	Int343 a;
	int test_length = 100000000;
	for (int i = 0; i < test_length; i++) {
		SetBit(a, i % INT343_SIZE);
		GetBit(a, i % INT343_SIZE);
	}

	for (int i  = 0; i < test_length; i++) {
		PopBit(a, i % INT343_SIZE);
		IsSet(a);
	}

	long long ts = GetTimePassed();
	printf("TestSetGetPop|Size=%d|Time=%s\n", test_length, GetTimeStampString(ts).c_str());

}

void TestOperators() {
	StartTimer();

	Int343 a; Int343 b; Int343 c;

	for (int i = 0; i < INT343_SIZE; i++) {
		if ((i % 2) == 0) SetBit(a, i);
		else SetBit(b, i);
	}

	int test_length = 100000000;
	bool test_bool = false;
	for (int i = 0; i < test_length; i++) {
		a |= b;
		a &= b;
		c = a | b;
		c = a & b;
		a ^= b;
		a ^= b; //here to flip a back to original state
		c = a ^ b;
		c = ~a;
		test_bool = (a == b);
		test_bool = !a;
	}

	long long ts = GetTimePassed();
	printf("TestOperators|Size=%d|Time=%s\n", test_length, GetTimeStampString(ts).c_str());
}

void TestLeftRightShifts() {
	StartTimer();

	int test_length = 100000000;
	Int343 a; Int343 b;

	for (int i = 0; i < test_length; i++) {
		SetBit(a, 0);
		b = a << i % INT343_SIZE;
		b = a >> i % INT343_SIZE;
	}

	long long ts = GetTimePassed();
	printf("TestLeftRightShifts|Size=%d|Time=%s\n", test_length, GetTimeStampString(ts).c_str());
}

void TestBitScanMethods() {
	StartTimer();

	Int343 bb; Int343 copy;

	int test_length = 100000000;
	int index = 0;
	int pos = 0;

	for (int i = 0; i < test_length; i++) {
		pos = i % INT343_SIZE;

		SetBit(bb, pos);
		index = BitScanForward(bb);
		PopBit(bb, index);

		if (index != pos) {
			printf("BitScanForward ERROR - index %d should be %d", index, pos);
			break;
		}

		SetBit(bb, pos);
		index = BitScanReverse(bb);
		PopBit(bb, index);

		if (index != pos) {
			printf("BitScanReverse ERROR - index %d should be %d", index, pos);
			break;
		}
	}

	long long ts = GetTimePassed();
	printf("TestBitScanMethods|Size=%d|Time=%s\n", test_length, GetTimeStampString(ts).c_str());
}

void TestForwardScanPop() {
	StartTimer();

	int test_length = 10000000;
	int index = 0;
	Int343 bb; Int343 copy;

	for (int i = 0; i < INT343_SIZE; i++) {
		SetBit(bb, i); //Set all bits
	}

	copy = bb;

	for (int i = 0; i < test_length; i++) {
		//Pop all bits
		while ((index = ForwardScanPop(&bb)) >= 0){ 
		}

		bb = copy; //reset bb
	}

	long long ts = GetTimePassed();
	printf("TestForwardScanPop|Size=%d|Time=%s\n", test_length, GetTimeStampString(ts).c_str());
}

void TestGetFirstBit() {
	StartTimer();

	Int343 bb;
	int test_length = 100000000;

	for (int i = 0; i < test_length; i++) {
		SetBit(bb, i % INT343_SIZE);
		GetFirstBit(bb); //Need to confirm this value 
	}

	long long ts = GetTimePassed();
	printf("TestGetFirstBit|Size=%d|Time=%s\n", test_length, GetTimeStampString(ts).c_str());
}

