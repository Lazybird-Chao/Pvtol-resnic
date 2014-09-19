/**
 *    File: Conduit.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *    March 2008: Ported from PVL by Jim Daly
 *
 *  \author  $LastChangedBy: jav5880 $
 *  \date    $LastChangedDate: 2009-02-24 16:28:21 -0500 (Tue, 24 Feb 2009) $
 *  \version $LastChangedRevision: 945 $
 *  \brief   Class definition  & inline methods of Conduit, ConduitInsertIf,
 *            ConduitExtractIf & ConduitInitComm
 *            The Conduit class provides a means to transport distributed
 *            data objects between two arbitrary task scopes and mappings.
 *
 *            The ConduitInsertIf class provides a constrained API for the
 *            source end of a conduit.
 *            The ConduitExtractIf class provides a constrained API for the
 *            destination end of a conduit.
 *
 *  $Id: Conduit.h 945 2009-02-24 21:28:21Z jav5880 $
 *
 * @author Glenn Schrader
 *
 */

#ifndef PVTOL_CONDUIT_H
#define PVTOL_CONDUIT_H

#include <Route.h>
#include <Transfer.h>
#include <SendRequest.h>
#include <ConduitHelperFuncs.h>
#include <PvtolProgram.h>
#include <LocalMap.h>
#include <DataMap.h>
#include <CdtLocalXferData.h>
#include <RingIndex.h>
#include <NTuple.h>

#include <string>
#include <iostream>
#include <vector>
#include <typeinfo>
#include <map>
#include <pthread.h>
#include <shared_ptr.hpp>

namespace ipvtol
{
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::map;
using boost::shared_ptr;


const Route::Flags TRANSPOSE_102 = Route::TRANSPOSE_102;

/* forward declarations needed for the Conduit class */
template <class DATATYPE, class TAGTYPE, bool USE_EOC> class ConduitInsertIf;
template <class DATATYPE, class TAGTYPE, bool USE_EOC> class ConduitExtractIf;

template <class DATATYPE, class TAGTYPE, bool USE_EOC> class DataTagSender;
template <class DATATYPE, class TAGTYPE, bool USE_EOC> class EocSender;
template <class DATATYPE, class TAGTYPE, bool USE_EOC> class EndpointDescript;
template <class DATATYPE, class TAGTYPE, bool USE_EOC> class Endpoint;
template <class DATATYPE, class TAGTYPE, bool USE_EOC> class Connection;
template <class TAGTYPE, bool USE_EOC> class TagSeqNumWrapper;
template <class SENDERTYPE> class SendManager;

class NullConduitTag {  };

// Forward declarations required for Conduit.cc and
//    other Conduit.cc related files

static void findInitialBufferIndices(int srcReplicaRank,
                                     int srcNumReplicas,
                                     int dstReplicaRank,
                                     int dstNumReplicas,
                                     int * srcBufferIndex,
                                     int * dstBufferIndex);

static void doubleModularInverse(int a, int b, int * pInvA, int * pInvB);

static void extendedEuclid(int a, int b, int * pA, int * pB, int * pGCD);

// static bool isDirectDescendant(ProcId procId, TaskBase tb);

//           ==========================================
//                       Conduit Class
//           ==========================================
/**
 * The Conduit class provides a means of transporting the contents
 * of distributed data objects between tasks. The tasks may be
 * co-mapped, partially co-mapped, or completely disjoint.
 *
 * The parameter type of a conduit will normally be a distributed
 * vector or matrix type such as Vector<float> or
 * Matrix<Complex<float>>.
 *
 * @see HierArray
 * @see Vector
 * @see Matrix
 *
 * */
template <class DATATYPE, class TAGTYPE = NullConduitTag, bool USE_EOC = true>
class Conduit {
  public:
    typedef ConduitInsertIf<DATATYPE, TAGTYPE, USE_EOC> InsertIf;
    typedef ConduitExtractIf<DATATYPE, TAGTYPE, USE_EOC> ExtractIf;

