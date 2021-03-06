#include "php_git2.h"
#include "php_git2_priv.h"
#include "revparse.h"

/* {{{ proto resource git_revparse_single(resource $repo, string $spec)
 */
PHP_FUNCTION(git_revparse_single)
{
	php_git2_t *result = NULL, *_repo = NULL;
	git_object *out = NULL;
	zval *repo = NULL;
	char *spec = NULL;
	size_t spec_len;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &spec, &spec_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_revparse_single(&out, PHP_GIT2_V(_repo, repository), spec);

	if (php_git2_check_error(error, "git_revparse_single" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_OBJECT, out, 1 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_RES(GIT2_RVAL_P(result));
}
/* }}} */


/* {{{ proto array git_revparse_ext(resource $repo, string $spec)
 */
PHP_FUNCTION(git_revparse_ext)
{
	zval *array, *a, *b;
	php_git2_t *result = NULL, *result2 = NULL;
	git_object *object_out = NULL;
	git_reference *reference_out = NULL;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *spec = NULL;
	size_t spec_len;
	int error = 0;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &spec, &spec_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_revparse_ext(&object_out, &reference_out, PHP_GIT2_V(_repo, repository), spec);
	if (php_git2_check_error(error, "git_revparse_ext" TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (php_git2_make_resource(&result, PHP_GIT2_TYPE_OBJECT, object_out, 0 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RES(a, GIT2_RVAL_P(result));

	if (php_git2_make_resource(&result2, PHP_GIT2_TYPE_REFERENCE, reference_out, 0 TSRMLS_CC)) {
		RETURN_FALSE;
	}

	ZVAL_RES(b, GIT2_RVAL_P(result2));

	array_init(array);
	add_next_index_zval(array, a);
	add_next_index_zval(array, b);
	RETURN_ZVAL(array, 0, 1);
}
/* }}} */

/* {{{ proto array git_revparse(resource $repo, string $spec)
 */
PHP_FUNCTION(git_revparse)
{
	git_revspec *revspec = NULL;
	zval *repo = NULL;
	php_git2_t *_repo = NULL;
	char *spec = NULL;
	size_t spec_len;
	int error;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"rs", &repo, &spec, &spec_len) == FAILURE) {
		return;
	}

	if ((_repo = (php_git2_t *) zend_fetch_resource(Z_RES_P(repo), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {
		RETURN_FALSE;
	}

	error = git_revparse(revspec, PHP_GIT2_V(_repo, repository), spec);

	if (php_git2_check_error(error, "git_revparse" TSRMLS_CC)) {
		RETURN_FALSE;
	}
	if (revspec == NULL) {
		RETURN_FALSE;
	}

	array_init(return_value);
	php_git2_git_revspec_to_array(revspec, return_value TSRMLS_CC);

	RETVAL_ARR(Z_ARRVAL_P(return_value));
}
/* }}} */

