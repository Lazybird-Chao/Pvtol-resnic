/**
 *    File: CommScope.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the CommScope class.
 *           A CommScope object provides communications funtionality across
 *              a set of processes.
 *           CommScope provides a layer of abstraction from the underlying
 *              communications library being used.
 *
 *     See also CommScope.inl for the inline methods.
 *
 *  $Id: CommScope.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */

#ifndef PVTOL_COMMSCOPE_H
#define PVTOL_COMMSCOPE_H


#include <PvtolBasics.h>
#include <Exception.h>
#include <PvtolRequest.h>
#include <Map.h>

#include <vector>
using std::vector;

namespace ipvtol
{
 class CommScope
  {
  public:
    // typedefs
#ifdef _STANDARD_MPI
    typedef MPI_Comm CommType;
#endif // _STANDARD_MPI

    // constructors
    CommScope(int worldSize);
    CommScope(const Map &map);
    
    // destructor
    ~CommScope(void);
 
       // Get the Comm object
       //--------------------------------------------------
    const CommType& comm(void) const;

       // Gets the next available tag value
       //--------------------------------------------------
    int getNextTag(void) throw(Exception);

       // Return Tag to available tag-pool
       //--------------------------------------------------
    void returnTag(int tag);

       // Returns the rank of a particular processor with
       //   respect to this CommScope
       //--------------------------------------------------
    int rank(ProcId proc) const;

       // Returns current processor's ProcId, this ID has
       //   global scope
       //--------------------------------------------------
    int getProcId() const;

       // Converts a rank within this CommScope to a
       //   global scope ProcId
       //--------------------------------------------------
    ProcId rankToProcId(int rank) const;

       // Returns the number of processors in this
       //   communications scope
       //--------------------------------------------------
    int getNumProcs() const;

    void  barSynch();

       // Send Data from current node and to a destination
       //   within this CommScope's scope
       //   This is a Blocking send. That is, when this routine
       //   returns the src buffer may be reused.
       //---------------------------------------------------------------
    void send(void *srcAddr, 
	      void *dstAddr, 
	      int byteCount, 
	      int destRank, 
	      int tag) const;

       // Send Data from current node and to a destination
       //   within this CommScope's scope
       //   This is a nonBlocking send. That is, when this routine
       //   returns the src buffer should NOT be reused until the
       //   PvtolRequest has been tested or asked to wait.
       //---------------------------------------------------------------
    void isend(void *srcAddr, 
	       void *dstAddr,
	       int byteCount, 
	       int destRank, 
	       int tag, 
	       PvtolRequest& req) const;

       // Set up for a persistant (a.k.a. deferred) send
       //---------------------------------------------------------------
    void sendInit(void *srcAddr, 
		  void *dstAddr,
		  int byteCount, 
		  int destRank, 
		  int tag, 
		  PvtolRequest& req) const;

       // Receive data at the current node from a source
       //   within this CommScope's scope
       //   This is a Blocking receive. That is, when this routine
       //   returns the data has been received.
       //---------------------------------------------------------------
    void recv(void *srcAddr, 
	      void *destAddr, 
	      int byteCount, 
	      int srcRank, 
	      int tag) const;

       // Receive data at the current node from a source
       //   within this CommScope's scope
       //   This is a NonBlocking receive. That is, when this routine
       //   returns the data has Not been received. The PvtolRequest
       //   may be queied as to when the receive has completed
       //---------------------------------------------------------------
    void irecv(void *srcAddr, 
	       void *destAddr, 
	       int byteCount, 
	       int srcRank, 
	       int tag, 
	       PvtolRequest& req) const;

       // Set up for a persistant (a.k.a. deferred) send
       //---------------------------------------------------------------
    void recvInit(void *srcAddr, 
		  void *destAddr, 
		  int byteCount, 
		  int srcRank, 
		  int tag, 
		  PvtolRequest& req) const;

  private:
    //   Private Data
    //-----------------------------------------------------
    static bool             m_firstConstructed;
    int                     m_tag;
    ProcId                  m_pid;  // The local ProcId
    int                     m_pRank; // The local Rank in this communicator
    vector<ProcId>          m_commProcsLongList;
    vector<ProcId>          m_commProcsShortList;
    CommType                m_comm;

    //   Private Methods
    //-------------------------------------
    void constructCommon(const CommType* parentComm,
			 const vector<ProcId>& parentCommProcs);

    // methods declared private to prevent their use
    //    Default Constructor, Assignment Operator, Copy Constructor
    CommScope(void);
    CommScope& operator=(const CommScope& commScope);
    CommScope(const CommScope& other);
  }; // end CommScope class


/**
 *                  rankToProcId()
 * \brief converts a rank in thise CommScope to its ProcId
 * \param int rank (starting at 0) of the process whose ProcId is desired
 * \return ProcId
 */

  inline
    ProcId CommScope::rankToProcId(int rank) const
      {
	return(m_commProcsLongList[rank]);
      }

//  +++++++++++++++++++++++++++++++++++++++++++++++++++++*
//                   comm()
//  +++++++++++++++++++++++++++++++++++++++++++++++++++++*
  inline
    const CommScope::CommType& CommScope::comm() const
      { return(m_comm); }
  
//  +++++++++++++++++++++++++++++++++++++++++++++++++++++*
//                   getNumProcs()
//  +++++++++++++++++++++++++++++++++++++++++++++++++++++*
  inline
    int CommScope::getNumProcs() const
      { return(m_commProcsShortList.size()); }

//  +++++++++++++++++++++++++++++++++++++++++++++++++++++*
//                   getProcId()
//  +++++++++++++++++++++++++++++++++++++++++++++++++++++*
  inline
    int CommScope::getProcId() const
      { return(m_pid); }

//  +++++++++++++++++++++++++++++++++++++++++++++++++++++*
//                   barSynch()
//  +++++++++++++++++++++++++++++++++++++++++++++++++++++*
  inline
    void CommScope::barSynch()
      {
#ifdef _STANDARD_MPI
        MPI_Barrier(m_comm);
#else
        cerr << "CommScope barSynch() not implimented if Not MPI" << endl;
#endif // _STANDARD_MPI
        return;
      }


}// end namespace ipvtol

#endif // PVTOL_COMMSCOPE_H not defined

