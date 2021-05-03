#pragma once

template <typename State, typename Command>
class CTransition
{
protected:
	State		startState;
	State		stopState;
	Command		command;
	Command		argument;

public:
	CTransition() = default;
	CTransition(const State& Start, const State& Stop, const Command& Comm, const Command& Arg) : 
		startState(Start), stopState(Stop), command(Comm), argument(Arg) {}
	CTransition(const State&& Start, const State&& Stop, const Command&& Comm, const Command&& Arg) :
		startState(Start), stopState(Stop), command(Comm), argument(Arg) {}

	const State&		getStart()  const { return startState; }
	const State&		getStop()  const { return stopState; }
	const Command&		getCommand()  const { return command; }
	const Command&		getArgument()  const { return argument; }
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
	template <int Pos, typename S, typename C, typename... Args, 
					class = std::enable_if_t<std::conjunction< std::is_same<State, S>, std::is_same<Command, C>>::value, void>>
	void setTransitionT(S&& first, S&& second, C&& command, C&& arg, Args&&... args) {
		if(states.find(first) == states.end() )
			std::cout << "Error : unknown state " << first << "\n";
		if (states.find(second) == states.end())
			std::cout << "Error : unknown state " << second << "\n";
		if (commands.find(command) == commands.end())
			std::cout << "Error : unknown command " << command << "\n";

		transitions[Pos] = std::move(CTransition<S, C>(first, second, command, arg));
		setTransitionT<Pos + 1, Args... >(std::move(args)...);
	}

	template <int Pos, typename S, typename C,
				class = std::enable_if_t<std::conjunction<std::is_same<State, S>, std::is_same<Command, C>>::value, void> >
	void setTransitionT(S&& first, S&& second, C&& command, C&& arg) {
		if (states.find(first) == states.end())
			std::cout << "Error : unknown state " << first << "\n";
		if (states.find(second) == states.end())
			std::cout << "Error : unknown state " << second << "\n";
		if (commands.find(command) == commands.end())
			std::cout << "Error : unknown command " << command << "\n";
		transitions[Pos] = std::move(CTransition<S, C>(first, second, command, arg));
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

	void setTransition(State&& start, State&& stop, Command&& command, Command&& arg) {
		transitions.emplace_back(start, stop, command, arg);
	}

	//   The number of arguments must be divisible by four in function. 
	//	The first argument in the pair is the start state, the second is the stop state ... 
	template <typename ...Args/*, class = std::enable_if_t<are_same<State, Args...>::value, void>*/>
	void	setTransitions(Args&& ... Transitions) {
		constexpr auto size = sizeof...(Args);
		static_assert(size%4 == 0, "Error : The number of arguments must be divisible by four in function setTransitions");
		static_assert(size >= 4, "Error : setTransitions must have at least four arguments");
		transitions.resize(size/4);
		setTransitionT< 0, Args... >(std::move(Transitions)...);
	}

	State&		getCurrentState() const { return   currentState; }

 	bool ProcessCommand(const Command& command, const Command& argument) {
		auto st = currentState;
		auto trans = std::find_if(transitions.begin(), transitions.end(),
			[&st, &command, &argument](const CTransition<State, Command>& tr)->bool {
				return tr.getStart() == st && tr.getCommand() == command && (tr.getArgument() == Command() || argument == tr.getArgument());
		} );
		if (trans != transitions.end()) {
			currentState = trans->getStop();
			return true;
		} 
		else return false;
	}

protected:
	bool				started = false;

	std::set<State>		states;
	State				currentState;

	std::set<Command>	commands;

	std::vector< CTransition< State, Command > >	transitions;
};

