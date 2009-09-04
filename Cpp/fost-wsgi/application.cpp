/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-wsgi.hpp"
#include <fost/python>
#include <fost/threading>
#include <fost/detail/wsgi.application.hpp>

#include <boost/lambda/bind.hpp>
#include <boost/python/stl_iterator.hpp>


using namespace fostlib;


namespace {
    /*
        Because g_response is currently global it must be protected in case this application gets used in a threaded
        environment.
    */
    boost::mutex g_mutex;
    std::auto_ptr< mime > g_response;

    boost::python::object start_response(boost::python::object status, boost::python::list headers) {
        mime::mime_headers response_headers;
        for ( boost::python::stl_input_iterator<boost::python::tuple> i(headers), e; i != e; ++i )
            response_headers.set( boost::python::extract<string>((*i)[0])(), boost::python::extract<string>((*i)[1])() );
        g_response = std::auto_ptr< mime >( new text_body( string("No body yet"), response_headers, L"text/plain" ) );
        return boost::python::object();
    }
}


fostlib::python::wsgi::application::application( const string &appname ) {
    std::size_t last_dot = appname.find_last_of(".");
    if ( last_dot == string::npos )
        throw exceptions::not_implemented("The application name must include a . as there must be a module to import it from", appname);
    m_module = boost::python::import(coerce< boost::python::str >(appname.substr(0, last_dot)));
    m_application = boost::python::getattr(m_module, coerce< boost::python::str >(appname.substr(last_dot + 1)));
}

std::auto_ptr< mime > fostlib::python::wsgi::application::operator () (
    http::server::request &req, boost::python::dict environ
) const {
    // Set up the environment for the request
    environ["PATH_INFO"] = boost::python::str(req.file_spec().underlying().underlying());
    for ( mime::mime_headers::const_iterator header( req.data().headers().begin() ); header != req.data().headers().end(); ++header ) {
        std::string name = replaceAll(header->first, L"-", L"_").std_str();
        for ( std::string::iterator c(name.begin()); c != name.end(); ++c)
            *c = std::toupper(*c);
        environ["HTTP_" + name] = header->second.value();
    }
    // Process the request
    std::auto_ptr< mime > response;
    string result;
    boost::mutex::scoped_lock lock(g_mutex); // Protect g_response
    boost::python::object strings = m_application(environ, start_response);
    for ( boost::python::stl_input_iterator<string> i(strings), e; i != e; ++i )
        result += *i;
    return g_response;
}
