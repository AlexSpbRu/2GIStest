#include "stdafx.h"

#include "FileReader.h"


//-------------------------------------------------------------------------------------------

	CFileReader::CFileReader(const std::string& FileName) : fin(FileName/*, std::ios::in | std::ios::binary*/) {
	}
	
	unsigned int CFileReader::CheckSumm()  const {
		unsigned int	length = fileContentText.size(),
						size = length/4,
						rest = length % 4;
		const unsigned int* data = (const unsigned int*)fileContentText.data();
		for (unsigned int i = 0; i < size; ++i)
			checkSumm += *data++;
		if (rest) {   //  size is not a multiple of 4 
			unsigned int data_rest = 0;
			memcpy((char*)&data_rest, fileContentText.data() + sizeof(unsigned int)*size, rest);
			checkSumm += data_rest;
		}
		return checkSumm;
	}

	unsigned int CFileReader::SeekWordInText(const std::string& Word)  const {
		unsigned int offset = 0, count  = 0;
		while ((offset = fileContentText.find_first_of(Word, offset)) != std::string::npos) {
			count++;
			offset += Word.length();
		}
		return count;
	}

	void CFileReader::PrepareFastSeek() {

	}

	unsigned int CFileReader::FastSeekWordInText(const std::string& Word) {
		return 0;
	}


//-------------------------------------------------------------------------------------------
	CTextFileReader :: CTextFileReader(const std::string& FileName) : CFileReader(FileName) {
		if (fin.is_open()) {
			ReadTextFromFile(fin);
		}
		else {
			std::cout << "ERROR : The file " << FileName.c_str() << " did not open.\n";
		}
 	}

	void CTextFileReader :: ReadTextFromFile(std::ifstream& file) {
		file >> std::noskipws;
		fileContentText = std::string(std::istream_iterator<char>(file), std::istream_iterator<char>());
		file.close();
	}