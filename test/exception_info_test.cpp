//Copyright (c) 2006-2009 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/exception/exception_info.hpp>
#include <boost/detail/lightweight_test.hpp>

int count=0;

struct tag1 { typedef int type; };
struct tag2 { typedef float type; };

class
test_exception
    {
    test_exception & operator=( test_exception const & )=delete;
    test_exception & operator=( test_exception && )=delete;
    public:
    test_exception()
        {
        ++count;
        }
    test_exception( test_exception const & )
        {
        ++count;
        }
    test_exception( test_exception && )
        {
        ++count;
        }
    virtual
    ~test_exception() throw() /*noexcept*/
        {
        --count;
        }
    };
template <class Tag,class E>
void
verify( E & e, typename Tag::type correct )
    {
    test_exception const & ec=e;
    boost::exception_info * xi=boost::get_exception_info(e);
    BOOST_TEST(xi!=0);
    boost::exception_info const * xic=boost::get_exception_info(ec);
    BOOST_TEST(xic!=0);
    typename Tag::type * v=xi->get<Tag>();
    BOOST_TEST(v!=0);
    typename Tag::type const * vc=xic->get<Tag>();
    BOOST_TEST(v==vc);
    BOOST_TEST_EQ(*v,correct);
    }
void
test_exception_info_copy()
    {
    boost::exception_info xi1;
    xi1.set<tag1>(42);
    xi1.set<tag2>(42.42f);
    BOOST_TEST_EQ(*xi1.get<tag1>(),42);
    BOOST_TEST_EQ(*xi1.get<tag2>(),42.42f);
    boost::exception_info xi2(xi1);
    BOOST_TEST_EQ(*xi1.get<tag1>(),42);
    BOOST_TEST_EQ(*xi1.get<tag2>(),42.42f);
    BOOST_TEST_EQ(*xi2.get<tag1>(),42);
    BOOST_TEST_EQ(*xi2.get<tag2>(),42.42f);
    BOOST_TEST(xi2.get<tag1>()!=xi1.get<tag1>());
    BOOST_TEST(xi2.get<tag2>()!=xi1.get<tag2>());
    }
void
test_exception_info_move()
    {
    boost::exception_info xi1;
    xi1.set<tag1>(42);
    xi1.set<tag2>(42.42f);
    BOOST_TEST_EQ(*xi1.get<tag1>(),42);
    BOOST_TEST_EQ(*xi1.get<tag2>(),42.42f);
    boost::exception_info xi2(std::move(xi1));
    BOOST_TEST(!xi1.get<tag1>());
    BOOST_TEST(!xi1.get<tag2>());
    BOOST_TEST_EQ(*xi2.get<tag1>(),42);
    BOOST_TEST_EQ(*xi2.get<tag2>(),42.42f);
    }
void
test_exception_info_assignment()
    {
    boost::exception_info xi1;
    xi1.set<tag1>(42);
    xi1.set<tag2>(42.42f);
    BOOST_TEST_EQ(*xi1.get<tag1>(),42);
    BOOST_TEST_EQ(*xi1.get<tag2>(),42.42f);
    boost::exception_info xi2; xi2=xi1;
    BOOST_TEST_EQ(*xi1.get<tag1>(),42);
    BOOST_TEST_EQ(*xi1.get<tag2>(),42.42f);
    BOOST_TEST_EQ(*xi2.get<tag1>(),42);
    BOOST_TEST_EQ(*xi2.get<tag2>(),42.42f);
    BOOST_TEST(xi2.get<tag1>()!=xi1.get<tag1>());
    BOOST_TEST(xi2.get<tag2>()!=xi1.get<tag2>());
    }
void
test_exception_info_rv_assignment()
    {
    boost::exception_info xi1;
    xi1.set<tag1>(42);
    xi1.set<tag2>(42.42f);
    BOOST_TEST_EQ(*xi1.get<tag1>(),42);
    BOOST_TEST_EQ(*xi1.get<tag2>(),42.42f);
    boost::exception_info xi2; xi2=std::move(xi1);
    BOOST_TEST(!xi1.get<tag1>());
    BOOST_TEST(!xi1.get<tag2>());
    BOOST_TEST_EQ(*xi2.get<tag1>(),42);
    BOOST_TEST_EQ(*xi2.get<tag2>(),42.42f);
    }
