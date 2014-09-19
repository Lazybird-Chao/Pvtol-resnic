/**
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Contains the prototypes for the functions that do most of the
 *           main work along with a struct for storing command line options
 *
 *  $Id: atlasPreProcUtils.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */

#ifndef __atlasPreProcUtils_h__
#define __atlasPreProcUtils_h_

#include <atlasPreProc.h>

struct userInputVars
{
   std::string defaultFile;
   std::string inputFile;
   std::string outputFile;
   std::string options;
};

struct taskMapInfoType
{
   std::string taskMapName;
   std::string rankListString;
   std::string gridString;
   std::string taskDistDescString;
   pvtol::Grid grid;
   pvtol::RankList rankList;
};

userInputVars parseInputArgs(int argc, char *argv[]);

DOMDocument* openXMLDocument(std::string fileName);

PvtolObjects* generatePvtolObjects(std::vector<DOMDocument *>);

void serializePvtolObjects(const PvtolObjects &, std::string);

void processGrid(DOMElement * element,
      std::string &name,
      pvtol::Grid **tempGrid);

void processRankList(DOMElement * element,
      std::string &name,
      pvtol::RankList **tempRankList);

void processTaskMap(DOMElement * element,
      taskMapInfoType &taskMapInfo);

#endif
