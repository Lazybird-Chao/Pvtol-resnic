/**
 *    File: Falls.inl
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *    February 2008: Ported from PVL by Jim Daly
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   inline methods of class Falls
 *
 *     See Falls.h for the class definition
 *
 *  $Id: Falls.inl 938 2009-02-18 17:39:52Z ka21088 $
 *
 */

  inline
  Falls::Falls(void) : m_l(0), m_r(0), m_s(1), m_n(1)
  {
  }

  inline
  Falls::Falls(unsigned int l,
               unsigned int r,
               unsigned int s,
               unsigned int n) : m_l(l), m_r(r), m_s(s), m_n(n)
  {
  }

  inline
  Falls::Falls(const Falls& other) 
    : m_l(other.m_l), m_r(other.m_r), m_s(other.m_s), m_n(other.m_n)
  {
  }

  inline
  Falls::~Falls(void)
  {
  }

  inline
  unsigned int Falls::getL(void) const   
  {
    return m_l;
  }

  inline
  unsigned int Falls::getR(void) const
  {
    return m_r;
  }

  inline
  unsigned int Falls::getS(void) const
  {
    return m_s;
  }
   
  inline
  unsigned int Falls::getN(void) const
  {
    return m_n;
  }

  inline
  void Falls::setL(unsigned int l)   
  {
    m_l = l;
  }

  inline
  void Falls::setR(unsigned int r)
  {
    m_r = r;
  }
 
  inline
  void Falls::setS(unsigned int s)
  {
    m_s = s;
  }
  
  inline
  void Falls::setN(unsigned int n)
  {
    m_n = n;
  }
  
  inline
  Falls& Falls::operator=(const Falls& other)
  {
    if (this != &other)
      {
	m_l  = other.m_l;
	m_r  = other.m_r;
	m_s  = other.m_s;
	m_n  = other.m_n;
      }
    return *this;
  }
   
  inline
  bool Falls::operator==(const Falls& other) const
  {
    return (m_r == other.m_r && m_l == other.m_l &&
	    m_s == other.m_s && m_n == other.m_n );
  }

  inline
  bool Falls::operator!=(const Falls& other) const
  {
    return !(*this == other);
  }
   
  inline
  ostream& operator<<(ostream& out, const Falls& f)
  {
    f.print(out);
    return out;
  }

  inline
  istream& operator>>(istream& in, Falls& f)
  {
    f.read(in);
    return in;
  }

