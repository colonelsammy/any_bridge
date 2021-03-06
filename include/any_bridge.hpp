//          Copyright Malcolm Noyes 2013-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ANY_BRIDGE_HPP_INCLUDED
#define ANY_BRIDGE_HPP_INCLUDED

#ifdef ANY_BRIDGE_CALL_WITH
#include "call_with.hpp"
#endif
#include <cstddef>
#include <utility>

#ifdef ANY_BRIDGE_USE_RTTI
#include <typeinfo>
#endif
#if __cplusplus > 199711L
#include <type_traits>
#endif

namespace any_bridge
{
#if __cplusplus > 199711L
    typedef std::remove_reference remove_reference;
    typedef std::remove_const remove_const;
#else
    template< class T > struct remove_reference      {typedef T type;};
    template< class T > struct remove_reference<T&>  {typedef T type;};
#ifdef _MSC_VER
    template< class T > struct remove_reference<T&&> {typedef T type;}; 
#endif
    template< class T > struct remove_const          {typedef T type;};
    template< class T > struct remove_const<const T>  {typedef T type;};
    template< class T > struct remove_const<const T*>  {typedef T* type;};
#endif

#ifdef ANY_BRIDGE_USE_RTTI
    template <typename InterfaceClass>
    class type_info
    {
    private:
        explicit type_info(const std::type_info& v)
            : m_value(v)
        {}
        type_info& operator=(const type_info&);
        type_info();

        const std::type_info& m_value;

    public:
        type_info(const type_info&)
            : m_value(rhs.m_value)
        {}

        template <typename T>
        static type_info type_id()
        {
            return type_info(typeid(typename remove_const<typename remove_reference<T>::type>::type));
        }
        bool operator == (const type_info& rhs)
        {
            return m_value.operator==(rhs.m_value);
        }
        bool operator != (const type_info& rhs)
        {
            return !operator==(rhs);
        }
        bool operator < (const type_info& rhs)
        {
            return (m_value.hash_code() < rhs.hash_code());
        }
        size_t hash_code() const {return m_value.hash_code();}
    };

#else // ANY_BRIDGE_USE_RTTI

    template <typename InterfaceClass>
    class type_info
    {
    private:
        explicit type_info(size_t v)
            : m_value(v)
        {}

        size_t m_value;

        static size_t s_type;

        template <typename T>
        static type_info base_type_id()
        {
            static bool init(false);
            static size_t result;
            if( !init )
            {
                result = ++s_type;
                init = true;
            }
            return type_info(result);
        }
    public:
        template <typename T>
        static type_info type_id()
        {
            return base_type_id<typename remove_const<typename remove_reference<T>::type>::type>();
        }
        bool operator == (const type_info& rhs)
        {
            return (m_value == rhs.m_value);
        }
        bool operator != (const type_info& rhs)
        {
            return !operator==(rhs);
        }
        bool operator < (const type_info& rhs)
        {
            return (m_value < rhs.m_value);
        }
        size_t hash_code() const {return m_value;}
    };

    template <typename InterfaceClass>
    //static
    size_t type_info<InterfaceClass>::s_type = 0;

#endif // ANY_BRIDGE_USE_RTTI

    //
    // Comparability classes...specialize for specific types as required
    //
    struct not_comparable
    {
        template <typename T, typename Interface>
        struct compare : public Interface
        {
            virtual ~compare() {}
        };
        template <typename Derived, typename T>
        struct compare_impl : public T
        {
            typedef T PlaceholderType;
        };
    };
    //
    // If your class supports equaility, but doesn't support operator==(), specialize this class...
    //
    struct equality_comparable
    {
        template <typename T, typename Interface>
        struct compare : public Interface
        {
            virtual ~compare() {}
            virtual bool equals(const T& other) const = 0;
        };
        template <typename Derived, typename T>
        struct compare_impl : public T
        {
            typedef T PlaceholderType;
            virtual bool equals(const T& other) const
            {
                if( this->type() == other.type() )
                {
                    // objects of the same type...value type comparison
                    const Derived* otherType = static_cast<const Derived*>(&other);
                    return equality_comparable::equals(static_cast<const Derived*>(this)->value(), otherType->value());
                }
                return false;
            }
        };
        template <typename T>
        static bool equals(const T& lhs, const T& rhs)
        {
            return (lhs == rhs);
        }
    };
    //
    // If your class supports less, but doesn't support operator<(), specialize this class...
    //
    struct less_than_comparable
    {
        template <typename T, typename Interface>
        struct compare : public Interface
        {
            virtual ~compare() {}
            virtual bool less(const T& other) const = 0;
        };
        template <typename Derived, typename T>
        struct compare_impl : public T
        {
            typedef T PlaceholderType;
            virtual bool less(const T& other) const
            {
                if( this->type() == other.type() )
                {
                    // objects of the same type...value type comparison
                    const Derived* otherType = static_cast<const Derived*>(&other);
                    return less_than_comparable::less(static_cast<const Derived*>(this)->value(), otherType->value());
                }
                if( this->type() < other.type() ) return true;
                
                return false;
            }
        };
        template <typename T>
        static bool less(const T& lhs, const T& rhs)
        {
            return (lhs < rhs);
        }
    };
    //
    // comparability for equals and less...
    //
    struct less_than_equals_comparable
    {
        template <typename T, typename Interface>
        struct compare : public Interface
        {
            virtual ~compare() {}
            virtual bool equals(const T& other) const = 0;
            virtual bool less(const T& other) const = 0;
        };
        template <typename Derived, typename T>
        struct compare_impl : public T
        {
            typedef T PlaceholderType;
            virtual bool equals(const T& other) const
            {
                if( this->type() == other.type() )
                {
                    // objects of the same type...value type comparison
                    const Derived* otherType = static_cast<const Derived*>(&other);
                    return equality_comparable::equals(static_cast<const Derived*>(this)->value(), otherType->value());
                }
                return false;
            }
            virtual bool less(const T& other) const
            {
                if( this->type() == other.type() )
                {
                    // objects of the same type...value type comparison
                    const Derived* otherType = static_cast<const Derived*>(&other);
                    return less_than_comparable::less(static_cast<const Derived*>(this)->value(), otherType->value());
                }
                if( this->type() < other.type() ) return true;
                
                return false;
            }
        };
    };

