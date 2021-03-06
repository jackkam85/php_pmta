/**
 * @file pmta_error.c
 * @date Sep 29, 2010 v0.1
 * @date Jul 11, 2013 v0.4
 * @author Vladimir Kolesnikov <vladimir@extrememember.com>
 * @brief Implementation of @c PmtaError, @c PmtaErrorConnection, @c PmtaErrorRecipient and @c PmtaErrorMessage classes
 * @details
@code{.php}
class PmtaError extends Exception
{
	const OUT_OF_MEMORY    = PmtaApiERROR_OutOfMemory;
	const ILLEGAL_STATE    = PmtaApiERROR_IllegalState;
	const ILLEGAL_ARGUMENT = PmtaApiERROR_IllegalArgument;
	const SECURITY         = PmtaApiERROR_Security;
	const IO               = PmtaApiERROR_IO;
	const SERVICE          = PmtaApiERROR_Service;
	const EMAIL_ADDRESS    = PmtaApiERROR_EmailAddress;
}

final class PmtaErrorConnection extends PmtaError {}
final class PmtaErrorRecipient  extends PmtaError {}
final class PmtaErrorMessage    extends PmtaError {}
@endcode
*/

#include "pmta_error.h"
#include <PmtaApi.h>

#ifdef DOXYGEN
#	undef PHPAPI
/**
 * @headerfile macros.h
 * @def PHPAPI
 * @internal
 */
#	define PHPAPI
#endif

#ifdef HAVE_SPL
/**
 * @brief A reference to @c RuntimeException class from the SPL
 */
extern PHPAPI zend_class_entry* spl_ce_RuntimeException;
#endif

/**
 * @brief @c PmtaError class entry
 */
static zend_class_entry* pmta_error_class;

void throw_pmta_error(zend_class_entry* error_class, int code, const char* message, zval** result TSRMLS_DC)
{
	zval* exception;

	MAKE_STD_ZVAL(exception);
	object_init_ex(exception, error_class);

#if ZEND_MODULE_API_NO > 20060613
	zend_update_property_string(error_class, exception, ZEND_STRL("message"), message TSRMLS_CC);
#else
	zend_update_property_string(error_class, exception, ZEND_STRL("message"), (char*)message TSRMLS_CC);
#endif
	zend_update_property_long(error_class, exception, ZEND_STRL("code"), code TSRMLS_CC);

	if (!result) {
		zend_throw_exception_object(exception TSRMLS_CC);
	}
	else {
		ZVAL_ZVAL(*result, exception, 1, 1);
	}
}

/**
 * @brief Class methods for @c PmtaError and derived classes
 */
static
#if ZEND_MODULE_API_NO > 20060613
const
#endif
zend_function_entry pmta_error_class_methods[] = {
	PHP_FE_END
};

/**
 * @brief Registers @c PmtaError, @c PmtaErrorConnection, @c PmtaErrorRecipient and @c PmtaErrorMessage classes with Zend
 * @param tsrm_ls Internally used by Zend
 */
void pmtaerror_register_class(TSRMLS_D)
{
	zend_class_entry e;

	INIT_CLASS_ENTRY(e, "PmtaError", pmta_error_class_methods);

#ifdef HAVE_SPL
	pmta_error_class = zend_register_internal_class_ex(&e, spl_ce_RuntimeException, NULL TSRMLS_CC);
#else
	pmta_error_class = zend_register_internal_class_ex(&e, zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC);
#endif

	zend_declare_class_constant_long(pmta_error_class, ZEND_STRL("OUT_OF_MEMORY"),    PmtaApiERROR_OutOfMemory     TSRMLS_CC);
	zend_declare_class_constant_long(pmta_error_class, ZEND_STRL("ILLEGAL_STATE"),    PmtaApiERROR_IllegalState    TSRMLS_CC);
	zend_declare_class_constant_long(pmta_error_class, ZEND_STRL("ILLEGAL_ARGUMENT"), PmtaApiERROR_IllegalArgument TSRMLS_CC);
	zend_declare_class_constant_long(pmta_error_class, ZEND_STRL("SECURITY"),         PmtaApiERROR_Security        TSRMLS_CC);
	zend_declare_class_constant_long(pmta_error_class, ZEND_STRL("IO"),               PmtaApiERROR_IO              TSRMLS_CC);
	zend_declare_class_constant_long(pmta_error_class, ZEND_STRL("SERVICE"),          PmtaApiERROR_Service         TSRMLS_CC);
	zend_declare_class_constant_long(pmta_error_class, ZEND_STRL("EMAIL_ADDRESS"),    PmtaApiERROR_EmailAddress    TSRMLS_CC);
	zend_declare_class_constant_long(pmta_error_class, ZEND_STRL("PHP_API"),          PmtaApiERROR_PHP_API         TSRMLS_CC);

	INIT_CLASS_ENTRY(e, "PmtaErrorConnection", pmta_error_class_methods);
	pmta_error_connection_class = zend_register_internal_class_ex(&e, pmta_error_class, NULL TSRMLS_CC);

	INIT_CLASS_ENTRY(e, "PmtaErrorRecipient", pmta_error_class_methods);
	pmta_error_recipient_class = zend_register_internal_class_ex(&e, pmta_error_class, NULL TSRMLS_CC);

	INIT_CLASS_ENTRY(e, "PmtaErrorMessage", pmta_error_class_methods);
	pmta_error_message_class = zend_register_internal_class_ex(&e, pmta_error_class, NULL TSRMLS_CC);

	pmta_error_connection_class->ce_flags |= ZEND_ACC_FINAL_CLASS;
	pmta_error_recipient_class->ce_flags  |= ZEND_ACC_FINAL_CLASS;
	pmta_error_message_class->ce_flags    |= ZEND_ACC_FINAL_CLASS;
}
