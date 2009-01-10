/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/python>
#include "connection.hpp"
#include "jsonblob.hpp"


using namespace fostlib;


BOOST_PYTHON_MODULE( _jsondb ) {
    using namespace boost::python;
    python_string_registration();
    python_json_registration();

    class_<
        jcursor
    >(
        "jcursor", init<>()
    )
    ;

    class_<
        jsondb::local, std::auto_ptr< jsondb::local >, boost::noncopyable
    >(
        "jsonblob_local", no_init
    )
        .add_property("json", get_json)
        .def(self == json())
    ;

    class_<
        jsondb, std::auto_ptr< jsondb >, boost::noncopyable
    >(
        "jsonblob", init< optional< string, json > >()
    )
        .add_property("filename", filename)
        .add_property("local", get_local)
    ;
}
