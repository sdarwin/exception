//Copyright (c) 2006-2009 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_1A590226753311DD9E4CCF6156D89593
#define UUID_1A590226753311DD9E4CCF6156D89593

#include <boost/config.hpp>
#include <boost/exception/exception.hpp>
#include <boost/exception/detail/error_info_impl.hpp>
#include <boost/exception/detail/type_info.hpp>
#include <boost/assert.hpp>

#if (__GNUC__*100+__GNUC_MINOR__>301) && !defined(BOOST_EXCEPTION_ENABLE_WARNINGS)
#pragma GCC system_header
#endif
#if defined(_MSC_VER) && !defined(BOOST_EXCEPTION_ENABLE_WARNINGS)
#pragma warning(push,1)
#endif

namespace
boost
    {
    namespace
    exception_detail
        {
        inline
        error_info_container *
        get_error_info_container( exception const & x )
            {
            return x.data_.get();
            }

        template <class ErrorInfo>
        typename ErrorInfo::value_type *
        get_info( exception const & x )
            {
            if( exception_detail::error_info_container * c=get_error_info_container(x) )
                if( exception_detail::error_info_base * eib = c->get(BOOST_EXCEPTION_STATIC_TYPEID(ErrorInfo)) )
                    {
#ifndef BOOST_NO_RTTI
                    BOOST_ASSERT( 0!=dynamic_cast<ErrorInfo *>(eib) );
#endif
                    ErrorInfo * w = static_cast<ErrorInfo *>(eib);
                    return &w->value();
                    }
            return 0;
            }

        template <>
        inline
        char const * *
        get_info<throw_function>( exception const & x )
            {
            char const * & v=access_throw_function(x);
            return v ? &v : 0;
            }

        template <>
        inline
        char const * *
        get_info<throw_file>( exception const & x )
            {
            char const * & v=access_throw_file(x);
            return v ? &v : 0;
            }

        template <>
        inline
        int *
        get_info<throw_line>( exception const & x )
            {
            int & v=access_throw_line(x);
            return v!=-1 ? &v : 0;
            }

        template <class T,class R>
        struct
        get_error_info_return_type
            {
            typedef R * type;
            };

        template <class T,class R>
        struct
        get_error_info_return_type<T const,R>
            {
            typedef R const * type;
            };
        }

#ifdef BOOST_NO_RTTI
    template <class ErrorInfo>
    inline
    typename ErrorInfo::value_type const *
    get_error_info( boost::exception const & x )
        {
        return exception_detail::get_info<ErrorInfo>(x);
        }
    template <class ErrorInfo>
    inline
    typename ErrorInfo::value_type *
    get_error_info( boost::exception & x )
        {
        return exception_detail::get_info<ErrorInfo>(x);
        }
#else
    template <class ErrorInfo,class E>
    inline
    typename exception_detail::get_error_info_return_type<E,typename ErrorInfo::value_type>::type
    get_error_info( E & some_exception )
        {
        if( exception const * x = dynamic_cast<exception const *>(&some_exception) )
            return exception_detail::get_info<ErrorInfo>(*x);
        else
            return 0;
        }
#endif
    }

#if defined(_MSC_VER) && !defined(BOOST_EXCEPTION_ENABLE_WARNINGS)
#pragma warning(pop)
#endif
#endif
