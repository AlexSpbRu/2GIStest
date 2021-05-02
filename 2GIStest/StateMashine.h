#pragma once

template <typename State, typename Command>
class CTransition
{
protected:
	State		startState;
	State		stopState;
	Command		command;

public:
	CTransition() = default;
	CTransition(const State& Start, const State& Stop, const Command& Comm) : startState(Start), stopState(Stop), command(Comm) {}
	CTransition(const State&& Start, const State&& Stop, const Command&& Comm) : startState(Start), stopState(Stop), command(Comm) {}
	State&		getStart() { return startState; }
	State&		getStop() { return stopState; }
	Command&	getCommand() { return command; }
};

template <typename State, typename Command>
class CStateMashine
{
public :
	template<typename T, typename... Args>
	using are_same = std::conjunction<std::is_same<T, Args>...>;

protected :
	//-----------------------------------
	template < typename Head, typename... Args>
	void setState(Head&& head, Args&&... args) {
		states.emplace(std::move(head));
		setState< Args... >( std::move(args)...);
	}

	template < typename Head>
	void setState(Head&& head) {
		states.emplace(std::move(head));
	}

	//-----------------------------------
	template <int Pos, typename First, typename Second, typename Comm, typename... Args, 
					class = std::enable_if_t<std::conjunction<std::is_same<State, First>, std::is_same<State, Second>, std::is_same<Command, Comm>>::value, void>>
	void setTransition(First&& first, Second&& second, Comm && command, Args&&... args) {
		if(states.find(first) == states.end() )
			std::cout << "Error : unknown state " << first << "\n";
		if (states.find(second) == states.end())
			std::cout << "Error : unknown state " << second << "\n";
		if (commands.find(command) == commands.end())
			std::cout << "Error : unknown command " << command << "\n";

		transitions[Pos] = std::move(CTransition<First, Comm>(first, second, command));
		setTransition<Pos + 1, Args... >(std::move(args)...);
	}

	template <int Pos, typename First, typename Second, typename Comm,
				class = std::enable_if_t<std::conjunction<std::is_same<State, First>, std::is_same<State, Second>, std::is_same<Command, Comm>>::value, void> >
	void setTransition(First&& first, Second&& second, Comm && command) {
		if (states.find(first) == states.end())
			std::cout << "Error : unknown state " << first << "\n";
		if (states.find(second) == states.end())
			std::cout << "Error : unknown state " << second << "\n";
		if (commands.find(command) == commands.end())
			std::cout << "Error : unknown command " << command << "\n";
		transitions[Pos] = std::move(CTransition<First, Comm>(first, second, command));
	}

public:
	void	setStartState(const State& startState) {
		currentState = startState;
	}

	void	setStates(std::initializer_list< State >&&  States) {
		for (auto& state : States) {
			states.emplace(std::move(state));
		}
	}

	template <typename ...Args>
	void	setStates(Args&& ... States) {
		constexpr auto size = sizeof...(Args);
		static_assert(size != 0, "Error : setStates must have at least one argument");
		setState< Args... >(std::move(States)...);
	}

	//  Only one variant of setting Commands
	void	setCommands(std::initializer_list< Command >&&  Commands) {
		for (auto& command : Commands) {
			commands.emplace(std::move(command));
		}
	}

	//   There must be an even number of arguments. 
	//	The first argument in the pair is the start state, the second is the stop state. 
	template <typename ...Args, class = std::enable_if_t<are_same<State, Args...>::value, void>>
	void	setTransitions(Args&& ... Transitions) {
		constexpr auto size = sizeof...(Args);
		static_assert(size%3 == 0, "Error : The number of arguments must be divisible by three in function setTransitions");
		static_assert(size >= 3, "Error : setTransitions must have at least three arguments");
		transitions.resize(size/2);
		setTransition< 0, Args... >(std::move(Transitions)...);
	}

	bool ProcessCommand(const Command& Command) {

	}

protected:
	bool				started = false;

	std::set<State>		states;
	State				currentState;

	std::set<Command>	commands;

	std::vector< CTransition<State, Command> >	transitions;
};

