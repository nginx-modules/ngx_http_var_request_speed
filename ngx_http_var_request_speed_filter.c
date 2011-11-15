#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

typedef struct {
    ngx_flag_t enable;
} ngx_http_var_request_speed_conf_t;

static char * ngx_http_var_request_speed_merge_loc_conf(ngx_conf_t*, void*, void*);
static void * ngx_http_var_request_speed_create_loc_conf(ngx_conf_t*);
static ngx_int_t ngx_http_var_request_speed_add_variables(ngx_conf_t *cf);
static ngx_int_t ngx_http_var_request_speed_variable(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);

static ngx_command_t ngx_http_var_request_speed_filter_commands[] = {
    { ngx_string("var_request_speed_filter"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_var_request_speed_conf_t, enable),
      NULL },

      ngx_null_command
};

static ngx_http_module_t ngx_http_var_request_speed_filter_ctx = {
    ngx_http_var_request_speed_add_variables,                                  /* preconfiguration */
    NULL,

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_var_request_speed_create_loc_conf, /* create location configuration */
    ngx_http_var_request_speed_merge_loc_conf   /* merge location configuration */
};

ngx_module_t  ngx_http_var_request_speed_filter_module = {
    NGX_MODULE_V1,
    &ngx_http_var_request_speed_filter_ctx, /* module context */
    ngx_http_var_request_speed_filter_commands,    /* module directives */
    NGX_HTTP_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};

static ngx_str_t ngx_http_request_speed = ngx_string("request_speed");

static void *
ngx_http_var_request_speed_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_var_request_speed_conf_t  *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_var_request_speed_conf_t));
    if (conf == NULL) {
        return NGX_CONF_ERROR;
    }

    conf->enable = NGX_CONF_UNSET;

    return conf;
}

static char *
ngx_http_var_request_speed_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_var_request_speed_conf_t *prev = parent;
    ngx_http_var_request_speed_conf_t *conf = child;

    ngx_conf_merge_value(conf->enable, prev->enable, 0);

    return NGX_CONF_OK;
}

static ngx_int_t
ngx_http_var_request_speed_add_variables(ngx_conf_t *cf)
{
    ngx_http_variable_t  *var;
    var = ngx_http_add_variable(cf, &ngx_http_request_speed, NGX_HTTP_VAR_NOHASH);
    if (var == NULL) {
        return NGX_ERROR;
    }
    var->get_handler = ngx_http_var_request_speed_variable;
    return NGX_OK;
}

static ngx_int_t
ngx_http_var_request_speed_variable(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{
    struct timeval tv;
    ngx_uint_t now_sec;
    ngx_uint_t req_sec;
    ngx_uint_t now_msec;
    ngx_uint_t req_msec;
    ngx_uint_t msec_diff;
    ngx_http_var_request_speed_conf_t *vrsl;
    vrsl = ngx_http_get_module_loc_conf(r, ngx_http_var_request_speed_filter_module);
    if (vrsl->enable == 1) {
	    v->valid = 1;
	    v->no_cacheable = 0;
	    v->not_found = 0;
        ngx_gettimeofday(&tv);
        now_sec = tv.tv_sec;
        now_msec = tv.tv_usec / 1000;
        req_sec = r->start_sec;
        req_msec = r->start_msec;
        msec_diff = (now_sec - req_sec) * 1000;
        msec_diff += now_msec - req_msec;
	    v->data = ngx_pnalloc(r->pool, NGX_INT32_LEN + 3);
	    if (v->data == NULL) {
	        return NGX_ERROR;
	    }
	    v->len = ngx_sprintf(v->data, "%i", msec_diff) - v->data;
	} else {
        v->not_found = 1;
        return NGX_OK;
	}
    return NGX_OK;
}
