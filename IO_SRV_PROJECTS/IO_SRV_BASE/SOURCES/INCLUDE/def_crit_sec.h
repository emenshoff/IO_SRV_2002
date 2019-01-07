#ifndef _DEF_CRIT_SEC_H
#define _DEF_CRIT_SEC_H

#ifdef WIN32
# include <windows.h>
# include <sys\stat.h>
#else
# include <pthread.h>
#endif

//----------------------------------------------------------------------
#ifdef MULTI_THREADED

# ifdef WIN32
#   define CRIT_SEC_DESCRIPT(Name) CRITICAL_SECTION Name;  
# else
#   define CRIT_SEC_DESCRIPT(Name)  pthread_mutex_t Name;
# endif //WIN32

#else
# define CRIT_SEC_DESCRIPT(x)  
#endif

//----------------------------------------------------------------------
#ifdef MULTI_THREADED

#ifdef WIN32
#  define CRIT_SEC_CREATE(Name) InitializeCriticalSection(&Name);  
#else
#  define CRIT_SEC_CREATE(Name) pthread_mutex_init(&Name,0);
#endif //WIN32

#else
# define CRIT_SEC_CREATE(x)  
# endif

//----------------------------------------------------------------------
#ifdef MULTI_THREADED

#ifdef WIN32
#  define CRIT_SEC_FREE(Name) DeleteCriticalSection(&Name);  
#else
#  define CRIT_SEC_FREE(Name)  pthread_mutex_destroy(&Name);
#endif //WIN32

#else
# define CRIT_SEC_FREE(x)  
# endif

//----------------------------------------------------------------------
#ifdef MULTI_THREADED

#ifdef WIN32
#  define CRIT_SEC_BEGIN(Name) EnterCriticalSection(&Name);  
#else
#  define CRIT_SEC_BEGIN(Name)  pthread_mutex_lock(&Name);
#endif //WIN32

#else
# define CRIT_SEC_BEGIN(x)  
# endif

//----------------------------------------------------------------------
#ifdef MULTI_THREADED

#ifdef WIN32
#  define CRIT_SEC_END(Name) LeaveCriticalSection(&Name);  
#else
#  define CRIT_SEC_END(Name)  pthread_mutex_unlock(&Name);
#endif //WIN32

#else
# define CRIT_SEC_END(x)  
# endif


#endif // _DEF_CRIT_SEC_H
