#include "gthr-lv2.h"
#include <stdlib.h>
#include <sys/ppu_thread.h>

#define SYNC_OBJECT_NAME        "_lgcmtx"  /* follow the naming rule */
#define KEY_MAX 4

typedef void (*dtor_t)(void *);

/* thread storage control */
typedef struct {
  char inuse;
  dtor_t dtor;
} tls_ctrl_t;

static tls_ctrl_t tls_ctrl[KEY_MAX];

static int nextkey = 0;

static __gthread_once_t key_once = __GTHREAD_ONCE_INIT;
static __gthread_mutex_t key_mutex;

static __gthread_mutexattr_t key_mutex_attr = {
  SYS_SYNC_PRIORITY,
  SYS_SYNC_NOT_RECURSIVE,
  SYNC_OBJECT_NAME
};

struct tls_data_entry {
  struct tls_data_entry *next;
  sys_ppu_thread_t id;
  void *data[KEY_MAX];
};

static struct _tls_data_list {
  struct tls_data_entry *next;
} tls_data_list = { NULL };

static __gthread_mutexattr_t _Mutex_attr = {
  SYS_SYNC_PRIORITY,
  SYS_SYNC_RECURSIVE,
  SYNC_OBJECT_NAME
};

static __gthread_mutex_t once_mutex;
static __gthread_once_t x_once = __GTHREAD_ONCE_INIT;

void __gthr_lv2_once_ctor(void);
void __gthr_lv2_once_dtor(void);

/* create once mutex */
void
__gthr_lv2_once_ctor(void)
{
  x_once = 2;
  if (sys_lwmutex_create(&once_mutex, &_Mutex_attr) != 0)
    abort();
}

/* destroy once mutex */
void
__gthr_lv2_once_dtor(void)
{
  if (sys_lwmutex_destroy(&once_mutex) != 0)
    abort();
}

void
__gthr_lv2_thread_cleanup(__gthread_key_t key)
{
  int i;
  int use_data = 0;
  void **data = NULL;
  struct tls_data_entry *prev = NULL;
  struct tls_data_entry *p;
  sys_ppu_thread_t thr;
  sys_ppu_thread_get_id(&thr);

  if (sys_lwmutex_lock(&key_mutex, 0) == 0)
    {
      p = tls_data_list.next;
      while (p)
	{
	  if (p->id == thr)
	    {
	      data = p->data;
	      if (prev == NULL)
		tls_data_list.next = p->next;
	      else
		prev->next = p->next;
	      break;
	    }
	  prev = p;
	  p = prev->next;
	}

      for (i = 0; i < KEY_MAX; ++i)
	{
	  if (tls_ctrl[i].inuse && tls_ctrl[i].dtor && data[i] != NULL)
	    {
	      use_data++;
	      if (key == i)
		{
		  /* destroy a datum */
		  void *tmp = data[i];
		  data[i] = NULL;
		  tls_ctrl[i].dtor(tmp);
		}
	    }
	}
      if (p && use_data <= 1)
        free (p);
      if (sys_lwmutex_unlock(&key_mutex) == 0)
	return;
    }
}

void
__gthr_lv2_all_cleanup(void)
{
  int i;
  void **data = NULL;
  struct tls_data_entry *prev;
  struct tls_data_entry *p;

  if (sys_lwmutex_lock(&key_mutex, 0) == 0)
    {
      p = tls_data_list.next;
      while (p)
        {
	  data = p->data;
	  for (i = 0; i < KEY_MAX; ++i)
	    {
	      if (tls_ctrl[i].inuse && tls_ctrl[i].dtor && data[i] != NULL)
		{
		  /* destroy a datum */
		  tls_ctrl[i].dtor(data[i]);
		}
	    }
	  prev = p;
	  p = prev->next;
	  free (prev);
	}
      tls_data_list.next = NULL;
      if (sys_lwmutex_unlock(&key_mutex) == 0)
        return;
    }
}

int
__gthr_lv2_once (__gthread_once_t *once, void (*func) (void))
{
// execute func exactly one time
  if (*once == __GTHREAD_ONCE_INIT) {
    if (x_once == __GTHREAD_ONCE_INIT)
      __gthr_lv2_once_ctor();
    if (sys_lwmutex_lock(&once_mutex, 0) == 0) {
      if (*once == __GTHREAD_ONCE_INIT) {
        // execute func, mark as executed
	func();
	*once = 2;
      }
      if (sys_lwmutex_unlock(&once_mutex) == 0)
	return 0;
    }
  }
  return 1;
}

