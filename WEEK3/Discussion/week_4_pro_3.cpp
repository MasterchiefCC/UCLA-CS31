#include <iostream>
using namespace std;


int main() {
	int R = rand() % 101;
	int input;
	do {
		scanf("%d", &input);
		if (input > R) cout << "Higher!";
		if (input < R)cout << "Lower!";
	} while (input!=R);

	cout << "You got it";
}