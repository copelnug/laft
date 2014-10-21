#ifndef LAFT_CORE_MIXIN_HPP_INCLUDED
#define LAFT_CORE_MIXIN_HPP_INCLUDED

namespace laft
{
	namespace core
	{
		/**
			\brief Utility class when implementating partial mixin.
			\tparam Implementation Contrete class that will use the mixin.
			
			This class is useful in the case when the mixin need to call
			functions of the concrete class. For example, a mixin that
			implement operator!= as the inverse of operator==.
			
			To do so, the solution is to use CRTP. This class is there 
			to prevent having to reimplent the two accessor methods to 
			the concrete class.
			
			Note, multiple inheritance can still be problematic.
		*/
		template <typename Implementation>
		class Mixin
		{
			protected:
				Implementation& self();
				Implementation const& self() const;
		};
	}
}


// Implementation
/**
	\brief Access the concrete class.
	\tparam Implementation Concrete class that will use the mixin.
	\return Reference on the concrete class.
*/
template <typename Implementation>
Implementation& laft::core::Mixin<Implementation>::self()
{
	return *static_cast<Implementation*>(this);
}
/**
	\brief Access the concrete class (const version).
	\tparam Implementation Concrete class that will use the mixin.
	\return Reference on the concrete class.
*/
template <typename Implementation>
Implementation const& laft::core::Mixin<Implementation>::self() const
{
	return *static_cast<Implementation const*>(this);
}

#endif
