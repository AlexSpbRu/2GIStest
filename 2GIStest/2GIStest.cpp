// 2GIStest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"
#include "FileReader.h"
#include "Dispatcher.h"
#include "StateMashine.h"

enum class EState : char {
	Start = 'S',
	Help = 'H',
	File = 'F',
	Word = 'W',
	V = 'V',
	Checksum = 'C'
};

int main(int argc, char* argv[])
{

	for (int i = 1; i < argc; ++i) {
		std::cout << argv[i] << " ";
	}
	std::cout << "\n";

	std::string path = argv[0];
	path = path.substr(0, path.find_last_of('\\')+1);
	CDispatcher<CTextFileReader>::func_type help_func =
		[](CDispatcher<CTextFileReader>::exec_type& , std::string&) {
			std::cout << "************************************************\n";
			std::cout << "***  2GIStest - simple text processor.       ***\n";
			std::cout << "***  Version 1.1                             ***\n";
			std::cout << "***  GNU General Public License              ***\n";
			std::cout << "***  Author : Kozlov Aleksey                 ***\n";
			std::cout << "************************************************\n\n";
			std::cout << "                 HELP.\n";
			std::cout << "-f fileName  - open file with name - fileName.\n";
			std::cout << "-m command   - execute command witt name - command.\n";
			std::cout << "                     Variants of commands:\n";
			std::cout << "               checksum - calculate checksum for file which was opened by command -f\n";
			std::cout << "               words - count how many times a word was in the file which was opened by command -f\n";
			std::cout << "-v word      - search word.\n";
			std::cout << "-h           - show hint.\n";
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
			std::cout << "The word " << word << " was founded " << count << "  times.\n";
		}
		else {
			std::cout << "The file did not open.\n";
		}
	};

	CDispatcher<CTextFileReader> dispatcher(CDispatcher<CTextFileReader>::data_type(std::string("-h"), help_func),
										CDispatcher<CTextFileReader>::data_type(std::string("-f"), open_file_func),
										CDispatcher<CTextFileReader>::data_type(std::string("-m"), command_func),
										CDispatcher<CTextFileReader>::data_type(std::string("-v"), seekWord_func));
	/*CDispatcher<CTextFileReader> dispatcher2({ CDispatcher<CTextFileReader>::data_type(std::string("-h"), help_func),
		CDispatcher<CTextFileReader>::data_type(std::string("-f"), open_file_func),
		CDispatcher<CTextFileReader>::data_type(std::string("-m"), command_func),
		CDispatcher<CTextFileReader>::data_type(std::string("-v"), seekWord_func) });*/

	CStateMashine<EState, std::string>		stateMashine;
	stateMashine.setStates(EState::Start, EState::Help,  EState::File, EState::Word, EState::V, EState::Checksum);

	stateMashine.setCommands({ std::string("-m"), std::string("-f"), std::string("-h"), std::string("-v") });
	
	/*stateMashine.setTransition(EState::Start, EState::Help, std::string("-h"), std::string());

	stateMashine.setTransition(EState::Start, EState::File, std::string("-f"), std::string());
		stateMashine.setTransition(EState::File, EState::Checksum, std::string("-m"), std::string("checksum"));

	stateMashine.setTransition(EState::Start, EState::File, std::string("-f"), std::string());
		stateMashine.setTransition(EState::File, EState::Word, std::string("-m"), std::string("word"));
			stateMashine.setTransition(EState::Word, EState::V, std::string("-v"), std::string());*/
	stateMashine.setTransitions( EState::Start, EState::Help, std::string("-h"), std::string(),
		EState::Start, EState::File, std::string("-f"), std::string(),
			EState::File, EState::Checksum, std::string("-m"), std::string("checksum"),
		EState::Start, EState::File, std::string("-f"), std::string(),
			 EState::File, EState::Word, std::string("-m"), std::string("word"),
			 EState::Word, EState::V, std::string("-v"), std::string() );

	stateMashine.setStartState(EState::Start);

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
		if (stateMashine.ProcessCommand(com, arg)) {
			dispatcher.ExecuteCommand(com, arg);
		}
		else 
			std::cout << " Error in command " << com.c_str() << "  " << arg.c_str() << "\n";
	}
	return 0;
 }
