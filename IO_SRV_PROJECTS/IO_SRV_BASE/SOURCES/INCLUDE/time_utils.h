/*****************************************************************************
              Функции для работы со временем.
                     created by A.O.  (changed by M.E.N.)
*****************************************************************************/
#ifndef _utils_h
#define _utils_h

#include <time.h>
#ifdef WIN32
# include <windows.h>
#else
# include <sys/time.h>
#endif
//-----------------------------------------------------------------
#ifdef WIN32
# define SLEEP(x) Sleep(x);
#else
# define SLEEP(x) usleep(x * 1000);
#endif
//-----------------------------------------------------------------
//получение текущего времени (Гринвич)
inline void GetCurrTime(struct timeval &tv)
{
#ifdef WIN32
  //:)))
  SYSTEMTIME time_current;
  GetSystemTime(&time_current);
  tv.tv_usec=time_current.wMilliseconds * 1e3;
  tv.tv_sec=time(0) + _timezone;
#else
  gettimeofday(&tv,NULL);
#endif
}
//-----------------------------------------------------------------
inline double Tv2Dbl(const struct timeval &tv)
{
  return (double)(tv.tv_sec * 1e6 + tv.tv_usec);
}
//-----------------------------------------------------------------
inline
bool operator== (const timeval& t1, const timeval& t2)
{
  return t1.tv_sec == t2.tv_sec && t1.tv_usec == t2.tv_usec;
}
//-----------------------------------------------------------------
inline
bool operator!= (const timeval& t1, const timeval& t2)
{
  return t1.tv_sec != t2.tv_sec || t1.tv_usec != t2.tv_usec;
}
//-----------------------------------------------------------------
inline
bool operator > (const timeval& t1, const timeval& t2)
{
  return t1.tv_sec > t2.tv_sec ||
         (t1.tv_sec == t2.tv_sec && t1.tv_usec > t2.tv_usec);
}
//-----------------------------------------------------------------
inline
bool operator < (const timeval& t1, const timeval& t2)
{
  return t1.tv_sec < t2.tv_sec ||
         (t1.tv_sec == t2.tv_sec && t1.tv_usec < t2.tv_usec);
}
//-----------------------------------------------------------------
inline
bool operator >= (const timeval& t1, const timeval& t2)
{
  return t1.tv_sec > t2.tv_sec ||
         (t1.tv_sec == t2.tv_sec && t1.tv_usec >= t2.tv_usec);
}
//-----------------------------------------------------------------
inline
bool operator <= (const timeval& t1, const timeval& t2)
{
  return t1.tv_sec < t2.tv_sec ||
         (t1.tv_sec == t2.tv_sec && t1.tv_usec <= t2.tv_usec);
}
//-----------------------------------------------------------------
inline
void operator ++ (timeval& tv)
{
  if (tv.tv_usec == 999999)
  { tv.tv_sec++; tv.tv_usec = 0;
  }
  else
    tv.tv_usec++;
}
//-----------------------------------------------------------------
inline
void operator -- (timeval& tv)
{
  if (tv.tv_usec == 0)
  { tv.tv_sec--; tv.tv_usec = 999999;
  }
  else
    tv.tv_usec--;
}
//-----------------------------------------------------------------
inline
void operator += (timeval& t1, const timeval& t2)
{
  t1.tv_sec += t2.tv_sec;
  t1.tv_usec += t2.tv_usec;
  if (t1.tv_usec > 1000000)
  { t1.tv_usec -= 1000000;
    t1.tv_sec++;
  }
}
//-----------------------------------------------------------------
inline
timeval operator - (const timeval& t1, const timeval& t2)
{
  timeval tv;
  if (t1.tv_usec < t2.tv_usec)
  { tv.tv_sec = t1.tv_sec - t2.tv_sec - 1;
    tv.tv_usec = t1.tv_usec + 1000000 - t2.tv_usec;
  }
  else
  { tv.tv_sec = t1.tv_sec - t2.tv_sec;
    tv.tv_usec = t1.tv_usec - t2.tv_usec;
  }
  return tv;
}
//-----------------------------------------------------------------
inline
timeval operator -= (const timeval& t1, const timeval& t2)
{
  timeval tv;
  tv.tv_sec=t1.tv_sec;
  tv.tv_usec=t1.tv_usec;
  if (t1.tv_usec < t2.tv_usec)
  { tv.tv_sec = tv.tv_sec - t2.tv_sec - 1;
    tv.tv_usec = tv.tv_usec + 1000000 - t2.tv_usec;
  }
  else
  { tv.tv_sec = tv.tv_sec - t2.tv_sec;
    tv.tv_usec = tv.tv_usec - t2.tv_usec;
  }
  return tv;
}
//-----------------------------------------------------------------
#endif