#pragma once
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include "typestring/typestring.hh"

template<typename String, typename Arg>
class named_param
{
public:
	typedef String name_type;
	typedef Arg value_type;
	named_param()
	{

	}

	named_param(const boost::optional<Arg>& val_optional)
		: _val( val_optional )
	{

	}

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

	boost::optional<Arg> _val;
};


template<typename T1, typename T2>
named_param< T1, T2 > named(T2&& val)
{
	return named_param< T1, T2 >(std::forward<T2>(val));
}

#define _named(param_name, val) named<typestring_is(param_name)>(val)

template<typename StringType>
struct named_wrapper
{
	named_wrapper() {}
	template<typename T>
	named_param<StringType, T> operator= (T&& val)
	{
		return named_param<StringType, std::remove_cv<T>::type	>(std::forward<T>(val));
	}
};

#define _name(param_name) named_wrapper<typestring_is(param_name)>()
