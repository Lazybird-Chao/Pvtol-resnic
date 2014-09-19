/**
 *    File: SendRequest.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition & Inline methods of the SendRequest class
 *    A SendRequest object is used to test for the completion of a non-
 *	   Blocking send from a Transfer, Route, or Broadcast object.
 *	   It may also be used to wait for the completion of a non-
 *         Blocking send from a Transfer, Route, or Broadcast object.
 *
 *    Note: completion is defined as the "send having completed". At the
 *	    a source node this implies the data has left the buffers and
 *	    a user may now safely reuse his send buffer. At a destination
 *	    node this implies the data has been received. The receive buffer
 *	    now holds the data which was sent.
 *
 *  $Id: SendRequest.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#ifndef PVTOL_SENDREQUEST_H
#define PVTOL_SENDREQUEST_H

#include <PvtolBasics.h>
#include <PvtolRequest.h>
#include <pthread.h>

namespace ipvtol
{
  class Transfer;
  class Route;
  class SrcXferInfo;
  class DestXferInfo;


  /** SendRequest object is used to check the completion status of a
   *  non-blocking send from a Transfer or Route object. It may also be
   *  used to wait for the local part of a non-blocking send to complete.
   *
   *  Note that, completion is defined as the "send having completed".
   *  At a source node this implies the data has left the buffers and
   *  a user may now safely reuse his send buffer. At a destination node
   *  this implies the data has been received. The receive buffer now holds
   *  the data which was sent.
   *
   *  A SendRequest object must be dedicated to a particular Transfer or
   *  Route object. The most efficient way to do this is to construct
   *  the SendRequest giving the object of interest in as a parameter or
   *  the SendRequest may be setup prior to its usage, via its setup
   *  method. After that, one may use and reuse the SendRequest for any
   *  and all non-blocking sends of the particular object it had been
   *  setup to handle. The user must not use the same SendRequest for
   *  multiple Transfer or Route instances.
   *
   * @author Jim Daly
   * @see Route, Transfer
   */
  class SendRequest
  {
  //++++++++++++++
    public:
  //++++++++++++++

    // constructors
    //---------------
    /**
     * Default Constructor, a SendRequest built this way may later be
     *   setup, via the setup method, or just used as a parameter to
     *   a isend call.
     *
     * @param  void, there are no input parameters.
     * @return void No return value.
     */
    SendRequest(void);

    /** Construct for use with a particular Transfer object.
     *
     * @param  Transfer ref This is the particular object which the SendRequest
     *                   is to used with for non-blocking send calls.
     * @return void No return value.
     */
    SendRequest(const Transfer &transfer);

    /** Construct for use with a particular Route object.
     *
     * @param  Route ref This is the particular object which the SendRequest
     *                   is to used with for non-blocking send calls.
     * @return void No return value.
     */
    SendRequest(Route &route);

    /** This method allows the user to customize the SendRequest for a
     *	 particular Transfer object. This allows a default construction
     *   and the efficiency of pre-allocating and initializing
     *   SendRequest data members.
     * @param  Transfer ref This is the particular object which the SendRequest
     *                   is to used with for non-blocking send calls.
     * @return void No return value.
     */
    void setup(const Transfer &transfer);

    /** This method allows the user to customize the SendRequest for a
     *	 particular Route object. This allows a default construction
     *   and the efficiency of pre-allocating and initializing
     *   SendRequest data members.
     * @param  Route ref This is the particular object which the SendRequest
     *                   is to used with for non-blocking send calls.
     * @return void No return value.
     */
    void setup(Route &route);


    /** Test whether the last isend completed its local part.
     * @param  void, there are no input parameters.
     * @return bool, true implies nonblocking send completed locally.
     *               false implies the send has not completed.
     */
    bool test();

    /** wait (perhaps block the process) until the nonblocking send completes.
     *
     * @param  void, there are no input parameters
     * @return bool, true implies nonblocking send completed locally.
     *               false implies the send will not complete, it failed.
     */
    bool wait();

    /** Cancel the request to free up any Comm library resources associated
     *  with it.  This should be called before destruction if the request
     *  has not finished.  (Calling it if the request has finished will have
     *  no effect except for some wasted time.)
     *
     * @param  void, there are no input parameters
     */
    void cancel();

    //              preset()
    //-----------------------------------------------------------
    bool preset() const;

    //              numSends()
    //-----------------------------------------------------------
    int numSends() const;

    //              numRecvs()
    //-----------------------------------------------------------
    int numRecvs() const;

    //              recvdCount()
    //-----------------------------------------------------------
    int recvdCount() const;

    //              disassociate()
    //-----------------------------------------------------------
    void disassociate(Route &route);

    // destructor
    ~SendRequest(void);

    //         Assignment Operator  =
    //=============================================
    const SendRequest& operator=(const SendRequest& rhs);

  //++++++++++++++
    private:
  //++++++++++++++
    //   Private Data
    //-------------------------------------
    bool                  m_preset;
    bool                  m_done;
    int                   m_recvdCount;
    int                   m_numRecvs;
    PvtolRequest         *m_recvRequest;
    int                   m_numSends;
    PvtolRequest         *m_sendRequest;
    bool                  m_iBuiltReqs;
    int                   m_eltSize;

    SrcXferInfo         **m_currRouteSrcXfer;
    DestXferInfo        **m_currRouteDestXfer;

    Route                *m_associatedRoute;
    const Transfer       *m_associatedTransfer;

    //   these pthread constructs are used when SendRequest
    //     is put into wait mode and the data transfer is between
    //     threads of the same process
    pthread_mutex_t       m_waitMutex;
    pthread_mutex_t       m_waitCvMutex;
    pthread_cond_t        m_waitCond;
    bool                  m_isWaiting;

    //   Private Methods, may be used by
    //     SendRequest friends.
    //-------------------------------------
    void setDone();
    void setNotDone();

    // methods declared private to prevent their use
    //    Copy Constructor
    //-------------------------------------
    SendRequest(const SendRequest& src);

    friend class Transfer;
    friend class Route;
  };


//                    I N L I N E   Methods
//------------------------------------------------------------

inline
  bool SendRequest::preset() const
   { return(m_preset); }
 
inline
  int SendRequest::numSends() const
   { return(m_numSends); }
 
inline
  int SendRequest::numRecvs() const
   { return(m_numRecvs); }
 
inline
  int SendRequest::recvdCount() const
   { return(m_recvdCount); }
 
inline
  void SendRequest::setDone()
   { m_done = true; }
 
inline
  void SendRequest::setNotDone()
   { m_done = false; }


}// end namespace


#endif // PVL_SENDREQUEST_H not defined

