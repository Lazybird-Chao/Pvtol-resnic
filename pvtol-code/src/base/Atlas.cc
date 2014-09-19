/**
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *
 *  Implementation of the Atlas class methods
 *
 *  $Id: Atlas.cc 938 2009-02-18 17:39:52Z ka21088 $
 *
 */

#include <Atlas.h>
//#include <fstream>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
//#include <boost/serialization/map.hpp>
//#include <boost/serialization/vector.hpp>
//#include <boost/serialization/list.hpp>

namespace ipvtol
{
   Atlas::Atlas(void)
   {
   }

   Atlas::~Atlas(void)
   {
   }

   /**
    * \param objectName std::string name of the object to be stored.[in]
    * \param object A pointer to the Grid object to be stored.[in]
    *
    * Stores the Grid along with a string name
    *
   bool Atlas::include(std::string fileName)
   {
      std::ifstream inFile;
      bool return_val = false;

      /// open the file
      inFile.open(fileName.c_str());

      /// make sure that the open worked
      if(inFile.is_open() == false)
      {
         std::cerr << "Atlas could not open file: " << fileName << std::endl;
      }
      else
      {
         PvtolObjects newPvtolObject;

         /// deserialize the stuff in the file
         deSerialize(newPvtolObject, inFile);

         /// make sure that there are no name conflicts
         return_val = reconcile(newPvtolObject);
      }

      return return_val;
   }

   bool Atlas::getGrid(std::string gridName,
         ipvtol::Grid *object)
   {
      GridMapType::iterator it;

      it = m_gridMap.find(gridName);

      if(it != m_gridMap.end())
      {
         /// object found
         *object = *(it->second);
         return true;
      }
      else
      {
         /// object not found
         object = NULL;
         return false;
      }
   }

   bool Atlas::getRankList(std::string rankListName,
         ipvtol::RankList *object)
   {
      RankListMapType::iterator it;

      it = m_rankListMap.find(rankListName);

      if(it != m_rankListMap.end())
      {
         /// object found
         *object = *(it->second);
         return true;
      }
      else
      {
         /// object not found
         object = NULL;
         return false;
      }
   }

   bool Atlas::getTaskMap(std::string taskMapName,
         ipvtol::TaskMap *object)
   {
      TaskMapMapType::iterator it;

      it = m_taskMapMap.find(taskMapName);

      if(it != m_taskMapMap.end())
      {
         /// object found
         *object = *(it->second);
         return true;
      }
      else
      {
         /// object not found
         object = NULL;
         return false;
      }
   }

   void Atlas::getGridNames(std::vector<std::string> *names)
   {
      GridMapType::iterator it;

      for(it = m_gridMap.begin();
          it != m_gridMap.end();
          it++)
      {
         names->push_back(it->first);
      }
   }

   void Atlas::getRankListNames(std::vector<std::string> *names)
   {
      RankListMapType::iterator it;

      for(it = m_rankListMap.begin();
          it != m_rankListMap.end();
          it++)
      {
         names->push_back(it->first);
      }
   }

   void Atlas::getTaskMapNames(std::vector<std::string> *names)
   {
      TaskMapMapType::iterator it;

      for(it = m_taskMapMap.begin();
          it != m_taskMapMap.end();
          it++)
      {
         names->push_back(it->first);
      }
   }

   void Atlas::deSerialize(PvtolObjects &object, std::istream &inFile)
   {
      boost::archive::text_iarchive ia(inFile);
      ia >> object;
   }

   bool Atlas::reconcile(PvtolObjects &object)
   {
      GridMapType newGridMap = object.dumpGrid();

      GridMapType::iterator gridMapIter;
      GridMapType::iterator gridIt;

      for(gridMapIter = newGridMap.begin();
          gridMapIter != newGridMap.end();
          gridMapIter++)
      {
         gridIt = m_gridMap.find(gridMapIter->first);

         if(gridIt != m_gridMap.end())
         {
            /// found a conflict
            std::cout << "rejecting:" << std::endl;
            std::cout << gridMapIter->first << std::endl;
         }
         else
         {
            /// no conflict found
            /// add object
            std::cout << "adding:" << std::endl;
            std::cout << gridMapIter->first << std::endl;
            m_gridMap.insert(*gridMapIter);
         }
      }

      RankListMapType newRankListMap = object.dumpRankList();

      RankListMapType::iterator rankListMapIter;
      RankListMapType::iterator rankListIt;

      for(rankListMapIter = newRankListMap.begin();
          rankListMapIter != newRankListMap.end();
          rankListMapIter++)
      {
         rankListIt = m_rankListMap.find(rankListMapIter->first);

         if(rankListIt != m_rankListMap.end())
         {
            /// found a conflict
            std::cout << "rejecting:" << std::endl;
            std::cout << rankListMapIter->first << std::endl;
         }
         else
         {
            /// no conflict found
            /// add object
            std::cout << "adding:" << std::endl;
            std::cout << rankListMapIter->first << std::endl;
            m_rankListMap.insert(*rankListMapIter);
         }
      }

      TaskMapMapType newTaskMapListMap = object.dumpTaskMap();

      TaskMapMapType::iterator taskMapMapIter;
      TaskMapMapType::iterator taskMapIt;

      for(taskMapMapIter = newTaskMapListMap.begin();
          taskMapMapIter != newTaskMapListMap.end();
          taskMapMapIter++)
      {
         taskMapIt = m_taskMapMap.find(taskMapMapIter->first);

         if(taskMapIt != m_taskMapMap.end())
         {
            /// found a conflict
            std::cout << "rejecting:" << std::endl;
            std::cout << taskMapMapIter->first << std::endl;
         }
         else
         {
            /// no conflict found
            /// add object
            std::cout << "adding:" << std::endl;
            std::cout << taskMapMapIter->first << std::endl;
            m_taskMapMap.insert(*taskMapMapIter);
         }
      }

      return true;
   }


   void Atlas::printGrid(void)
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
   } */
}

