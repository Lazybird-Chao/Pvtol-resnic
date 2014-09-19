/** @file	UndefDist.h
 *  @author	Matthew Marzilli
 *  @date	6/8/2007
 *  @brief	UndefDist class definition
 
 Modified	6/11/2007
 
*/

#ifndef UNDEFDIST_H
#define UNDEFDIST_H

/* PVTOL Includes */
#include <PvtolBasics.h>
#include "DataDistType.h"

namespace ipvtol { 

/**
 * 	\brief An UNDEFINED type of data distribution
 *
 */
class UndefDist : public DataDistType
{
	
	public:

		/**
		 * \brief Creates a new UNDEF data distribution
		 */
		UndefDist() {
			m_distType = UNDEF;
		}
		
};


} /*namespace ipvtol*/

#endif
