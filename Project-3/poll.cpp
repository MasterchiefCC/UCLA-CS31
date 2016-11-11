#include <iostream>
#include <string>
#include<cctype>
#include <cassert>
using namespace std;

bool isValidUppercaseStateCode(string stateCode)
{
	const string codes =
		"AL.AK.AZ.AR.CA.CO.CT.DE.DC.FL.GA.HI.ID.IL.IN.IA.KS."
		"KY.LA.ME.MD.MA.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC."
		"ND.OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
	return (stateCode.size() == 2 &&
		stateCode.find('.') == string::npos  &&  // no '.' in stateCode
		codes.find(stateCode) != string::npos);  // match found
}

bool hasCorrectSyntax(string pollData) {
	int kase = 0;
	if (pollData.size() == 0)return 1;
	if (pollData.size() < 4) return 0;
	for (int a = 0;
		a < pollData.size();
		a++) {
		if (!isdigit(pollData[a]) && !isalpha(pollData[a]))
			return 0;

		if (isdigit(pollData[a])) {
			if (a - 2 >= 0 && isalpha(pollData[a - 1]) && isalpha(pollData[a - 2])) {
				string state;
				state += toupper(pollData[a - 2]);
				state += toupper(pollData[a - 1]);
				if (!isValidUppercaseStateCode(state))return 0;
			}
			else return 0;
			if (a + 1 < pollData.size() && isdigit(pollData[a + 1]))++a;//move to the end of digit
			if (a + 1 >= pollData.size() || !isalpha(pollData[a + 1]))return 0;//next position is a letter
			if (++a == (pollData.size() - 1)) { kase++; break; }
			for (int k = 1; k <= 2; k++) {//if the position is not in the end of string, next two position should be letter
				if (!(a + k < pollData.size()) || !isalpha(pollData[a + k]))return 0;
				if (k == 2)
					if ((a + k + 1) >= pollData.size() || !isdigit(pollData[a + k + 1]))return 0;
			}
			kase++;
		}
	}

	return kase;
}

int countVotes(string pollData, char party, int& voteCount) {
	if (!hasCorrectSyntax(pollData))return 1;//if can't pass the function 1, return 1

	for (int a = 0; a < pollData.size(); a++) {//judge whether contain 0 or 00
		if (pollData[a] == '0') {
			if (pollData[a+1]=='0'||(isalpha(pollData[a+1])&&isalpha(pollData[a-1])))
				return 2;
		}
	}

	if (!isalpha(party))return 3;//if the party input is not a valid char return 3

	if (pollData.size() == 0) {
		voteCount = 0;
		return 0;
	}

	int result = 0;

	for (int a = 0; a < pollData.size(); a++) {//add the result
		if (isdigit(pollData[a])) {
			if (isdigit(pollData[a + 1])) {
				++a;
				if (tolower(party) == tolower(pollData[a + 1]))
					result += (pollData[a - 1] - '0') * 10 + (pollData[a] - '0');
			}
			else {
				if (tolower(party) == tolower(pollData[a + 1]))
					result += pollData[a] - '0';
			}
		}
	}
	voteCount = result;
	return 0;
}

