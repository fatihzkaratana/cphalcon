
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

#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/require.h"
#include "kernel/concat.h"

/**
 * Phalcon\Loader
 *
 * This component helps to load your project classes automatically based on some conventions
 *
 *
 */

PHP_METHOD(Phalcon_Loader, __construct){

	zval *a0 = NULL, *a1 = NULL, *a2 = NULL, *a3 = NULL;

	PHALCON_MM_GROW();

	
	PHALCON_ALLOC_ZVAL_MM(a0);
	array_init(a0);
	zend_update_property(phalcon_loader_ce, this_ptr, SL("_classes"), a0 TSRMLS_CC);
	
	PHALCON_ALLOC_ZVAL_MM(a1);
	array_init(a1);
	add_next_index_stringl(a1, SL("php"), 1);
	zend_update_property(phalcon_loader_ce, this_ptr, SL("_extensions"), a1 TSRMLS_CC);
	
	PHALCON_ALLOC_ZVAL_MM(a2);
	array_init(a2);
	zend_update_property(phalcon_loader_ce, this_ptr, SL("_namespaces"), a2 TSRMLS_CC);
	
	PHALCON_ALLOC_ZVAL_MM(a3);
	array_init(a3);
	zend_update_property(phalcon_loader_ce, this_ptr, SL("_directories"), a3 TSRMLS_CC);

	PHALCON_MM_RESTORE();
}

PHP_METHOD(Phalcon_Loader, setEventsManager){

	zval *events_manager = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &events_manager) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	phalcon_update_property_zval(this_ptr, SL("_eventsManager"), events_manager TSRMLS_CC);
	
	PHALCON_MM_RESTORE();
}

PHP_METHOD(Phalcon_Loader, setExtensions){

	zval *extensions = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &extensions) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (Z_TYPE_P(extensions) != IS_ARRAY) { 
		PHALCON_THROW_EXCEPTION_STR(phalcon_loader_exception_ce, "Parameter $extensions must be an Array");
		return;
	}
	phalcon_update_property_zval(this_ptr, SL("_extensions"), extensions TSRMLS_CC);
	
	RETURN_CCTOR(this_ptr);
}

/**
 * Register namespaces and their related directories
 *
 * @param array $namespaces
 */
PHP_METHOD(Phalcon_Loader, registerNamespaces){

	zval *namespaces = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &namespaces) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (Z_TYPE_P(namespaces) != IS_ARRAY) { 
		PHALCON_THROW_EXCEPTION_STR(phalcon_loader_exception_ce, "Parameter $namespaces must be an Array");
		return;
	}
	phalcon_update_property_zval(this_ptr, SL("_namespaces"), namespaces TSRMLS_CC);
	
	RETURN_CCTOR(this_ptr);
}

/**
 * Register directories on which "not found" classes could be found
 *
 * @param array $directories
 */
PHP_METHOD(Phalcon_Loader, registerDirs){

	zval *directories = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &directories) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (Z_TYPE_P(directories) != IS_ARRAY) { 
		PHALCON_THROW_EXCEPTION_STR(phalcon_loader_exception_ce, "Parameter $directories must be an Array");
		return;
	}
	phalcon_update_property_zval(this_ptr, SL("_directories"), directories TSRMLS_CC);
	
	PHALCON_MM_RESTORE();
}

/**
 * Register classes and their locations
 *
 * @param array $directories
 */
PHP_METHOD(Phalcon_Loader, registerClasses){

	zval *classes = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &classes) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (Z_TYPE_P(classes) != IS_ARRAY) { 
		PHALCON_THROW_EXCEPTION_STR(phalcon_loader_exception_ce, "Parameter $classes must be an Array");
		return;
	}
	phalcon_update_property_zval(this_ptr, SL("_classes"), classes TSRMLS_CC);
	
	RETURN_CCTOR(this_ptr);
}

/**
 * Register the autoload method
 */
