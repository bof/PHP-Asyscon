/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.0 of the PHP license,       |
  | that is available through the world-wide-web at the following url:   |
  | http://www.php.net/license/3_0.txt.                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Patrick Schaaf <github@bof.de>                               |
  | Site: http://github.com/bof/PHP-Asyscon/                             |
  +----------------------------------------------------------------------+
*/

#define _GNU_SOURCE

#define ASYSCON_VERSION "1.0"
#define ASYSCON_EXTNAME "asyscon"

#define VALUELEN 255

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_UTSNAME
#include <sys/utsname.h>
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINFO_FUNCTION(asyscon);
PHP_MINIT_FUNCTION(asyscon);
PHP_MSHUTDOWN_FUNCTION(asyscon);

static function_entry asyscon_functions[] = {
	{NULL, NULL, NULL}
};

PHP_INI_BEGIN()
PHP_INI_ENTRY("asyscon.host-name", "", PHP_INI_SYSTEM, NULL )
PHP_INI_ENTRY("asyscon.host-domain", "", PHP_INI_SYSTEM, NULL )
PHP_INI_ENTRY("asyscon.host-fqdn", "", PHP_INI_SYSTEM, NULL )
PHP_INI_ENTRY("asyscon.os-name", "", PHP_INI_SYSTEM, NULL )
PHP_INI_ENTRY("asyscon.os-release", "", PHP_INI_SYSTEM, NULL )
PHP_INI_ENTRY("asyscon.os-version", "", PHP_INI_SYSTEM, NULL )
PHP_INI_ENTRY("asyscon.os-machine", "", PHP_INI_SYSTEM, NULL )
PHP_INI_END()

enum asyscon_ini_settings_id {
	INI_ID_SET_HOST_NAME,
	INI_ID_SET_HOST_DOMAIN,
	INI_ID_SET_HOST_FQDN,
	INI_ID_SET_OS_NAME,
	INI_ID_SET_OS_RELEASE,
	INI_ID_SET_OS_VERSION,
	INI_ID_SET_OS_MACHINE,
};

struct asyscon_ini_setting {
	char *name;
};

static struct asyscon_ini_setting ini_settings[] = {
[INI_ID_SET_HOST_NAME] =	{ "asyscon.host-name" },
[INI_ID_SET_HOST_DOMAIN] =	{ "asyscon.host-domain" },
[INI_ID_SET_HOST_FQDN] =	{ "asyscon.host-fqdn" },
[INI_ID_SET_OS_NAME] =		{ "asyscon.os-name" },
[INI_ID_SET_OS_RELEASE] =	{ "asyscon.os-release" },
[INI_ID_SET_OS_VERSION] =	{ "asyscon.os-version" },
[INI_ID_SET_OS_MACHINE] =	{ "asyscon.os-machine" },
};
#define NR_INI_SETTINGS (sizeof(ini_settings)/sizeof(ini_settings[0]))

zend_module_entry asyscon_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    ASYSCON_EXTNAME,
    asyscon_functions,
    PHP_MINIT(asyscon),
    PHP_MSHUTDOWN(asyscon),
    NULL,
    NULL,
    PHP_MINFO(asyscon),
#if ZEND_MODULE_API_NO >= 20010901
    ASYSCON_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_ASYSCON
ZEND_GET_MODULE(asyscon)
#endif

PHP_MINFO_FUNCTION(asyscon) {
    php_info_print_table_start();
    php_info_print_table_row(2, "asyscon support", "enabled");
    php_info_print_table_row(2, "asyscon version", ASYSCON_VERSION);
    php_info_print_table_end();
}

