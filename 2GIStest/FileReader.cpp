#include "stdafx.h"

#include "FileReader.h"


//-------------------------------------------------------------------------------------------

	CFileReader::CFileReader(const std::string& FileName) : fin(FileName, /*std::ios::in |*/ std::ios::binary) {
	}
	
	unsigned int CFileReader::CheckSumm()  const {
		const std::uint32_t* data32 = nullptr;

		if (vdata != nullptr && dataSize != 0u) {
			data32 = static_cast<const std::uint32_t*>(vdata);
		} else
		if (!data.empty()) {
			data32 = (const std::uint32_t*)(data.data());
		}
			
		for (unsigned int i = 0; i < dataSize / 4; ++i) {
			checkSumm += *data32;
			data32++;
		}

		return checkSumm;
	}

	unsigned int CFileReader::SeekWordInText(const std::string& Word)  const {
		unsigned int offset = 0, count  = 0;
		while ((offset = fileContentText.find(Word, offset)) != std::string::npos) {
			count++;
			offset += Word.length();
		}
		return count;
	}

	void CFileReader::PrepareFastSeek() {

	}

	unsigned int CFileReader::FastSeekWordInText(const std::string& ) {
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
		const auto begin = file.tellg();
		file.seekg(0, std::ios_base::end);
		const auto stop = file.tellg();
		file.seekg(0);
		auto size = ((stop - begin + 3) / 4)*4;
		data.resize(size);
		data = std::vector<uint8_t>(std::istream_iterator<uint8_t>(file), std::istream_iterator<uint8_t>());
		data.resize(size);
		dataSize = size;
		file.close();
	} 

	void CTextFileReader :: ReadTextFromFile(const std::string& FileName)  {
		FILE* p_file = fopen(FileName.c_str(), "rb");
		if (p_file != nullptr) {
			fseek(p_file, 0, SEEK_END);
			auto size = ftell(p_file);
			dataSize = ((size + 3) / 4) * 4;

			vdata = new char[dataSize];
			if (vdata != nullptr) {
				auto sizeRead =  fread(vdata, sizeof(char), size, p_file);
				if (sizeRead != size) {
					if (feof(p_file)) {
						std::cout << "ERROR : Unexpected end of file.\n";
					}
					else if (ferror(p_file)) {
						std::cout << "ERROR : error reading file " << FileName.c_str() << " : " << ferror(p_file) << ".\n";
					}
				}
			}

			fclose(p_file);
		}

	}