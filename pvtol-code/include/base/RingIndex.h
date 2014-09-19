/**
 *    File: RingIndex.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *    March 2008: Ported from PVL by Jim Daly
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of RingIndex class.  A RingIndex is an index into a
 *         ring buffer. A RingIndex behaves like a regular integer, except
 *         when the RingIndex is incremented to or assigned a value greater
 *         than the size of the ring buffer it is an index into, or decremented
 *         to or assigned a negative value. In these cases, the RingIndex
 *         "wraps around" and takes the value of itself modulo the ring buffer
 *         size.  That is, if a RingIndex is assigned a value
 *         (v = ax + b), where "x" is the (positive integer) size of the ring
 *         buffer, "a" is an integer, and "b" is a non-negative integer less
 *         than "x", then the RingIndex takes on the value "b".
 *
 *  $Id: RingIndex.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 *  Author: Eddie Rutledge
 */
#ifndef PVTOL_RINGINDEX_H
#define PVTOL_RINGINDEX_H

#include <Exception.h>

namespace ipvtol
{

  /**
   * The RingIndex class represents an index into a RingBuffer.  It behaves
   * like an integer, except when the RingIndex is incremented to or assigned 
   * a value greater than the size of the ring buffer it is an index into, or
   * a value less than zero.  In that case, the RingIndex "wraps around" and 
   * takes the value of itself modulo the ring buffer size.  That is, if a 
   * RingIndex is assigned a value (v = ax + b), where "x" is the (positive 
   * integer) size of the ring buffer, "a" is an integer, and "b" is a 
   * non-negative integer less than "x", then the RingIndex takes on the 
   * value "b".
   *
   * @author Eddie Rutledge
   */
  class RingIndex
  {
  public:
    /**
     * This constructor builds a RingIndex for indexing into a buffer of the
     * specified size, and calls its assignment operator passing in the "val"
     * argument.
     *
     * @param  bufSize  The size of the ring buffer that the RingIndex will
     *                  index into.
     * @param  val  The value that the RingIndex will be assigned to 
     *              initially.
     * @exception  BadParameter thrown if bufSize is less than 1.
     */
    RingIndex(int bufSize = 1, int val = 0) throw(BadParameter);

    /** This type conversion operator converts the RingIndex to an integer. */
    operator int() const throw();
    
    /**
     * The assignment operator assigns the RingIndex to the specified value,
     * "wrapping around" as described in the class description if the value 
     * is greater than the size of the buffer, or less than zero.
     */
    RingIndex& operator=(int rhs) throw();
    
    /** += with "wrap-around" as described in class description */
    RingIndex& operator+=(int rhs) throw();

    /** -= with "wrap-around" as described in class description */
    RingIndex& operator-=(int rhs) throw();

    /** *= with "wrap-around" as described in class description */
    RingIndex& operator*=(int rhs) throw();

    /** /= with "wrap-around" as described in class description */
    RingIndex& operator/=(int rhs) throw();

    /** %= with "wrap-around" as described in class description */
    RingIndex& operator%=(int rhs) throw();

    /** 
     * Prefix increment.  Increments the RingIndex, wrapping to zero if 
     * incrementing causes the RingIndex to be equal to the buffer size.
     */
    RingIndex& operator++(void) throw();

    /** 
     * Postfix increment.  Increments the RingIndex, wrapping to zero if 
     * incrementing causes the RingIndex to be equal to the buffer size.
     */
    RingIndex operator++(int) throw();

    /** 
     * Prefix decrement.  Decrements the RingIndex, wrapping to 
     * (bufferSize - 1) if decrementing causes the RingIndex to be negative.
     */
    RingIndex& operator--(void) throw();

    /** 
     * Postfix decrement.  Decrements the RingIndex, wrapping to 
     * (bufferSize - 1) if decrementing causes the RingIndex to be negative.
     */
    RingIndex operator--(int) throw();

    /** + with "wrap-around" as described in class description */
    RingIndex operator+(int x) const;

    /** - with "wrap-around" as described in class description */
    RingIndex operator-(int x) const;

    /** * with "wrap-around" as described in class description */
    RingIndex operator*(int x) const;

    /** / with "wrap-around" as described in class description */
    RingIndex operator/(int x) const;

