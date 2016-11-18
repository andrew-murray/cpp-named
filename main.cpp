#include <iostream>

#include "named_param_map.hpp"

constexpr bool constexpr_strequal(char const *one, char const *two) {
	return (*one && *two) ? (*one == *two && constexpr_strequal(one + 1, two + 1))
		: (!*one && !*two);
}

template<typename NP0>
auto params(NP0&& first)
-> named_param_map<NP0,void>
{
	return named_param_map<NP0, void>(std::forward<NP0>(first));
}

template<typename NP0, typename... Args>
auto params(NP0&& first, Args... args)
	-> named_param_map<NP0, decltype(params(std::forward<Args>(args)...)) >
{
	typedef decltype(params(std::forward<Args>(args)...)) other_map_type;
	return named_param_map<NP0, other_map_type>(std::forward<NP0>(first), params(std::forward<Args>(args)...));
}

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
	named<typestring_is("f0")>(decl_val<float>()),
	named<typestring_is("other_param")>(decl_val<int>()),
	named<typestring_is("class-type")>(decl_val<basic_class>()),
	named<typestring_is("another-thing")>(decl_val<float>())
)) entry_point_arg_type;

int main(const int argn, const char* argv[])
{
	auto numX = named<typestring_is("num")>(5);
	static_assert(constexpr_strequal(numX.name(),"num"), "the name of num is wrong!");

	// show some funky syntaxes
	auto param_map = params(
		_named("f0", 3.0f),
		named<typestring_is("other_param")>(2),
		_name("class-type") = basic_class()
	);

	std::cout << param_map.at<typestring_is("f0")>() << std::endl;
	std::cout << param_map.at<typestring_is("other_param")>() << std::endl;
	std::cout << param_map.at<typestring_is("class-type")>() << std::endl;
	//FIXME: doesn't support move-only types currently, which is desirable
	// std::cout << param_map.at<typestring_is("move-only-type")>() << std::endl;


	// before I can do this, I need to switch to optionals everywhere
	entry_point_arg_type for_entry_point = param_map;

	std::cout << for_entry_point.at<typestring_is("f0")>() << std::endl;
	std::cout << for_entry_point.at<typestring_is("other_param")>() << std::endl;
	std::cout << for_entry_point.at<typestring_is("class-type")>() << std::endl;

	// the below fails (as it should) but with a horrible error 
	// "can't convert boost::optional<basic_class> to boost::optional<float>
	// or similar, need a ... far better error message
	// decltype(param_map) compressed = for_entry_point;

	return 1;
}
