/**
 *    File: PvtolStatus.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the PvtolStatus & CopyStatus classes
 *    A PvtolStatus object contains information about a received message.
 *
 *  $Id: PvtolStatus.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#ifndef PvtolStatus_H
#define PvtolStatus_H

#include <mpi.h>

#include <PvtolBasics.h>


namespace ipvtol
{

  // This class contains status information for copy communications.
  class CopyStatus
  {
  public:
    int getSource(void) const;
    int getTag(void) const;
    int getError(void) const;
    int getCount(void) const;
    
    bool m_isSrc;
    int m_error;
    int m_source;
    int m_tag;
    int m_count;
  };


  class PvtolStatus
  {
  public:
    int getSource(void) const;
    int getTag(void) const;
    int getError(void) const;
    int getCount(void) const;
    bool getIsLocal(void) const;
    void setIsLocal(bool isLocal);

    operator MPI_Status*();
    operator CopyStatus&();

  private:
    bool m_isLocal;
    mutable MPI_Status m_mpiStatus;
    CopyStatus m_copyStatus;
  };


/////////////////////////////////////////////////////////////////////////////
//                       CopyStatus inlines
/////////////////////////////////////////////////////////////////////////////
/**
 *                  CopyStatus::getSource()
 * \brief 
 * \param 
 * \return 
 */
  inline
  int CopyStatus::getSource(void) const
  {
    if (m_isSrc)
      {
	return 0;
      }
    return m_source;
  }

  inline 
  int CopyStatus::getTag(void) const
  {
    if (m_isSrc)
      {
	return 0;
      }
    return m_tag;
  }

  inline
  int CopyStatus::getError(void) const
  {
    return m_error;
  }

  inline
  int CopyStatus::getCount(void) const
  {
    if (m_isSrc)
      {
	return 0;
      }
    return m_count;
  }


/////////////////////////////////////////////////////////////////////////////
//                       PvtolStatus inlines
/////////////////////////////////////////////////////////////////////////////
  inline 
  int PvtolStatus::getSource(void) const
  {
    if (!m_isLocal)
      {
	return m_mpiStatus.MPI_SOURCE;
      }
    else
      {
	return m_copyStatus.getSource();
      }
  }

  inline 
  int PvtolStatus::getTag(void) const
  {
    if (!m_isLocal)
      {
	return m_mpiStatus.MPI_TAG;
      }
    else
      {
	return m_copyStatus.getTag();
      }
  }

  inline 
  int PvtolStatus::getError(void) const
  {
    if (!m_isLocal)
      {
	return m_mpiStatus.MPI_ERROR;
      }
    else
      {
	return m_copyStatus.getError();
      }
  }

  inline
  int PvtolStatus::getCount(void) const
  {
    if (!m_isLocal)
      {
	int count;
	MPI_Get_count(&m_mpiStatus, MPI_CHAR, &count);
	return count;
      }
    else
      {
	return m_copyStatus.getCount();
      }
  }

  inline
  bool PvtolStatus::getIsLocal(void) const
  {
    return m_isLocal;
  }

  inline 
  void PvtolStatus::setIsLocal(bool isLocal)
  {
    m_isLocal = isLocal;
  }

  inline
  PvtolStatus::operator MPI_Status*()
  {
    return &m_mpiStatus;
  }

  inline
  PvtolStatus::operator CopyStatus&()
  {
    return m_copyStatus;
  }

}// end Namespace

#endif // PvtolStatus_H not defined