void
test_throw_catch()
    {
    try
        {
        boost::throw_with_info(test_exception());
        BOOST_ERROR("throw_with_info didn't throw");
        }
    catch( test_exception & e )
        {
        test_exception const & ec=e;
        BOOST_TEST(boost::get_exception_info(e)==boost::get_exception_info(ec));
        BOOST_TEST(dynamic_cast<boost::exception_info *>(&e)!=0);
        BOOST_TEST(dynamic_cast<boost::exception_info *>(&e)==boost::get_exception_info(e));
        }
    catch(...)
        {
        BOOST_ERROR("throw_with_info catch failure");
        }
    }                                           
void
test_throw_catch_info()
    {
    try
        {
        boost::throw_with_info(test_exception(),boost::exception_info(__FILE__,__LINE__,__func__).set<tag1>(42).set<tag2>(42.42f));
        BOOST_ERROR("throw_with_info didn't throw");
        }
    catch( test_exception & e )
        {
        verify<tag1>(e,42);
        verify<tag2>(e,42.42f);
            {
            boost::exception_info * xi=boost::get_exception_info(e);
            BOOST_TEST(strcmp(xi->file(),__FILE__)==0);
            BOOST_TEST_EQ(xi->line(),138);
            BOOST_TEST(strstr(xi->function(),"test_throw_catch_info")!=0);
            BOOST_TEST(xi!=0);
            xi->set<tag1>(43);
            xi->set<tag2>(43.43f);
            }
        verify<tag1>(e,43);
        verify<tag2>(e,43.43f);
            {
            boost::exception_info * xi=boost::get_exception_info(e);
            BOOST_TEST(xi!=0);
            xi->unset<tag1>();
            xi->unset<tag2>();
            BOOST_TEST(xi->get<tag1>()==0);
            BOOST_TEST(xi->get<tag2>()==0);
            }
        }
    catch(...)
        {
        BOOST_ERROR("throw_with_info catch failure");
        }
    }
void
test_throw_catch_add_info()
    {
    try
        {
        try
            {
            boost::throw_with_info(test_exception(),boost::exception_info().set<tag1>(42));
            BOOST_ERROR("throw_with_info didn't throw");
            }
        catch( boost::exception_info & xi )
            {
            xi.set<tag2>(42.42f);
            BOOST_TEST(xi.file()==0);
            BOOST_TEST_EQ(xi.line(),0);
            BOOST_TEST(xi.function()==0);
            throw;
            }
        catch(...)
            {
            BOOST_ERROR("throw_with_info catch failure");
            }
        }
    catch( test_exception & e )
        {
        verify<tag1>(e,42);
        verify<tag2>(e,42.42f);
            {
            boost::exception_info * xi=boost::get_exception_info(e);
            BOOST_TEST(xi!=0);
            xi->set<tag1>(43);
            xi->set<tag2>(43.43f);
            }
        verify<tag1>(e,43);
        verify<tag2>(e,43.43f);
            {
            boost::exception_info * xi=boost::get_exception_info(e);
            BOOST_TEST(xi!=0);
            xi->unset<tag1>();
            xi->unset<tag2>();
            BOOST_TEST(xi->get<tag1>()==0);
            BOOST_TEST(xi->get<tag2>()==0);
            }
        }
    catch(...)
        {
        BOOST_ERROR("throw_with_info catch failure");
        }
    }
void
test_diagnostic_info()
    {
    std::string s1;
    try
        {
        boost::throw_with_info(test_exception());
        BOOST_ERROR("boost::throw_with_info didn't throw");
        }
    catch( boost::exception_info & xi )
        {
        s1=xi.diagnostic_info();
        }
    BOOST_TEST(!s1.empty());
    std::string s2;
    try
        {
        boost::throw_with_info(test_exception());
        BOOST_ERROR("boost::throw_with_info didn't throw");
        }
    catch( test_exception & e )
        {
        s2=boost::exception_diagnostic_info(e);
        }
    BOOST_TEST_LE(s1.size(),s2.size());
    BOOST_TEST(s2.find(s1)!=std::string::npos);
    std::string s3;
    try
        {
        boost::throw_with_info(test_exception());
        BOOST_ERROR("boost::throw_with_info didn't throw");
        }
    catch( ... )
        {
        s3=boost::exception_diagnostic_info(boost::current_exception());
        }
    BOOST_TEST_EQ(s3,s2);
    }
int
main()
    {
    test_exception_info_copy();
    test_exception_info_move();
    test_exception_info_assignment();
    test_exception_info_rv_assignment();
    test_throw_catch();
    test_throw_catch_info();
    test_throw_catch_add_info();
    test_diagnostic_info();
    BOOST_TEST(count==0);
    return boost::report_errors();
    }
