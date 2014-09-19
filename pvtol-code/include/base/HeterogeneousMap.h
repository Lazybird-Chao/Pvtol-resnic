/**
 *  File: HeterogeneousMap.h
 *
 *  \file    HeterogeneousMap.h
 *  \author  $LastChangedBy: $
 *  \date    $LastChangedDate: $
 *  \version $LastChangedRevision: $
 *  \brief   Definition of the HeterogeneousMap class.
 *
 *  Author: James Brock
 *
 *  $Id: $
 */
#ifndef PVTOL_HETEROGENEOUSMAP_H_
#define PVTOL_HETEROGENEOUSMAP_H_

namespace ipvtol {

  /// \brief class HeterogeneousMap
  /// The HeterogeneousMap class is used to provide mapping information for 
  /// Heterogeneous Tasks.  It inherits from the abstract base Map class.
  class HeterogeneousMap {
    
  public:
    /// \brief Default Constructor
    HeterogeneousMap();    

    /// \brief Constructor
    HeterogeneousMap(const HTaskInfo * ranks, const int size);
    
    /// \brief Copy contructor
    HeterogeneousMap(const HeterogeneousMap & other);
    
    /// \brief Destructor
    ~HeterogeneousMap();
    
	 /// \brief setMap function
	 void setMap(vector<HTaskInfo> map);

    /// \brief getMap function
    vector<HTaskInfo> * getMap();

	 /// \brief getSize function
	 int getSize();
    
    /// \brief getTaskInfo function
    HTaskInfo * getTaskInfo(int idx = 0);
    
    vector<HTaskInfo> hMap;
   
  }; // class HeterogeneousMap

  //------------------
  // INLINE FUNCTIONS
  //------------------

  // \brief Destructor
  inline
  HeterogeneousMap::~HeterogeneousMap() { hMap.clear(); }

  // \brief Default constructor
  inline
  HeterogeneousMap::HeterogeneousMap() {
   static HTaskInfo tmp_info = { LOC_CPU, 0, 0, PLAT_C};
   hMap.push_back(tmp_info);
  }

  // \brief Copy contructor
  inline
  HeterogeneousMap::HeterogeneousMap(const HeterogeneousMap & other) { hMap = other.hMap; }

  /// \brief Constructor
  inline
  HeterogeneousMap::HeterogeneousMap(const HTaskInfo * ranks, const int size) {
   for (int idx = 0; idx < size; idx++) { hMap.push_back(ranks[idx]); }
  }

  /// \brief getMap function
  inline
  void HeterogeneousMap::setMap(vector<HTaskInfo> map) { hMap.clear(); hMap = map; }

  /// \brief getMap function
  inline
  vector<HTaskInfo> * HeterogeneousMap::getMap() { return &hMap; }
  
  /// \brief getSize
  inline
  int HeterogeneousMap::getSize() { return hMap.size(); }

  /// \brief getTaskInfo function
  inline
  HTaskInfo * HeterogeneousMap::getTaskInfo(int idx) { return &(hMap[idx]); } 
  
} // namespace ipvtol


#endif // PVTOL_HETEROGENEOUSMAP_H_ not defined


