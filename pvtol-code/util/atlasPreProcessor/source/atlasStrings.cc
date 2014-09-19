/**
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Contains all of the strings needed to parse the XML files. The
 *           strings are stored in a Xerces container (XMLString).
 *
 *  $Id: atlasStrings.cc 938 2009-02-18 17:39:52Z ka21088 $
 *
 */

#include <atlasPreProc.h>
#include <atlasStrings.h>

atlasStrings::atlasStrings(void)
{
   PVTOL_MAP_ROOT = XMLString::transcode("PvtolMaps");
   GRID = XMLString::transcode("Grid");
   RANK_LIST = XMLString::transcode("RankList");
   NAME = XMLString::transcode("name");
   ALL = XMLString::transcode("*");
   TASK_MAP = XMLString::transcode("TaskMap");
   TASK_DIST_DESC = XMLString::transcode("TaskDistDesc");
   /**/
   MAP_SPECS = XMLString::transcode("MapSpecs");
   TASK_SPECS = XMLString::transcode("TaskSpecs");
   TASK_GRID = XMLString::transcode("TaskGrid");
   SIZE = XMLString::transcode("size");
   INDEX = XMLString::transcode("Index");
   VALUE = XMLString::transcode("value");
   TASK_DIST = XMLString::transcode("TaskDistDescript");
   NUMBER = XMLString::transcode("number");
   OVERLAP = XMLString::transcode("overlap");
   REPLICATED_TASK = XMLString::transcode("ReplicatedTask");
   AGILE_TASK = XMLString::transcode("AgileTask");
   COMP_MAP = XMLString::transcode("ComputationMap");
   COMP_GRID = XMLString::transcode("ComputationGrid");
   COMP_DIST = XMLString::transcode("ComputationDistDescript");
   TYPE = XMLString::transcode("type");
   GRID1D = XMLString::transcode("Grid1d");
   MAP1D = XMLString::transcode("Map1d");
   _LOCAL = XMLString::transcode("LOCAL");
   _COOLEY_TUKEY = XMLString::transcode("COOLEY_TUKEY");
   _SIX_STEP = XMLString::transcode("SIX_STEP");
   _FOUR_STEP = XMLString::transcode("FOUR_STEP");
   _NOTSET = XMLString::transcode("NOTSET");
   BLOCK_CYCLIC = XMLString::transcode("BlockCyclic");
   BLOCK = XMLString::transcode("Block");
   CYCLIC = XMLString::transcode("Cyclic");
   REPLICATED = XMLString::transcode("Replicated");
   BLOCK_SIZE = XMLString::transcode("BlockSize");
   MIN_SIZE = XMLString::transcode("MinSize");
   MODULO_SIZE = XMLString::transcode("ModuloSize");
   VECTOR_DIST = XMLString::transcode("VectorDistDescript");
   MATRIX_DIST = XMLString::transcode("MatrixDistDescript");
   DIST_3D = XMLString::transcode("DistDescript3d");
   GRID2D = XMLString::transcode("Grid2d");
   GRID3D = XMLString::transcode("Grid3d");
   MAP2D = XMLString::transcode("Map2d");
   MAP3D = XMLString::transcode("Map3d");
   ROWS = XMLString::transcode("rows");
   COLS = XMLString::transcode("cols");
   PLANES = XMLString::transcode("planes");
   POSSIBLE_EL_LIST = XMLString::transcode("PossibleElementList");
   VECTOR_OVL_DESCRIPT = XMLString::transcode("VectorOvlDescript");
   MATRIX_OVL_DESCRIPT = XMLString::transcode("MatrixOvlDescript");
   OVL_DESCRIPT_3D = XMLString::transcode("OvlDescript3d");
   OVERLAP_DESCRIPT = XMLString::transcode("OverlapDescript");
   OVL = XMLString::transcode("ovl");
   COLLECTION_STRATEGY = XMLString::transcode("collectionStrategy");
   END_STRATEGY = XMLString::transcode("endStrategy");
   _READ_ONLY = XMLString::transcode("READ_ONLY");
   _DO_NOT_CARE = XMLString::transcode("DO_NOT_CARE");
   _WRAP = XMLString::transcode("WRAP");
   _NO_FILL = XMLString::transcode("NO_FILL");
   EVAL = XMLString::transcode("Eval");
   ASSERT = XMLString::transcode("Assert");
   ENABLE = XMLString::transcode("enable");
   SECTION = XMLString::transcode("Section");
   ECHO = XMLString::transcode("Echo");
}

