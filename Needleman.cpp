#include "Needleman.h"
#include "Exceptions.h"
#include "FASTA.h"
#include <wx/msgdlg.h>

void Needleman::VerifyInputs(DNAAlignDlg& files)
{
	std::string inputA = files.GetInputAPath();
	std::string inputB = files.GetInputBPath();
	std::string mOutputPath = files.GetOutputPath();
	mInputPathA = new FASTA(inputA);
	mInputPathB = new FASTA(inputB);

	try
	{
		mInputA = mInputPathA->GetSequence();
		mInputB = mInputPathB->GetSequence();
		if (mInputA.size() == 0 || mInputB.size() == 0)
		{
			wxMessageBox("Error retrieving data from FASTA file(s)", "Error", wxOK | wxICON_ERROR);
			throw FileLoadExcept();
		}
		mOutStream = std::ofstream(mOutputPath);
		if (!mOutStream.is_open())
		{
			wxMessageBox("Error opening output file", "Error", wxOK | wxICON_ERROR);
			throw FileLoadExcept();
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	for (unsigned int i = 0; i < mInputB.size(); i++)
	{
		mScoreGrid.push_back({});
		mDirectionGrid.push_back({});
	}
	for (unsigned int i = 0; i < mScoreGrid.size(); i++)
	{
		for (unsigned int v = 0; v < mInputA.size(); v++)
		{
			mScoreGrid[i].push_back(NULL);
			mDirectionGrid[i].push_back(NULL_P);
		}
	}
}

void Needleman::RunNeedleman()
{
	mScoreGrid[0][0] = 0;
	for (unsigned int i = 1; i < mScoreGrid[0].size(); i++)
	{
		mScoreGrid[0][i] = i*-1;
		mDirectionGrid[0][i] = LEFT;
	}
	for (unsigned int i = 1; i < mScoreGrid.size(); i++)
	{
		mScoreGrid[i][0] = i*-1;
		mDirectionGrid[i][0] = UP;
	}

	int upLeft = 0;
	int up = 0;
	int left = 0;
	int currScore = 0;
	Pointers currPoint = NULL_P;

	//up-left = [i-1][v-1]
	//up = [i-1][v]
	//left = [i][v-1]
	//Loop through score grid left to right, top to bottom
	for (unsigned int i = 1; i < mScoreGrid.size(); i++)
	{
		for (unsigned int v = 1; v < mScoreGrid[i].size(); v++)
		{
			//determine possible scores for current cell
			if (mInputA[v-1] == mInputB[i-1])
			{
				upLeft = mScoreGrid[i - 1][v - 1] + 1;
				up = mScoreGrid[i - 1][v] - 1;
				left = mScoreGrid[i][v - 1] - 1;
			}
			else
			{
				upLeft = mScoreGrid[i - 1][v - 1] - 1;
				up = mScoreGrid[i - 1][v] - 1;
				left = mScoreGrid[i][v - 1] - 1;
			}
			//determine which is greatest
			if (upLeft > up && upLeft > left)
			{
				currScore = upLeft;
				currPoint = UP_LEFT;
			}
			else if (up > upLeft && up > left)
			{
				currScore = up;
				currPoint = UP;
			}
			else if (left > upLeft && left > up)
			{
				currScore = left;
				currPoint = LEFT;
			}
			else
			{
				if (upLeft == up || upLeft == left)
				{
					currScore = upLeft;
					currPoint = UP_LEFT;
				}
				else if (up == left)
				{
					currScore = left;
					currPoint = LEFT;
				}
				else
				{
					currScore = up;
					currPoint = UP;
				}
			}
			//set current cell to greates score and direction cell to correct direction
			mScoreGrid[i][v] = currScore;
			mDirectionGrid[i][v] = currPoint;
		}
	}
	unsigned int row = mInputA.size() - 1;
	unsigned int col = mInputB.size() - 1;
	if (mInputA.size() > mInputB.size())
	{
		for (unsigned int i = 0; i < mInputA.size()-1; i++)
		{
			mSequenceA.push_back(' ');
			mSequenceB.push_back(' ');
		}
	}
	else
	{
		for (unsigned int i = 0; i < mInputB.size()-1; i++)
		{
			mSequenceA.push_back(' ');
			mSequenceB.push_back(' ');
		}
	}
	unsigned int seq = 0;

	try
	{
		mFinalScore = mScoreGrid[col][row];
		while (row >= 0 && col >= 0)
		{
			switch (mDirectionGrid[col][row])
			{
			case 0:
				return;
			case 1:
				mSequenceA[seq] = mInputA[row-1];
				mSequenceB[seq] = mInputB[col-1];
				row--;
				col--;
				seq++;
				break;
			case 2:
				mSequenceA[seq] = mInputA[row-1];
				mSequenceB[seq] = '_';
				row--;
				seq++;
				break;
			case 3:
				mSequenceA[seq] = '_';
				mSequenceB[seq] = mInputB[col-1];
				col--;
				seq++;
				break;
			}
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}

void Needleman::PrintResult()
{
	std::reverse(mSequenceA.begin(), mSequenceA.end());
	std::reverse(mSequenceB.begin(), mSequenceB.end());

	mOutStream << "A: " << mInputPathA->GetHeader() << "\nB: " << mInputPathB->GetHeader() << "\nScore: " << mFinalScore << "\n\n";

	unsigned int aCount = 0;
	unsigned int bCount = 0;
	unsigned int midCount = 0;
	while (bCount < mSequenceA.size())
	{
		while (aCount < mSequenceA.size())
		{
			mOutStream << mSequenceA[aCount];
			aCount++;
			if (aCount % 70 == 0)
			{
				break;
			}
		}
		mOutStream << "\n";
		while (midCount < mSequenceA.size())
		{
			if (mSequenceA[midCount] == mSequenceB[midCount])
			{
				mOutStream << "|";
			}
			else
			{
				mOutStream << " ";
			}
			midCount++;
			if (midCount % 70 == 0)
			{
				break;
			}
		}
		mOutStream << "\n";
		while (bCount < mSequenceB.size())
		{
			mOutStream << mSequenceB[bCount];
			bCount++;
			if (bCount % 70 == 0)
			{
				break;
			}
		}
		mOutStream << "\n\n";
	}
}