    /** % with "wrap-around" as described in class description */
    RingIndex operator%(int x) const;

    /** Returns the buffer size attribute of the RingIndex. */
    int getBufSize(void) const throw();

    /** 
	Modifies the buffer size attribute of the RingIndex.
	@param  bufSize  the new buffer size
	@exception  BadParameter  thrown if bufSize is less than 1.
    */
    void setBufSize(int bufSize) throw(BadParameter);

  private:
    // attributes
    int m_bufSize;
    int m_val;
  };
  
  //-----------------------------------------------------------------------
  // RingIndex non-method operators
  //-----------------------------------------------------------------------
  /** + with "wrap-around" as described in RingIndex class description */
  RingIndex operator+(int x, const RingIndex& y);

  /** * with "wrap-around" as described in RingIndex class description */
  RingIndex operator*(int x, const RingIndex& y);


//                 I N L I N E      Methods
//-----------------------------------------------------------
  inline 
  RingIndex::RingIndex(int bufSize, int val) throw(BadParameter) 
    : m_bufSize(bufSize)
  {
    if (bufSize <= 0)
      {
	throw BadParameter(__FILE__, __LINE__);
      }
    *this = val;
  }

  inline 
  RingIndex::operator int() const throw()
  {
    return m_val;
  }

  inline
  RingIndex& RingIndex::operator=(int rhs) throw()
  {
    if (rhs >= 0)
      {
	m_val = rhs % m_bufSize;
      }
    else
      {
	m_val = rhs % m_bufSize + m_bufSize;
      }
    return *this;
  }

  inline
  RingIndex& RingIndex::operator+=(int rhs) throw()
  {
    return *this = m_val + rhs;
  }

  inline
  RingIndex& RingIndex::operator-=(int rhs) throw()
  {
    return *this = m_val - rhs;
  }

  inline
  RingIndex& RingIndex::operator*=(int rhs) throw()
  {
    return *this = m_val * rhs;
  }

  inline
  RingIndex& RingIndex::operator/=(int rhs) throw()
  {
    return *this = m_val / rhs;
  }

  inline
  RingIndex& RingIndex::operator%=(int rhs) throw()
  {
    return *this = m_val % rhs;
  }

  inline
  RingIndex& RingIndex::operator++(void) throw()
  {
    if (m_val + 1 < m_bufSize)
      {
	++m_val;
      }
    else
      {
	m_val = 0;
      }
    return *this;
  }

  inline
  RingIndex RingIndex::operator++(int) throw()
  {
    RingIndex tmp(*this);
    ++(*this);
    return tmp;
  }

  inline
  RingIndex& RingIndex::operator--(void) throw()
  {
    if (m_val > 0)
      {
	--m_val;
      }
    else
      {
	m_val = m_bufSize - 1;
      }
    return *this;
  }

  inline
  RingIndex RingIndex::operator+(int x) const
  {
    RingIndex temp(*this);
    return temp += x;
  }

  inline
  RingIndex RingIndex::operator-(int x) const
  {
    RingIndex temp(*this);
    return temp -= x;
  }

  inline
  RingIndex RingIndex::operator*(int x) const
  {
    RingIndex temp(*this);
    return temp *= x;
  }

  inline
  RingIndex RingIndex::operator/(int x) const
  {
    RingIndex temp(*this);
    return temp /= x;
  }

  inline
  RingIndex RingIndex::operator%(int x) const
  {
    RingIndex temp(*this);
    return temp %= x;
  }

  inline
  RingIndex RingIndex::operator--(int) throw()
  {
    RingIndex tmp(*this);
    --(*this);
    return tmp;
  }

  inline
  int RingIndex::getBufSize(void) const throw()
  {
    return m_bufSize;
  }

  inline
  void RingIndex::setBufSize(int bufSize) throw(BadParameter)
  {
    if (bufSize <= 0)
      {
	throw BadParameter(__FILE__, __LINE__);
      }
    else
      {
	m_bufSize = bufSize;
	m_val %= bufSize;
      }
  }

  inline
  RingIndex operator+(int x, const RingIndex& y)
  {
    return y + x;
  }

  inline
  RingIndex operator*(int x, const RingIndex& y)
  {
    return y * x;
  }


}//end namespace


#endif // PVL_RINGINDEX_H not defined
  
