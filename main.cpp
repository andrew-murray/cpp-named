#include <iostream>

#include "named_param_map.hpp"

class basic_class
{

};

std::ostream& operator<<(std::ostream& os, const basic_class&)
{
	os << "basic instance";
	return os;
}

class move_only_class
{
public:
	move_only_class()
	{}
	move_only_class(move_only_class&&)
	{}
	move_only_class(const move_only_class& v) = delete;
};

std::ostream& operator<<(std::ostream& os, const move_only_class&)
{
	os << "move-only instance";
	return os;
}

template<typename T>
T decl_val();

typedef decltype(params(
	_name("f0") = decl_val<float>(),
	_name("other_param") = decl_val<int>(),
	_name("class-type") = decl_val<basic_class>(),
	_name("another-thing") = decl_val<float>()
)) entry_point_arg_type;

template<typename Args>
struct printer
{
	printer(const Args& a)
		: _args(a)
	{

	}
	template<typename T>
	void operator()(T)
	{
		const auto& optionalArg = _args.at< typename T::name_type >();
		if (optionalArg)
		{
			std::cout << optionalArg << std::endl;
		}
		else
		{
			std::cout << "<unset/>" << std::endl;
		}
	}
	Args _args;
};

// maybe these two functions could be macro'd up?
// is that desirable?

void example_api(
	const int& x,
	const int& y,
	const entry_point_arg_type& optionals = entry_point_arg_type()
)
{
	// just some example code to exercise that it works
	std::cout << "<api>" << std::endl;
	boost::mpl::for_each<entry_point_arg_type::param_set_type>(
		printer<entry_point_arg_type>(optionals)
	);
	std::cout << "</api>" << std::endl;
}

template<typename ...Args>
void example_api(
	const int& x,
	const int& y,
	Args... args
)
{
	return example_api(x, y, entry_point_arg_type(params(args...)));
}

int main(const int argn, const char* argv[])
{
	//FIXME: doesn't support move-only types currently, which is desirable
	// std::cout << param_map.at<typestring_is("move-only-type")>() << std::endl;
	
	// the below fails (as it should) but with a horrible error 
	// "can't convert boost::optional<basic_class> to boost::optional<float>
	// need a ... far better error message
	// decltype(param_map) compressed = for_entry_point;
	
	// example of my initial intended syntax
	// it's not zero-footprint on the api, but it's lightweight

	example_api(
		0,
		1
	);

	example_api(
		0,
		1,
		_name("other_param") = 2,
		_name("class-type") = basic_class()
	);

	example_api(
		0,
		1,
		_name("class-type") = basic_class(),
		_name("other_param") = 2
	);

	// few

	example_api(
		0,
		1,
		_named("f0", 3.0f),
		named<typestring_is("other_param")>(2),
		_name("class-type") = basic_class()
	);

	// the configuration object can obviously be messed with

	auto configuration = params(
		_named("f0", 3.0f),
		_name("class-type") = basic_class()
	);

	std::cout << configuration.at<typestring_is("f0")>() << std::endl;
	const bool user_supplied_change = true;
	if (user_supplied_change)
	{
		configuration.at<typestring_is("f0")>() = 1729.0f; // as all those annoying  users request
	}

	const bool changed_my_mind = true;
	if (changed_my_mind)
	{
		configuration.at<typestring_is("class-type")>() = boost::none;
	}

	// it'd be relatively easy to define functions on the named_param_map
	// such that we can add together parameter sets sensibly and such
	// or define & ( same as + and ^ ) and | ,  operators sensibly... even subtract

	return 1;
}
