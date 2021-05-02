#pragma once

//  Base class for processing text files. 
class CFileReader
{
protected :
	std::ifstream	fin;
	std::string		fileContentText;
	std::string		seekText;

	bool			seek = false;
	bool			calcCheckSumm = false;
	mutable unsigned int	checkSumm = 0;
public :
	CFileReader(const std::string& FileName);
	virtual ~CFileReader() {}

	virtual void			ReadTextFromFile(std::ifstream& file) = 0;
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

	virtual void ReadTextFromFile(std::ifstream& file);
};


//  Class for processing  XML  files with text . (future purpose ... ) 

class CXMLFileReader : public CFileReader
{
public:
	CXMLFileReader(const std::string& FileName) : CFileReader(FileName) {};
	virtual ~CXMLFileReader() {}

	virtual void ReadTextFromFile() {}
};