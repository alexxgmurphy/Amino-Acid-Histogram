#pragma once
#include <string>
#include <iostream>
#include <fstream>

class FASTA
{
public:
	FASTA(std::string &filepath);
	std::string GetHeader();
	std::string GetSequence();

private:
	std::string mHeader;
	std::string mSequence;
};