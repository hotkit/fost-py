# -*- coding: utf-8 -*-
# Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#     http://www.boost.org/LICENSE_1_0.txt

import sys
sys.path.append(r'Python')

from Fost.settings import database
print "Banner display is", "on" if database()['fpython', 'Banner'] else "off"