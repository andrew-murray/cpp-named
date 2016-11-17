#pragma once
#include "typestring/typestring.hh"

template<typename String, typename Arg>
class named_param
{
public:
	typedef String name_type;
	typedef Arg value_type;
	named_param(Arg&& v)
		: _val( std::move( v ) )
	{
		
	}

	named_param(const Arg& v)
		: _val(v)
	{

	}

	named_param(named_param&& v)
		: _val(std::move(v._val))
	{
		
	}

	named_param(const named_param& v)
		: _val(v._val)
	{

	}

	static constexpr const char* name()
	{
		return name_type::data();
	}

	const Arg _val;
};


template<typename T1, typename T2>
named_param< T1, T2 > named(T2&& val)
{
	return named_param< T1, T2 >(std::forward<T2>(val));
}