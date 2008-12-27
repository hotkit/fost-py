/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "jsonblob.hpp"


using namespace fostlib;


jsonblob::jsonblob() {
}
jsonblob::jsonblob( const string &filename )
: filename( filename ) {
}
jsonblob::jsonblob( const string &filename, const string &json )
: filename( filename ) {
}