    typedef DATATYPE DataType;
    typedef Endpoint<DATATYPE, TAGTYPE, USE_EOC> EP;
    typedef typename Endpoint<DATATYPE, TAGTYPE, USE_EOC>::EndpointType EPtype;

    typedef enum
    {
        Broadcast,
        RoundRobin
    } DistributionType;

    static const int PREPOST_FOREVER = -1;

    // CONSTRUCTORS

    /**
     * @param name the name of the conduit
     * @param distType the type of distribution used transmitting to RepTasks
     *
     */
    Conduit(const string& name,
            Route::Flags flags = Route::DEFAULT_FLAG,
            DistributionType distType = RoundRobin);

    ~Conduit() throw();

    InsertIf  getInsertIf();
    ExtractIf getExtractIf();

    //     class ConduitInitComm
    //=================================
    class ConduitInitComm {
       public:
         ConduitInitComm(const string& name, int maxSize);
         ~ConduitInitComm();

         void exchangeEndpoints(map<NTuple, void* >&  thdInfo,
	                        const string &name);

         void exchangeEndpoints(vector< EP > & endpoints,
                                bool & endpointsInitialized,
                                const string & name,
	                        EPtype endpointType);

       private:
         int * beginLocal();
         int   size();

         const string&       m_name;
         int                 m_transferSize;
         bool                m_masterIsLocal;
         int                *m_initCommBuff;
         vector< Transfer* > m_toMaster;
         vector< Transfer* > m_fromMaster;
    };//end class ConduitInitComm

    typedef Endpoint<DATATYPE, TAGTYPE, USE_EOC> EndpointX;

    //     class ConduitThreadInfo
    //=================================
    class ConduitThreadInfo {
       public:
	 ConduitThreadInfo();

         EndpointX             m_src;
         bool                 m_srcInitialized;
         bool                 m_srcSetup;
         EndpointX             m_dst;
         bool                 m_dstInitialized;
         bool                 m_dstSetup;
    };//end class ConduitThreadInfo

    void *getThreadInfo();

    void setupSrc(const DataMap & srcMap,
                  int depth, const unsigned int lengths[]);
    void setupSrc(DATATYPE & srcObj, int depth);

    void setSlotSize(vector<EndpointX>& epv, int size);

    inline bool insertAvailable();
    inline bool insertEOCAvailable();
    inline DATATYPE & getInsertHandle();
    inline TAGTYPE & getInsertTagHandle();
    void insert();
    inline void insertEOC();

    void setupDest(const DataMap & destMap, int depth,
                   const unsigned int lengths[]);
    void setupDest(DATATYPE & destObj, int depth);
    inline void extractAtLeast(int count = 0);
    inline bool extractReady();
    bool isAtEOC();
    inline void clearEOC();
    DATATYPE & getExtractHandle();
    inline TAGTYPE & getExtractTagHandle();
    inline void release();
    void waitForLocalData();
    void localRelease();
    void insertLocalData();
    void waitForInsert();
    void waitForInsertBuff();
    void insertLocalEOC();

    void setupComplete(ConduitInitComm & initComm, bool useTag);
    void setupComplete();

    friend class ConduitInsertIf<DATATYPE, TAGTYPE, USE_EOC>;
    friend class ConduitExtractIf<DATATYPE, TAGTYPE, USE_EOC>;

  private:
    void validateEndpoints();

    string               m_name;
    bool                 m_useTag;
    int                  m_tag;
    DistributionType     m_distType;
    bool                 m_srcIsReplicated;
    bool                 m_destIsReplicated;
    bool                 m_setupCompleteDone;
    unsigned int         m_numCommGroups;
    bool                 m_doLocalXfer;
    unsigned int         m_srcsPerGroup;
    unsigned int         m_dstsPerGroup;
    Route::Flags         m_transposeFlag;

