//Copyright (c) 2006-2009 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_4CDEC1F0086811E7AD630C0D63EDCBF1
#define UUID_4CDEC1F0086811E7AD630C0D63EDCBF1

#ifndef BOOST_EXCEPTION_ENABLE_EXCEPTION_INFO
#error This header requires BOOST_EXCEPTION_ENABLE_EXCEPTION_INFO
#endif

#include <boost/exception/diagnostic_information.hpp>
#include <boost/exception_ptr.hpp>

namespace
boost
    {
    template <class E>
    void
    throw_with_info( E const & e, exception_info const & xi )
        {
        struct
        wrapper:
            E,
            exception_info
            {
            wrapper( E const & e, exception_info const & xi ):
                E(e),
                exception_info(xi)
                {
                }
            };
        throw enable_current_exception(wrapper(e,xi));
        }
    template <class E>
    void
    throw_with_info( E && e, exception_info && xi=exception_info() )
        {
        struct
        wrapper:
            E,
            exception_info
            {
            wrapper( E  && e, exception_info && xi ):
                E(e),
                exception_info(xi)
                {
                }
            };
        throw enable_current_exception(wrapper(std::move(e),std::move(xi)));
        }

    ////////////////////////////////////////////////////////////////////////

    template <class E>
    exception_info const *
    get_exception_info( E const & e )
        {
        return dynamic_cast<exception_info const *>(&e);
        }
    template <class E>
    exception_info *
    get_exception_info( E & e )
        {
        return dynamic_cast<exception_info *>(&e);
        }

    ////////////////////////////////////////////////////////////////////////

    template <class E>
    std::string
    exception_diagnostic_info( E const & e )
        {
        return diagnostic_information(e);
        }
    inline
    std::string
    exception_diagnostic_info( exception_ptr const & p )
        {
        return diagnostic_information(p);
        }

    ////////////////////////////////////////////////////////////////////////

    inline
    exception_info::
    exception_info( char const * file, int line, char const * function ) /*noexcept*/
        {
        exception_detail::access_throw_function(*this)=function;
        exception_detail::access_throw_file(*this)=file;
        exception_detail::access_throw_line(*this)=line;
        }
    inline
    exception_info::
    exception_info( exception_info && r ) /*noexcept*/
        {
        *this=std::move(r);
        }
    inline
    exception_info &
    exception_info::
    operator=( exception_info const & r )
        {
        exception_detail::copy_boost_exception(this,&r);
        return *this;
        }
    inline
    exception_info &
    exception_info::
    operator=( exception_info && r ) /*noexcept*/
        {
        struct
        wrapper:
            exception
            {
            virtual
            ~wrapper() throw()
                {
                }
            };
        static_cast<exception &>(*this)=static_cast<exception const &>(r);
        static_cast<exception &>(r)=wrapper();
        return *this;
        }
    inline
    char const *
    exception_info::
    file() const /*noexcept*/
        {
        return exception_detail::access_throw_file(*this);
        }
    inline
    int
    exception_info::
    line() const /*noexcept*/
        {
        return exception_detail::access_throw_line(*this);
        }
    inline
    char const *
    exception_info::
    function() const /*noexcept*/
        {
        return exception_detail::access_throw_function(*this);
        }
    template <class Tag>
    exception_info &
    exception_info::
    unset()
        {
        exception_detail::unset_info<error_info<Tag,typename Tag::type> >(*this);
        return *this;
        }
    template <class Tag>
    exception_info &
    exception_info::
    set( typename Tag::type const & x )
        {
        exception_detail::set_info(*this,error_info<Tag,typename Tag::type>(x));
        return *this;
        }
    template <class Tag>
    exception_info &
    exception_info::
    set( typename Tag::type && x )
        {
        exception_detail::set_info(*this,error_info<Tag,typename Tag::type>(x));
        return *this;
        }
    template <class Tag>
    typename Tag::type const *
    exception_info::
    get() const
        {
        return boost::get_error_info<error_info<Tag,typename Tag::type> >(*this);
        }
    template <class Tag>
    typename Tag::type *
    exception_info::
    get()
        {
        return boost::get_error_info<error_info<Tag,typename Tag::type> >(*this);
        }
    inline
    char const *
    exception_info::
    diagnostic_info() const
        {
        return diagnostic_information_what(*this);
        }

    ////////////////////////////////////////////////////////////////////////

    struct xi_file_name { typedef std::string type; };
    struct xi_file { typedef FILE * type; };
    struct xi_fileno { typedef int type; };
    struct xi_errno { typedef int type; };
    struct xi_api_function { typedef char const * type; };
    }

#endif
