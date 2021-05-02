// 2GIStest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"
#include "FileReader.h"
#include "Dispatcher.h"
#include "StateMashine.h"

int main(int argc, char* argv[])
{
	std::string path = argv[0];
	path = path.substr(0, path.find_last_of('\\')+1);
	CDispatcher<CTextFileReader>::func_type help_func =
		[](CDispatcher<CTextFileReader>::exec_type& executor, std::string&) {
			std::cout << "--------------HELP---------------------------.\n";
			std::cout << "-f fileName  - open file with name - fileName.\n";
			std::cout << "-m command   - execute command witt name - command.\n";
			std::cout << "                     Variants of commands:\n";
			std::cout << "               checksum - calculate checksum for file which was opened by command -f\n";
			std::cout << "               words - count how many times a word was in the file which was opened by command -f\n";
			std::cout << "-v word      - search word.\n";
			std::cout << "-h           - show hint.\n";
			std::cout << "---------------------------------------------.\n";
		};

	CDispatcher<CTextFileReader>::func_type open_file_func =
		[&path](CDispatcher<CTextFileReader>::exec_type& executor, std::string& fileName) {
			if (bool(executor)) {
				std::cout << "The file has already opened.\n";
			}
			else {		// create Executor and Open File.
				executor = std::make_unique<CTextFileReader>(path + fileName);
			}
		};

	CDispatcher<CTextFileReader>::func_type command_func =
		[](CDispatcher<CTextFileReader>::exec_type& executor, std::string& command) {
		if (bool(executor)) {
			if (command == "checksum") {
				auto checkSum = executor->CheckSumm();
				std::cout << "checksum = " << checkSum << ".\n";
			}
			else if (command == "word")
				executor->SetSeek();
		}
		else {	
			std::cout << "The file did not open.\n";
		}
	};

	CDispatcher<CTextFileReader>::func_type seekWord_func =
		[](CDispatcher<CTextFileReader>::exec_type& executor, std::string& word) {
		if (bool(executor)) {
			auto count = executor->SeekWordInText(word);
			std::cout << "The word " << word << " was found " << count << "  times.\n";
		}
		else {
			std::cout << "The file did not open.\n";
		}
	};

	CDispatcher<CTextFileReader> dispatcher(CDispatcher<CTextFileReader>::data_type(std::string("-h"), help_func),
										CDispatcher<CTextFileReader>::data_type(std::string("-f"), open_file_func),
										CDispatcher<CTextFileReader>::data_type(std::string("-m"), command_func),
										CDispatcher<CTextFileReader>::data_type(std::string("-v"), seekWord_func));
	CDispatcher<CTextFileReader> dispatcher2({ CDispatcher<CTextFileReader>::data_type(std::string("-h"), help_func),
		CDispatcher<CTextFileReader>::data_type(std::string("-f"), open_file_func),
		CDispatcher<CTextFileReader>::data_type(std::string("-m"), command_func),
		CDispatcher<CTextFileReader>::data_type(std::string("-v"), seekWord_func) });

	CStateMashine<std::string, std::string>		stateMashine;
	stateMashine.setStates(std::string("start"), std::string("-h"), std::string("-f"), std::string("-v"), std::string("-mword"), std::string("-mchecksum") );
	stateMashine.setCommands({ std::string("checksum"), std::string("word"), std::string("-f"), std::string("-h"), std::string("-v") });
	
	stateMashine.setTransitions( std::string("start"), std::string("-h"), std::string("-h"),
		std::string("start"), std::string("-f"), std::string("-f"), std::string("-f"), std::string("-mchecksum"), std::string("checksum"),
		std::string("start"), std::string("-f"), std::string("-f"), std::string("-f"), std::string("-mword"),  std::string("word"), std::string("-mword"), std::string("-v"), std::string("-v") );
	stateMashine.setStartState("start");

	for (int i = 1; i < argc; ++i ) {
		std::string com;
		std::string arg; 
		if(argv[i][0] == '-')
			com = argv[i];
		else
			std::cout << "Error in command " << argv[i] << "\n";
		if (i+1 < argc && argv[i + 1][0] != '-') {
			arg = argv[i + 1];
			++i;
		}
		dispatcher.ExecuteCommand(com, arg);
	}
	return 0;
 }
