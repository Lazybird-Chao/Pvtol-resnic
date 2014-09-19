/**
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *
 *  Contains the Atlas class definition.
 *
 *  To use the Atlas, create a new empty Atlas object. Then call
 *  include(<fileName>) to add objects to the Atlas object. If the name of
 *  the object is know to the program, then it can be retrieved by calling
 *  get<object>(). Be sure to check the return status of this call. It will
 *  return NULL if the string passed in is not present in the Atlas. To see
 *  what objects are available in the Atlas, call the get<object>Name() method.
 *
 *  $Id: Atlas.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */

#ifndef Atlas_h
#define Atlas_h

//#include <Pvtol.h>
//#include <PvtolObjects.h>
#include <string>
#include <map>

/**
 * This is the object that gets serialized and deserialized when the Atlas
 * PreProcessor wants to pass information to the Atlas in the PVTOL library.
 */
namespace ipvtol
{
   class Atlas
   {
      public:
         /// Default Constructor
         Atlas(void);

         /// Default Destructor
         ~Atlas(void);
/*
         /// Use this method to add objects to an existing Atlas.
         /// param fileName The name of the file that contains the
         ///       serialized PvtolObjects object.
         /// return TRUE if file successfully added; FALSE if otherwise
         bool include(std::string);

         /// This method populates a Grid object with the Grid
         /// that is stored in the Atlas under the name given.
         /// param name The name of the Grid object to return
         /// param *object A pointer to the object that will get filled in
         /// return TRUE if file successfully assigned; FALSE if otherwise
         bool getGrid(std::string, ipvtol::Grid*);
         /// Use this method to get a vector of strings that contains the
         /// names of the Grids that the Atlas knows about.
         /// param nameList A pointer to a vector of strings that will hold
         ///       the list of Grids that the Atlas knows about and can be
         ///       accessed by getGrid()
         void getGridNames(std::vector<std::string>*);
         /// Prints to the screen information about all of the Grids that the
         /// Atlas knows about.
         void printGrid(void);

         /// This method populates a RankList object with the RankList
         /// that is stored in the Atlas under the name given.
         /// param name The name of the RankList object to return
         /// param *object A pointer to the object that will get filled in
         /// return TRUE if file successfully assigned; FALSE if otherwise
         bool getRankList(std::string, ipvtol::RankList*);
         /// Use this method to get a vector of strings that contains the
         /// names of the RankLists that the Atlas knows about.
         /// param nameList A pointer to a vector of strings that will hold
         ///       the list of RankLists that the Atlas knows about and can be
         ///       accessed by getRankList()
         void getRankListNames(std::vector<std::string>*);

         /// This method populates a TaskMap object with the TaskMap
         /// that is stored in the Atlas under the name given.
         /// param name The name of the TaskMap object to return
         /// param *object A pointer to the object that will get filled in
         /// return TRUE if file successfully assigned; FALSE if otherwise
         bool getTaskMap(std::string, ipvtol::TaskMap*);
         /// Use this method to get a vector of strings that contains the
         /// names of the TaskMaps that the Atlas knows about.
         /// param nameList A pointer to a vector of strings that will hold
         ///       the list of TaskMaps that the Atlas knows about and can be
         ///       accessed by getTaskMap()
         void getTaskMapNames(std::vector<std::string>*);

      private:

         GridMapType m_gridMap;

         RankListMapType m_rankListMap;

         TaskMapMapType m_taskMapMap;

         /// This method encapsulates the process of deserializing the
         /// objects from the file located at the spot passed into include()
         /// above. The objects get deserialized into a temporary object to
         /// ensure that there are no collisions in the name of the objects.
         void deSerialize(PvtolObjects&, std::istream&);

         /// This method attempts to merge the objects that have been
         /// deserialized by the deSerialize() method into the master list
         /// of objects that is available to the Atlas.
         bool reconcile(PvtolObjects&);
         */
   };
}

#endif //Atlas_h
