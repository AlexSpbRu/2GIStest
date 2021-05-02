#include "stdafx.h"
#include "Dispatcher.h"


template<>
CDispatcher<T> :: CDispatcher(std::initializer_list< data_type >&  Commands) {
	commands.resize(Commands.size());
	int pos = 0;
	for (auto& comm : Commands) {
		commands[pos] = std::move(comm);
	}
}


void CDispatcher::ExecuteCommand(std::string& Command, std::string& Arg) {
	std::vector<data_type>::iterator comm = std::find_if(commands.begin(), commands.end(), [&Command](data_type& d) {
		return d.first == Command; }  );
	if (comm != commands.end()) {
		comm->second(Arg);
	}
	else {
		std::cout << "Wrong command " << Command.c_str() << " in the command line\n";
	}
		
}
