AC_INIT([My Bittorrent], [1,0], [], [my-bittorrent], [])

AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_PROG_CC_C99

#AC_CHECK_LIB([libcurl], [curl_easy_init], [],
#    [AC_MSG_ERROR([Unable to find libcurl])]

#AC_CHECK_LIB([openssl], [EVP_DigestInit], [],
#     [AC_MSG_ERROR([Unable to find openssl])]


#AC_CHECK_HEADERS([openssl/evp.h])
#AC_CHECK_HEADERS([curl/curl.h])
AC_CHECK_HEADERS([string.h])

AC_CHECK_FUNCS([getdelim])

AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT


