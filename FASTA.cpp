#include "FASTA.h"
#include "Exceptions.h"
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <wx/msgdlg.h>

FASTA::FASTA(std::string &filepath)
{
	try
	{
		std::ifstream::pos_type size;
		std::stringstream stream;

		// Open the file for input, and start ATE (at the end)
		std::ifstream file(filepath, std::ios::in | std::ios::out | std::ios::ate);
		if (file.is_open())
		{
			size = file.tellg(); // Save the size of the file
			mSequence.reserve(static_cast<unsigned int>(size));
			file.seekg(0, std::ios::beg); // Seek back to start of file
			stream << file.rdbuf();
			bool header = true;
			while (!stream.eof())
			{
				if (header)
				{
					mHeader += stream.get();
					if (stream.peek() == '\n' || stream.peek() == '\r')
					{
						header = false;
					}
				}
				else if (stream.peek() != '\n' && stream.peek() != '\r')
				{
					mSequence += stream.get();
				}
				else
				{
					stream.get();
				}

			}
		}
		else
		{
			wxMessageBox("FASTA file is invalid", "Error", wxOK | wxICON_ERROR);
			throw FileLoadExcept();
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

}

std::string FASTA::GetHeader()
{
	return mHeader;
}

std::string FASTA::GetSequence()
{
	return mSequence;
}
