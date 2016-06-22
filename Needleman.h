#pragma once
#include <vector>
#include "FASTA.h"
#include "DNAAlignDlg.h"
#include <fstream>

enum Pointers : char
{
	NULL_P = 0,
	UP_LEFT = 1,
	LEFT = 2,
	UP = 3,
};

class Needleman
{
public:
	void VerifyInputs(DNAAlignDlg& files);
	void RunNeedleman();
	void PrintResult();

private:
	DNAAlignDlg mFilePaths;

	FASTA* mInputPathA = nullptr;
	FASTA* mInputPathB = nullptr;
	std::string mOutputPath;
	std::ofstream mOutStream;

	std::string mInputA;
	std::string mInputB;

	std::vector<std::vector<short int>> mScoreGrid;
	std::vector<std::vector<Pointers>> mDirectionGrid;

	std::string mSequenceA;
	std::string mSequenceB;

	int mFinalScore = 0;

};