/**
 * File: Falls.cc
 *    September 2000 - written by Jim Daly
 *    February 2008 - ported from PVL by Jim Daly
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline methods of the NTuple class.
 *
 *  $Id: NTuple.cc 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#include <NTuple.h>
#include <iostream>
#include <unistd.h>


namespace ipvtol
{
 
//------------------------------------------------------------------------
//  Method:     Constructor
//      NTuple(int numDims, const int *len)
//
//  Description: Constructor
//
//  Inputs:   1: integer # of dimensions
//            2: pointer to an array of integers which are the lengths
//                 of each dimension
//
//  Returns: nothing
//------------------------------------------------------------------------
NTuple::NTuple(int numDims, const int *len) : m_numDims(numDims)
 {
     m_coord  = new int[numDims];
     m_length = new int[numDims];

     for (int i=0; i<numDims; i++) {
	   m_coord[i]  = 0;
	   m_length[i] = len[i];
     }

     return;
 }


//------------------------------------------------------------------------
//  Method:     Copy Constructor
//      NTuple(const NTuple &src)
//
//  Description: Copy Constructor
//
//  Inputs:   a NTuple to make this one just like
//
//  Returns: nothing
//------------------------------------------------------------------------
NTuple::NTuple(const NTuple& src)
 {
     m_numDims = src.m_numDims;
     m_coord   = new int[m_numDims];
     m_length  = new int[m_numDims];

     for (int i=0; i<m_numDims; i++) {
	   m_coord[i]  = src.m_coord[i];
	   m_length[i] = src.m_length[i];
     }

     return;
 }


//------------------------------------------------------------------------
//  Method:     Assignment Operator =
//      NTuple& operator=(const NTuple& rhs)
//
//  Description: Copy Constructor
//
//  Inputs:   a NTuple to make this one just like
//
//  Returns: ref to NTuple
//------------------------------------------------------------------------
NTuple& NTuple::operator=(const NTuple& rhs)
 {
   if (rhs.m_numDims > m_numDims)
     {//             rhs has more dims than lhs
       if (m_numDims)
	 {
	  delete m_coord;
	  delete m_length;
	 }

	m_coord = new int[rhs.m_numDims];
	m_length   = new int[rhs.m_numDims];
     }
    else if ((rhs.m_numDims == 0) && m_numDims)
     {//             rhs MT has 
	delete m_coord;
	delete m_length;
     }

   m_numDims = rhs.m_numDims;

   for (int i=0; i<m_numDims; i++) {
	m_coord[i]  = rhs.m_coord[i];
	m_length[i] = rhs.m_length[i];
   }

   return *this;
 }


//------------------------------------------------------------------------
//  Method:     Prefix increment operator++
//      NTuple& operator++()
//
//  Description: Increment the coordinate.
//
//  Inputs:   nothing
//
//  Returns: ref to this NTuple
//------------------------------------------------------------------------
NTuple& NTuple::operator++()
 {
   int i;
   if (m_numDims)
     {
       for (i=m_numDims-1; i>-1; i--) {
	 ++m_coord[i];
	 if (m_coord[i] >= m_length[i])
	      m_coord[i] = 0;
	  else
	      break;
       }//endFor each dim
     }

   return *this;
 }


//------------------------------------------------------------------------
//  Method:     Post increment operator++
//      NTuple& operator++()
//
//  Description: Increment the coordinate
//
//  Inputs:   a dummy int
//
//  Returns: ref to NTuple
//------------------------------------------------------------------------
NTuple& NTuple::operator++(int)
 {
   int i;
   if (m_numDims)
     {
       for (i=m_numDims-1; i>-1; i--) {
	 ++m_coord[i];
	 if (m_coord[i] >= m_length[i])
	      m_coord[i] = 0;
	  else
	      break;
       }//endFor each dim
     }

   return *this;
 }


//------------------------------------------------------------------------
//  Method:     serialIndex()
//
//  Description: Return the serial index of the tuple.
//
//  Inputs:   nothing
//
//  Returns: an integer representing the serial index of the current
//           tuple's value.
//------------------------------------------------------------------------
int NTuple::serialIndex() const
 {
   int   i, j, idx=0;
   int   mult;

   for (i=0; i<m_numDims; i++) {
     mult = 1;
     for (j=i+1; j<m_numDims; j++) {
        mult *= m_length[j];
     }//end calc multiplier

     idx += mult * m_coord[i];
   }//endFor each Dim

   return(idx);
 }//end serialIndex()


bool NTuple::operator>(const NTuple& rhs) const
 {
   bool rc = true;
   int  i;
   for (i=0; i<m_numDims; i++) {
     if (m_coord[i] > rhs.m_coord[i])   
       {
         break;
       }
     else if (m_coord[i] < rhs.m_coord[i])   
       {
         rc = false;
         break;
       }
   }//endFor each dim

   if (i == m_numDims)
         rc = false;// they are equal
  
   return(rc);
 }


bool NTuple::operator<(const NTuple& rhs) const
 {
   bool rc = true;
   int  i;
   for (i=0; i<m_numDims; i++) {
     if (m_coord[i] < rhs.m_coord[i])   
       {
         break;
       }
     else if (m_coord[i] > rhs.m_coord[i])   
       {
         rc = false;
         break;
       }
   }//endFor each dim

   if (i == m_numDims)
         rc = false;// they are equal
  
   return(rc);
 }


//------------------------------------------------------------------------
//  Method:     ostream operator
//
//  Description: overload insertion operator << to output an ntuple
//
//  Inputs:   a NTuple
//
//  Returns: ref to ostream
//------------------------------------------------------------------------
ostream& operator<<(ostream& out, NTuple& ntup)
  {
     out << "(" << ntup[0];

     for (int i=1; i<ntup.getDims(); i++) {
       out << ", " << ntup[i];
     }//endPut out the lengths

     out << ")";
     return out;
  }//end ostream operator


}//end namespace

