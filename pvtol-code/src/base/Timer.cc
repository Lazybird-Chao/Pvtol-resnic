/**
 * File: Timer.cc
 *    August 2008 - ported from PVL by Jim Daly
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline methods of the Timer class.
 *           A Timer object is used to measure the time it takes
 *                between two points in a program.
 *
 *  $Id: Timer.cc 938 2009-02-18 17:39:52Z ka21088 $
 *
 */

#define GETTIMEOFDAY    // use gettimeofday() for timing

#define MIN_TIMER_RESOLUTION 10 // if gTimerResolution less than this, set to 1

#include <Timer.h>

#include <iostream>  // for cout
#include <stdio.h>   // for stderr
#include <unistd.h>  // for sleep, pause
#include <errno.h>

#include <sys/time.h>
#define SEC2USEC (1.0E6)
#define TV( tv ) (((tv.tv_sec-Timer_firstSec) * SEC2USEC) + tv.tv_usec)


namespace ipvtol
{
  using std::cout;
  using std::endl;
  using std::ends;
  using std::string;


  static TimerValue gTimerResolution = 0;
  static TimerValue Timer_firstSec = 0;

//------------------------------------------------------------------------
//  Method:     Default Constructor
//          Timer()
//
//  Description: Constructs an empty Timer
//
//  Inputs: none
//  Returns: void
//
//------------------------------------------------------------------------
Timer::Timer(void)
{
  Timer::init("unnamed");
}

Timer::Timer(const string& name)
{
  Timer::init(name);
}

void Timer::init(const string& name)
{
  m_name = name;
  m_start = 0;
  m_stop = 0;
  m_value = 0;
  m_count = 0;
  m_sum = 0;
#ifdef TIMER_MIN_MAX
  m_min = 0;
  m_max = 0;
#endif // def TIMER_MIN_MAX
  m_verbose = false;
  Timer::Calibrate();
  return;
}//end construct

//------------------------------------------------------------------------
//  Method:     Calibrate
//          Calibrate
//
//  Description: private; calibrate resolution
//
//  Inputs: none
//  Returns: void
//
//------------------------------------------------------------------------
void Timer::Calibrate()
{
  // skip Calibrate if using VxWorks
  TimerValue firstRead, nextRead;
  TimerValue previousResolution(gTimerResolution);
  struct timeval timeVal;

  if ( gettimeofday( &timeVal, NULL ) )
    { cout << "Timer::Calibrate - gettimeofday() failed, errno = "
	   << errno << endl; }
  Timer_firstSec = timeVal.tv_sec;
  firstRead = nextRead = (TimerValue) TV( timeVal );
  while ( nextRead == firstRead )
    {
      if ( gettimeofday( &timeVal, NULL ) )
	{ cout << "Timer::Calibrate - gettimeofday() failed, errno = "
	       << errno << endl; }
      nextRead = (TimerValue) TV( timeVal );
    }

  gTimerResolution = nextRead - firstRead;
  if ( gTimerResolution < MIN_TIMER_RESOLUTION )
    gTimerResolution = 1; // too close to call

  if ( previousResolution != gTimerResolution )
    {	// report only first time, or if gTimerResolution changes
      if ( m_verbose )
	{
      cout << "Timer::Calibrate using gettimeofday()";

      cout << "  res = " << gTimerResolution
	   << "  ( " << nextRead
	   << " - " << firstRead << " )"
	   << endl;
	} // end if ( m_verbose )
    } // gTimerResolution change

  return;
}//end Calibrate


//------------------------------------------------------------------------
//  Method:     start a timer
//          start( )
//
//  Description: start timer
//
//  Inputs: none
//  Returns: void
//
//------------------------------------------------------------------------
void Timer::start()
{
  struct timeval timeVal;

  if ( gettimeofday( &timeVal, NULL ) )
    { cout << "Timer::start - gettimeofday() failed, errno = "
	   << errno << endl; }
  m_start = (TimerValue) TV( timeVal );

  m_stop = 0;
  return;
}//end start


//------------------------------------------------------------------------
//  Method:     stop a timer
//          stop()
//
//  Description: stop a timer
//
//  Inputs: none
//  Returns: value, i.e. (stop - start)
//
//------------------------------------------------------------------------
TimerValue Timer::stop()
{
  struct timeval timeVal;

  if ( gettimeofday( &timeVal, NULL ) )
    { cout << "Timer - gettimeofday() failed, errno = "
	   << errno << endl; }
  m_stop = (TimerValue) TV( timeVal );

  m_value = ( m_stop / gTimerResolution ) - ( m_start / gTimerResolution );
  m_sum += m_value;
#ifdef TIMER_MIN_MAX
  if ( m_count == 0 )
    {
      m_min = m_max = m_value;
    }
  else
    {
      if ( m_value < m_min )
	m_min = m_value;
      if ( m_value > m_max )
	m_max = m_value;
    }
#endif // def TIMER_MIN_MAX
  ++m_count;
  if ( m_value == 0 )
    // this would be a great place to use a Timer name !!!
    if ( m_verbose )
      {
	cout << "Timer: " << m_name << " measured ZERO time" << endl
	     << " start " << m_start
	     << "   end " << m_stop
	     << " value " << m_value
	     << " count " << m_count
	     << endl;
      }
  return m_value;
}//end stop


//------------------------------------------------------------------------
//  Method:     
//          average()
//
//  Description: 
//
//  Inputs: none
//  Returns: average up to now, i.e. sum / count
//
//------------------------------------------------------------------------
TimerValue Timer::average()
{
  if ( m_count )
    return m_sum / m_count;
  else
    return 0;
}//end average


//------------------------------------------------------------------------
//  Method:     
//          resolution()
//
//  Description: 
//
//  Inputs: none
//  Returns: resolution as a fraction of a second
//
//------------------------------------------------------------------------
float Timer::resolution()
{
  return (float) gTimerResolution / (float) CLOCKS_PER_SEC;
}//end resolution


//------------------------------------------------------------------------
//  Method:     
//          setName( "name" )
//
//  Description: 
//
//  Inputs: name of Timer
//  Returns: nothing
//
//------------------------------------------------------------------------
void Timer::setName( const string& name )
{
  m_name = name;
}//end setName


//------------------------------------------------------------------------
//  Method:
//          setVerbose( flagValue )
//
//  Description: 
//
//  Inputs: value of verbose flag
//  Returns: nothing
//
//------------------------------------------------------------------------
void Timer::setVerbose( const bool verbose )
{
  m_verbose = verbose;
}//end setVerbose


//------------------------------------------------------------------------
//  Method:     
//          seconds()
//
//  Description: 
//
//  Inputs: none
//  Returns: floating average seconds as a fraction of a second
//
//------------------------------------------------------------------------
float Timer::seconds()
{
  return  ((float) m_sum / (float) m_count) * Timer::resolution();
}//end seconds


//------------------------------------------------------------------------
//  Method:
//          report()
//
//  Description: a one line report of the state of the Timer
//
//  Inputs: none
//  Outputs: report by cout
//  Returns: void
//
//------------------------------------------------------------------------
void Timer::report()
{
  cout << m_name << " " << Timer::seconds() << " seconds "
       << " in " << m_count << " cycles"
#ifdef TIMER_MIN_MAX
       << " min " << m_min * Timer::resolution()
       << " max " << m_max * Timer::resolution()
#endif // def TIMER_MIN_MAX
       << endl;
} // end report()

//------------------------------------------------------------------------
//  Method:     reset
//          reset()
//
//  Description: resets a Timer to its state at construct time
//
//  Inputs: none
//  Returns: void
//
//------------------------------------------------------------------------
void Timer::reset()
{

  m_start = 0;
  m_stop = 0;
  m_value = 0;
  m_count = 0;
  m_sum = 0;
  gTimerResolution = 0;	// make noise if verbose
  Timer::Calibrate();
  return;
}//end reset


//------------------------------------------------------------------------
//  Method: destructor
//
//  Description: destroy the Timer obj.
//
//  Inputs: none
//
//  Return: none
//
//------------------------------------------------------------------------
Timer::~Timer()
{
  return;
}


}//end namespace ipvtol