PHP_METHOD(Phalcon_Loader, register){

	zval *a0 = NULL;

	PHALCON_MM_GROW();
	PHALCON_ALLOC_ZVAL_MM(a0);
	array_init(a0);
	phalcon_array_append(&a0, this_ptr, PH_SEPARATE TSRMLS_CC);
	add_next_index_stringl(a0, SL("autoLoad"), 1);
	PHALCON_CALL_FUNC_PARAMS_1_NORETURN("spl_autoload_register", a0);
	
	RETURN_CCTOR(this_ptr);
}

/**
 * Makes the work of autoload registered classes
 *
 * @param string $className
 * @return boolean
 */
PHP_METHOD(Phalcon_Loader, autoLoad){

	zval *class_name = NULL, *events_manager = NULL, *classes = NULL;
	zval *file_path = NULL, *extensions = NULL, *namespaces = NULL, *ds = NULL;
	zval *directory = NULL, *preffix = NULL, *file_name = NULL, *extension = NULL;
	zval *path = NULL;
	zval *c0 = NULL, *c1 = NULL, *c2 = NULL, *c3 = NULL, *c4 = NULL, *c5 = NULL, *c6 = NULL;
	zval *c7 = NULL, *c8 = NULL;
	zval *r0 = NULL, *r1 = NULL, *r2 = NULL, *r3 = NULL, *r4 = NULL, *r5 = NULL, *r6 = NULL;
	zval *r7 = NULL, *r8 = NULL;
	zval *t0 = NULL;
	HashTable *ah0, *ah1, *ah2, *ah3;
	HashPosition hp0, hp1, hp2, hp3;
	zval **hd;
	char *hash_index;
	uint hash_index_len;
	ulong hash_num;
	int hash_type;
	int eval_int;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &class_name) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	PHALCON_INIT_VAR(events_manager);
	phalcon_read_property(&events_manager, this_ptr, SL("_eventsManager"), PH_NOISY_CC);
	if (zend_is_true(events_manager)) {
		PHALCON_INIT_VAR(c0);
		ZVAL_STRING(c0, "loader:beforeCheckClass", 1);
		PHALCON_CALL_METHOD_PARAMS_2_NORETURN(events_manager, "fire", c0, this_ptr, PH_NO_CHECK);
	}
	
	PHALCON_INIT_VAR(classes);
	phalcon_read_property(&classes, this_ptr, SL("_classes"), PH_NOISY_CC);
	eval_int = phalcon_array_isset(classes, class_name);
	if (eval_int) {
		PHALCON_INIT_VAR(file_path);
		phalcon_array_fetch(&file_path, classes, class_name, PH_NOISY_CC);
		if (zend_is_true(events_manager)) {
			phalcon_update_property_zval(this_ptr, SL("_foundPath"), file_path TSRMLS_CC);
			
			PHALCON_INIT_VAR(c1);
			ZVAL_STRING(c1, "loader:pathFound", 1);
			PHALCON_CALL_METHOD_PARAMS_2_NORETURN(events_manager, "fire", c1, this_ptr, PH_NO_CHECK);
		}
		
		if (phalcon_require(file_path TSRMLS_CC) == FAILURE) {
			return;
		}
		PHALCON_MM_RESTORE();
		RETURN_TRUE;
	}
	
	PHALCON_INIT_VAR(extensions);
	phalcon_read_property(&extensions, this_ptr, SL("_extensions"), PH_NOISY_CC);
	
	PHALCON_INIT_VAR(namespaces);
	phalcon_read_property(&namespaces, this_ptr, SL("_namespaces"), PH_NOISY_CC);
	
	PHALCON_ALLOC_ZVAL_MM(r0);
	phalcon_fast_count(r0, namespaces TSRMLS_CC);
	if (zend_is_true(r0)) {
		PHALCON_INIT_VAR(ds);
		zend_get_constant(SL("DIRECTORY_SEPARATOR"), ds TSRMLS_CC);
		if (!phalcon_valid_foreach(namespaces TSRMLS_CC)) {
			return;
		}
		
		ah0 = Z_ARRVAL_P(namespaces);
		zend_hash_internal_pointer_reset_ex(ah0, &hp0);
		fes_0c08_0:
			if(zend_hash_get_current_data_ex(ah0, (void**) &hd, &hp0) != SUCCESS){
				goto fee_0c08_0;
			}
			
			PHALCON_INIT_VAR(preffix);
			PHALCON_GET_FOREACH_KEY(preffix, ah0, hp0);
			PHALCON_INIT_VAR(directory);
			ZVAL_ZVAL(directory, *hd, 1, 0);
			PHALCON_INIT_VAR(r1);
			PHALCON_CALL_FUNC_PARAMS_1(r1, "strlen", class_name);
			PHALCON_INIT_VAR(r2);
			PHALCON_CALL_FUNC_PARAMS_1(r2, "strlen", preffix);
			PHALCON_INIT_VAR(r3);
			is_smaller_function(r3, r2, r1 TSRMLS_CC);
			if (zend_is_true(r3)) {
				PHALCON_INIT_VAR(c2);
				ZVAL_LONG(c2, 0);
				PHALCON_INIT_VAR(r4);
				PHALCON_CALL_FUNC_PARAMS_1(r4, "strlen", preffix);
				PHALCON_INIT_VAR(r5);
				PHALCON_CALL_FUNC_PARAMS_3(r5, "substr", class_name, c2, r4);
				PHALCON_INIT_VAR(r6);
				is_equal_function(r6, r5, preffix TSRMLS_CC);
				if (zend_is_true(r6)) {
					PHALCON_INIT_VAR(r7);
					PHALCON_CONCAT_VS(r7, preffix, "\\");
					PHALCON_INIT_VAR(c3);
					ZVAL_STRING(c3, "", 1);
					PHALCON_INIT_VAR(file_name);
					phalcon_fast_str_replace(file_name, r7, c3, class_name TSRMLS_CC);
					if (zend_is_true(file_name)) {
						if (!phalcon_valid_foreach(extensions TSRMLS_CC)) {
							return;
						}
						
						ah1 = Z_ARRVAL_P(extensions);
						zend_hash_internal_pointer_reset_ex(ah1, &hp1);
						fes_0c08_1:
							if(zend_hash_get_current_data_ex(ah1, (void**) &hd, &hp1) != SUCCESS){
								goto fee_0c08_1;
							}
							
							PHALCON_INIT_VAR(extension);
							ZVAL_ZVAL(extension, *hd, 1, 0);
							PHALCON_INIT_VAR(c4);
							ZVAL_STRING(c4, "\\", 1);
							PHALCON_INIT_VAR(r8);
							PHALCON_CONCAT_VVSV(r8, directory, file_name, ".", extension);
							PHALCON_INIT_VAR(path);
							phalcon_fast_str_replace(path, c4, ds, r8 TSRMLS_CC);
							if (zend_is_true(events_manager)) {
								phalcon_update_property_zval(this_ptr, SL("_checkedPath"), path TSRMLS_CC);
								
								PHALCON_INIT_VAR(c5);
								ZVAL_STRING(c5, "loader:beforeCheckPath", 1);
								PHALCON_CALL_METHOD_PARAMS_2_NORETURN(events_manager, "fire", c5, this_ptr, PH_NO_CHECK);
							}
							
							if (phalcon_file_exists(path TSRMLS_CC) == SUCCESS) {
								if (zend_is_true(events_manager)) {
									phalcon_update_property_zval(this_ptr, SL("_foundPath"), file_path TSRMLS_CC);
									
									PHALCON_INIT_VAR(c6);
									ZVAL_STRING(c6, "loader:pathFound", 1);
									PHALCON_CALL_METHOD_PARAMS_2_NORETURN(events_manager, "fire", c6, this_ptr, PH_NO_CHECK);
								}
								if (phalcon_require(path TSRMLS_CC) == FAILURE) {
									return;
								}
								PHALCON_MM_RESTORE();
								RETURN_TRUE;
							}
							zend_hash_move_forward_ex(ah1, &hp1);
							goto fes_0c08_1;
						fee_0c08_1:
						if(0){}
						
					}
				}
			}
			zend_hash_move_forward_ex(ah0, &hp0);
			goto fes_0c08_0;
		fee_0c08_0:
		if(0){}
		
	}
	
	PHALCON_ALLOC_ZVAL_MM(t0);
	phalcon_read_property(&t0, this_ptr, SL("_directories"), PH_NOISY_CC);
	if (!phalcon_valid_foreach(t0 TSRMLS_CC)) {
		return;
	}
	
	ah2 = Z_ARRVAL_P(t0);
	zend_hash_internal_pointer_reset_ex(ah2, &hp2);
	fes_0c08_2:
		if(zend_hash_get_current_data_ex(ah2, (void**) &hd, &hp2) != SUCCESS){
			goto fee_0c08_2;
		}
		
		PHALCON_INIT_VAR(directory);
		ZVAL_ZVAL(directory, *hd, 1, 0);
		if (!phalcon_valid_foreach(extensions TSRMLS_CC)) {
			return;
		}
		
		ah3 = Z_ARRVAL_P(extensions);
		zend_hash_internal_pointer_reset_ex(ah3, &hp3);
		fes_0c08_3:
			if(zend_hash_get_current_data_ex(ah3, (void**) &hd, &hp3) != SUCCESS){
				goto fee_0c08_3;
			}
			
			PHALCON_INIT_VAR(extension);
			ZVAL_ZVAL(extension, *hd, 1, 0);
			PHALCON_INIT_VAR(path);
			PHALCON_CONCAT_VVSV(path, directory, class_name, ".", extension);
			if (zend_is_true(events_manager)) {
				phalcon_update_property_zval(this_ptr, SL("_checkedPath"), path TSRMLS_CC);
				
				PHALCON_INIT_VAR(c7);
				ZVAL_STRING(c7, "loader:beforeCheckPath", 1);
				PHALCON_CALL_METHOD_PARAMS_2_NORETURN(events_manager, "fire", c7, this_ptr, PH_NO_CHECK);
			}
			
			if (phalcon_file_exists(path TSRMLS_CC) == SUCCESS) {
				if (phalcon_require(path TSRMLS_CC) == FAILURE) {
					return;
				}
				PHALCON_MM_RESTORE();
				RETURN_TRUE;
			}
			zend_hash_move_forward_ex(ah3, &hp3);
			goto fes_0c08_3;
		fee_0c08_3:
		if(0){}
		
		zend_hash_move_forward_ex(ah2, &hp2);
		goto fes_0c08_2;
	fee_0c08_2:
	if(0){}
	
	if (zend_is_true(events_manager)) {
		PHALCON_INIT_VAR(c8);
		ZVAL_STRING(c8, "loader:afterCheckClass", 1);
		PHALCON_CALL_METHOD_PARAMS_2_NORETURN(events_manager, "fire", c8, this_ptr, PH_NO_CHECK);
	}
	
	
	RETURN_CCTOR(this_ptr);
}

PHP_METHOD(Phalcon_Loader, getFoundPath){

	zval *t0 = NULL;

	PHALCON_MM_GROW();
	PHALCON_ALLOC_ZVAL_MM(t0);
	phalcon_read_property(&t0, this_ptr, SL("_foundPath"), PH_NOISY_CC);
	
	RETURN_CCTOR(t0);
}

PHP_METHOD(Phalcon_Loader, getCheckedPath){

	zval *t0 = NULL;

	PHALCON_MM_GROW();
	PHALCON_ALLOC_ZVAL_MM(t0);
	phalcon_read_property(&t0, this_ptr, SL("_checkedPath"), PH_NOISY_CC);
	
	RETURN_CCTOR(t0);
}

