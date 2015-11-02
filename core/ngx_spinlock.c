
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>

/*[p] ������ ע�����ԣ�http://blog.csdn.net/walkerkalr/article/details/38237147?utm_source=tuicool&utm_medium=referral
lock��������ԭ�ӱ�������������lockֵΪ0ʱ����ʾ���Ǳ��ͷŵģ���lockֵ��Ϊ0ʱ���ʾ���Ѿ���ĳ�����̳����ˣ�
value������ʾϣ������û�б��κν��̳���ʱ����lockֵ��Ϊvalue��ʾ��ǰ���̳���������
spin������ʾ�ڶദ����ϵͳ�ڣ���ngx_spinlock����û���õ���ʱ����ǰ�������ں˵�һ�ε����У��÷����ȴ������������ͷ�����ʱ�䡣
*/
void
ngx_spinlock(ngx_atomic_t *lock, ngx_atomic_int_t value, ngx_uint_t spin)
{

#if (NGX_HAVE_ATOMIC_OPS) //[p]֧��ԭ�Ӳ���

    ngx_uint_t  i, n;
	//[p] һֱ����ѭ���У�ֱ����ȡ����
    for ( ;; ) {
		//[p] lockΪ0��ʾû���������̳���������ʱ��lockֵ����Ϊvalue������ʾ��ǰ���̳������� ,ngx_atomic_cmp_setΪԭ�Ӳ���
        if (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
            return;
        }
		//[p]����Ƕദ����ϵͳ
        if (ngx_ncpu > 1) {
			/*[p]�ڶദ�����£�������������������ռ��ʱ����ǰ���̲����������ó�����ʹ�õ�CPU�����������ǵȴ�һ��ʱ�䣬���������������ϵĽ����Ƿ���ͷ����������ٽ��̼��л��Ĵ�����*/
            for (n = 1; n < spin; n <<= 1) {
				//[p] ���ŵȴ��Ĵ���Խ��Խ�࣬ʵ��ȥ������ļ��ʱ��Խ��Խ��  
                for (i = 0; i < n; i++) {
					//[p]ngx_cpu_pause�����ܹ���ϵ��ר��Ϊ�����������ṩ��ָ��������CPU���ڴ����������ȴ�״̬��ͨ��һ��CPU�Ὣ�Լ����ڽ���״̬�����͹��ġ����ǵ�ǰ���̲�û���ó�����ʹ�õĴ�������
                    ngx_cpu_pause();
                }
				/*[p]������Ƿ��ͷ��ˣ����lockֵΪ0���ͷ������󣬾Ͱ�����ֵ��Ϊvalue����ǰ���̳������ɹ������� */
                if (*lock == 0 && ngx_atomic_cmp_set(lock, 0, value)) {
                    return;
                }
            }
        }
		/*[p]��ǰ�����ó�������������Ȼ���ڿ�ִ��״̬��ʹ�ô��������ȵ���������ִ��״̬�Ľ��̣��������ڽ��̱��ں��ٴε���ʱ����forѭ�������п����������������ͷ�����*/
        ngx_sched_yield();
    }

#else

#if (NGX_THREADS)

#error ngx_spinlock() or ngx_atomic_cmp_set() are not defined !

#endif

#endif

}
