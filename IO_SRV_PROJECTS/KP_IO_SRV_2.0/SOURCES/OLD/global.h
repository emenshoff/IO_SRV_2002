#ifndef _kp_global_h
#define _kp_global_h

#ifdef WIN32
# include <windows.h>
#else
	
#endif


#ifdef MULTI_THREADED
# ifdef WIN32
#   define ENTER_CRITICAL(x)   EnterCriticalSection(&x);
# else
#   define ENTER_CRITICAL(x)   pthread_mutex_lock(&x);
# endif
#else
# define ENTER_CRITICAL(x)  ;
# endif
//--------------------------------------------------------------------------------

#ifdef MULTI_THREADED
# ifdef WIN32
#   define LEAVE_CRITICAL(x)  LeaveCriticalSection(&x);
# else
#   define LEAVE_CRITICAL(x)  pthread_mutex_unlock(&x);
# endif
#else
# define LEAVE_CRITICAL(x)  ;
#endif



#ifdef WIN32
# ifdef MULTI_THREADED
  //CRITICAL_SECTION MainCrtSection;  
# endif
#endif

#endif  //_kp_global_h