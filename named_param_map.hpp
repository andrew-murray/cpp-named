#pragma once
#include <type_traits>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/set.hpp>
#include "named_param.hpp"

template< typename ParamType, typename MapRecurseType >
class named_param_map
{
private:
	ParamType _param;
	MapRecurseType _others;
public:

	typedef typename ParamType::value_type named_value_type;
	typedef typename ParamType::name_type name_type;

	typedef typename boost::mpl::insert< typename MapRecurseType::param_set_type, ParamType >::type param_set_type;

	named_param_map()
	{

	}

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

	template<typename OParamType, typename OMapRecurseType>
	named_param_map(const named_param_map<OParamType, OMapRecurseType>& children);
	
	template<typename String, typename _nulltype = std::enable_if<std::is_same<name_type, String>::value>::type>
	const boost::optional<named_value_type>& at() const
	{
		return _param._val;
	}

	template<typename String, typename _nulltype = std::enable_if<std::is_same<name_type, String>::value>::type>
	boost::optional<named_value_type>& at()
	{
		return _param._val;
	}
	
	template<typename String, typename _nulltype = typename std::enable_if<!std::is_same<name_type,String>::value>::type>
	auto at() const -> decltype(_others.at<String>())
	{
		return _others.at<String>();
	}

	template<typename String, typename _nulltype = typename std::enable_if<!std::is_same<name_type, String>::value>::type>
	auto at() -> decltype(_others.at<String>())
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

	typedef boost::mpl::set< ParamType > param_set_type;

	named_param_map()
	{

	}

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
	const boost::optional<named_value_type>& at() const
	{
		// TODO: this error message is horrendous
		static_assert(false, "Param type not present.");
	}

	template<>
	const boost::optional<named_value_type>& at<name_type>() const
	{
		return _param._val;
	}

	template<typename String>
	boost::optional<named_value_type>& at()
	{
		// TODO: this error message is horrendous
		static_assert(false, "Param type not present.");
	}

	template<>
	boost::optional<named_value_type>& at<name_type>()
	{
		return _param._val;
	}

	ParamType _param;
};


namespace imp
{
	template<typename ParamType, typename MapRecurseType, typename OParamType, typename OMapRecurseType>
	class apply_child {
	public:
		apply_child(
			named_param_map<ParamType, MapRecurseType>& self,
			const named_param_map<OParamType, OMapRecurseType>& other
		)
			: _self(self)
			, _other(other)
		{

		}

		template<typename ParamType>
		void operator()(ParamType)
		{
			typedef ParamType::name_type name_type;
			// force move - if required?
			_self.at< name_type >() = _other.at< name_type >();
		}

		named_param_map<ParamType, MapRecurseType>& _self;
		const named_param_map<OParamType, OMapRecurseType>& _other;
	};
}

template<typename ParamType, typename MapRecurseType>
template<typename OParamType, typename OMapRecurseType>
named_param_map<ParamType, MapRecurseType>::named_param_map(const named_param_map<OParamType, OMapRecurseType>& children)
{
	typedef named_param_map<OParamType, OMapRecurseType> input_param_type;
	auto applyer = imp::apply_child<ParamType, MapRecurseType, OParamType, OMapRecurseType>(
		*this,
		children
	);
	boost::mpl::for_each<input_param_type::param_set_type>( applyer );
}

// when provided with a flat function call, 
// packs together into one thing
template<typename NP0>
auto params(NP0&& first)
-> named_param_map<std::remove_cv_t<std::remove_reference_t<NP0>>, void>
{
	return named_param_map<std::remove_cv_t<std::remove_reference_t<NP0>>, void>(std::forward<NP0>(first));
}

template<typename NP0, typename... Args>
auto params(NP0&& first, Args... args)
-> named_param_map<std::remove_cv_t<std::remove_reference_t<NP0>>, decltype(params(std::forward<Args>(args)...)) >
{
	typedef decltype(params(std::forward<Args>(args)...)) other_map_type;
	return named_param_map<std::remove_cv_t<std::remove_reference_t<NP0>>, other_map_type>(std::forward<NP0>(first), params(std::forward<Args>(args)...));
}
