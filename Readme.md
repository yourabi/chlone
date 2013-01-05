Chlone
====

Chlone is a simple tool to clone file permissions. Similar results can be achieved with a combination of stat (stat -x), chown, chmod, and chflags... However, scripting this functionality introduces another set of cross-platform intricacies (and headaches).

Chlone has been tested, and is supported on: Mac OS X 10.8, FreeBSD, and Ubuntu 12.04.1 LTS. It should work on any POSIX compliant system however cloning of "user-defined" flags is only supported on FreeBSD and Mac OS X at the moment.

Get Chlone
--------

Browse the source on GitHub: <http://github.com/yourabi/chlone>


Installation
--------
make

make install (or cp chlone /usr/local/bin)


Usage
--------

chlone *source* *destination*


Issues
------
Find a bug? Want a feature? Submit an [issue
here](http://github.com/yourabi/chlone/issues). Patches welcome!

License
------
Chlone is released under the [New BSD license](http://en.wikipedia.org/wiki/BSD_licenses).


Authors
-------
 * [Yousef Ourabi][1]


 [1]: http://github.com/yourabi