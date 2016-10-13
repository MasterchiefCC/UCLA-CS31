#include <iostream>
#include <string>
using namespace std;

int main() {
	//variables and judgement
	printf("Identification: ");
	string identification;
	getline(cin, identification);
	if (identification == "") {
		cout << "---\nYou must enter a property identification.\n";
		return 0;
	}

	//variables and judgement
	printf("Expected revenue (in millions): ");
	double revenue;
	scanf("%lf", &revenue);
	if (revenue < 0) {
		cout << "---\nThe expected revenue must be nonnegative.\n";
		return 0;
	}
	cin.ignore(10000, '\n');

	//variables and judgement
	printf("Country:");
	string Country;
	getline(cin, Country);
	if (Country == "") {
		cout << "---\nYou must enter a country."; return 0;
	}

	//judgement
	double result;
	if (Country == "UAE" || Country == "Turkey") {
		if (revenue <= 20) {
			result = revenue*0.181;
			printf("---\nThe license fee for %s is $%.3lf million.\n", identification.c_str(), result);
		}

		else if (revenue > 20 && revenue <= 50) {
			result = 3.62 + (revenue - 20)*0.217;
			printf("---\nThe license fee for %s is $%.3lf million.\n", identification.c_str(), result);
		}

		else if (revenue > 50) {
			result = 3.62 + 6.51 + (revenue - 50)*0.23;
			printf("---\nThe license fee for %s is $%.3lf million.\n", identification.c_str(), result);
		}
		else printf("---\nThe license fee for identification is $amount million.\n");
	}
	else {
		if (revenue <= 20) {
			result = revenue*0.181;
			printf("---\nThe license fee for %s is $%.3lf million.\n", identification.c_str(), result);
		}
		else if (revenue > 20 && revenue <= 50) {
			result = 3.62 + (revenue - 20)*0.203;
			printf("---\nThe license fee for %s is $%.3lf million.\n", identification.c_str(), result);
		}
		else if (revenue > 50) {
			result = 3.62 + 6.09 + (revenue - 50)*0.23;
			printf("---\nThe license fee for %s is $%.3lf million.\n", identification.c_str(), result);
		}
		else printf("---\nThe license fee for identification is $amount million.\n");
	}
	//end
}
