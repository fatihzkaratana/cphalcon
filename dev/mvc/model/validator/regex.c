
/*
  +------------------------------------------------------------------------+
  | Phalcon Framework                                                      |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2012 Phalcon Team (http://www.phalconphp.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@phalconphp.com so we can send you a copy immediately.       |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
  |          Eduar Carvajal <eduar@phalconphp.com>                         |
  +------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_phalcon.h"
#include "phalcon.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"
#include "kernel/memory.h"

#include "kernel/fcall.h"
#include "kernel/exception.h"
#include "kernel/array.h"
#include "kernel/concat.h"

/**
 * Phalcon\Mvc\Model\Validator\Regex
 *
 * Allows to validate if the value of a field matches a regular expression
 *
 *
 *
 */

/**
 * Executes the validator
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model_Validator_Regex, validate){

	zval *record = NULL, *field_name = NULL, *value = NULL, *failed = NULL, *matches = NULL;
	zval *pattern = NULL;
	zval *c0 = NULL, *c1 = NULL, *c2 = NULL, *c3 = NULL;
	zval *r0 = NULL, *r1 = NULL, *r2 = NULL, *r3 = NULL, *r4 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &record) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	PHALCON_INIT_VAR(c0);
	ZVAL_STRING(c0, "field", 1);
	PHALCON_INIT_VAR(field_name);
	PHALCON_CALL_METHOD_PARAMS_1(field_name, this_ptr, "getoption", c0, PH_NO_CHECK);
	if (Z_TYPE_P(field_name) != IS_STRING) {
		PHALCON_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Field name must be a string");
		return;
	}
	
	PHALCON_INIT_VAR(c1);
	ZVAL_STRING(c1, "pattern", 1);
	
	PHALCON_ALLOC_ZVAL_MM(r0);
	PHALCON_CALL_METHOD_PARAMS_1(r0, this_ptr, "issetoption", c1, PH_NO_CHECK);
	if (!zend_is_true(r0)) {
		PHALCON_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Validator requires a perl-compatible regex pattern");
		return;
	}
	
	PHALCON_INIT_VAR(value);
	PHALCON_CALL_METHOD_PARAMS_1(value, record, "readattribute", field_name, PH_NO_CHECK);
	
	PHALCON_INIT_VAR(failed);
	ZVAL_BOOL(failed, 0);
	
	PHALCON_INIT_VAR(matches);
	ZVAL_NULL(matches);
	
	PHALCON_INIT_VAR(c2);
	ZVAL_STRING(c2, "pattern", 1);
	
	PHALCON_INIT_VAR(pattern);
	PHALCON_CALL_METHOD_PARAMS_1(pattern, this_ptr, "getoption", c2, PH_NO_CHECK);
	Z_SET_ISREF_P(matches);
	
	PHALCON_ALLOC_ZVAL_MM(r1);
	PHALCON_CALL_FUNC_PARAMS_3(r1, "preg_match", pattern, value, matches);
	Z_UNSET_ISREF_P(matches);
	if (zend_is_true(r1)) {
		PHALCON_ALLOC_ZVAL_MM(r2);
		phalcon_array_fetch_long(&r2, matches, 0, PH_NOISY_CC);
		PHALCON_ALLOC_ZVAL_MM(r3);
		is_not_equal_function(r3, r2, value TSRMLS_CC);
		if (zend_is_true(r3)) {
			PHALCON_INIT_VAR(failed);
			ZVAL_BOOL(failed, 1);
		}
	} else {
		PHALCON_INIT_VAR(failed);
		ZVAL_BOOL(failed, 1);
	}
	
	if (zend_is_true(failed)) {
		PHALCON_ALLOC_ZVAL_MM(r4);
		PHALCON_CONCAT_SVS(r4, "Value of field '", field_name, "' doesn't match regular expression");
		PHALCON_INIT_VAR(c3);
		ZVAL_STRING(c3, "regex", 1);
		PHALCON_CALL_METHOD_PARAMS_3_NORETURN(this_ptr, "appendmessage", r4, field_name, c3, PH_NO_CHECK);
		PHALCON_MM_RESTORE();
		RETURN_FALSE;
	}
	
	PHALCON_MM_RESTORE();
	RETURN_TRUE;
}