    int                  m_clxdKey;
    CdtLocalXferData    *m_clxdEntry;
    RingIndex            m_localXferBufferHeadIndex;
    RingIndex            m_localXferBufferTailIndex;
    int                  m_depth;//used only in local xfers

//   per thread info is accessed 1st by task ID then by thread rank
    pthread_mutex_t      m_thdInfoMutex;
    // map<TaskId, map<int, void* > > m_thdInfo;
    map<NTuple, void* > m_thdInfo;
};//end Conduit class

/**
 * The ConduitInsertIf class provides a constrained API for the
 * source end of a conduit.
 *
 * The parameter type of a ConduitInsertIf must be the same as the
 * parameter type of the Conduit class which it is used with (i.e. a
 * vector or matrix type such as Vector<float> or
 * Matrix<Complex<float>>).
 *
 * @see Conduit
 *
 * @author Glenn Schrader
 *
 * */

template <class DATATYPE, class TAGTYPE = NullConduitTag, bool USE_EOC = true>
class ConduitInsertIf {

    public:

#ifdef NOT_YET
    typedef Block<typename DATATYPE::ElType> BlockType;
#endif // NOT_YET

    // INITIALIZATION METHODS

    ConduitInsertIf();

    /**
     * @param cdt the conduit for which this ConduitInsertIf is an interface
     *
     */
    ConduitInsertIf(Conduit<DATATYPE, TAGTYPE, USE_EOC>& cdt);

    inline ConduitInsertIf& operator=(ConduitInsertIf& sif);

    // CONDUIT METHODS
    /**
     * Note that a default map which has only the local node is used
     *
     * @param depth the multi-buffering depth for the source end of
     * the conduit
     *
     * @param lengths length of each dimension of the source object */
    void
    setup(int depth,                     // multi-buffering depth
          const unsigned int lengths[]); // length for each dimension

#ifdef NOT_YET
    void
    setup(const BlockType& block,
          int depth,                     // multi-buffering depth
          const unsigned int lengths[]); // length for each dimension
#endif // NOT_YET

    /**
     * @param srcMap the map which defines the distribution of the source
     * end of the conduit.
     *
     * @param depth the multi-buffering depth for the source end of
     * the conduit
     *
     * @param lengths length of each dimension of the source object */
    void
    setup(const DataMap& srcMap,         // source endpoint object
          int depth,                     // multi-buffering depth
          const unsigned int lengths[]); // length for each dimension

#ifdef NOT_YET
    void
    setup(const DataMap& srcMap,         // source endpoint object
          const BlockType& block,
          int depth,                     // multi-buffering depth
          const unsigned int lengths[]); // length for each dimension
#endif // NOT_YET

    /**
     * @param srcObj the object which describes the storage used by
     * the source end of the conduit.
     *
     * @param depth the multi-buffering depth for the source end of
     * the conduit. */
    void
    setup(DATATYPE& srcObj,    // user supplied endpoint object
          int depth=1);        // multi-buffering depth

    /**
     * Queries whether the conduit is able to provide the buffer space
     * needed for an outgoing transfer.
     *
     * @return returns true when the source end distributed data object
     *         has been set up in a free portion of its underlying storage.

     * */
    inline bool available();

    /**
     * Queries whether the conduit is able to provide the buffer space
     * needed for an outgoing EOC transfer.
     *
     * @return returns true when the source end has space to buffer
     *          an outgoing EOC.
     * */
    inline bool eocAvailable();

    /**
     * Gives access to the data object which provides a distributed
     * view of the next available buffer space. Note that getHandle()
     * does not modify the contents of the returned data object
     * therefore there is no guarantee that the initial contents of
     * newly acquired buffer space has any particular value.
     *
     * @return a reference to the conduit distributed data object.
     *
     * */
    inline DATATYPE& getHandle();

    /**
     * Gives access to the Tag data object. Note that getTagHandle()
     * does not modify the contents of the returned data object
     * therefore there is no guarantee that the initial contents of
     * newly acquired buffer space has any particular value.
     *
     * @return a reference to the conduit's Tag data object.
     *
     * */
    inline TAGTYPE& getTagHandle();

