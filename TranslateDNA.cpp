#include "TranslateDNA.h"
#include <wx/msgdlg.h>

Translation::Translation()
{
	mBases.insert(std::make_pair('F', "Phenylalanine"));
	mBases.insert(std::make_pair('L', "Leucine"));
	mBases.insert(std::make_pair('I', "Isoleucine"));
	mBases.insert(std::make_pair('M', "Methionine"));
	mBases.insert(std::make_pair('V', "Valine"));
	mBases.insert(std::make_pair('S', "Serine"));
	mBases.insert(std::make_pair('P', "Proline"));
	mBases.insert(std::make_pair('T', "Threonine"));
	mBases.insert(std::make_pair('A', "Alanine"));
	mBases.insert(std::make_pair('Y', "Tyrosine"));
	mBases.insert(std::make_pair('H', "Histidine"));
	mBases.insert(std::make_pair('Q', "Glutamine"));
	mBases.insert(std::make_pair('N', "Asparagine"));
	mBases.insert(std::make_pair('K', "Lysine"));
	mBases.insert(std::make_pair('D', "Aspartic acid"));
	mBases.insert(std::make_pair('E', "Glutamic acid"));
	mBases.insert(std::make_pair('C', "Cysteine"));
	mBases.insert(std::make_pair('W', "Tryptophan"));
	mBases.insert(std::make_pair('R', "Arginine"));
	mBases.insert(std::make_pair('G', "Glycine"));

	for (auto it = mBases.begin(); it != mBases.end(); ++it)
	{
		mHistoValues.insert(std::make_pair(it->second, 0));
	}

	mStateMachine = {
	//  | T |  C |  A |  G  |
		//0
		{ 0,   0,   1,   0 },
		//1 
		{ 2,   0,   1,   0 },
		//2
		{ 0,   0,   1,  'M' },
		//3
		{ 4,   9,   14,  19 },
		//4
		{ 5,   6,   7,   8 },
		//5
		{ 'F', 'F', 'L', 'L' },
		//6
		{ 'S', 'S', 'S', 'S' },
		//7
		{ 'Y', 'Y',  0,   0 },
		//8
		{ 'C', 'C',  0,  'W' },
		//9
		{ 10,  11,  12,  13 },
		//10
		{ 'L', 'L', 'L', 'L' },
		//11
		{ 'P', 'P', 'P', 'P' },
		//12
		{ 'H', 'H', 'Q', 'Q' },
		//13
		{ 'R', 'R', 'R', 'R' },
		//14
		{ 15,  16,  17,  18 },
		//15
		{ 'I', 'I', 'I', 'M' },
		//16
		{ 'T', 'T', 'T', 'T' },
		//17
		{ 'N', 'N', 'K', 'K' },
		//18
		{ 'S', 'S', 'R', 'R' },
		//19
		{ 20,  21,  22,  23 },
		//20
		{ 'V', 'V', 'V', 'V' },
		//21
		{ 'A', 'A', 'A', 'A' },
		//22
		{ 'D', 'D', 'E', 'E' },
		//23
		{ 'G', 'G', 'G', 'G' }
	};
}

void Translation::TranslateSequence(const std::string & sequence)
{
	unsigned int count = 0;
	int state = 0;
	int code = 0;
	while (count < (sequence.size()))
	{
		if (sequence[count] == 'T')
		{
			code = 0;
		}
		else if (sequence[count] == 'C')
		{
			code = 1;
		}
		else if (sequence[count] == 'A')
		{
			code = 2;
		}
		else if (sequence[count] == 'G')
		{
			code = 3;
		}
		else if (sequence[count] == '\n')
		{
			count++;
			continue;
		}
		else if (sequence[count] == -1)
		{
			break;
		}
		else
		{
			wxMessageBox("FASTA file is invalid", "Error", wxOK | wxICON_ERROR);
			break;
		}

		state = mStateMachine[state][code];
		
		if (mBases.find(state) != mBases.end())
		{
			std::string protein = mBases.find(state)->second;
			mHistoValues.at(protein) += 1;
			state = 3;
		}
		count++;
	}
}

std::unordered_map<std::string, int> Translation::GetHistoValues()
{
	return mHistoValues;
}
