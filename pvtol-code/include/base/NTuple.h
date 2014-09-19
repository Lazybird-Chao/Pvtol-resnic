/**
 *    File: NTuple.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *    March 2008: Ported from PVL by Jim Daly
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition and inline methods of the NTuple class.
 *           A NTuple object is an N-Dimensional coordinate
 *
 *  $Id: NTuple.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 *    September 2000 - Written by Jim Daly
 */
#ifndef PVTOL_N_TUPLE_H
#define PVTOL_N_TUPLE_H

#include <PvtolBasics.h>
#include <iostream>


namespace ipvtol
{
  using std::ostream;


  class NTuple
  {
  //++++++++++++++
    public:
  //++++++++++++++

    // constructors
    //---------------
    NTuple(void);

    NTuple(int numDims, const int *len);

    // destructor
    ~NTuple(void);

    //         Assignment Operator  =
    //=============================================
    NTuple& operator=(const NTuple& rhs);

    //    Copy Constructor
    //=============================================
    NTuple(const NTuple& src);

    NTuple& operator++();
    NTuple& operator++(int);
    int&    operator[](int);


    bool operator==(const NTuple& rhs) const;
    bool operator<(const NTuple& rhs) const;
    bool operator>(const NTuple& rhs) const;

    const int *getLen() const;
    const int *getCoord() const;
    int        getDims() const;

    int        serialIndex() const;

  //++++++++++++++
    private:
  //++++++++++++++
    //   Private Data
    //-------------------------------------
    int        *m_coord;
    int        *m_length;
    int         m_numDims;

    //   Private Methods
    //-------------------------------------
    // methods declared private to prevent their use

  };//end NTuple class definition


ostream& operator<<(ostream& out, NTuple& ntup);

///////////////////////////////////////////////////////////////////////////
//             Inline function definitions
///////////////////////////////////////////////////////////////////////////
inline NTuple::NTuple() : m_coord(NULL),
		          m_length(NULL),
		          m_numDims(0)
   { return; }


inline NTuple::~NTuple()
   {
     if (m_coord != NULL)
	delete [] m_coord;

     if (m_length != NULL)
	delete [] m_length;

     return;
   }//end Destructor

inline const int *NTuple::getCoord() const
 { return(m_coord); }

inline const int *NTuple::getLen() const
 { return(m_length); }

inline int& NTuple::operator[](int dim)
 { return(m_coord[dim]); }

inline int NTuple::getDims() const
 { return(m_numDims); }

inline bool NTuple::operator==(const NTuple& rhs) const
 {
   bool rc = true;
   for (int i=0; i<m_numDims; i++) {
     if ((m_coord[i] != rhs.m_coord[i]) 
                     ||
         (m_length[i] != rhs.m_length[i]))
       {
         rc = false;
	 break;
       }
   }//endFor each dim

   return(rc);
 }



}//end namespace


#endif // PVTOL_N_TUPLE_H not defined