    /**
     * Sends the current source buffer (the view of which was
     * returned by getHandle()) to the current destination buffer. The
     * buffer space which is being sent will be unavailable until the
     * source buffer is transferred to the destination buffer. Note
     * that this method kicks off any communications which are needed
     * to send the data to the destination.
     *
     * */
    inline void insert();

    /**
     * Sends an end-of-cycle (EOC).
     *
     * */
    inline void insertEOC();


    private:

    Conduit<DATATYPE, TAGTYPE, USE_EOC> * m_conduit;
}; //end ConduitInsertIf



/**
 * The ConduitExtractIf class provides a constrained API for the
 * destination end of a conduit.
 *
 * The parameter type of a ConduitExtractIf must be the same as the
 * parameter type of the Conduit class which it is used with (i.e. a
 * vector or matrix type such as Vector<float> or
 * Matrix<Complex<float>>).
 *
 * @see Conduit
 *
 * @author Glenn Schrader
 *
 * */

template <class DATATYPE, class TAGTYPE = NullConduitTag, bool USE_EOC = true>
class ConduitExtractIf {
    public:
#ifdef NOT_YET
    typedef Block<typename DATATYPE::ElType> BlockType;
#endif // NOT_YET

    // CONSTRUCTORS

    ConduitExtractIf();

    /**
     * @param cdt the conduit for which this ConduitExtractIf is an interface
     *
     */
    ConduitExtractIf(Conduit<DATATYPE, TAGTYPE, USE_EOC>& cdt);

    // ASSIGNMENT OPERATOR

    inline ConduitExtractIf& operator=(ConduitExtractIf& dif);

    // CONDUIT ACCESSORS

    // Setup methods
    /**
     * Note that a default map which has only the local node is used
     * and all of the conduit's modes are set up in an idential
     * manner.
     *
     * @param depth the multi-buffering depth for the source end of
     * the conduit
     *
     * @param lengths length of each dimension of the source object */
    void
    setup(int depth,                     // multi-buffering depth
          const unsigned int lengths[]); // length for each dimension

#ifdef NOT_YET
    void
    setup(const BlockType& block,
          int depth,                     // multi-buffering depth
          const unsigned int lengths[]); // length for each dimension
#endif // NOT_YET

    /**
     * Note that this method sets up all of the conduit's modes in an
     * idential manner.
     *
     * @param srcMap the map which defines the distribution of the source
     * end of the conduit.
     *
     * @param depth the multi-buffering depth for the source end of
     * the conduit
     *
     * @param lengths length of each dimension of the source object */
    void
    setup(const DataMap& destMap,        // destination endpoint object
          int depth,                     // multi-buffering depth
          const unsigned int lengths[]); // length for each dimension

#ifdef NOT_YET
    void
    setup(const DataMap& destMap,        // destination endpoint object
          const BlockType& block,
          int depth,                     // multi-buffering depth
          const unsigned int lengths[]); // length for each dimension
#endif // NOT_YET

    void
    setup(DATATYPE& dstObj,
          int depth = 1);   // user supplied endpoint object

    inline void atLeast(int count = 0);

    /**
     * Queries whether the conduit has received new data.
     *
     * @return Returns true if the next input buffer has data
     * which has not yet been extracted.
     */

    inline bool ready();

    /**
     *      isAtEOC()
     * Queries whether the conduit has received an EOC signal.
     *
     * @return true if an End-Of-Cycle was received.
     *    The EOC is considered received ONLY if it followed
     *    a data transfer which has already been extracted.
     */
    inline bool isAtEOC();

    /**
     *      clearEOC()
     * Clears any pending end-of-cycle (EOC) signal.
     *
     * @return void
     */
    inline void clearEOC();

    /**
     * Gives access to the data object which provides a distributed
     * view of the next available destination buffer space. Once
     * extractReady() has returned true, the contents of the
     * destination distributed data object is valid.
     *
     * This is a Blocking call and will not return until data is availble
     *
     * @return a reference to the conduit's distributed data object.  */
    inline DATATYPE& getHandle();

