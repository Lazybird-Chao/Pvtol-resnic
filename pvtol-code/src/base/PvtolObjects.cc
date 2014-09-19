/**
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *
 *  Implementation of the PvtolObjects class methods
 *
 *  $Id: PvtolObjects.cc 938 2009-02-18 17:39:52Z ka21088 $
 *
 */

#include <PvtolObjects.h>
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>

void PvtolObjects::storeNewRankList(std::string objectName,
                      pvtol::RankList *object)
{
   m_rankListMap.insert(make_pair(objectName, object));
}

RankListMapType PvtolObjects::dumpRankList(void)
{
   return m_rankListMap;
}

pvtol::RankList* PvtolObjects::getRankList(std::string name)
{
   return m_rankListMap.find(name)->second;
}

void PvtolObjects::printRankList(void)
{
   RankListMapType::iterator rankListIter;
   std::vector<pvtol::RankId> rankVector;
   std::vector<pvtol::RankId>::iterator rankVectorIter;

   for(rankListIter = m_rankListMap.begin();
       rankListIter != m_rankListMap.end();
       rankListIter++)
   {
      std::cout << "--" << std::endl;
      std::cout << "RankList " << rankListIter->first << ": ";
      rankListIter->second->getRankListVector(rankVector);

      for(rankVectorIter = rankVector.begin();
          rankVectorIter != rankVector.end();
          rankVectorIter++)
      {
         std::cout << *rankVectorIter << " ";
      }
      std::cout << std::endl;
   }
}

/**
 * \param objectName std::string name of the object to be stored.[in]
 * \param object A pointer to the Grid object to be stored.[in]
 *
 * Stores the Grid along with a string name
 */
void PvtolObjects::storeNewGrid(std::string objectName,
                                pvtol::Grid *object)
{
   m_gridMap.insert(make_pair(objectName, object));
}

GridMapType PvtolObjects::dumpGrid(void)
{
   return m_gridMap;
}

pvtol::Grid* PvtolObjects::getGrid(std::string name)
{
   return m_gridMap.find(name)->second;
}

/**
 * Prints to the screen the contents of the Grid Map
 */
void PvtolObjects::printGrid(void)
{
   GridMapType::iterator GridMapIter;
   int numDims;
   int dim1;
   int dim2;
   int dim3;

   std::cout << "--" << std::endl;
   for(GridMapIter = m_gridMap.begin();
         GridMapIter != m_gridMap.end();
         GridMapIter++)
   {
      numDims = GridMapIter->second->getNumDimensions();
      if(numDims == 1)
      {
         dim1 = GridMapIter->second->getDimension(0);
         std::cout << "Grid " << GridMapIter->first << ": " << dim1 << std::endl;
      }
      else
      {
         if(numDims == 2)
         {
            dim1 = GridMapIter->second->getDimension(0);
            dim2 = GridMapIter->second->getDimension(1);
            std::cout << "Grid " << GridMapIter->first << ": " << dim1 << ", " << dim2 << std::endl;
         }
         else
         {
            if(numDims == 3)
            {
               dim1 = GridMapIter->second->getDimension(0);
               dim2 = GridMapIter->second->getDimension(1);
               dim3 = GridMapIter->second->getDimension(2);
               std::cout << "Grid " << GridMapIter->first << ": " << dim1 << ", " << dim2 << ", " << dim3 << std::endl;
            }
            else
            {
               std::cout << "PvtolObjects does not currently support more than three Grid dimensions" << endl;
            }
         }
      }
   }
   std::cout << "--" << std::endl;
}

void PvtolObjects::storeNewTaskMap(std::string objectName,
                                pvtol::TaskMap *object)
{
   m_taskMapMap.insert(make_pair(objectName, object));
}

TaskMapMapType PvtolObjects::dumpTaskMap(void)
{
   return m_taskMapMap;
}

pvtol::TaskMap* PvtolObjects::getTaskMap(std::string name)
{
   return m_taskMapMap.find(name)->second;
}

/**
 * Prints to the screen the contents of the Grid Map
 */
void PvtolObjects::printTaskMap(void)
{
   TaskMapMapType::iterator TaskMapMapIter;

   std::cout << "--" << std::endl;
   for(TaskMapMapIter = m_taskMapMap.begin();
         TaskMapMapIter != m_taskMapMap.end();
         TaskMapMapIter++)
   {
      std::cout << "TaskMap " << TaskMapMapIter->first << std::endl;
   }
   std::cout << "--" << std::endl;
}

PvtolObjects::~PvtolObjects(void)
{
}

#if 0
template<class Archive>
void PvtolObjects::serialize(Archive & ar, const unsigned int version) const
{
   ar & m_gridMap;
}
#endif

