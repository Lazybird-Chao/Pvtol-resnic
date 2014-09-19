/**
 *    File: Transfer.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *    January 2008: Ported from PVL by Jim Daly
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Transfer provides the means to move data point to point 
 *           The data must be contiguous and non-distributed
 *
 *  $Id: Transfer.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#ifndef PVTOL_TRANSFER_H
#define PVTOL_TRANSFER_H

//   I do Not understand why I need to include string.h here
//     I will happily buy beer for anyone who can explain it to me.
#include <string.h>

#include <PvtolBasics.h>
#include <PvtolRequest.h>
#include <CommScope.h>
#include <TaskBase.h>


namespace ipvtol
{

  class SendRequest;

  /** Transfer is an object used to perform point-to-point communications.
   *    Transfer supports blocking, nonblocking, and persistant versions
   *    of point-to-point communications.
   *
   *   Transfer's send methods are expected to be used in a SPMD manner
   *    throughout the task(s) using the send and receive buffers
   *    The Transfer object determines what is to be
   *    done on each node. The same send command covers both the source
   *    end (i.e. the sender) and the destination end (i.e. the receiver).
   *    This API has been intentionally built neutral as to whether the
   *    underlying comm protocol is a two sided or a one sided protocol.
   *
   *   The "i" prefix on the isend method implies a non-blocking call.
   *    That is, the send returns immediately without waiting for the
   *    sending or receiving of data to complete. All isend methods
   *    take a SendRequest ref as a parameter. After the call this
   *    SendRequest object may be queried obout the completion of the
   *    send.
   *
   *   There are various overloaded versions of the send and isend methods.
   *    These version differ on whether the user want to alter the count
   *    being transferred and/or use offsets into the src and dest buffers.
   *
   *   @author Jim Daly
   *   @see SendRequest, Route
   */
  class Transfer
  {
  //++++++++++
    public:
  //++++++++++
    enum Flags {
      DEFAULT_FLAG=0,
      STATIC=1
    };


    /** Constructor.
     *  @param srcRank   TaskRank identifying the node data will come from.
     *  @param srcAddr   char *, src address of starting byte
     *  @param destRank  TaskRank identifying the node data will go to.
     *  @param destAddr  char *, dest address of starting byte
     *  @param byteCount integer specifying num of bytes to be
     *                            transferred.
     *  @param flags     optional: Bitflag which identifies special
     *                   attributes of this instance. These flags are used
     *                   to facilitate optimal performance when the 
     *                   communication is performed.
     *                   The following flags are supported:
     *                       DEFAULT FLAG This implies there is
     *                              nothing special. The user is free to
     *                              use all forms of send and isend later on.
     *                       STATIC  indicates count will not
     *                              change and no offsets will be used
     *                              when send is performed. Note: this
     *                              is a promise by the user Not to use
     *                              new counts or offsets at send-time.
     * @return void, there is not any return value.
     */
    Transfer(int    srcRank,  char *srcAddr,
	     int    destRank, char *destAddr,
	     int    byteCount,
	     Flags  flags=DEFAULT_FLAG);

    // destructor
    ~Transfer(void);
 
    /** Blocking send routine. All parameters are taken from construction.
     *
     * @return void No return value
     */
    void send(void);
 
    /** Blocking send routine. A new count of elements to transfer is
     *   provided. The new count overrides the one given at construct time.
     *
     * @param newCount  integer count of block elements to transfer. This is
     *                   only used when the count differs to that given at
     *                   construction time.
     * @return void No return value.
     */
    void send(int newCount);
 
    /** Blocking send routine. New src and dest offsets are given. These
     *   override those given at construction time.
     *
     * @param srcOff    integer offset, in num of block types, into the src
     *                   buffer where the send is to begin.
     * @param destOff   integer offset, in num of block types, into the dest
     *                   buffer where the data is to be sent.
     * @return void No return value.
     */
    void send(int srcOffset, int destOff);
 
    /** Blocking send routine. New count along with src and dest offsets
     *   are provided, overriding the parameters given at construct time.
     *
     * @param newCount  integer count of block elements to transfer. This is
     *                   only used when the count differs to that given at
     *                   construction time.
     * @param srcOff    integer offset, in num of block types, into the src
     *                   buffer where the send is to begin.
     * @param destOff   integer offset, in num of block types, into the dest
     *                   buffer where the data is to be sent.
     * @return void No return value.
     */
    void send(int newCount, int srcOffset, int destOff);

    /** Non-blocking send.
     *
     * @param sendReq   SendRequest ref This object is later used to determine
     *                   when the local part of the send has completed.
     * @return void No return value.
     */
    void isend(SendRequest &sendReq);

    /** Non-blocking send changing the send count.
     *
     * @param newCount  integer count of block elements to transfer. This is
     *                   only used when the count differs to that given at
     *                   construction time.
     * @param sendReq   SendRequest ref This object is later used to determine
     *                   when the local part of the send has completed.
     * @return void No return value.
     */
    void isend(int newCount, SendRequest &sendReq);

    /** Non-blocking send changing the src and dest offsets.
     *
     * @param srcOff    integer offset, in num of block types, into the src
     *                   buffer where the send is to begin.
     * @param destOff   integer offset, in num of block types, into the dest
     *                   buffer where the data is to be sent.
     * @param sendReq   SendRequest ref This object is later used to determine
     *                   when the local part of the send has completed.
     * @return void No return value.
     */
    void isend(int srcOff, int destOff, SendRequest &sendReq);

    /** Non-blocking send, chnaging the count and offsets.
     *
     * @param newCount  integer count of block elements to transfer. This is
     *                   only used when the count differs to that given at
     *                   construction time.
     * @param srcOff    integer offset, in num of block types, into the src
     *                   buffer where the send is to begin.
     * @param destOff   integer offset, in num of block types, into the dest
     *                   buffer where the data is to be sent.
     * @param sendReq   SendRequest ref This object is later used to determine
     *                   when the local part of the send has completed.
     * @return void No return value.
     */
    void isend(int newCount, int srcOff, int destOff, SendRequest &sendReq);

    /** Get the internal communications tag used by the Transfer.
     *  For internal use only!
     *
     *  @return integer communications tag used by the Transfer
     */

    int getTag() const;

    bool isDest() const;
    bool isSrc() const;

    /**
     *  Set the internal communications tag used by the Transfer.
     *  Because this affects the allocation of tags, it must
     *  be done in a SPMD way across the task in which the transfer
     *  was created.
     *
     *  For internal use only!
     *
     *  @param integer communications tag to be used by the Transfer
     */

    void setTag(int tag);
 
  //++++++++++
    private:
  //++++++++++
    enum Trait {
      NULL_TRAIT=0,
      NO_LOCAL_DATA_MOVEMENT,
      DATA_MOVEMENT_IS_THREAD_LOCAL,
      DATA_MOVEMENT_IS_PROC_LOCAL,
      STATIC_TRANSFER,
      NON_STATIC_TRANSFER
    };

    //  Methods
    void constructHelper(int    srcRank,  char *srcAddr,
                         int    destRank, char *destAddr,
		         int    byteCount, Flags flags);

    void localCopy(int count, int srcOffset, int destOffset);
    void iLocalCopy(int count, int srcOffset, int destOffset, SendRequest &req);

    void staticSend();
    void iStaticSend(SendRequest &req);

    void nonStaticSend(int count, int srcOff, int destOff);
    void iNonStaticSend(int count, int srcOff, int destOff, SendRequest &req);

    // Member data
    Trait        m_trait;
    int          m_eltSize;
    int          m_sendCount;
    int          m_sendSize;
    bool         m_isSrc;
    bool         m_isDest;
    bool         m_localCopy;
    int          m_recvdCount;
    int          m_tag;
    int          m_srcRank;
    int          m_destRank;
    int          m_myRank;
    ProcId       m_srcProc;
    ProcId       m_destProc;
    int          m_srcProcRank;
    int          m_destProcRank;

    void        *m_srcAddr;
    void        *m_destAddr;

    PvtolRequest         m_req;

    CommScope           *m_commScope;
    int                  m_itxdKey;
    InterThreadXferData *m_itxdEntry;


    // methods declared private to prevent their use
    //    Default Constructor, Assignment Operator, Copy Constructor
    Transfer(void);
    Transfer& operator=(const Transfer& transfer);
    Transfer(const Transfer& other);

    friend class SendRequest;
  };

  inline
  void Transfer::send(void)
   { send(this->m_sendCount, 0, 0); }

  inline
  void Transfer::send(int newCount)
   { send(newCount, 0, 0); }

  inline
  void Transfer::send(int srcOff, int destOff)
   { send(this->m_sendCount, srcOff, destOff); }

  inline
  void Transfer::isend(SendRequest &req)
   { isend(this->m_sendCount, 0, 0, req); }

  inline
  void Transfer::isend(int srcOff, int destOff, SendRequest &req)
   { isend(this->m_sendCount, srcOff, destOff, req); }

  inline
  void Transfer::isend(int newCount, SendRequest &req)
   { isend(newCount, 0, 0, req); }

  inline
  bool Transfer::isSrc() const
   {
      return(m_isSrc);
   }

  inline
  bool Transfer::isDest() const
   {
      return(m_isDest);
   }


}//end namespace

#endif // PVTOL_TRANSFER_H not defined

