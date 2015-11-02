
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_SHMTX_H_INCLUDED_
#define _NGX_SHMTX_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>

/*[p]�������ṹ 
ngx_shmtx_t�ṹ�漰�����꣺NGX_HAVE_ATOMIC_OPS��NGX_HVE_POIX_SEM�����������Ӧ�Ż�������3�ֲ�ͬʵ�֡�
��1��ʵ�֣�����֧��ԭ�Ӳ���ʱ����ʹ���ļ�����ʵ��ngx_hmtx_t����������ʱ������fd��name��Ա����������Աʹ��������ܵ��ļ������ṩ�������������Ļ�������
��2��ʵ�֣�֧��ԭ�Ӳ���ȴ�ֲ�֧���ź�����
��3��ʵ�֣���֧��ԭ�Ӳ�����ͬʱ������ϵͳҲ֧���ź�����
*/
typedef struct {
#if (NGX_HAVE_ATOMIC_OPS)
	//[p]ԭ�ӱ�����  
    ngx_atomic_t  *lock;
#if (NGX_HAVE_POSIX_SEM)
	//semaphoreΪ1 ʱ��ʾ��ȡ��������ʹ�õ����ź���  
    ngx_uint_t     semaphore;
    sem_t          sem;
#endif
#else
	//[p]ʹ���ļ���ʱfd��ʾʹ�õ��ļ����  
    ngx_fd_t       fd;
    u_char        *name; //[p] �ļ���
#endif
	//[p]������������ʾ������״̬�µȴ�����������������ͷŵ�ʱ�䡣���ļ���ʵ�֣�spinû���κ�����
    ngx_uint_t     spin;
} ngx_shmtx_t;


ngx_int_t ngx_shmtx_create(ngx_shmtx_t *mtx, void *addr, u_char *name);
void ngx_shmtx_destory(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_trylock(ngx_shmtx_t *mtx);
void ngx_shmtx_lock(ngx_shmtx_t *mtx);
void ngx_shmtx_unlock(ngx_shmtx_t *mtx);


#endif /* _NGX_SHMTX_H_INCLUDED_ */
