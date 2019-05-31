/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2018 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_iMoocLog.h"

/* If you declare any globals in php_iMoocLog.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(iMoocLog)
*/

/* True global resources - no need for thread safety here */
static int le_iMoocLog;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("iMoocLog.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_iMoocLog_globals, iMoocLog_globals)
    STD_PHP_INI_ENTRY("iMoocLog.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_iMoocLog_globals, iMoocLog_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_iMoocLog_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(write)
{
	int argc = ZEND_NUM_ARGS();
	zval *messages = NULL;
	zval *context =NULL;
	char *log_path = "/tmp/test_log.log";
	zend_string *logger = NULL;

	if (zend_parse_parameters( argc TSRMLS_CC, "z|zS", &messages, &context, &logger) == FAILURE) {
		return;
	}

	zend_string *s = zval_get_string(messages);
	char *log_message = (char *) malloc(ZSTR_LEN(s)+1);
	sprintf(log_message, "%s%s", ZSTR_VAL(s),"\n");

	php_stream *stream;
	size_t written;
	stream = php_stream_open_wrapper(log_path, "a", IGNORE_URL_WIN, NULL);
	written  = php_stream_write(stream, log_message, strlen(log_message));

	zend_string_release(s);
	free(log_message);
	php_stream_close(stream);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_iMoocLog_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_iMoocLog_init_globals(zend_iMoocLog_globals *iMoocLog_globals)
{
	iMoocLog_globals->global_value = 0;
	iMoocLog_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(iMoocLog)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(iMoocLog)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(iMoocLog)
{
#if defined(COMPILE_DL_IMOOCLOG) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(iMoocLog)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(iMoocLog)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "iMoocLog support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ iMoocLog_functions[]
 *
 * Every user visible function must have an entry in iMoocLog_functions[].
 */
const zend_function_entry iMoocLog_functions[] = {
	PHP_FE(write,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in iMoocLog_functions[] */
};
/* }}} */

/* {{{ iMoocLog_module_entry
 */
zend_module_entry iMoocLog_module_entry = {
	STANDARD_MODULE_HEADER,
	"iMoocLog",
	iMoocLog_functions,
	PHP_MINIT(iMoocLog),
	PHP_MSHUTDOWN(iMoocLog),
	PHP_RINIT(iMoocLog),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(iMoocLog),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(iMoocLog),
	PHP_IMOOCLOG_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_IMOOCLOG
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(iMoocLog)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
