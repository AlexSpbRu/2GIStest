#pragma once

//  Base class for processing text files. 
class CFileReader
{
protected :
	std::ifstream				fin;
	std::string					fileContentText;
	std::vector<std::uint8_t>	data;
	void*						vdata = nullptr;
	uint32_t					dataSize = 0u;

	std::string					seekText;

	bool						seek = false;
	bool						calcCheckSumm = false;
	mutable uint32_t			checkSumm = 0;
public :
	CFileReader(const std::string& FileName);
	CFileReader() = default;
	virtual ~CFileReader() {}

	virtual void			ReadTextFromFile(std::ifstream& file) = 0;
	virtual void			ReadTextFromFile(const std::string& FileName) = 0;
	virtual unsigned int	CheckSumm() const;
	void					SetSeek() { seek = true; }
	bool					GetSeek() const { return seek; }

	virtual unsigned int	SeekWordInText(const std::string& Word) const;

	//  Scan the entire file and find all words. Can be realized with help of unordered_map.
	virtual void			PrepareFastSeek();
	virtual unsigned int	FastSeekWordInText(const std::string& Word);
};


//  Class for processing  plain  text files.

class CTextFileReader : public CFileReader
{
public:
	CTextFileReader(const std::string& FileName);
	virtual ~CTextFileReader() {}

	virtual void ReadTextFromFile(std::ifstream& file) override;
	virtual void ReadTextFromFile(const std::string& FileName) override;
};