static void
init (void)
{
  if (sys_lwmutex_create(&key_mutex, &key_mutex_attr) != 0)
    abort();
  atexit (__gthr_lv2_all_cleanup);
}

int
__gthr_lv2_key_create (__gthread_key_t *key, void (*dtor) (void *))
{
  int ret = 1;
  __gthr_lv2_once(&key_once, init);

  if (sys_lwmutex_lock(&key_mutex, 0) == 0) {
    while (nextkey < KEY_MAX && tls_ctrl[nextkey].inuse)
      nextkey++;

    if (nextkey < KEY_MAX) {
      *key = nextkey++;
      tls_ctrl[*key].inuse = 1;
      tls_ctrl[*key].dtor = dtor;
      ret = 0;
    }

    if (sys_lwmutex_unlock(&key_mutex) == 0)
      return ret;
  }
  abort();
}

int
__gthr_lv2_key_delete (__gthread_key_t key)
{
  if (key < 0 || KEY_MAX <= key)
    return 1;

  if (sys_lwmutex_lock(&key_mutex, 0) == 0) {
    tls_ctrl[key].inuse = 0;
    if (key < nextkey)
      nextkey = key;

    if(sys_lwmutex_unlock(&key_mutex) == 0)
      return 0;
  }
  abort();
}	

void *
__gthr_lv2_getspecific (__gthread_key_t key)
{
  void *data=NULL;
  struct tls_data_entry *p;
  sys_ppu_thread_t thr;
  sys_ppu_thread_get_id(&thr);

  if ((key < 0) || (KEY_MAX <= key) || (tls_ctrl[key].inuse == 0))
    return 0;

  if (sys_lwmutex_lock(&key_mutex, 0) == 0) {
    p = tls_data_list.next;
    while(p) {
      if (p->id == thr) {
        data = p->data[key];
        break;
      }
      p = p->next;
    }
    
    if (sys_lwmutex_unlock(&key_mutex) == 0)
      return data;
  }
  abort();
}

int
__gthr_lv2_setspecific (__gthread_key_t key, const void *ptr)
{
  int i;
  int ret=1;
  struct tls_data_entry *p;
  sys_ppu_thread_t thr;
  sys_ppu_thread_get_id(&thr);

  if ((key < 0) || (KEY_MAX <= key) || (tls_ctrl[key].inuse == 0))
    return 1;

  if (sys_lwmutex_lock(&key_mutex, 0) == 0) {
    p = tls_data_list.next;
    while(p) {
      if (p->id == thr) {
	p->data[key] = (void *)ptr;
        ret = 0;
        break;
      }
      p = p->next;
    }

    if(p == NULL) {
        if ((p = (struct tls_data_entry *)malloc
			(sizeof(struct tls_data_entry))) == NULL)
          abort();
        else {
	  for (i = 0; i < KEY_MAX; i++)
	    p->data[i] = NULL;
          p->next = tls_data_list.next;
          p->data[key] = (void *)ptr;
          p->id = thr;
          tls_data_list.next = p;
          ret = 0;
        }
    }

    if (sys_lwmutex_unlock(&key_mutex) == 0)
      return ret;
  }
  abort();
}

static __gthread_mutex_t _malloc_lock_obj;

__gthread_mutex_t
__gthr_lv2_init_malloc_lock0(void)
{
  if (sys_lwmutex_create(&_malloc_lock_obj, &_Mutex_attr) != 0)
    abort();

  return _malloc_lock_obj;
}

void
__gthr_lv2_mutex_init_function (__gthread_mutex_t *mutex)
{
// initialize mutex
  if (sys_lwmutex_create(mutex, &_Mutex_attr) != 0)
    abort();
}

int
__gthr_lv2_mutex_destroy (__gthread_mutex_t *mutex)
{
//delete mutex
  if (sys_lwmutex_destroy(mutex) != 0)
    return 1;

  return 0;
}

int
__gthr_lv2_mutex_lock (__gthread_mutex_t *mutex)
{
//lock mutex
  if (sys_lwmutex_lock(mutex, 0) != 0)
    return 1;

  return 0;
}

int
__gthr_lv2_mutex_unlock (__gthread_mutex_t *mutex)
{
//unlock mutex
  if (sys_lwmutex_unlock(mutex) != 0)
    return 1;

  return 0;
}

