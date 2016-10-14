#include <iostream>
using namespace std;


int main() {
	int human;
	scanf("%d", &human);

	int computer;
	do {
		computer = rand() % 101;
		if (computer > human)
			printf("I guess %d. Was my guess higher (h) or lower (l) or correct (c)?\nh\n", computer);
		if (computer < human)
			printf("I guess %d. Was my guess higher (h) or lower (l) or correct (c)?\nh\n", computer);
	} while (computer!=human);
	printf("I guess %d. Was my guess higher (h) or lower (l) or correct (c)?\nc\n", computer);
}