    /**
     * Gives access to the Tag data object Once extractReady() has
     * returned true, the contents of the tag data is valid and repeated calls
     * to getTagHandle() will return a view of the same buffer space.
     *
     * @return a reference to the conduit's Tag object.  */
    inline TAGTYPE& getTagHandle();

    /**
     * Releases the buffer space so that it can be re-used by another
     * transfer.
     */
    inline void release();

    private:
      Conduit<DATATYPE, TAGTYPE, USE_EOC> * m_conduit;
};// end ConduitExtractIf


/////////////////////////////////////////////////////////////////////////////
// struct ItemTransferInfo
/////////////////////////////////////////////////////////////////////////////

// This structure is used for bookkeeping for both tags and EOCs
struct ItemTransferInfo
{
    int    m_index;
    ProcId m_fromNode;
    ProcId m_toNode;
    shared_ptr< SendRequest > m_sendRequest;
};

//                  I N L I N E        Methods
//--------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// ConduitInsertIf methods
/////////////////////////////////////////////////////////////////////////////
template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline
ConduitInsertIf<DATATYPE, TAGTYPE, USE_EOC>::ConduitInsertIf()
            : m_conduit(0)
{
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline
ConduitInsertIf<DATATYPE, TAGTYPE, USE_EOC>::ConduitInsertIf(
                                    Conduit<DATATYPE, TAGTYPE, USE_EOC>& cdt)
            : m_conduit(&cdt)
{
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline
ConduitInsertIf<DATATYPE, TAGTYPE, USE_EOC> &
ConduitInsertIf<DATATYPE, TAGTYPE, USE_EOC>::operator=(ConduitInsertIf& sif)
{
    m_conduit = sif.m_conduit;
    return sif;
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline
void ConduitInsertIf<DATATYPE, TAGTYPE, USE_EOC>::setup(
                                                int depth,
                                                const unsigned int lengths[])
{
    LocalMap lm;
    setup(lm, depth, lengths);
}

#ifdef NOT_YET
template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline
void ConduitInsertIf<DATATYPE, TAGTYPE, USE_EOC>::setup(
                                                const BlockType& block,
                                                int depth,
                                                const unsigned int lengths[])
{
    LocalMap lm;
    setup(lm, block, depth, lengths);
}
#endif // NOT_YET


template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline
void ConduitInsertIf<DATATYPE, TAGTYPE, USE_EOC>::setup(
    const DataMap & srcMap,
    int depth,
    const unsigned int lengths[])
{
    m_conduit->setupSrc(srcMap, depth, lengths);
}

#ifdef NOT_YET
template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline
void ConduitInsertIf<DATATYPE, TAGTYPE, USE_EOC>::setup(
    const DataMap & srcMap,
    const BlockType & block,
    int depth,
    const unsigned int lengths[])
{
    m_conduit->setupSrc(srcMap, block, depth, lengths);
}
#endif // NOT_YET

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline
void ConduitInsertIf<DATATYPE, TAGTYPE, USE_EOC>::setup(
    DATATYPE & srcObj,
    int depth)
{
    m_conduit->setupSrc(srcObj, depth);
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline bool ConduitInsertIf<DATATYPE, TAGTYPE, USE_EOC>::available()
{
    return m_conduit->insertAvailable();
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline bool ConduitInsertIf<DATATYPE, TAGTYPE, USE_EOC>::eocAvailable()
{
    return m_conduit->insertEOCAvailable();
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline DATATYPE & ConduitInsertIf<DATATYPE, TAGTYPE, USE_EOC>::getHandle()
{
    return m_conduit->getInsertHandle();
}


template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline TAGTYPE& ConduitInsertIf<DATATYPE, TAGTYPE, USE_EOC>::getTagHandle()
{
    return m_conduit->getInsertTagHandle();
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void ConduitInsertIf<DATATYPE, TAGTYPE, USE_EOC>::insert()
{
    m_conduit->insert();
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void ConduitInsertIf<DATATYPE, TAGTYPE, USE_EOC>::insertEOC()
{
    m_conduit->insertEOC();
}


/////////////////////////////////////////////////////////////////////////////
// ConduitExtractIf methods
/////////////////////////////////////////////////////////////////////////////

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline
ConduitExtractIf<DATATYPE, TAGTYPE, USE_EOC>::ConduitExtractIf()
            : m_conduit(0)
{
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline
ConduitExtractIf<DATATYPE, TAGTYPE, USE_EOC>::ConduitExtractIf(
                                    Conduit<DATATYPE, TAGTYPE, USE_EOC>& cdt)
            : m_conduit(&cdt)
{
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline ConduitExtractIf<DATATYPE, TAGTYPE, USE_EOC> &
ConduitExtractIf<DATATYPE, TAGTYPE, USE_EOC>::operator=(ConduitExtractIf& dif)
{
    m_conduit = dif.m_conduit;
    return dif;
}


template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline
void ConduitExtractIf<DATATYPE, TAGTYPE, USE_EOC>::setup(
                                                int depth,
                                                const unsigned int lengths[])
{
    LocalMap lm;
    setup(lm, depth, lengths);
}

#ifdef NOT_YET
template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline
void ConduitExtractIf<DATATYPE, TAGTYPE, USE_EOC>::setup(
                                                const BlockType& block,
                                                int depth,
                                                const unsigned int lengths[])
{
    LocalMap lm;
    setup(lm, block, depth, lengths);
}
#endif // NOT_YET



template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline
void ConduitExtractIf<DATATYPE, TAGTYPE, USE_EOC>::setup(
                                            const DataMap & destMap,
                                            int depth,
                                            const unsigned int lengths[])
{
  m_conduit->setupDest(destMap, depth, lengths);
}

#ifdef NOT_YET
template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline
void ConduitExtractIf<DATATYPE, TAGTYPE, USE_EOC>::setup(
                                            const DataMap & destMap,
                                            const BlockType & block,
                                            int depth,
                                            const unsigned int lengths[])
{
  m_conduit->setupDest(destMap, block, depth, lengths);
}
#endif // NOT_YET


template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void ConduitExtractIf<DATATYPE, TAGTYPE, USE_EOC>::atLeast(int count)
{
    m_conduit->extractAtLeast(count);
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline bool ConduitExtractIf<DATATYPE, TAGTYPE, USE_EOC>::ready()
{
    return m_conduit->extractReady();
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline bool ConduitExtractIf<DATATYPE, TAGTYPE, USE_EOC>::isAtEOC()
{
    return m_conduit->isAtEOC();
}


template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void ConduitExtractIf<DATATYPE, TAGTYPE, USE_EOC>::clearEOC()
{
    m_conduit->clearEOC();
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline DATATYPE & ConduitExtractIf<DATATYPE, TAGTYPE, USE_EOC>::getHandle()
{
    return m_conduit->getExtractHandle();
}


template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline TAGTYPE & ConduitExtractIf<DATATYPE, TAGTYPE, USE_EOC>::getTagHandle()
{
    return m_conduit->getExtractTagHandle();
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void ConduitExtractIf<DATATYPE, TAGTYPE, USE_EOC>::release()
{
    m_conduit->release();
}


/////////////////////////////////////////////////////////////////////////////
// Conduit methods
/////////////////////////////////////////////////////////////////////////////
template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline
ConduitInsertIf<DATATYPE, TAGTYPE, USE_EOC>
Conduit<DATATYPE, TAGTYPE, USE_EOC>::getInsertIf()
{
    ConduitInsertIf<DATATYPE, TAGTYPE, USE_EOC> iff(*this);
    return iff;
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline
ConduitExtractIf<DATATYPE, TAGTYPE, USE_EOC>
Conduit<DATATYPE, TAGTYPE, USE_EOC>::getExtractIf()
{
  ConduitExtractIf<DATATYPE, TAGTYPE, USE_EOC> eif( *this );
  return eif;
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline
void
Conduit<DATATYPE, TAGTYPE, USE_EOC>::setSlotSize( vector<EndpointX>& epv,
                                                        int size)
{
    for ( int i = 0; i < epv.size(); i++ )
    {
        epv[i].setSlotSize(size);
    }
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline bool Conduit<DATATYPE, TAGTYPE, USE_EOC>::insertAvailable()
{
    ConduitThreadInfo *myinfo = (ConduitThreadInfo *)getThreadInfo();

    return myinfo->m_src.insertAvailable();
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline bool Conduit<DATATYPE, TAGTYPE, USE_EOC>::insertEOCAvailable()
{
    ConduitThreadInfo *myinfo = getThreadInfo();

    return myinfo->m_src.insertEOCAvailable();
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline DATATYPE & Conduit<DATATYPE, TAGTYPE, USE_EOC>::getInsertHandle()
{
    ConduitThreadInfo *myinfo = (ConduitThreadInfo *)getThreadInfo();

#ifdef PVTOL_DEBUG
    cout << "Cdt:" << m_name << " getInsertHandle() Tail="
         << myinfo->m_dst.getTailBuffIdx()
	 << " Head=" << m_dst.getHeadBuffIdx()
	 << " DataAdd=" << m_dst.getDataAddress()
	 << endl;
#endif // PVTOL_DEBUG

    if ((m_doLocalXfer && m_clxdEntry->noBuffAvailable)
                            ||
        (!m_doLocalXfer && myinfo->m_src.noBuffAvailable()))
                  waitForInsertBuff();

    return *(myinfo->m_src.getDataAddress());
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline TAGTYPE & Conduit<DATATYPE, TAGTYPE, USE_EOC>::getInsertTagHandle()
{
    ConduitThreadInfo *myinfo = (ConduitThreadInfo *)getThreadInfo();

    return myinfo->m_src.getInsertTagHandle();
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void Conduit<DATATYPE, TAGTYPE, USE_EOC>::insertEOC()
{
    ConduitThreadInfo *myinfo = (ConduitThreadInfo *)getThreadInfo();

    myinfo->m_src.insertEOC();
    if (m_doLocalXfer)
               insertLocalEOC();
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void Conduit<DATATYPE, TAGTYPE, USE_EOC>::extractAtLeast(int count)
{
    ConduitThreadInfo *myinfo = getThreadInfo();

    myinfo->m_dst.extractAtLeast(count, m_setupCompleteDone);
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline bool Conduit<DATATYPE, TAGTYPE, USE_EOC>::extractReady()
{
    ConduitThreadInfo *myinfo = (ConduitThreadInfo *)getThreadInfo();

    return myinfo->m_dst.extractReady();
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void Conduit<DATATYPE, TAGTYPE, USE_EOC>::clearEOC()
{
    ConduitThreadInfo *myinfo = (ConduitThreadInfo *)getThreadInfo();
    myinfo->m_dst.clearEOC();
    if (m_doLocalXfer)
            --(m_clxdEntry->eocPosted);
    return;
}


template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void Conduit<DATATYPE, TAGTYPE, USE_EOC>::release()
{
    ConduitThreadInfo *myinfo = (ConduitThreadInfo *)getThreadInfo();
    myinfo->m_dst.release();
    if (m_doLocalXfer)
                 localRelease();
}



/////////////////////////////////////////////////////////////////////////////
// ConduitInitComm methods
/////////////////////////////////////////////////////////////////////////////
template< class DATATYPE, class TAGTYPE, bool USE_EOC>
inline
int * Conduit<DATATYPE, TAGTYPE, USE_EOC>::ConduitInitComm::beginLocal()
{
    return(m_initCommBuff);
}

template< class DATATYPE, class TAGTYPE, bool USE_EOC>
inline
int Conduit<DATATYPE, TAGTYPE, USE_EOC>::ConduitInitComm::size()
{
    return(m_transferSize);
}


}//end namespace

#include <Conduit.inl>

#endif // PVTOL_CONDUIT_H

