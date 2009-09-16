/*
    Copyright 2008-2009 Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/pyhost>
#include <fost/wsgi>
#include <fost/main>
#include <fost/internet>
#include <fost/detail/http.server.hpp>
#include <fost/threading>


using namespace fostlib;


namespace {
    setting< string > c_host( L"fwsgi", L"Server", L"Bind to", L"localhost", true );
    setting< int > c_port( L"fwsgi", L"Server", L"Port", 8001, true );

    setting< string > c_application( L"fwsgi", L"WSGI", L"Application", "wsgiref.simple_server.demo_app", true );

    bool service( python::wsgi::application &app, boost::shared_ptr< http::server::request > req ) {
        (*req)( *app(*req) );
        return true;
    }
}


FSL_MAIN(
    L"fwsgi",
    L"Simple HTTP WSGI server\nCopyright (c) 2008-2009, Felspar Co. Ltd."
)( fostlib::ostream &o, fostlib::arguments &args ) {
    // Create a worker pool to service the requests
    workerpool pool;
    // Work out the IP and port bindings
    http::server server( host( args[1].value(c_host.value()) ), c_port.value() );
    o << L"Answering requests on http://" << server.binding() << L":" << server.port() << L"/" << std::endl;
    // Create the Python host
    fostlib::python::inproc_host host;
    // Build the application object that will handle the web service
    python::wsgi::application app( c_application.value() );
    // Keep serving forever
    for ( bool process( true ); process; ) {
        boost::shared_ptr< http::server::request > req( server().release() );
        o << req->method() << L" " << req->file_spec() << " peak threads: " << pool.peak_used() << std::endl;
        // Threaded version breaks in the Python libraries when under siege
        //pool.f<bool>( boost::lambda::bind( service, boost::ref(app), req ) );
        service(app, req);
    }
    return 0;
}
