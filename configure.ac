AC_INIT([mysane], [1.2.3], [jjainam@yahoo.com])
AM_INIT_AUTOMAKE([subdir-objects])  #This allows helloworld_sources to be from subdirectories
#AM_INIT_AUTOMAKE
AC_PROG_CC

V_MAJOR=`echo $VERSION | $AWK -F. '{print $1}'`
V_MINOR=`echo $VERSION | $AWK -F. '{print $2}'`

AC_MSG_NOTICE([Checking if version is getting printed $V_MINOR----------------------------------- ])

AC_DEFINE_UNQUOTED(SANE_DLL_V_MAJOR, $V_MAJOR, [SANE DLL major number]) 
AC_DEFINE_UNQUOTED(SANE_DLL_V_MINOR, $V_MINOR, [SANE DLL minor number])

AC_SUBST(V_MAJOR) #Passes this to Makefile.am file
AC_SUBST(V_MINOR)

AM_CPPFLAGS="-DV_MAJOR=${V_MAJOR} -DV_MINOR=${V_MINOR}" 

AC_SUBST(AM_CPPFLAGS)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT