/**
 *    File: Falls.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *    February 2008: Ported from PVL by Jim Daly
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definintion of class Falls
 *                The Falls class is a FALLS representation of a data
 *                distribution in one dimension.
 *
 *     See also Falls.inl for the inline methods.
 *
 *  $Id: Falls.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#ifndef PVTOL_FALLS_H
#define PVTOL_FALLS_H

#include <PvtolBasics.h>
#include <vector>
#include <iostream>


namespace ipvtol
{
  using std::istream;
  using std::ostream;
  using std::vector;

  class Falls
  {
  public:
    // constructors
    Falls(void);
    Falls(unsigned int l, unsigned int r, unsigned int s, unsigned int n);
    Falls(const Falls& other);

    // destructor
    ~Falls(void);

    // accessors
    unsigned int getL(void) const;      
    unsigned int getR(void) const;
    unsigned int getS(void) const;
    unsigned int getN(void) const;
    void print(ostream& out) const;

    // mutators
    void setL(unsigned int l);      
    void setR(unsigned int r);
    void setS(unsigned int s);
    void setN(unsigned int n);
    void read(istream& in);

    // operators
    Falls& operator=(const Falls& other);
    bool operator==(const Falls& other) const;
    bool operator!=(const Falls& other) const;
    
  private:
    // attributes
    unsigned int m_l;   
    unsigned int m_r;
    unsigned int m_s;
    unsigned int m_n;
  };

  // external operators
  ostream& operator<<(ostream& out, const Falls& f);
  istream& operator>>(istream& in, Falls& f);

  // see if two Falls have a non-empty intersection
  bool intersects(const Falls& f1, const Falls& f2);
  
  // calculate the intersection of two Falls
  vector<Falls> intersection(const Falls& f1, const Falls& f2);
  void intersection(const Falls& f1, const Falls& f2, vector<Falls>& result);

  // return the size of the set of Falls that would result from intersecting
  // two Falls
  unsigned int intersectionSize(const Falls& f1, const Falls& f2);

#include <Falls.inl>

}//end namespace


#endif // PVTOL_FALLS_H not defined

