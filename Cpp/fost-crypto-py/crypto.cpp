/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <boost/python/module.hpp>
#include <boost/python/def.hpp>


char const* greet()
{
   return "hello, world";
}


BOOST_PYTHON_MODULE(crypto)
{
    using namespace boost::python;
    def("greet", greet);
}

