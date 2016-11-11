#include <iostream>
#include <cstring>
#include <cctype>
#include <cassert>

const int MAX_WORD_LENGTH = 20;
const int MAX_DOC_LENGTH = 200;
void remove(char word1[][MAX_WORD_LENGTH + 1],char word2[][MAX_WORD_LENGTH + 1],int distance[], int &nRules, int &a) {
	for (int k = a + 1; k < nRules; k++) {//move string behind a one position forward
		strcpy(word1[k - 1], word1[k]);
		strcpy(word2[k - 1], word2[k]);
		distance[k - 1] = distance[k];
	}
	distance[nRules - 1] = 0;//delete end of string
	memset(word1[nRules - 1], 0, MAX_WORD_LENGTH);
	memset(word2[nRules - 1], 0, MAX_WORD_LENGTH);
	nRules--;//interest position decrees 1
	a--;
}

int normalizeRules(char word1[][MAX_WORD_LENGTH + 1],char word2[][MAX_WORD_LENGTH + 1],int distance[],int nRules) {
	if (nRules <= 0)// If n is not positive, return 0.
		return 0;

	for (int a = 0; a < nRules; a++) {
		if (distance[a] <= 0) {//Every word is at least one letter long
			remove(word1, word2, distance, nRules, a);
			continue;
		}
		else if (!strlen(word1[a]) || !strlen(word2[a])) {//empty string
			remove(word1, word2, distance, nRules, a);
			continue;
		}
		else {
			int ok = 0;
			for (int k = 0; word1[a][k] != 0; k++) {//check each char in word1 is alpha, otherwise remove whole string
				if (!isalpha(word1[a][k])) {
					remove(word1, word2, distance, nRules, a);
					ok = 1;
					break;
				}
				word1[a][k] = tolower(word1[a][k]);
			}
			if (ok)continue;
			for (int k = 0; word2[a][k] != 0; k++) {//check each char in word2 is alpha, otherwise remove whole string
				if (!isalpha(word2[a][k])) {
					remove(word1, word2, distance, nRules, a);
					break;
				}
				word2[a][k] = tolower(word2[a][k]);
			}
		}
	}
	//above, change all char to lower case and erase rules distance is not positive and contain not alpha char 
		for (int a = 0; a < nRules; a++) {//Checks for repeat rules and removes repeats
			for(int k=0; k<nRules; k++){
				if ((!strcmp(word1[a], word2[k]) && !strcmp(word1[k], word2[a])) || (a != k && !strcmp(word1[a], word1[k]) && !strcmp(word2[a], word2[k]))) {
					if (distance[a] >= distance[k]) {
						remove(word1, word2, distance, nRules, k);
						break;
					}
					else {
						remove(word1, word2, distance, nRules, a);
						break;
				}
			}
		}
	}
	return nRules;
}

int calculateSatisfaction(const char word1[][MAX_WORD_LENGTH + 1],const char word2[][MAX_WORD_LENGTH + 1],const int distance[],int nRules,const char document[]) {
	int matchedRules = 0;
	char map[MAX_DOC_LENGTH + 1][MAX_DOC_LENGTH + 1];
	int pos = 0;

	if (nRules <= 0)// If n is not positive, return 0.
		return 0;
	int len = strlen(document);


	for (int a = 0; a < len; a++) {//put every word in document to a array, which is in a two-dimension array
		if (isalpha(document[a])) {
			int t = 0;
			for (; document[a] != ' ' && a < len; a++) {
				if (isalpha(document[a]))
					map[pos][t++] = tolower(document[a]);
			}
			map[pos][t] = 0;
			pos++;
		}
	}

		for (int a = 0; a < nRules; a++) {//loop to check each rule
			int ok = 0;
			for (int k = 0; k < pos; k++) {
				if (strcmp(map[k], word1[a]) == 0) {
					for (int l = 0; l < pos; l++) {
						if ((strcmp(map[l], word2[a]) == 0) && (abs(k - l) <= distance[a]) && k != l) {
							ok++;//if match with the first rule and the second rule and the distance between two rule smaller than distance, open the switch
							break;//break
						}
					}
				}
			}
			if (ok)//if the switch is open, means rule can be applied into the document
				matchedRules++;
		}
		return matchedRules;
	}

int main() {
	const int TEST2_NRULES = 7;
	char test2w1[TEST2_NRULES][MAX_WORD_LENGTH + 1] = { "mad",        "robot", "NEFARIOUS", "half-",          "robot",        "plot",           "have" };
	char test2w2[TEST2_NRULES][MAX_WORD_LENGTH + 1] = { "scientist", "robot", "PLOT",            "assistant",    "deranged",  "Nefarious", "mad" };
	int test2dist[TEST2_NRULES] = { 2, 4, 1 ,3,2,1,13 };

	int a = normalizeRules(test2w1, test2w2, test2dist, TEST2_NRULES);


	const int TEST1_NRULES = 4;
	char test1w1[TEST1_NRULES][MAX_WORD_LENGTH + 1] = {
		"mad",       "deranged", "nefarious", "have"
	};
	char test1w2[TEST1_NRULES][MAX_WORD_LENGTH + 1] = {
		"scientist", "robot",    "plot",      "mad"
	};
	int test1dist[TEST1_NRULES] = {
		2,           4,          1,           13
	};
	assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
		"The mad UCLA scientist unleashed a deranged evil giant robot.") == 2);
	assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
		"The mad UCLA scientist unleashed    a deranged robot.") == 2);
	assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
		"**** 2016 ****") == 0);
	assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
		"  That plot: NEFARIOUS!") == 1);
	assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
		"deranged deranged robot deranged robot robot") == 1);
	assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
		"That scientist said two mad scientists suffer from deranged-robot fever.") == 0);
	
	printf("%d\n", a);


	return 0;
}
