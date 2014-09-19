/**
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *
 *  Contains the PvtolObjects class definition.
 *  This class is the common class between the Atlas PreProcessor and the
 *  Atlas contained in the PVTOL library. The PreProcessor parses one or
 *  more PVTOL XML files to generate PVTOL objects. These objects are then
 *  serialized out to a text file that can be read by the Atlas in a PVTOL
 *  program. The Atlas, after reading the text file, can then reconstitue
 *  the PVTOL objects for use by the PVTOL program.
 *
 *  $Id: PvtolObjects.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */

#ifndef PvtolObjects_h
#define PvtolObjects_h

#include <Pvtol.h>
#include <string>
#include <map>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>

typedef std::map<std::string, pvtol::Grid*> GridMapType;
typedef std::map<std::string, pvtol::RankList*> RankListMapType;
typedef std::map<std::string, pvtol::TaskMap*> TaskMapMapType;

/**
 * This is the object that gets serialized and deserialized when the Atlas
 * PreProcessor wants to pass information to the Atlas in the PVTOL library.
 */
class PvtolObjects
{
   friend class boost::serialization::access;
   public:
      PvtolObjects(void) { };

      void storeNewRankList(std::string, pvtol::RankList *);
      RankListMapType dumpRankList(void);
      pvtol::RankList* getRankList(std::string);
      void printRankList(void);

      void storeNewGrid(std::string, pvtol::Grid *);
      GridMapType dumpGrid(void);
      pvtol::Grid* getGrid(std::string);
      void printGrid(void);

      void storeNewTaskMap(std::string, pvtol::TaskMap *);
      TaskMapMapType dumpTaskMap(void);
      pvtol::TaskMap* getTaskMap(std::string);
      void printTaskMap(void);

      ~PvtolObjects(void);

      template<class Archive>
      void serialize(Archive & ar, const unsigned int version)
      {
         ar & m_gridMap;
         ar & m_rankListMap;
         ar & m_taskMapMap;
      }

   private:

      RankListMapType m_rankListMap;
      GridMapType m_gridMap;
      TaskMapMapType m_taskMapMap;
};

#endif //PvtolObjects_h