void testone(int n)
{
	int v = 666;
	switch (n)
	{
	default: {
		cout << "Bad argument" << endl;
	} break; case  1: {
		assert(!hasCorrectSyntax("3"));
	} break; case  2: {
		assert(!hasCorrectSyntax("#"));
	} break; case  3: {
		assert(!hasCorrectSyntax("3A"));
	} break; case  4: {
		assert(!hasCorrectSyntax("#A"));
	} break; case  5: {
		assert(!hasCorrectSyntax("C3"));
	} break; case  6: {
		assert(!hasCorrectSyntax("C#"));
	} break; case  7: {
		assert(!hasCorrectSyntax("CX"));
	} break; case  8: {
		assert(!hasCorrectSyntax("CA"));
	} break; case  9: {
		assert(!hasCorrectSyntax("CA3"));
	} break; case 10: {
		assert(!hasCorrectSyntax("CA73"));
	} break; case 11: {
		assert(!hasCorrectSyntax("CAD"));
	} break; case 12: {
		assert(!hasCorrectSyntax("CA$"));
	} break; case 13: {
		assert(!hasCorrectSyntax("CA7%"));
	} break; case 14: {
		assert(!hasCorrectSyntax("CA73%"));
	} break; case 15: {
		assert(!hasCorrectSyntax("CA738"));
	} break; case 16: {
		assert(!hasCorrectSyntax("CA738D"));
	} break; case 17: {
		assert(!hasCorrectSyntax("CA006D"));
	} break; case 18: {
		assert(!hasCorrectSyntax("XU44D"));
	} break; case 19: {
		assert(!hasCorrectSyntax("Xu44D"));
	} break; case 20: {
		assert(!hasCorrectSyntax("xU44D"));
	} break; case 21: {
		assert(!hasCorrectSyntax("xu44D"));
	} break; case 22: {
		assert(!hasCorrectSyntax("CA 55D"));
	} break; case 23: {
		assert(!hasCorrectSyntax("TX12R CA55D"));
	} break; case 24: {
		assert(!hasCorrectSyntax("TX12R3NY29D"));
	} break; case 25: {
		assert(!hasCorrectSyntax("TX12R#NY29D"));
	} break; case 26: {
		assert(!hasCorrectSyntax("TX12R3ANY29D"));
	} break; case 27: {
		assert(!hasCorrectSyntax("TX12R#ANY29D"));
	} break; case 28: {
		assert(!hasCorrectSyntax("TX12RC3NY29D"));
	} break; case 29: {
		assert(!hasCorrectSyntax("TX12RC#NY29D"));
	} break; case 30: {
		assert(!hasCorrectSyntax("TX12RCXNY29D"));
	} break; case 31: {
		assert(!hasCorrectSyntax("TX12RCANY29D"));
	} break; case 32: {
		assert(!hasCorrectSyntax("TX12RCA3NY29D"));
	} break; case 33: {
		assert(!hasCorrectSyntax("TX12RCA73NY29D"));
	} break; case 34: {
		assert(!hasCorrectSyntax("TX12RCADNY29D"));
	} break; case 35: {
		assert(!hasCorrectSyntax("TX12RCA$NY29D"));
	} break; case 36: {
		assert(!hasCorrectSyntax("TX12RCA7%NY29D"));
	} break; case 37: {
		assert(!hasCorrectSyntax("TX12RCA73%NY29D"));
	} break; case 38: {
		assert(!hasCorrectSyntax("TX12RCA738NY29D"));
	} break; case 39: {
		assert(!hasCorrectSyntax("TX12RCA738DNY29D"));
	} break; case 40: {
		assert(!hasCorrectSyntax("TX12RCA006DNY29D"));
	} break; case 41: {
		assert(!hasCorrectSyntax("TX12RCA 55DNY29D"));
	} break; case 42: {
		assert(hasCorrectSyntax(""));
	} break; case 43: {
		assert(hasCorrectSyntax("Ca4D"));
	} break; case 44: {
		assert(hasCorrectSyntax("cA4D"));
	} break; case 45: {
		assert(hasCorrectSyntax("ca4D"));
	} break; case 46: {
		assert(hasCorrectSyntax("CA42D"));
	} break; case 47: {
		assert(hasCorrectSyntax("Ca42D"));
	} break; case 48: {
		assert(hasCorrectSyntax("cA42D"));
	} break; case 49: {
		assert(hasCorrectSyntax("ca42D"));
	} break; case 50: {
		assert(hasCorrectSyntax("CA9D"));
	} break; case 51: {
		assert(hasCorrectSyntax("CA4D"));
	} break; case 52: {
		assert(hasCorrectSyntax("CA0D"));
	} break; case 53: {
		assert(hasCorrectSyntax("CA89D"));
	} break; case 54: {
		assert(hasCorrectSyntax("CA09D"));
	} break; case 55: {
		assert(hasCorrectSyntax("CA00D"));
	} break; case 56: {
		assert(hasCorrectSyntax("CA4d"));
	} break; case 57: {
		assert(hasCorrectSyntax("CA42d"));
	} break; case 58: {
		assert(hasCorrectSyntax("CA4Z"));
	} break; case 59: {
		assert(hasCorrectSyntax("CA42Z"));
	} break; case 60: {
		assert(hasCorrectSyntax("ND3RCT7D"));
	} break; case 61: {
		assert(hasCorrectSyntax("OH18RCT7D"));
	} break; case 62: {
		assert(hasCorrectSyntax("ND3RIL20D"));
	} break; case 63: {
		assert(hasCorrectSyntax("OH18RIL20D"));
	} break; case 64: {
		assert(hasCorrectSyntax("ND3RCT7DOR7DID4RSD3RRI4D"));
	} break; case 65: {
		assert(hasCorrectSyntax("CA55DWA12DTX38RMO10RTN11RMN10DNY29D"));
	} break; case 66: {
		assert(countVotes("3#QQ## QQ####", 'D', v) == 1);
	} break; case 67: {
		countVotes("3#QQ## QQ####", 'D', v);
		assert(v == 666);
	} break; case 68: {
		assert(countVotes("CA0D", 'D', v) == 2);
	} break; case 69: {
		countVotes("CA0D", 'D', v);
		assert(v == 666);
	} break; case 70: {
		assert(countVotes("CA00D", 'D', v) == 2);
	} break; case 71: {
		countVotes("CA00D", 'D', v);
		assert(v == 666);
	} break; case 72: {
		assert(countVotes("ND3RCT7DCA0DSD3R", 'D', v) == 2);
	} break; case 73: {
		assert(countVotes("TX38RNY29DCA00DTN11R", 'D', v) == 2);
	} break; case 74: {
		assert(countVotes("CA0D", 'R', v) == 2);
	} break; case 75: {
		assert(countVotes("CA00D", 'R', v) == 2);
	} break; case 76: {
		assert(countVotes("ND3RCT7DCA0DSD3R", 'R', v) == 2);
	} break; case 77: {
		assert(countVotes("TX38RNY29DCA00DTN11R", 'R', v) == 2);
	} break; case 78: {
		assert(countVotes("WV5R", '5', v) == 3);
	} break; case 79: {
		countVotes("WV5R", '5', v);
		assert(v == 666);
	} break; case 80: {
		assert(countVotes("TX38R", '5', v) == 3);
	} break; case 81: {
		countVotes("TX38R", '5', v);
		assert(v == 666);
	} break; case 82: {
		int r = countVotes("3#QQ## QQ####", '%', v);
		assert(r == 1 || r == 3);
	} break; case 83: {
		int r = countVotes("CA0D", '%', v);
		assert(r == 2 || r == 3);
	} break; case 84: {
		assert(countVotes("SD3R", 'R', v) == 0 && v == 3);
	} break; case 85: {
		assert(countVotes("SD3r", 'R', v) == 0 && v == 3);
	} break; case 86: {
		assert(countVotes("SD3R", 'r', v) == 0 && v == 3);
	} break; case 87: {
		assert(countVotes("SD3r", 'r', v) == 0 && v == 3);
	} break; case 88: {
		assert(countVotes("NY29D", 'D', v) == 0 && v == 29);
	} break; case 89: {
		assert(countVotes("NY29d", 'D', v) == 0 && v == 29);
	} break; case 90: {
		assert(countVotes("NY29D", 'd', v) == 0 && v == 29);
	} break; case 91: {
		assert(countVotes("NY29d", 'd', v) == 0 && v == 29);
	} break; case 92: {
		assert(countVotes("UT6L", 'D', v) == 0 && v == 0);
	} break; case 93: {
		assert(countVotes("WA11G", 'D', v) == 0 && v == 0);
	} break; case 94: {
		assert(countVotes("ND3RCT7DOR7DID4RSD3RRI4D", 'D', v) == 0 && v == 18);
	} break; case 95: {
		assert(countVotes("ND3RCT7DOR7DID4RSD3RRI4D", 'R', v) == 0 && v == 10);
	} break; case 96: {
		assert(countVotes("ND3RCT7DOR7DID4RSD3RRI4D", 'Z', v) == 0 && v == 0);
	} break; case 97: {
		assert(countVotes("CA55DWA12DTX38RMO10RTN11RMN10DNY29D", 'D', v) == 0 && v == 106);
	} break; case 98: {
		assert(countVotes("CA55DWA12DTX38RMO10RTN11RMN10DNY29D", 'R', v) == 0 && v == 59);
	} break; case 99: {
		assert(countVotes("CA55DWA12DTX38RMO10RTN11RMN10DNY29D", 'Z', v) == 0 && v == 0);
	} break; case 100: {
		assert(countVotes("CA55DWA12DTX38RMO10RTN11RMN10DNY29DND3RCT7DOR7DID4RSD3RRI4D", 'D', v) == 0 && v == 124);
	}
	}
}

int main()
{
	for (int a = 1; a <=100; a++) {
		testone(a);
	}
	cout << "Passed!" << endl;
}
