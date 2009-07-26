# -*- coding: utf-8 -*-
# Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#     http://www.boost.org/LICENSE_1_0.txt

import cookielib, datetime, urllib2, urlparse
from Fost.crypto import sha1_hmac
from Fost.settings import database
from _internet import url_filespec_encode, url_filespec_assert_valid


def fetch(*args, **kwargs):
    return agent().fetch(*args, **kwargs)


class agent(object):
    def __init__(self, base = "http://localhost/"):
        self.url = base
        # Fost settings for authentication
        self.fost = {}
        # Enable cookie jar
        self.cj = cookielib.LWPCookieJar()
        class RedirectStop(urllib2.HTTPRedirectHandler):
            def redirect_request(self, req, fp, code, msg, hdrs, newurl):
                return None
        self.opener = urllib2.build_opener(RedirectStop(), urllib2.HTTPCookieProcessor(self.cj))

    def fost_authenticate(self, key, secret, headers = {}):
        self.fost['key'] = key
        self.fost['secret'] = secret
        self.fost['headers'] = headers

    def fetch(self, url, data = None, headers = {}):
        """
            Fetches a single URL using either GET or POST depending on whether a body (data) is present
        """
        if len(self.fost):
            signed, signed_headers = 'X-FOST-Headers', []
            for header, value in self.fost['headers'].items():
                signed += ' ' + header
                signed_headers.append(value)
                headers[ header ] = value
            utcnow = unicode(datetime.datetime.utcnow())
            path = urlparse.urlsplit(url).path
            url_filespec_assert_valid(path)
            document = '%s %s\n%s\n%s\n%s' % (
                "POST" if data else "GET", path,
                utcnow,
                '\n'.join([signed] + signed_headers),
                data or urlparse.urlsplit(url).query
            )
            headers['X-FOST-Timestamp'] = utcnow
            headers['X-FOST-Headers'] = signed
            headers['Authorization'] = "FOST %s:%s" % (self.fost['key'], sha1_hmac(self.fost['secret'], document))
            #print document
        self.url = url
        return self.opener.open(urllib2.Request(url, data, headers))

    def process(self, url, configuration = {"parse_result":True}, data = None):
        """
            Processes a JSON request configuration starting at the specified URL with the specified body
        """
        try:
            url = urlparse.urljoin(self.url, url)
            from BeautifulSoup import BeautifulSoup
            response = self.fetch(url, data, configuration.get("headers", {}))
            response.mime_type = response.headers['Content-Type'].split(';')[0]
            if configuration.get("parse_result", True) and (
                response.mime_type  == 'text/html' or response.mime_type == 'text/xml'
            ):
                response.soup = BeautifulSoup(response.read())
            else:
                response.soup = BeautifulSoup('')
            return response
        except urllib2.HTTPError, e:
            status = int(str(e).split()[2][0:3])
            if status in configuration.get('status', [200, 301, 302, 303]):
                # This is OK -- the status matches what we're expecting
                class response(object):
                    soup = BeautifulSoup('')
                    def __init__(self, u):
                        self.url = u
                return response(url)
            raise
