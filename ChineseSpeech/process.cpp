#include "utility.h"

using namespace std;

#define pinyindir L"./pron/"

bool getHzPytable(string filename, unordered_map<wchar_t, wstring> &HzPytable) {		//get the table of chinese word to the chinese pinyin
	ifstream fin;
	string line;
	wstring wstr, words, pinyin;
	unordered_map<wchar_t, wstring>::const_iterator it;
	char cc[3] = { '\0' };
	char pc[3] = { '\0' };
	int i;

	fin.open(filename, ios::in | ios::binary);
	if (fin.is_open()) {
		fin.read(cc, 2);									//get the unicode file flag 0xfffe
		while (fin.read(cc, 2))
		{
			line += cc[0];
			line += cc[1];
			if ((pc[0] == '\x0d') && (pc[1] == '\x00')
				&& (cc[0] == '\x0a') && (cc[1] == '\x00'))	// change line \r\n
			{
				line.erase(line.end() - 4, line.end());
				if (line.empty())							//if the last line is \r\n
					continue;
				wstr = bytes2wstr(line);
				for (i = 0; i < wstr.length(); ++i)
				{
					if (isLetter(wstr[i]))
					{
						pinyin.push_back(wstr[i]);			//get the chinese pinyin
					}
					else
					{
						break;
					}
				}
				wstr.erase(wstr.begin(), wstr.begin() + i);//get the words
				if (!wstr.empty()) {
					wstr.erase(wstr.begin(), wstr.begin() + 1);
					for (i = 0; i < wstr.length(); ++i) {	//map the chinese word to chinese pinyin
						it = HzPytable.find(wstr[i]);		
						if (it == HzPytable.end()) {		//if polyphone esixts,use the first pron 
							HzPytable[wstr[i]] = pinyin;
						}
					}
				}
				line.clear();
				pinyin.clear();
			}
			strcpy_s(pc, cc);								//save the previous two bytes
			memset(cc, 0, 3);
		}

	}
	else
		return 0;

	fin.close();

	return 1;
}

int readThePinyin(wstring pinyin) {
	MCI_OPEN_PARMS OpenParms;																//open the device
	MCI_PLAY_PARMS PlayParms;																//play the file
	wstring pinyinfile;
	size_t error;

	pinyinfile = pinyindir;
	pinyinfile.append(pinyin).append(L".mp3");												//get the dir of the pinyin file

	OpenParms.lpstrDeviceType = NULL;														// "mpegvideo";
	OpenParms.lpstrElementName = pinyinfile.c_str();
	OpenParms.wDeviceID = NULL;
	error = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)&OpenParms);			//open the radio file
	if (error != 0)
		return -1;

	PlayParms.dwFrom = 0;																	//radio strat pos
	error = mciSendCommand(OpenParms.wDeviceID, MCI_PLAY, MCI_WAIT, (DWORD)&PlayParms);		//play the radio
	if (error != 0)
		return -1;
	error = mciSendCommand(OpenParms.wDeviceID, MCI_CLOSE, NULL, NULL);

	return 1;
}

void readWords(wstring words, const unordered_map<wchar_t, wstring>& HzPytable) {
	int i;
	unordered_map<wchar_t, wstring>::const_iterator it;

	for (i = 0; i < words.length(); ++i) {
		it = HzPytable.find(words[i]);
		if (it == HzPytable.end())
			return;
		else {
			readThePinyin((*it).second);
		}
	}
}

int main() {
	unordered_map<wchar_t, wstring> HzPytable;
	string word;
	wstring w1;

	getHzPytable("hzpytable.txt", HzPytable);
	cin >> word;
	w1 = str2wstr(word);
	readWords(w1, HzPytable);

	return 0;
}