#include <iostream>

#include "named_param_map.hpp"

constexpr bool constexpr_strequal(char const *one, char const *two) {
	return (*one && *two) ? (*one == *two && constexpr_strequal(one + 1, two + 1))
		: (!*one && !*two);
}

template<typename NP0>
auto collect(NP0&& first)
-> named_param_map<NP0,void>
{
	return named_param_map<NP0, void>(std::forward<NP0>(first));
}

template<typename NP0, typename... Args>
auto collect(NP0&& first, Args... args)
	-> named_param_map<NP0, decltype(collect(std::forward<Args>(args)...)) >
{
	typedef decltype(collect(std::forward<Args>(args)...)) other_map_type;
	return named_param_map<NP0, other_map_type>(std::forward<NP0>(first), collect(std::forward<Args>(args)...));
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

int main(const int argn, const char* argv[])
{
	auto numX = named<typestring_is("num")>(5);
	static_assert(constexpr_strequal(numX.name(),"num"), "the name of num is wrong!");

	auto param_map = collect(
		named<typestring_is("f0")>(3.0f),
		named<typestring_is("other_param")>(2),
		named<typestring_is("class-type")>(basic_class())
	);

	std::cout << param_map.at<typestring_is("f0")>() << std::endl;
	std::cout << param_map.at<typestring_is("other_param")>() << std::endl;
	std::cout << param_map.at<typestring_is("class-type")>() << std::endl;
	//FIXME: doesn't support move-only types currently, which is desirable
	// std::cout << param_map.at<typestring_is("move-only-type")>() << std::endl;




	return 1;
}
