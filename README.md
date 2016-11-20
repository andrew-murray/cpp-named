An experiment implementing a pure-library-solution for compile-time specified c++ named arguments functionality.
```c++

typedef decltype(params(
	_name("f0") = decl_val<float>(),
	_name("other_param") = decl_val<int>(),
	_name("class-type") = decl_val<basic_class>(),
	_name("another-thing") = decl_val<float>()
)) entry_point_arg_type;

void example_api(
	const int& x,
	const int& y,
	const entry_point_arg_type& optionals = entry_point_arg_type()
);

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
}
```

Issues
======

- Parameter types are required to be copyable currently
- Error messages are pretty dire for the most part, work to be done to put static_asserts (or similar) in the right place
- Conversion hasn't been considered/managed... if a parameter is unsigned int, but user provides an int ... that's probably a horrible error message to parse.
- No capability for runtime provided arguments
