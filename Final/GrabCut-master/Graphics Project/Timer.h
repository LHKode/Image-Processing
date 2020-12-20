/** @name Header file: timer.h

    Header file for class Timer.

    See source for details.

$Revision: 1.1 $
$Id: Timer.h,v 1.1 2004/05/26 14:37:37 shaun Exp $
*/

#ifndef ___Included_Timer

#define ___Included_Timer

#ifdef __WINDOWSCORGI
   #include <time.h>
   #include <dos.h>
#else
   #include <sys/time.h>
   #include <unistd.h>
#endif

/** class Timer.
  
    Provides a timer for measuring time delays, without the hassle of
    doing the donkey work.
*/
#ifdef __WINCORGIDLL
__declspec(dllexport)
#endif
class Timer
  {
    protected:
      /// start of timing interval.
      struct timeval      lasttime;

    public:
      Timer ();
      ~Timer ();
 
      /// mark the start of a timing interval.
      void mark ();
      /// return the interval length.
      double interval ();
      /** Block for the specified period, measured in microseconds. This allows
          small delays without suffering the context switch and large delay
          associated with small sleeps. Should be used with caution, since
          it will bring everything else to a halt. */
      void busywait (double interval);
  };

#endif

