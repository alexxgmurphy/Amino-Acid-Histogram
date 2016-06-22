#pragma once
#include <unordered_map>

class Translation
{
public:
	Translation();
	void TranslateSequence(const std::string &sequence);
	std::unordered_map<std::string, int> GetHistoValues();

private:
	bool mInSequence = false;
	std::string mCurrCodon;
	std::unordered_map<std::string, int> mHistoValues;
	std::unordered_map<char, std::string> mBases;
	std::vector<std::vector<int>> mStateMachine;
};