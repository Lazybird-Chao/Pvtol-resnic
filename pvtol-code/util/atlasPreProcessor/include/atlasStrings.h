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
 *  $Id: atlasStrings.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */

#ifndef __atlasStrings_h__
#define __atlasStrings_h__
class atlasStrings
{
   public:
      atlasStrings(void);

      const XMLCh *PVTOL_MAP_ROOT;
      const XMLCh *GRID;
      const XMLCh *RANK_LIST;
      const XMLCh *NAME;
      const XMLCh *ALL;
      const XMLCh *TASK_MAP;
      const XMLCh *TASK_DIST_DESC;
      /**/
      const XMLCh *MAP_SPECS;
      const XMLCh *TASK_SPECS;
      const XMLCh *TASK_GRID;
      const XMLCh *SIZE;
      const XMLCh *INDEX;
      const XMLCh *VALUE;
      const XMLCh *TASK_DIST;
      const XMLCh *NUMBER;
      const XMLCh *OVERLAP;
      const XMLCh *REPLICATED_TASK;
      const XMLCh *AGILE_TASK;
      const XMLCh *COMP_MAP;
      const XMLCh *COMP_GRID;
      const XMLCh *COMP_DIST;
      const XMLCh *TYPE;
      const XMLCh *GRID1D;
      const XMLCh *MAP1D;
      const XMLCh *_LOCAL;
      const XMLCh *_COOLEY_TUKEY;
      const XMLCh *_SIX_STEP;
      const XMLCh *_FOUR_STEP;
      const XMLCh *_NOTSET;
      const XMLCh *BLOCK_CYCLIC;
      const XMLCh *BLOCK;
      const XMLCh *CYCLIC;
      const XMLCh *REPLICATED;
      const XMLCh *BLOCK_SIZE;
      const XMLCh *MIN_SIZE;
      const XMLCh *MODULO_SIZE;
      const XMLCh *VECTOR_DIST;
      const XMLCh *MATRIX_DIST;
      const XMLCh *DIST_3D;
      const XMLCh *GRID2D;
      const XMLCh *GRID3D;
      const XMLCh *MAP2D;
      const XMLCh *MAP3D;
      const XMLCh *ROWS;
      const XMLCh *COLS;
      const XMLCh *PLANES;
      const XMLCh *POSSIBLE_EL_LIST;
      const XMLCh *VECTOR_OVL_DESCRIPT;
      const XMLCh *MATRIX_OVL_DESCRIPT;
      const XMLCh *OVL_DESCRIPT_3D;
      const XMLCh *OVERLAP_DESCRIPT;
      const XMLCh *OVL;
      const XMLCh *COLLECTION_STRATEGY;
      const XMLCh *END_STRATEGY;
      const XMLCh *_READ_ONLY;
      const XMLCh *_DO_NOT_CARE;
      const XMLCh *_WRAP;
      const XMLCh *_NO_FILL;
      const XMLCh *EVAL;
      const XMLCh *ASSERT;
      const XMLCh *ENABLE;
      const XMLCh *SECTION;
      const XMLCh *ECHO;
};

#endif //__atlasStrings_h__
