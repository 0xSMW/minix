/* Minimal pthread type stubs to satisfy userland build without libpthread. */
#ifndef _PTHREAD_TYPES_H_
#define _PTHREAD_TYPES_H_

typedef void* pthread_t;
typedef void* pthread_attr_t;
typedef void* pthread_mutex_t;
typedef void* pthread_mutexattr_t;
typedef void* pthread_cond_t;
typedef void* pthread_condattr_t;
typedef void* pthread_rwlock_t;
typedef void* pthread_rwlockattr_t;
typedef void* pthread_key_t;
typedef void* pthread_once_t;

#endif /* _PTHREAD_TYPES_H_ */

