PHP_ARG_ENABLE(asyscon, for Asyscon support, [  --enable-asyscon         Enable Asyscon support])

if test "$PHP_ASYSCON" != "no"; then
  AC_DEFINE(HAVE_ASYSCON, 1, [ ])
  PHP_NEW_EXTENSION(asyscon, asyscon.c, $ext_shared)
fi

AC_CACHE_CHECK([for utsname], ac_cv_have_utsname, [
  AC_TRY_COMPILE([
    #define _GNU_SOURCE
    #include <sys/utsname.h>
  ],[
    return sizeof(struct utsname);
  ],[
    ac_cv_have_utsname=yes
  ],[
    ac_cv_have_utsname=no
  ])
])
if test ac_cv_have_utsname=yes; then
  AC_DEFINE(HAVE_UTSNAME, 1, [We have sys/utsname.h and struct utsname])
fi

AC_CACHE_CHECK([for utsname.domainname], ac_cv_have_utsname_domainname, [
  AC_TRY_COMPILE([
    #define _GNU_SOURCE
    #include <sys/utsname.h>
  ],[
    return sizeof(((struct utsname *)0)->domainname);
  ],[
    ac_cv_have_utsname_domainname=yes
  ],[
    ac_cv_have_utsname_domainname=no
  ])
])
if test ac_cv_have_utsname_domainname=yes; then
  AC_DEFINE(HAVE_UTSNAME_DOMAINNAME, 1, [Whether struct utsname has domainname])
fi
