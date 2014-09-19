/**
 * File: Pitfalls.cc
 *    Author: Eddie Rutledge
 *    February 2008 - ported from PVL by Jim Daly
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline methods of Pitfalls
 *
 *  $Id: Pitfalls.cc 938 2009-02-18 17:39:52Z ka21088 $
 *
 */

#include <Pitfalls.h>
#include <Map.h>
#include <DataDistBase.h>
#include <DataDistDescription.h>
#include <iostream>
#include <typeinfo>  // for bad_cast


namespace ipvtol
{
  using std::ostream;
  using std::istream;
  using std::bad_cast;
  using std::cout;
  using std::endl;

  void Pitfalls::constructHelper(unsigned int dim, 
				 unsigned int numEls, 
				 const Map& map) 
  {
    unsigned int blkSize;
    unsigned int blkSizeWithOverlap;

    m_l = 0;
    m_p = map.getGrid().getDimension(dim);

    try
      {
    	const DistDescription& dist = map.getDistDescription();
    	
    	const DataDistDescription& dd = dynamic_cast<const DataDistDescription&>(dist);
	 

	switch(dd.getDataDistType(dim))
	  {

	  case DataDistBase::BLOCK:
	    // calculate block size, rounding up if the number of grid
	    // els doesn't divide evenly into the number of data els.
            blkSize = (numEls + m_p - 1) / m_p;
            blkSizeWithOverlap = (unsigned int)
	            ((float)(((float)numEls + (((float)m_p - 1)*(float)m_o)) / (float)m_p) + .5);
 
            // If there is no overlap or if there is only one node,
            // set the block size for getting falls and compute r
            if ((m_o == 0) || (m_p == 1))
            {
              m_d = blkSize;
              m_r = blkSize - 1;
            }

            // If there is overlap on multiple nodes, set the block size for
            // for getting falls and compute r
            else
            {
              m_d = blkSizeWithOverlap - m_o;  
              m_r = blkSizeWithOverlap - 1;
            }
 
            // Calculate stride -- block size (no overlap) * number of processors
            m_s = m_p * blkSize;
	    break;
	    
	  case DataDistBase::CYCLIC:
	    m_d  = 1;
            m_r  = m_d - 1;
 
            // Calculate stride -- block size * number of processors
            m_s = m_p * m_d;
	    break;
	    
	  case DataDistBase::BLOCK_CYCLIC:
	    m_d  = dd.getBlockSize(dim);
            m_r  = m_d - 1;
 
            // Calculate stride -- block size * number of processors
            m_s = m_p * m_d;
	    break;
	
	  default:
	    // ### throw "not supported yet" exception here
	    cout << "Pitfalls constructor: only BLOCK, CYCLIC, and "
		 << "BLOCKCYCLIC supported!" << endl;
	    break;
	  } // switch

      }// try
    catch (bad_cast& e)
      {
	// ### throw "bad map" exception
	cout << "wrong kind of map passed to Pitfalls constructor!" << endl;
      }

    // Calculate the number of cycles, rounding up if the last cycle is
    // an incomplete one.  Cycle length is the same as stride.
    m_n = (numEls + m_s - 1) / m_s;

  }//end constructHelper()

  void Pitfalls::print(ostream& out) const
  {
    out << "(l=" << m_l << ", r=" << m_r << ", s=" << m_s << ", n=" << m_n
	<< ", d=" << m_d << ", p=" << m_p << ", o=" << m_o << ")";
  }//end print()

  void Pitfalls::read(istream& in)
  {
    in.ignore(3);
    in >> m_l;
    in.ignore(4);
    in >> m_r;
    in.ignore(4);
    in >> m_s;
    in.ignore(4);
    in >> m_n;
    in.ignore(4);
    in >> m_d;
    in.ignore(4);
    in >> m_p;
    in.ignore(4);
    in >> m_o;
    in.ignore(1);
  }//end read()

}//end Namspace


