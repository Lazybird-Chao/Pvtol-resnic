/**
 *    File: HeterogeneousConduit.h
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *s
 *  \author  $LastChangedBy: $
 *  \date    $LastChangedDate: $
 *  \version $LastChangedRevision: $
 *  \brief   Class definition & inline methods of HeterogeneousConduit. The Heterogeneous
 *           Conduit class provides a means to transport data objects between two tasks
 *           where at least one task is located on a Heterogeneous-enabled GPU.
 *
 *  Author: James Brock
 *  	$Id: $
 *
 */

#ifndef HETEROGENEOUSCONDUIT_H_
#define HETEROGENEOUSCONDUIT_H_

#include <PvtolProgram.h>
#include <HeterogeneousQueue.h>
#include <ThreadManager.h>
#include <string>
#include <typeinfo>
#include <vector>
#include <queue>
#include <hUtil.h>

namespace ipvtol {

using std::string;
using std::vector;

//           ==========================================
//                   HeterogeneousConduit Class
//           ==========================================

template <class DATATYPE> class HeterogeneousConduit {
public:
	/////////////////////////////////////////////////////////////////////////
	// PUBLIC TYPES
	/////////////////////////////////////////////////////////////////////////
	//typedef DATATYPE* handle;
	
	typedef DATATYPE DataType;
	typedef typename HeterogeneousQueue<DataType>::DataTypePtr handle;

	/////////////////////////////////////////////////////////////////////////
	// INSERT INTERFACE CLASSES
	/////////////////////////////////////////////////////////////////////////
	class ConduitInsertIf {
	public:
		// INITIALIZATION METHODS
		ConduitInsertIf();

		/**
	 	* @param cdt the conduit for which this ConduitInsertIf is an interface
	 	*
	 	*/
		ConduitInsertIf(HeterogeneousConduit<DATATYPE> &cdt);

		//Destructor
		~ConduitInsertIf();

		// = Operator
 		inline ConduitInsertIf& operator=(ConduitInsertIf& sif);

	   // CONDUIT INSERT INTERFACE METHODS
	   /**
		*
	 	* @param depth the multi-buffering depth for the source end of
	 	* the conduit
	 	*/
		void setup(int depth=1, HTaskInfo * info=NULL, int * dims=NULL); // multi-buffering depth
		
	   /**
	 	* @param vecHandles Vector containing Handles pointing to allocated memory. 
	 	*
	 	*/
		void setup(vector<handle>& vecHandles, HTaskInfo * info=NULL, int * dims=NULL); // user supplied endpoint object

	   /**
	 	* Queries whether the conduit is able to provide the buffer space
	 	* needed for an outgoing transfer. 
	 	*
	 	* @return returns true if space is available 
	 	*/
		inline bool available();

	   /**
     	*
	 	* @return a reference to the conduit data object.
	 	*
	 	*/
		inline DATATYPE& getData();

	   /**
	 	* Puts back the data object in the conduit. 
	 	*
	 	* @return void
	 	*
	 	*/
		inline void releaseData();
		
		//Insert the EOC
		inline void insertEOC();

		//Clear the EOC
		inline void clearEOC();
		
		// Lock the conduit
		inline void lock();
		
		// Unlock the conduit
		inline void unlock();

		// PRIVATE VARIABLES
	private:
		HeterogeneousConduit<DATATYPE> * m_conduit; // Pointer to parent conduit
	};
	
	/////////////////////////////////////////////////////////////////////////
	// EXTRACT INTERFACE CLASSES
	/////////////////////////////////////////////////////////////////////////
	class ConduitExtractIf {
	public:
		// CONSTRUCTORS
		ConduitExtractIf();

		/**
	 	* @param cdt the conduit for which this ConduitExtractIf is an interface
	 	*
	 	*/
		ConduitExtractIf(HeterogeneousConduit<DATATYPE> &cdt);

		//Desructor
		~ConduitExtractIf();

		// ASSIGNMENT OPERATOR
		inline ConduitExtractIf& operator=(ConduitExtractIf& dif);

	   // CONDUIT EXTRACT INTERFACE METHODS
	   /**
		*
	 	* @param depth the multi-buffering depth for the destination end of
	 	* the conduit
	 	*/
		void setup(int depth=1, HTaskInfo * info=NULL, int * dims=NULL); // multi-buffering depth
		
	   /**
	 	* @param vecHandles Vector containing Handles pointing to allocated memory. 
	 	*
	 	*/
		void setup(vector<handle>& vecHandles, HTaskInfo * info, int * dims); // user supplied endpoint object

		/**
		 * Queries whether the conduit has unread  data.
		 *
		 * @return Returns true if the next input buffer has data
		 * which has not yet been extracted.
		 */

		inline bool ready();

	   /**
     	*
	 	* @return a reference to the conduit data object.
	 	*
	 	*/
		inline DATATYPE& getData();

	   /**
	 	* Puts back the data object in the conduit. 
	 	*
	 	* @return void
	 	*
	 	*/
		inline void releaseData();
		
		//IsAtEOC
		inline bool isAtEOC();
		
		// Lock the conduit
		inline void lock();
		
		// Unlock the conduit
		inline void unlock();


		// PRIVATE VARIABLES
	private:
		HeterogeneousConduit<DATATYPE> * m_conduit; // Pointer to parent conduit
   };

	///////////////////////////////////////////////////////////////////////// 
	// Friend classes
	/////////////////////////////////////////////////////////////////////////
	friend class ConduitInsertIf;
	friend class ConduitExtractIf;

	/////////////////////////////////////////////////////////////////////////
	// PUBLIC TYPES FOR USING NESTED CLASSES
	/////////////////////////////////////////////////////////////////////////
	typedef ConduitInsertIf Writer;
	typedef ConduitExtractIf Reader;

	/////////////////////////////////////////////////////////////////////////
	// PUBLIC METHODS
	/////////////////////////////////////////////////////////////////////////

	// CONSTRUCTORS
	/**
	 * \brief    Constructor
	 * \param    name   name of the conduit
	 *
	 */
	HeterogeneousConduit(const string& name = "", bool shared = false);

	///Destructor
	virtual ~HeterogeneousConduit() throw();

	///Get the Writer interface
	Writer getWriter();

	///Get the Reader interface
	Reader getReader();
	
	///Get the name
	string getName();

	///Setup the source buffers using new DATATYPE
	void setupSrc(int depth = 1, HTaskInfo * info = NULL, int * dims = NULL);
	
	///Use user-supplied source  buffers
	void setupSrc(std::vector<handle> &vecHandles, HTaskInfo * info = NULL, int * dims = NULL);

	///Clear the Src buffers
	void clearSrc();

	///Setup the destination buffers (noop)
	void setupDst(int depth = 1, HTaskInfo * info = NULL, int * dims = NULL);

	///Setup the destination buffers (noop)
	void setupDst(vector<handle> &vecHandles, HTaskInfo * info = NULL, int * dims = NULL);
	
	///Clear the destination buffers (noop)
	void clearDst();

	///Can we write to the conduit
	inline bool insertAvailable();

	///Can we read from conduit?
	inline bool extractReady();

	//Finish setting up the conduit (noop)
	void setupComplete();

	// Sends an end-of-cycle (EOC).
	inline void insertEOC();

	// Clears any end-of-cycle (EOC) signal.
	inline void clearEOC();

	// Queries whether the conduit has received an EOC signal.
	inline bool isAtEOC();
	
	// Locks the conduit so that no data transfers occur
	inline void lock();
	
	// Unlocks the conduit so that data transfers will resume
	inline void unlock();

	/////////////////////////////////////////////////////////////////////////
	// PRIVATE METHODS
	/////////////////////////////////////////////////////////////////////////
private:
	///Get the src data for buffer to write
	inline DATATYPE & getSrcData();

	///Release the source data
	inline void releaseSrcData();
	
	//Get the destination data for read
	inline DATATYPE & getDstData();

	//Release the data 
	inline void releaseDstData();
	
	/////////////////////////////////////////////////////////////////////////
	// PRIVATE MEMBERS
	/////////////////////////////////////////////////////////////////////////
	
private:
	string m_name;
	int m_bufferLength;
	HeterogeneousQueue<DATATYPE> m_src;
	HeterogeneousQueue<DATATYPE> m_dst;
	handle m_readData;
	handle m_writeData;
	bool m_setupCompleteDone;
	bool m_srcsInitialized;
	bool m_dstsInitialized;
   pthread_mutex_t m_initSrcMutex;
   pthread_mutex_t m_initDstMutex;
	bool m_EOC;
	HTaskInfo m_srcInfo;
	HTaskInfo m_dstInfo;
	int m_srcDims[HNDIMS];
	int m_dstDims[HNDIMS];
	int m_srcStride;
	int m_dstStride;
	bool m_cpyLock;
	bool m_shared;
};

////////////////////////////////////////////////////////////////////
//               I N L I N E        M E T H O D S
////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ConduitInsertIf (Writer) methods
/////////////////////////////////////////////////////////////////////////////
template <class DATATYPE> inline HeterogeneousConduit<DATATYPE>::ConduitInsertIf::ConduitInsertIf() : m_conduit(NULL) { }
template <class DATATYPE> inline HeterogeneousConduit<DATATYPE>::ConduitInsertIf::ConduitInsertIf(HeterogeneousConduit<DATATYPE> & cdt) : m_conduit(&cdt) { }
template <class DATATYPE> inline HeterogeneousConduit<DATATYPE>::ConduitInsertIf::~ConduitInsertIf() {
   try { m_conduit->clearSrc(); m_conduit->clearDst(); } catch(...) {}
}

template <class DATATYPE> inline typename HeterogeneousConduit<DATATYPE>::ConduitInsertIf&
	HeterogeneousConduit<DATATYPE>::ConduitInsertIf::operator=(HeterogeneousConduit<DATATYPE>::ConduitInsertIf & sif) {
	m_conduit = sif.m_conduit; return sif;
}

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::ConduitInsertIf::setup(std::vector<handle> & vecHandles,
      HTaskInfo * info, int * dims) {
	m_conduit->setupSrc(vecHandles, info, dims);
}

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::ConduitInsertIf::setup(int depth, HTaskInfo * info, int * dims) {
	m_conduit->setupSrc(depth, info, dims);
}

template <class DATATYPE>
inline bool HeterogeneousConduit<DATATYPE>::ConduitInsertIf::available() {
	return m_conduit->insertAvailable();
}

template <class DATATYPE>
inline DATATYPE & HeterogeneousConduit<DATATYPE>::ConduitInsertIf::getData() {
	return m_conduit->getSrcData();
}

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::ConduitInsertIf::releaseData() {
	m_conduit->releaseSrcData();
}

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::ConduitInsertIf::insertEOC() {
	m_conduit->insertEOC();
}

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::ConduitInsertIf::clearEOC() {
	m_conduit->clearEOC();
}

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::ConduitInsertIf::lock() {
	m_conduit->lock();
}

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::ConduitInsertIf::unlock() {
	m_conduit->unlock();
}

/////////////////////////////////////////////////////////////////////////////
// ConduitExtractIf (Reader) methods
/////////////////////////////////////////////////////////////////////////////
template <class DATATYPE> inline HeterogeneousConduit<DATATYPE>::ConduitExtractIf::ConduitExtractIf() : m_conduit(NULL) { }
template <class DATATYPE> inline HeterogeneousConduit<DATATYPE>::ConduitExtractIf::ConduitExtractIf(HeterogeneousConduit<DATATYPE>& cdt) : m_conduit(&cdt) { }
template <class DATATYPE> inline HeterogeneousConduit<DATATYPE>::ConduitExtractIf::~ConduitExtractIf() {
	try { m_conduit->clearDst(); } catch(...) {}
}

template <class DATATYPE> inline  typename HeterogeneousConduit<DATATYPE>::ConduitExtractIf &
HeterogeneousConduit<DATATYPE>::ConduitExtractIf::operator=(HeterogeneousConduit<DATATYPE>::ConduitExtractIf& dif) {
	m_conduit = dif.m_conduit; return dif;
}

template <class DATATYPE> inline void HeterogeneousConduit<DATATYPE>::ConduitExtractIf::setup(int depth,
         HTaskInfo * info, int * dims) {
   if (m_conduit->m_shared) {
      m_conduit->setupSrc(depth, info, dims);
   } else {
	   m_conduit->setupDst(depth, info, dims);
   }
}

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::ConduitExtractIf::setup(std::vector<handle> & vecHandles,
         HTaskInfo * info, int * dims) {
   if (m_conduit->m_shared) {
	   m_conduit->setupSrc(vecHandles, info, dims);
	} else {
	   m_conduit->setupDst(vecHandles, info, dims);
   }
}

template <class DATATYPE>
inline bool HeterogeneousConduit<DATATYPE>::ConduitExtractIf::ready() {
	return m_conduit->extractReady();
}

template <class DATATYPE>
inline DATATYPE & HeterogeneousConduit<DATATYPE>::ConduitExtractIf::getData() {
	return m_conduit->getDstData();
}

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::ConduitExtractIf::releaseData() {
	m_conduit->releaseDstData();
}

template <class DATATYPE>
inline bool HeterogeneousConduit<DATATYPE>::ConduitExtractIf::isAtEOC() {
	return m_conduit->isAtEOC();
}

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::ConduitExtractIf::lock() {
	m_conduit->lock();
}

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::ConduitExtractIf::unlock() {
	m_conduit->unlock();
}

/////////////////////////////////////////////////////////////////////////////
// HeterogeneousConduit methods
/////////////////////////////////////////////////////////////////////////////

//Constructor
template <class DATATYPE> HeterogeneousConduit<DATATYPE>::HeterogeneousConduit(const string & name, bool shared) :
	m_name(name),
	m_setupCompleteDone(false),
	m_srcsInitialized(false),
	m_dstsInitialized(false),
	m_EOC(false),
	m_cpyLock(false),
	m_shared(shared) {
		pthread_mutex_init(&m_initSrcMutex, NULL);
		pthread_mutex_init(&m_initDstMutex, NULL);
		m_srcInfo.location = LOC_INVALID;
		m_srcInfo.device = -1;
		m_srcInfo.process = -1;
		m_srcInfo.platform = PLAT_INVALID;
		m_dstInfo.location = LOC_INVALID;
		m_dstInfo.device = -1;
		m_dstInfo.process = -1;
		m_dstInfo.platform = PLAT_INVALID;
}//end Cdt Construct()

//Destructor
template <class DATATYPE>
HeterogeneousConduit<DATATYPE>::~HeterogeneousConduit() throw() { return; }

template <class DATATYPE>
inline typename HeterogeneousConduit<DATATYPE>::Writer HeterogeneousConduit<DATATYPE>::getWriter() {
	ConduitInsertIf iff(*this); return iff;
}

template <class DATATYPE>
inline typename HeterogeneousConduit<DATATYPE>::Reader HeterogeneousConduit<DATATYPE>::getReader() {
	ConduitExtractIf eif( *this ); return eif;
}

template <class DATATYPE>
inline bool HeterogeneousConduit<DATATYPE>::insertAvailable() {
	return !m_src.is_full();
}

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::setupSrc( std::vector<handle> & vecHandles, HTaskInfo * info, int * dims ) {
   pthread_mutex_lock(&m_initSrcMutex);
   if (!m_srcsInitialized) {
      m_srcInfo = *info;
      for (int idx = 0; idx < HNDIMS; idx++) { m_srcDims[idx] = dims[idx]; }
      m_srcStride = 1;
	   m_bufferLength = vecHandles.size();
	   m_src.setCapacity(m_bufferLength);
	   for (int i=0; i< m_bufferLength; ++i) {
		   m_src.insert(vecHandles[i]);
	   }
	   m_src.reset();
      m_srcsInitialized = true;
   } else {
      std::cerr << "HeterogeneousConduit setup: memory already allocated" << std::endl;
   }
   pthread_mutex_unlock(&m_initSrcMutex);
}

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::setupSrc( int depth, HTaskInfo * info, int * dims ) {
   pthread_mutex_lock(&m_initSrcMutex);
   if (!m_srcsInitialized) {
      m_srcInfo = *info;
      for (int idx = 0; idx < HNDIMS; idx++) { m_srcDims[idx] = dims[idx]; }
      m_srcStride = 1;
	   m_bufferLength = depth;
	   m_src.setCapacity(m_bufferLength);
	   for (int i=0; i< m_bufferLength; ++i) {
		   handle temp;
		   temp = (handle)initMem(m_srcDims, sizeof(DATATYPE), &m_srcStride, m_name.c_str(), &m_srcInfo, 0);
		   printf("Initializing memory in conduit %s src on %d device: %X\n",m_name.c_str(), m_srcInfo.location, temp);
		   m_src.insert(temp);
	   }
	   m_src.reset();
      m_srcsInitialized = true;
   }
   pthread_mutex_unlock(&m_initSrcMutex);
}

template <class DATATYPE>
inline DATATYPE & HeterogeneousConduit<DATATYPE>::getSrcData() {
	if (m_src.beginInsert(m_writeData)) {
	   return *m_writeData;
	} else {
		throw Exception("Conduit is not accepting more data. It is in Writelocked state");
	}
}

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::releaseSrcData() {
	if (!m_shared) {
	   if (!m_dst.beginInsert(m_readData)) {
	      throw Exception("Conduit destination is not accepting more data.");
      }
      if (!m_cpyLock) {
	      printf("Moving memory in conduit %s src: From %d:%X to %d:%X\n", m_name.c_str(), m_srcInfo.location, m_writeData, m_dstInfo.location, m_readData);
	      moveData(m_readData, m_dstStride, &m_dstInfo, m_writeData, m_srcStride, &m_srcInfo,
	               m_srcDims, sizeof(DATATYPE), m_name.c_str());
      }
      if (!m_src.endInsert(m_writeData)) { printf("Note 1: problem in src->endInsert call\n"); }
      if (!m_src.beginRemove(m_writeData)) { printf("Note 2: problem in src->endInsert call\n"); }
      if (!m_src.endRemove(m_writeData)) { printf("Note 3: problem in src->endInsert call\n"); }
      if (!m_dst.endInsert(m_readData)) { printf("Note 4: problem in src->endInsert call\n"); }
   } else {
      if (!m_src.endInsert(m_writeData)) { printf("Shared conduit release src.\n"); }
   }
   return;
}

template <class DATATYPE>
inline bool HeterogeneousConduit<DATATYPE>::extractReady() {
	return ! (m_dst.is_empty());
}

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::setupDst( int depth, HTaskInfo * info, int * dims ) {
	pthread_mutex_lock(&m_initDstMutex);
   if (!m_dstsInitialized) {
      m_dstInfo = *info;
      for (int idx = 0; idx < HNDIMS; idx++) { m_dstDims[idx] = dims[idx]; }
      m_dstStride = 1;
	   m_bufferLength = depth;
	   m_dst.setCapacity(m_bufferLength);
	   for (int i=0; i< m_bufferLength;++i) {
		   handle temp = (handle)initMem(m_dstDims, sizeof(DATATYPE), &m_dstStride, m_name.c_str(), &m_dstInfo, 0);
		   printf("Initializing memory in conduit %s src on %d device: %X\n",m_name.c_str(), m_dstInfo.location, temp);
		   m_dst.insert(temp);
	   }
	   m_dst.reset();
      m_dstsInitialized = true;
   }
   pthread_mutex_unlock(&m_initDstMutex);
}

template <class DATATYPE>
inline
void HeterogeneousConduit<DATATYPE>::setupDst( vector<handle>& vecHandles, HTaskInfo * info, int * dims ) {
   pthread_mutex_lock(&m_initDstMutex);
   if (!m_dstsInitialized) {
      m_dstInfo = *info;
      for (int idx = 0; idx < HNDIMS; idx++) { m_dstDims[idx] = dims[idx]; }
      m_dstStride = 1;
	   m_bufferLength = vecHandles.size();
	   m_dst.setCapacity(m_bufferLength);
	   for (int i=0; i< m_bufferLength; ++i) {
		   m_dst.insert(vecHandles[i]);
	   }
	   m_dst.reset();
      m_dstsInitialized = true;
   } else {
      std::cerr << "HeterogeneousConduit setup: memory already allocated" <<std::endl;
   }
   pthread_mutex_unlock(&m_initDstMutex);
}

template <class DATATYPE>
inline DATATYPE & HeterogeneousConduit<DATATYPE>::getDstData() {
   if (!m_shared) {
	   if (m_dst.beginRemove(m_readData)) {
	      return *m_readData;
	   } else {
		   throw Exception("Conduit is not accepting more data. It is in Writelocked state");
	   }
   } else {
      if (m_src.beginRemove(m_readData)) {
         return *m_readData;
      } else {
         throw Exception("Shared Conduit is not accepting more data. It is in Writelocked state");
      }
   }
}

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::releaseDstData() {
   if (!m_shared) {
	   m_dst.endRemove(m_readData);
   } else {
      m_src.endRemove(m_readData);
   }
}

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::setupComplete() {
	//m_src.reset(); m_dst.reset();
}

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::insertEOC() { m_EOC = true; }

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::clearEOC() { m_EOC = false; }

template <class DATATYPE>
inline bool HeterogeneousConduit<DATATYPE>::isAtEOC() { return m_EOC; }

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::lock() { m_cpyLock = true; }

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::unlock() { m_cpyLock = false; }

template <class DATATYPE>
inline string HeterogeneousConduit<DATATYPE>::getName() { return m_name; }

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::clearSrc() {
	///@todo: 
	//If it is desired that the data is deleted when the ConduitXXIf dies then
	//uncomment the following
	m_src.clear();
}

template <class DATATYPE>
inline void HeterogeneousConduit<DATATYPE>::clearDst() {
	///@todo: 
	//If it is desired that the data is deleted when the ConduitXXIf dies then
	//uncomment the following
	m_dst.clear();
}

}//end namespace

#endif /*HETEROGENEOUSCONDUIT_H_*/
