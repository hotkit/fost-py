import _crypto

assert _crypto.sha1("The quick brown fox jumps over the lazy dog") \
    == "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12", \
    _crypto.sha1("The quick brown fox jumps over the lazy dog")

assert _crypto.sha1(u"The quick brown fox jumps over the lazy dog") \
    == "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12", \
    _crypto.sha1(u"The quick brown fox jumps over the lazy dog")
