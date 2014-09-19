/**
 *    File: Timer.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *    August 2008: Ported from PVL by Jim Daly
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition and inline methods of the Timer class.
 *           A Timer object is used to measure the time it takes
 *                between two points in a program.
 *
 *  $Id: Timer.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 * @author  Patrick Richardson
 *
 */
#ifndef PVTOL_TIMER_H
#define PVTOL_TIMER_H

#include <time.h>
#include <iostream>
#include <string>

#define TIMER_MIN_MAX

// VxWorks defines "min" and "max" macros
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

namespace ipvtol
{
  using std::cout;
  using std::endl;
  using std::ends;
  using std::string;


  typedef unsigned long TimerValue;

  extern TimerValue Timer_resolution;

  /**
   * The Timer class hides much of the details of timing blocks of execution.
   * To make a single timing measurement, construct, start() and stop() are
   * sufficient. Additional methods allow averaging, and returning averages
   * and the resolution of the measurements in seconds.
   *
   * @author Patrick Richardson
   */
  class Timer
  {
  //++++++++++++++
    public:
  //++++++++++++++

    /**
     * Construct a Timer.
     */
    Timer(void);

    /**
     * Construct a Timer with a name.
     */
    Timer(const string& name);

    /**
     * Start a Timer.
     */
    void start( );

    /**
     * Stop a Timer and return it's last value (stop - start). Also
     * updates count and sum for averages.
     */
    TimerValue stop( );

    /**
     * Return the value of a Timer from it last timing (stop - start).
     */
    TimerValue value( );

    /**
     * Return the number of times a Timer has been cycled (start / stop).
     */
    TimerValue count();

    /**
     * Return the sum of all timings for a Timer.
     */
    TimerValue sum();

    /**
     * Return the average value for a Timer.
     */
    TimerValue average();

    /**
     * Return the resolution of the Timer class implementation as a
     * floating point fraction of a second.
     */
    float resolution();

    /**
     * Return the average of a Timer as the floating point fraction of a second.
     */
    float seconds();

    /**
     * Return the Timer name.
     */
    string getName();
    
#ifdef TIMER_MIN_MAX

    /**
     * Return the minimum Timer value measured.
     */
    TimerValue min();

    /**
     * Return the maximum Timer value measured.
     */
    TimerValue max();
#endif // def TIMER_MIN_MAX

    /**
     * Set name of Timer
     */
    void setName( const string& name );

    /**
     * Set verbose flag
     */
    void setVerbose( const bool verbose );

    /**
     * Report the current state of the Timer
     */
    void report( );

    /**
     * Reset a Timer, i.e. reset value, count, sum, average, and cause
     * resolution to be recalculated.
     */
    void reset( );

    /**
     * Destroy a Timer.
     */
    ~Timer(void);

  //++++++++++++++
    private:
  //++++++++++++++
    //   Private Data
    //-------------------------------------

    string	  m_name;
    TimerValue    m_start;
    TimerValue    m_stop;
    TimerValue    m_value;
    TimerValue    m_count;
    TimerValue    m_sum;
#ifdef TIMER_MIN_MAX
    TimerValue	  m_min;
    TimerValue	  m_max;
#endif // def TIMER_MIN_MAX

    bool	m_verbose;
    //   Private Methods
    //-------------------------------------
    // methods declared private to prevent their use

    // Calibrate, i.e. calcuate resolution
    void Calibrate();

    // initialize Timer
    void init(const string& name);

  };


///////////////////////////////////////////////////////////////////////////
//             Inline function definitions
///////////////////////////////////////////////////////////////////////////

inline
  TimerValue Timer::value()
   { return(m_value); }

inline
  TimerValue Timer::count()
   { return(m_count); }
 
inline
   TimerValue Timer::sum()
   { return(m_sum); }

inline
   string Timer::getName()
   { return(m_name); }
 
#ifdef TIMER_MIN_MAX
inline
  TimerValue Timer::min()
   { return(m_min); }

 inline
  TimerValue Timer::max()
   { return(m_max); }
#endif // def TIMER_MIN_MAX

}

#endif // PVTOL_TIMER_H not defined

