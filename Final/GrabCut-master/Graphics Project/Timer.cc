/** @name Source File: timer.cc

    Implentation of the class Timer.

    Created: Shaun Bangay,,,, - June 25 1998
*/

#include <iostream>
#include <Timer.h>

using namespace std;

/// flag to allowing debugging statements within the file
//#define DEBUGTIMER

Timer::Timer ()

{
#ifdef DEBUGTIMER
  cout << "Creating Timer\n";
#endif

  mark ();
}

Timer::~Timer ()

{
#ifdef DEBUGTIMER
  cout << "Destroying Timer\n";
#endif

#ifdef DEBUGTIMER
  cout << "Destroyed Timer\n";
#endif
}

void Timer::mark ()

{
  gettimeofday (&lasttime, NULL);
}

double Timer::interval ()

{
  struct timeval currtime;
  gettimeofday (&currtime, NULL);

  double ct = currtime.tv_sec * 1000000.0 + currtime.tv_usec;
  double lt = lasttime.tv_sec * 1000000.0 + lasttime.tv_usec;
  return (ct - lt) / 1000000.0;
}

void Timer::busywait (double interval)

{
  struct timeval currtime;
  struct timeval starttime;
  double ct;
  double st;

  gettimeofday (&starttime, NULL);
  st = starttime.tv_sec * 1000000.0 + starttime.tv_usec;

  do
    {
      gettimeofday (&currtime, NULL);

      ct = currtime.tv_sec * 1000000.0 + currtime.tv_usec;
    }
  while (ct - st < interval);
}  
