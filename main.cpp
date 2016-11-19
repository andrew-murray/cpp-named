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

int main(const int argn, const char* argv[])
{
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
