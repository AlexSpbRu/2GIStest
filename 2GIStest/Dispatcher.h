#pragma once

#include <functional>
template <typename T>

class CDispatcher
{
public:
	using exec_type = std::unique_ptr<T>;
	using func_type = std::function< void (exec_type&, std::string&) >;
	using data_type = std::pair< std::string, func_type >;
	
protected :
	exec_type					executor;
	std::vector<data_type>		commands;

#if __cplusplus < 201703L
	template <typename Head, typename... Args>
	void InitCommandsList(Head&& head, Args&&... args) {
		commands.emplace_back(head);
		InitCommandsList< Args... >(std::forward<Args>(args)...); 
	}

	template <typename Head>
	void InitCommandsList(Head&& head) {
		commands.emplace_back(head);
	}
#endif
public :
	
	CDispatcher(const std::initializer_list< data_type >&  Commands) {
		commands.resize(Commands.size());
		int pos = 0;
		for (auto& comm : Commands) {
			commands[pos] = std::move(comm);
		}
	}

	template<class... Args>
	using are_same = std::conjunction<std::is_same<data_type, Args>...>;

	template <typename... Args, class = std::enable_if_t<are_same<Args...>::value, void>>
	CDispatcher(Args&&... args) {
#if __cplusplus >= 201703L
		(commands.emplace_back(std::forward<Args>(args)), ...);
#else
		InitCommandsList< Args... >(std::forward<Args>(args)...);
#endif
	
	}

	void ExecuteCommand(std::string& Command, std::string& Arg) {
		typename std::vector<data_type>::iterator comm = std::find_if(commands.begin(), commands.end(), [&Command](data_type& d) {
			return d.first == Command; });
		if (comm != commands.end()) {
			comm->second(executor, Arg);
		}
		else {
			std::cout << "Wrong command " << Command.c_str() << " in the command line\n";
		}

	}
};

