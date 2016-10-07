#include <iostream>
#include <string>
using namespace std;

int main() {
	printf("Identification: ");
	string identification;
	getline(cin, identification);
	if (identification == "")
		cout << "You must enter a property identification.\n";

	printf("Expected revenue (in millions): ");
	double revenue;
	scanf_s("%lf", &revenue);
	if (revenue <0)
		cout << "The expected revenue must be nonnegative.\n"; 
	cin.ignore(10000, '\n');
	
	printf("Country:");
	string Country;
	getline(cin, Country);
	if (Country == "")
		cout << "You must enter a country.";
	
	double result;
	if (Country == "UAE" || Country == "Turkey") {
		if (revenue <= 20)result = revenue*0.181;
		if (revenue > 20 && revenue <= 50)result = 3.62 + (revenue - 20)*0.217;
		if (revenue > 50)result = 3.62 + 6.51 + (revenue - 50)*0.23;
	}
	else {
		if (revenue <= 20)result = revenue*0.181;
		if (revenue > 20 && revenue <= 50)result = 3.62 + (revenue - 20)*0.203;
		if (revenue > 50)result = 3.62 + 6.09 + (revenue - 50)*0.23;
	}
	printf("---\nThe license fee for %s is $%.3lf million.\n", identification.c_str(), result);

}
