/**
 *    File: PvtolRequest.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the PvtolRequest & CopyRequest classes
 *    A PvtolRequest object is used to test for the completion of
 *	   a non-Blocking send or receive from a CommScope
 *	   It may also be used to wait for the completion of a
 *         non-Blocking send or receive.
 *
 *    CopyRequest, an alternate version of PvtolRequest that is used
 *    to manage communications that only involve a memcpy from source
 *    to destination.
 *
 *  $Id: PvtolRequest.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#ifndef PvtolRequest_H
#define PvtolRequest_H

#include <mpi.h>
#include <PvtolBasics.h>
#include <PvtolStatus.h>
#include <string.h>

namespace ipvtol
{
  // This class is used to hold request information for communications using
  // memcpy().
  class CopyRequest
  {
  public:
    void wait(CopyStatus& status);
    void test(int* flag, CopyStatus& status);
    void start(void);
    void requestFree(void);
    void cancel(void);

    bool m_isSrc;
    void* m_srcAddr;
    void* m_dstAddr;
    int m_srcRank;
    int m_dstRank;
    int m_tag;
    int m_numBytes;
  };

  class PvtolRequest
  {
  public:
    void wait(PvtolStatus& status);
    void test(int* flag, PvtolStatus& status);
    void start(void);
    void requestFree(void);
    void cancel(void);
    bool getIsLocal(void) const;
    void setIsLocal(bool isLocal);
    operator MPI_Request*();
    operator CopyRequest&();

  private:
    // True if this is a local transfer (i.e. a memcpy())
    bool m_isLocal;
    MPI_Request m_mpiRequest;
    CopyRequest m_copyRequest;
  };

//                    I N L I N E     Methods
//------------------------------------------------------------

/**
 *                  CopyRequest::start()
 * \brief starts the copy, all work is done at src end
 * \param void
 * \return void
 */
inline
void CopyRequest::start(void)
{
  if (m_isSrc)
  {
    memcpy(m_dstAddr, m_srcAddr, m_numBytes);
  }
}//end start()

inline
void CopyRequest::requestFree(void)
{ return; }

inline
void CopyRequest::cancel(void)
{ return; }


  inline 
  void PvtolRequest::wait(PvtolStatus& status)
  {
    status.setIsLocal(m_isLocal);
    if (m_isLocal)
      {
	m_copyRequest.wait(status);
      }
    else
      {
	MPI_Wait(&m_mpiRequest, status);
      }
  }

  inline 
  void PvtolRequest::test(int* flag, PvtolStatus& status)
  {
    status.setIsLocal(m_isLocal);
    if (m_isLocal)
      {
	m_copyRequest.wait(status);
      }
    else
      {
	MPI_Test(&m_mpiRequest, flag, status);
      }
  }

  inline 
  void PvtolRequest::start(void)
  {
    if (m_isLocal)
      {
	m_copyRequest.start();
      }
    else
      {
	MPI_Start(&m_mpiRequest);
      }
  }

  inline
  void PvtolRequest::requestFree(void)
  {
    if (!m_isLocal)
      {
	MPI_Request_free(&m_mpiRequest);
      }
  }

  inline
  void PvtolRequest::cancel(void)
  {
    if (m_isLocal)
      {
	m_copyRequest.cancel();
      }
    else
      {
	MPI_Cancel(&m_mpiRequest);
      }
  }
  
  inline 
  bool PvtolRequest::getIsLocal(void) const
  {
    return m_isLocal;
  }

  inline
  void PvtolRequest::setIsLocal(bool isLocal)
  {
    m_isLocal = isLocal;
  }

  inline
  PvtolRequest::operator CopyRequest&()
  {
    return m_copyRequest;
  }

  inline
  PvtolRequest::operator MPI_Request*()
  {
    return &m_mpiRequest;
  }


}// end namespace

#endif // PvtolRequest_H not defined