#define PR_ERR(FMT, ...) \
	php_error(E_ERROR, "asyscon: " FMT, ## __VA_ARGS__)
#define PR_WARN(FMT, ...) \
	php_error(E_WARNING, "asyscon: " FMT, ## __VA_ARGS__)

static void add_string_constant(
	int module_number,
	char *N,
	char *V
TSRMLS_DC) {
	zend_constant zc[1];

	zc->name_len = strlen(N)+1;
	zc->name = zend_strndup(N, zc->name_len-1);
	zc->value.value.str.len = strlen(V);
	zc->value.value.str.val = zend_strndup(V, zc->value.value.str.len);
	zc->value.type = IS_STRING;
	zc->flags = CONST_CS | CONST_PERSISTENT;
	zc->module_number = module_number;
	zend_register_constant(zc TSRMLS_CC);
}

static inline char *ini_str(char *name)
{
	return zend_ini_string(name, strlen(name)+1, 0);
}

PHP_MINIT_FUNCTION(asyscon) {
	REGISTER_INI_ENTRIES();

	enum asyscon_ini_settings_id id;
#ifdef HAVE_UTSNAME
	int have_uts = -1;
	struct utsname uts;
#endif

	for (id = 0; id < NR_INI_SETTINGS; id++) {
		char *D, V[VALUELEN+1];
		int Vlen;
		char *N = ini_str(ini_settings[id].name);
		if (!N || !*N)
			continue;
#if HAVE_UTSNAME
		if (0 > have_uts)
			have_uts = (0 == uname(&uts));
		if (0 >= have_uts) {
			PR_WARN("uname() failed, cannot set '%s'", N);
			continue;
		}

		switch (id) {
		   case INI_ID_SET_HOST_NAME:
			Vlen = strcspn(uts.nodename, ".");
			if (Vlen > VALUELEN) Vlen = VALUELEN;
			if (Vlen > 0) memcpy(V, uts.nodename, Vlen);
			break;
		   case INI_ID_SET_HOST_DOMAIN:
#ifdef HAVE_UTSNAME_DOMAINNAME
			Vlen = strlen(uts.domainname);
			if (Vlen > VALUELEN) Vlen = VALUELEN;
			if (Vlen > 0) {
			       	memcpy(V, uts.domainname, Vlen);
				break;
			}
#endif
			if (!(D = strchr(uts.nodename, '.'))) {
				Vlen = 0;
				break;
			}
			Vlen = strlen(++D);
			if (Vlen > VALUELEN) Vlen = VALUELEN;
			if (Vlen > 0) memcpy(V, D, Vlen);
			break;
		   case INI_ID_SET_HOST_FQDN:
			Vlen = strlen(uts.nodename);
			if (Vlen > VALUELEN) Vlen = VALUELEN;
			if (Vlen > 0) memcpy(V, uts.nodename, Vlen);
#ifdef HAVE_UTSNAME_DOMAINNAME
			V[Vlen] = '\0';
			if (strchr(V, '.') || VALUELEN == Vlen)
				break;
			V[Vlen++] = '.';
			int Dlen = strlen(uts.domainname);
			if ((Vlen + Dlen) > VALUELEN) Dlen = VALUELEN - Vlen;
			if (Dlen > 0) {
				memcpy(V+Vlen, uts.domainname, Dlen);
				Vlen += Dlen;
			}
			break;
#endif
		   case INI_ID_SET_OS_NAME:
			Vlen = strlen(uts.sysname);
			if (Vlen > VALUELEN) Vlen = VALUELEN;
			memcpy(V, uts.sysname, Vlen);
			break;
		   case INI_ID_SET_OS_RELEASE:
			Vlen = strlen(uts.release);
			if (Vlen > VALUELEN) Vlen = VALUELEN;
			memcpy(V, uts.release, Vlen);
			break;
		   case INI_ID_SET_OS_VERSION:
			Vlen = strlen(uts.version);
			if (Vlen > VALUELEN) Vlen = VALUELEN;
			memcpy(V, uts.version, Vlen);
			break;
		   case INI_ID_SET_OS_MACHINE:
			Vlen = strlen(uts.machine);
			if (Vlen > VALUELEN) Vlen = VALUELEN;
			memcpy(V, uts.machine, Vlen);
			break;
		}
		V[Vlen] = '\0';
		add_string_constant(module_number, N, V TSRMLS_CC);
#else
		PR_WARN("no uname() support, cannot set '%s'", name);
#endif
	}
	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(asyscon) {
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}