    template <typename Derived, typename Base, typename ValueType>
    class value_type_operations;

    template <typename Derived>
    class forwarder;

    template <int N>
    struct null_base { virtual ~null_base() {} };

    template <typename I0 = null_base<0>,
                typename I1 = null_base<1>,
                typename I2 = null_base<2>,
                typename I3 = null_base<3>,
                typename I4 = null_base<4>,
                typename I5 = null_base<5>,
                typename I6 = null_base<6> >
    struct interfaces : public I0, public I1, public I2, public I3, public I4, public I5, public I6
    {};

    template <typename Interface = interfaces<>, typename Comparable = less_than_equals_comparable>
    class any : public forwarder<any<Interface,Comparable> >
    {
        // CRTP base class has access to 'content'
        friend class forwarder<any>;
    public:
        typedef any AnyType;
    private:
        class placeholder : public Comparable::template compare<placeholder, Interface>
        {
        public: // queries
            virtual type_info<any> type() const  = 0;
            virtual placeholder* clone() const = 0;
        };
    public:
        //
        // This has to be the most derived class so that 'clone' doesn't slice,
        // so we use CRTP to enforce this condition
        //
        template<typename T>
        class holder : public value_type_operations<holder<T>, typename Comparable::template compare_impl<holder<T>,placeholder>, T>
        {
            // CRTP base class has access to 'held'
            friend class value_type_operations<holder<T>, typename Comparable::template compare_impl<holder<T>,placeholder>, T>;
        public: // structors
            typedef any AnyType;
            typedef T ValueType;

            explicit holder(const ValueType & v)
                : held(v)
            {
                //std::cout << "size=" << sizeof(holder<T>) << ", " << sizeof(T) << ", " << __alignof(holder<T>) << std::endl;
            }

        public: // queries

            ValueType value() const { return held; }

            virtual type_info<any> type() const
            {
                return type_info<any>::template type_id<ValueType>();
            }
            virtual placeholder* clone() const
            {
                return new holder(*this);
            }

        private: // intentionally left unimplemented
            holder & operator=(const holder &);

        private: // representation

            ValueType held;
        };

    public: // structors

        any()
            : content(0)
        {
        }

        template<typename ValueType>
        any(const ValueType & value)
            : content(new holder<ValueType>(value))
        {
        }

        any(const any & other)
            : content(other.content ? other.content->clone() : 0)
        {
        }

        ~any()
        {
            delete content;
        }

    public: // modifiers

        any & swap(any & rhs)
        {
            std::swap(content, rhs.content);
            return *this;
        }

        any & operator=(any rhs)
        {
            any(rhs).swap(*this);
            return *this;
        }

#ifdef ANY_BRIDGE_CALL_WITH
        template <typename Function>
        call_with<placeholder, Function, member_function_traits<Function>::arity> call(Function fn)
        {
            return call_with<placeholder, Function, member_function_traits<Function>::arity>(content, fn);
        }
#endif

    public: // queries

        bool empty() const
        {
            return !content;
        }

    public: // comparisons
        // equality
        friend bool operator==(const any& lhs, const any& rhs)
        {
            return lhs.content->equals(*rhs.content);
        }
        friend bool operator!=(const any& lhs, const any& rhs) {return !static_cast<bool>(lhs == rhs);}

        // less than comparable
        friend bool operator<(const any& lhs, const any& rhs)
        {
            return lhs.content->less(*rhs.content);
        }
        friend bool operator>(const any& lhs, const any& rhs)  { return rhs < lhs; }
        friend bool operator<=(const any& lhs, const any& rhs) { return !static_cast<bool>(rhs < lhs); }
        friend bool operator>=(const any& lhs, const any& rhs) { return !static_cast<bool>(lhs < rhs); }

    private: // types

    private: // representation

        placeholder* content;
    };

}

#endif // ANY_BRIDGE_HPP_INCLUDED
