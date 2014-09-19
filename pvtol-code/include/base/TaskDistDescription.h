/**
 *  File: TaskDistDescription.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    TaskDistDescription.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition and inline methods for the TaskDistDescription class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: TaskDistDescription.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_TASKDISTDESCRIPTION_H_
#define PVTOL_TASKDISTDESCRIPTION_H_


#include <DistDescription.h>

namespace ipvtol
{

  /// \brief class TaskDistDescription
  /// The TaskDistDescription class is used to provide information
  /// for distributing tasks.  For the initial implementation,
  /// this class mainly provides information concerning replicated
  /// tasks, but may be extended in the future if another task
  /// distribution is desired.
  class TaskDistDescription : public DistDescription {
     friend class boost::serialization::access;
    
  public:
    
    /// \brief enum TaskDistType
    /// The TaskDistType enumeration provides information as to what
    /// type of distribution for the task is desired.
    enum TaskDistType {
      TASK_DIST_STANDARD,
      TASK_DIST_REPLICATED
    };
    
    /// \brief Constructor
    /// Construct a TaskDistDescription object with a standard distribution.  
    /// I.e., not replicated.
    TaskDistDescription();
    
    /// \brief Constructor
    /// Construct a TaskDistDescription, given replicated distribution 
    /// information.
    TaskDistDescription(const TaskDistType distType, 
			const unsigned int numReplicas, 
			const unsigned int replicaSize,
			const unsigned int replicaOverlap = 0);
    
    /// \brief Copy contructor
    TaskDistDescription(const TaskDistDescription &);

    /// \brief Destructor
    virtual ~TaskDistDescription();

    /// \brief clone
    /// Create and return a copy of the object.  It is the user's 
    /// responsibility to delete the returned object.
    virtual TaskDistDescription * clone() const;

    /// \brief getTaskDistType
    TaskDistType getTaskDistType() const;
    
    /// \brief getNumReplicas
    unsigned int getNumReplicas() const;
    
    /// \brief getReplicaSize
    unsigned int getReplicaSize() const;
    
    /// \brief getOverlap
    unsigned int getOverlap() const;
    
    /// \brief getSerializedSize
    /// Return size of the serialized object.
    virtual int getSerializedSize() const;
    
    /// \brief serialize
    /// Store all object information within the supplied integer
    /// buffer.  Return the buffer, incremented to the next available storage
    /// location.
    virtual int * serialize(int * buffer) const;
    
    /// \brief deserialize
    /// Initialize the object with the information supplied within
    /// the buffer.  Return the buffer, incremented to the next available 
    /// storage location.
    virtual int * deserialize(int * buffer);

    /// \brief serialize
    /// This method is needed by the Boost::serialize methods for Atlas.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);

    /// \brief Assignment operator
    virtual const TaskDistDescription & operator=(const TaskDistDescription &);
    
    /// \brief Equality operator
    virtual bool operator==(const TaskDistDescription &) const;
    
    /// \brief Inequality operator
    virtual bool operator!=(const TaskDistDescription &) const;
    
  private:
    TaskDistType  m_distType;
    unsigned int  m_numReplicas;
    unsigned int  m_replicaSize;
    unsigned int  m_overlap;
    
  }; // class DistDescription
  

  // \brief Constructor
  // Construct a TaskDistDescription object with a standard distribution.  
  // I.e., not replicated.
  inline
  TaskDistDescription::TaskDistDescription()
    : DistDescription(DistDescription::TASK_DIST_DESCRIPTION),
      m_distType(TASK_DIST_STANDARD),
      m_numReplicas(1),
      m_replicaSize(0),
      m_overlap(0)
  {
  }
  
  // \brief Constructor
  // Construct a TaskDistDescription, given replicated distribution 
  // information.
  inline
  TaskDistDescription::TaskDistDescription(const TaskDistType distType, 
					   const unsigned int numReplicas, 
					   const unsigned int replicaSize,
					   const unsigned int replicaOverlap)
    : DistDescription(DistDescription::TASK_DIST_DESCRIPTION),
      m_distType(distType),
      m_numReplicas(numReplicas),
      m_replicaSize(replicaSize),
      m_overlap(replicaOverlap)
  {
  }
  
  // \brief Copy contructor
  inline
  TaskDistDescription::TaskDistDescription(const TaskDistDescription & other)
    : DistDescription(other),
      m_distType(other.m_distType),
      m_numReplicas(other.m_numReplicas),
      m_replicaSize(other.m_replicaSize),
      m_overlap(other.m_overlap)
  {
  }
  
  // \brief Destructor
  inline
  TaskDistDescription::~TaskDistDescription() {
  }

  // \brief clone
  // Create and return a copy of the object.  It is the user's responsibility
  // to delete the returned object.
  inline
  TaskDistDescription * TaskDistDescription::clone() const {
    return (new TaskDistDescription(*this));
  }
  
  // \brief getTaskDistType
  inline TaskDistDescription::TaskDistType 
  TaskDistDescription::getTaskDistType() const {
    return m_distType;
  }
  
  // \brief getNumReplicas
  inline
  unsigned int TaskDistDescription::getNumReplicas() const {
    return m_numReplicas;
  }
  
  // \brief getReplicaSize
  inline
  unsigned int TaskDistDescription::getReplicaSize() const {
    return m_replicaSize;
  }
  
  // \brief getOverlap
  inline
  unsigned int TaskDistDescription::getOverlap() const {
    return m_overlap;
  }

  // \brief getSerializedSize
  // Return size of the serialized object.
  inline
  int TaskDistDescription::getSerializedSize() const {
    return 4;
  }

  // \brief serialize
  // This method is needed by the Boost::serialize methods for Atlas.
  template<class Archive>
  inline
  void TaskDistDescription::serialize(Archive & ar, 
				      const unsigned int version) {
    // This line tells Boost::serialize that it also needs to serialize
    // the base class
    ar & boost::serialization::base_object<DistDescription>(*this);
    ar & m_distType;
    ar & m_numReplicas;
    ar & m_replicaSize;
    ar & m_overlap;
  }  

  // \brief Inequality operator
  inline
  bool TaskDistDescription::operator!=(const TaskDistDescription & 
				       other) const {
    return !(*this == other);
  }
    
} // namespace ipvtol


#endif // PVTOL_TASKDISTDESCRIPTION_H_ not defined

