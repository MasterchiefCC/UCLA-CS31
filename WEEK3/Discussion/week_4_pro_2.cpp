#include <iostream>
using namespace std;


int main() {
	cout << "input";
	cout << "a=";
	int a;
	scanf("%d", &a);
	cout << "b=";
	int b;
	scanf("%d", &b);
	int result = a;
	
	for (int x = 0; x < b-1; x++)
		result *= a;
	
	cout << result << endl;
}