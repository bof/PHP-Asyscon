PHP Asyscon Extension
=====================

PHP Asyscon is an almost trivial PHP extension to add constants at start time
of the engine, for the entire runtime, with values related to the information
the **uname** system call returns, like **hostname**.

The name Asyscon, is short for **A**utomatic **SYS**tem **CON**stants

If anybody comes up with additional + easily accessible system related
material of interest to PHP scripts, that might be added to this extension.

php.ini Settings
----------------

When Asyscon is enabled, using the usual **extension=asyscon.so** somewhere,
you have available a number of php.ini settings, each giving the name a
constant for a certain part of the uname information should have.

In other words: you, the administrator, decide what information is
important to your PHP scripts, and how each constant should be named.

Without any of these php.ini settings, the extension does nothing at all.

* **asyscon.host-name** gives you the local host name, like what you
  see when running 'uname -n'. But **NOTE** that for consistencies sake,
  the name is *always* stripped at the first dot.
* **asyscon.host-fqdn** gives you the fully qualified host name.
  If running 'uname -n' shows you something with a dot in it, that
  will be used; otherwise the 'uname -n' name will be completed
  by what running 'domainname' shows you, with a dot in between.
* **asyscon.host-domain** gives you the local domain name. That will
  be either the thing shown when running 'domainname', if that is not empty,
  or possibly the stuff after the first dot in 'uname -n' output, if present.
* **asyscon.os-name** gives you the name of the OS, like 'uname -s'.
* **asyscon.os-release** gives you the release of the OS, like 'uname -r'.
* **asyscon.os-version** gives you the version of the OS, like 'uname -v'.
* **asyscon.os-machine** gives you the machine type of the OS, like 'uname -m'.

Some operating systems do not support the **uname** system call.
That will be detected at compile time. The php.ini settings are then
simply ignored - the constants will NOT be set. This might change in
the future, if somebody extends the code to cover these systems.

Some operating systems do not support the domainname information
in the **uname** syscall. That will be detected at compile time,
and the domainname will then simply only be derived from the part
after the first dot of the hostname.

Installation
============

* Download asyscon source package
* Unpack asyscon source package
* Go to asyscon folder and type "phpize && ./configure && make"
* Maybe run the included test cases:
	make test PHP_EXECUTABLE=/usr/bin/php5 TEST_PHP_ARGS="-q"
* If all is well, run "make install", as root, to copy modules/asyscon.so
  to your PHP extension directory.
* Make sure you have extension=asyscon.so in your php.ini, or in a file
  like /etc/php5/conf.d/asyscon.ini
* Add configuration php.ini or somewhere else, which lists the names
  of the constants you would like to have set, as detailled above.

