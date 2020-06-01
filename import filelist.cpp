#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

struct FoldCollec                          /// Simple Structure Storing Folder Data...
{
	string FName;
	int FSize;
	int Ffiles;
};

int files;                                 /// Global Variable to store Number of Files...

bool validPATH(string PATH)                /// Simple Function to check if the PATH is OK...
{
	const char* st = PATH.c_str();
	if (PATH.length() == 2)
	{
		if (st[1] == ':')
			return true;
	}
	else
	{
		if (st[1] == ':' && st[2] == '\\' && st[PATH.length()] != '\\')
			/// Drive should be 1 letter only, Next char to drive should be \\ and last character shouldn't be \\ ...
			return true;
		return false;
	}
}

int FindFolder(string P, string n, int fs) /// Searches for a Folder in the Path Entered...
{
	srand(time(NULL));
	int ic = 0;
	WIN32_FIND_DATAA FindFileData;
	HANDLE hF;
	string sP;
	P = n.data();
	P.append("\\");
	sP = P;
	sP.append("*");
	hF = FindFirstFileA(sP.data(), &FindFileData);
	do {
		fs += FindFileData.nFileSizeLow;
		if (FindFileData.dwFileAttributes == 16) // 16 represents a folder...
		{
			ic++;
			if (ic > 2)
				FindFolder(P, FindFileData.cFileName, FindFileData.nFileSizeLow);
		}
		else
			files++;
	} while (FindNextFileA(hF, &FindFileData));
	FindClose(hF);
	return fs; //returns File Size...
}

int main()
{
	cout << "\t\t\t   FOLDER DETAILS EXTRACTOR" << endl;
	cout << "\t\t\t   ------------------------" << endl << endl;
	string UP, sT, sP;
	cout << "Enter The Path to Check Folders From - " << endl << endl;
	getline(cin, UP);
	cout << endl;
	if (validPATH(UP))
	{
		sT = UP;
		sT.append("\\");
	}
	else
		cout << "INVALID PATH! ABORTING...", exit(EXIT_SUCCESS);
	WIN32_FIND_DATAA FindFileData;
	int ic = 0;
	HANDLE hF;
	vector<FoldCollec>V;
	FoldCollec Ob;
	sP = sT;
	sP.append("*");
	hF = FindFirstFileA(sP.data(), &FindFileData);
	do {
		if (FindFileData.dwFileAttributes == 16)
		{
			ic++;
			if (ic > 2)
			{
				Ob.FName.assign(FindFileData.cFileName);
				files = 0;
				Ob.FSize = FindFolder(sT, FindFileData.cFileName, 0);
				Ob.Ffiles = files;
				V.push_back(Ob);
			}
		}
	} while (FindNextFileA(hF, &FindFileData));
	FindClose(hF);
	ofstream fout;
	string gP = "F:\\Invoice - " + V.at(rand() % (ic - 2)).FName + ",etc. - Folder List.txt";
	fout.open(gP.c_str(), ios::trunc);
	for (int i = 0; i < V.size(); i++)
	{
		cout << "Folder Name - " << V.at(i).FName.data() << endl;
		cout << "Folder Size - " << V.at(i).FSize << " B" << endl;
		cout << "Folder Files - " << V.at(i).Ffiles << endl << endl << endl;
		fout << V.at(i).FName.data() << " - " << V.at(i).Ffiles << endl;
	}
	fout.close();
	cout << "Total Folders - " << ic - 2 << endl << endl;
	return 0;
}
