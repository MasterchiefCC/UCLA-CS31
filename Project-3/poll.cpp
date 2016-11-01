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
			if (a + 1 >= pollData.size()||!isalpha(pollData[a+1]))return 0;//next position is a letter
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
			if (pollData[a + 1] == '0' || isalpha(pollData[a + 1]))
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

int main(){
	assert(!hasCorrectSyntax("C11D"));
	assert(!hasCorrectSyntax("CA11")); 
	assert(!hasCorrectSyntax("CA11DTX"));
	assert(!hasCorrectSyntax("CA11TX3D"));
	assert(!hasCorrectSyntax("CD1D"));
	assert(!hasCorrectSyntax("AL38dC55D")); 
	assert(!hasCorrectSyntax("AL38DCa5"));
	assert(hasCorrectSyntax("Ca1d"));
	assert(hasCorrectSyntax("ny29dtx38rhi4d"));
	assert(!hasCorrectSyntax("cA1dal 38lak40r"));
	assert(!hasCorrectSyntax("asdfdfsf"));
	int votes;
	votes = -999;  
	assert(countVotes("TX38RCA55DMs6rnY29dUT06Lar3dak03d", 'd', votes) == 0 && votes == 90);
	votes = -999;  
	assert(countVotes("TX38RCA55D", '%', votes) == 3 && votes == -999);
	assert(countVotes("TX00RCA55D", 'd', votes) == 2 && votes == -999);
	assert(countVotes("TX0RCA55D", 'd', votes) == 2 && votes == -999);
	assert(countVotes("", 'd', votes) == 0 && votes == 0);
	cout << "All tests succeeded" << endl;
	return 0;
}