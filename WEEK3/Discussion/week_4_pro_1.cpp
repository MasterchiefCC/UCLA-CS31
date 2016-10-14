#include <iostream>
using namespace std;

int main() {
	int age;
	char name[128];
	printf("input:");
	scanf("%d,%s", &age, name);
	cout << "output: Hello " << name << ", I see you are " << age << " years old.\n";
	return 0;
}