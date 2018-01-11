
/*
 * Copyright (C) Roman Arutyunyan
 */


#ifndef _NGX_RTMP_MPEGTS_H_INCLUDED_
#define _NGX_RTMP_MPEGTS_H_INCLUDED_



#include <ngx_config.h>
#include <ngx_core.h>
#include <openssl/aes.h>
#include <ngx_rtmp.h>
#include "ngx_linux_list.h"
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>

#include "../ngx_rtmp_cmd_module.h"

typedef struct {
    ngx_fd_t    fd;
    ngx_log_t  *log;
    unsigned    encrypt:1;
    unsigned    size:4;
    u_char      buf[16];
    u_char      iv[16];
    AES_KEY     key;
} ngx_rtmp_mpegts_file_t;


typedef struct {
    uint64_t    pts;
    uint64_t    dts;
    ngx_uint_t  pid;
    ngx_uint_t  sid;
    ngx_uint_t  cc;
    unsigned    key:1;
} ngx_rtmp_mpegts_frame_t;


typedef struct {
	struct list_head 	node;
	
	ngx_rtmp_header_t 	*h;
	ngx_chain_t			*in;
	struct timeval 		tv;

}ngx_cache_node_t;


typedef struct {
	struct list_head 	node;
	struct list_head 	list;
	int					full;
} ngx_cache_list_t;

typedef struct {
	struct list_head 	node;
	struct list_head 	list;
	char				playpath[1024];
	int					newpublish;
	struct list_head	*curlist;
	int					active;
	pthread_t			pid;

	
	void				*session;
	void				*newsession;
	int					open;
	uint64_t 			dts;
	int 				finalclose;
	
	int					lcount;
	int					newseg;
	int 				clear;

} ngx_split_list_t;

typedef struct {
	struct list_head 	list;
	pthread_mutex_t		lock;
	
} ngx_cache_global_list_t;

ngx_split_list_t*	ngx_split_list_create();
ngx_cache_global_list_t* ngx_cache_global_list_create();

ngx_cache_node_t* ngx_cache_node_create();
ngx_cache_list_t* ngx_cache_list_create();

ngx_int_t ngx_cache_node_free(ngx_cache_node_t *node);
ngx_int_t ngx_cache_list_free(ngx_cache_list_t *list);

ngx_int_t ngx_cache_list_sleep_time(ngx_cache_list_t *list);

ngx_int_t ngx_cache_global_list_free(ngx_cache_global_list_t *list);
ngx_int_t ngx_split_list_free(ngx_split_list_t *list);


ngx_int_t ngx_rtmp_mpegts_init_encryption(ngx_rtmp_mpegts_file_t *file,
    u_char *key, size_t key_len, uint64_t iv);
ngx_int_t ngx_rtmp_mpegts_open_file(ngx_rtmp_mpegts_file_t *file, u_char *path,
    ngx_log_t *log);
ngx_int_t ngx_rtmp_mpegts_close_file(ngx_rtmp_mpegts_file_t *file);
ngx_int_t ngx_rtmp_mpegts_write_frame(ngx_rtmp_mpegts_file_t *file,
    ngx_rtmp_mpegts_frame_t *f, ngx_buf_t *b);

ngx_int_t
ngx_rtmp_check_stream_active(ngx_rtmp_session_t *s);

ngx_int_t
ngx_rtmp_hls_close_stream(ngx_rtmp_session_t *s, ngx_rtmp_close_stream_t *v);

ngx_int_t
ngx_rtmp_check_stream_clear(char* path);

#endif /* _NGX_RTMP_MPEGTS_H_INCLUDED_ */

