#pragma once
#include <type_traits>
#include "named_param.hpp"

template< typename ParamType, typename MapRecurseType >
class named_param_map
{
private:
	const ParamType _param;
	const MapRecurseType _others;
public:

	typedef typename ParamType::value_type named_value_type;
	typedef typename ParamType::name_type name_type;

	named_param_map(ParamType&& x, MapRecurseType&& others)
		: _param(std::move(x))
		, _others(std::move(others))
	{

	}

	named_param_map(const ParamType& x, const MapRecurseType& others)
		: _param(x)
		, _others(others)
	{

	}

	named_param_map(named_param_map&& x)
		: _param(std::move(x._param))
		, _others(std::move(x._others))
	{

	}

	named_param_map(const named_param_map& x)
		: _param(x._param)
		, _others(x._others)
	{

	}

	template<typename String, typename _nulltype = std::enable_if<std::is_same<name_type, String>::value>::type>
	named_value_type at() const
	{
		return _param._val;
	}
	
	template<typename String, typename _nulltype = typename std::enable_if<!std::is_same<name_type,String>::value>::type>
	auto at() const -> decltype(_others.at<String>())
	{
		return _others.at<String>();
	}	
	
};


template<typename ParamType>
class named_param_map<ParamType, void>
{
public:

	typedef typename ParamType::value_type named_value_type;
	typedef typename ParamType::name_type name_type;

	named_param_map(ParamType&& x)
		: _param(std::move(x))
	{

	}

	named_param_map(const ParamType& x)
		: _param(x)
	{

	}

	named_param_map(const named_param_map& that)
		: _param(that._param)
	{

	}

	named_param_map(named_param_map&& that)
		: _param(std::move(that._param))
	{

	}

	template<typename String>
	named_value_type at() const
	{
		// TODO: this error message is horrendous
		static_assert(false, "Param type not present.");
	}

	template<>
	named_value_type at<name_type>() const
	{
		return _param._val;
	}

	const ParamType _param;
};
