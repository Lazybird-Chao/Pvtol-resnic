/**
 *    File: Pitfalls.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *    February 2008: Ported from PVL by Jim Daly
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definintion & inline methods of class Pitfalls
 *            the Pitfalls class is a PITFALLS representation of a data
 *            distribution in one dimension.
 *
 *     See also Pitfalls.inl for the inline methods.
 *
 *  $Id: Pitfalls.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#ifndef PVTOL_PITFALLS_H
#define PVTOL_PITFALLS_H

#include <Falls.h>
#include <Map.h>
#include <iostream>

namespace ipvtol
{
  using std::ostream;
  using std::istream;


  class Pitfalls
  {
  public:
    // constructors
    Pitfalls(void);
    Pitfalls(unsigned int dim, unsigned int numEls, const Map& map);
    Pitfalls(unsigned int numEls, const Map& map);
    Pitfalls(unsigned int dim, unsigned int numEls, unsigned int overlap, const Map& map);
#ifdef NOT_YET
    Pitfalls(unsigned int numEls, unsigned int overlap, const Map& map);
#endif // NOT_YET
    Pitfalls(unsigned int l,   
	     unsigned int r, 
	     unsigned int s, 
	     unsigned int n,
	     unsigned int d, 
	     unsigned int p);
    Pitfalls(const Pitfalls& other);

    // destrictor
    ~Pitfalls(void);
    
    // accessors
    unsigned int getL(void) const;   
    unsigned int getR(void) const;
    unsigned int getS(void) const;
    unsigned int getN(void) const;
    unsigned int getD(void) const;
    unsigned int getP(void) const;
    Falls getFalls(int p) const;
    void print(ostream& out) const;

    // mutators
    void setL(unsigned int l);  
    void setR(unsigned int r);
    void setS(unsigned int s);
    void setN(unsigned int n);
    void setD(unsigned int d);
    void setP(unsigned int p);
    void read(istream& in);

    // operators
    Pitfalls& operator=(const Pitfalls& other);
    bool operator==(const Pitfalls& other) const;
    bool operator!=(const Pitfalls& other) const;

  private:
    // methods
    void constructHelper(unsigned int dim, 
			 unsigned int numEls, 
			 const Map& map);

    // data
    unsigned int m_l;   
    unsigned int m_r;
    unsigned int m_s;
    unsigned int m_n;
    unsigned int m_d;
    unsigned int m_p;
    unsigned int m_o;
  };

  // external operators
  ostream& operator<<(ostream& out, const Pitfalls& p);
  istream& operator>>(istream& in, Pitfalls& p);

//                 I N L I N E      Methods
//-----------------------------------------------------------
  inline
  Pitfalls::Pitfalls(void) : m_l(0), m_r(0), m_s(1), m_n(1), m_d(1), m_p(1), m_o(0)
  {
  }

  inline
  Pitfalls::Pitfalls(unsigned int dim, unsigned int numEls, const Map& map) : m_o(0)
  {
    constructHelper(dim, numEls, map);
  }

  inline
  Pitfalls::Pitfalls(unsigned int dim, unsigned int numEls, unsigned int overlap, const Map& map) : m_o(overlap)
  {
    constructHelper(dim, numEls, map);   
  }

  inline
  Pitfalls::Pitfalls(unsigned int numEls, const Map& map) : m_o(0)
  {
    constructHelper(0, numEls, map);
  }

#ifdef NOT_YET
  inline
  Pitfalls::Pitfalls(unsigned int numEls, unsigned int overlap, const Map1d& map) : m_o(overlap)
  {
    constructHelper(0, numEls, map);
  }
#endif // NOT_YET

  inline
  Pitfalls::Pitfalls(unsigned int l,   
		     unsigned int r, 
		     unsigned int s, 
		     unsigned int n,
		     unsigned int d, 
		     unsigned int p) 
    : m_l(l), m_r(r), m_s(s), m_n(n), m_d(d), m_p(p), m_o(0)
  {
  }

  inline 
  Pitfalls::Pitfalls(const Pitfalls& other) 
    : m_l(other.m_l), m_r(other.m_r), m_s(other.m_s), m_n(other.m_n),
      m_d(other.m_d), m_p(other.m_p), m_o(other.m_o) 
  {
  }

  inline
  Pitfalls::~Pitfalls(void)
  {
  }
  
  inline
  unsigned int Pitfalls::getL(void) const  
  {
    return m_l;  
  }

  inline
  unsigned int Pitfalls::getR(void) const
  {
    return m_r;
  }
  
  inline
  unsigned int Pitfalls::getS(void) const
  {
    return m_s;
  }

  inline
  unsigned int Pitfalls::getN(void) const
  {
    return m_n;
  }
  
  inline
  unsigned int Pitfalls::getD(void) const
  {
    return m_d;
  }

  inline
  unsigned int Pitfalls::getP(void) const
  {
    return m_p;
  }

  inline
  Falls Pitfalls::getFalls(int p) const
  {
    return Falls(m_l + p * m_d,   // left
                 m_r + p * m_d,   // right
                 m_s,             // stride
                 m_n);            // number of cycles
  }

  inline
  void Pitfalls::setL(unsigned int l)  
  {
    m_l = l;
  }

  inline
  void Pitfalls::setR(unsigned int r)
  {
    m_r = r;
  }

  inline
  void Pitfalls::setS(unsigned int s)
  {
    m_s = s;
  }

  inline
  void Pitfalls::setN(unsigned int n)
  {
    m_n = n;
  }

  inline
  void Pitfalls::setD(unsigned int d)
  {
    m_d = d;
  }

  inline
  void Pitfalls::setP(unsigned int p)
  {
    m_p = p;
  }

  inline
  Pitfalls& Pitfalls::operator=(const Pitfalls& other)
  {
    m_l = other.m_l;
    m_r = other.m_r;
    m_s = other.m_s;
    m_n = other.m_n;
    m_d = other.m_d;
    m_p = other.m_p;
    m_o = other.m_o;

    return *this;
  }
    
  inline
  bool Pitfalls::operator==(const Pitfalls& other) const
  {
    return (m_l == other.m_l && m_r == other.m_r && m_s == other.m_s &&
	    m_n == other.m_n && m_d == other.m_d && m_p == other.m_p &&
            m_o == other.m_o );
  }

  inline
  bool Pitfalls::operator!=(const Pitfalls& other) const
  {
    return !(*this == other);
  }

  inline
  ostream& operator<<(ostream& out, const Pitfalls& p)
  {
    p.print(out);
    return out;
  }

  inline
  istream& operator>>(istream& in, Pitfalls& p)
  {
    p.read(in);
    return in;
  }


}//end namespace

#endif // PVTOL_PITFALLS_H not defined

