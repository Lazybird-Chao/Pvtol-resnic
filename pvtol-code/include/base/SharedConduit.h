/**
 *    File: SharedConduit.h
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *s
 *  \author  $LastChangedBy: $
 *  \date    $LastChangedDate: 2009-08-12 13:26:09 -0400 (Wed, 12 Aug 2009) $
 *  \version $LastChangedRevision: 1144 $
 *  \brief   Class definition  & inline methods of SharedConduit, Writer,
 *            ConduitExtractIf 
 *            The Shared Conduit class provides a means to transport 
 *            data objects between two tasks within the same process using shared memory
 *
 *            The ConduitInsertIf class provides a constrained API for the
 *            source end of a conduit.
 *            The ConduitExtractIf class provides a constrained API for the
 *            destination end of a conduit.
 *
 * Author: Sanjeev Mohindra
 *  $Id: SharedConduit.h 1144 2009-08-12 17:26:09Z  $
 *
 */

#ifndef SHAREDCONDUIT_H_
#define SHAREDCONDUIT_H_

#include <PvtolProgram.h>
#include <BlockingCQueue.h>
#include <ThreadManager.h>
#include <string>
#include <typeinfo>
#include <shared_ptr.hpp>
#include <vector>
#include <queue>

namespace ipvtol
{
using std::string;
using std::vector;
using boost::shared_ptr;

//           ==========================================
//                       SharedConduit Class
//           ==========================================

template <class DATATYPE> class SharedConduit
{
public:
	/////////////////////////////////////////////////////////////////////////
	// PUBLIC TYPES
	/////////////////////////////////////////////////////////////////////////	  
	typedef DATATYPE DataType;
	typedef typename BlockingCQueue<DataType>::DataTypePtr DataTypePtr;
	typedef typename BlockingCQueue<DataType>::DataTypePtr Handle;

	/////////////////////////////////////////////////////////////////////////
	// NESTED CLASSES
	/////////////////////////////////////////////////////////////////////////

	class ConduitInsertIf
	{
	public:

		// INITIALIZATION METHODS

		ConduitInsertIf();

		/**
		 * @param cdt the conduit for which this ConduitInsertIf is an interface
		 *
		 */
		ConduitInsertIf(SharedConduit<DATATYPE>& cdt);

		//Destructor
		~ConduitInsertIf();

		inline ConduitInsertIf& operator=(ConduitInsertIf& sif);

		// CONDUIT METHODS
		/**
		 *
		 * @param depth the multi-buffering depth for the source end of
		 * the conduit
		 */
		void setup(int depth=1); // multi-buffering depth   
		/**
		 *
		 * @param depth the multi-buffering depth for the source end of
		 * the conduit
		 *
		 * @param T contains the argument to the constructor for the source object */
		template <class T> void setup(T par, int depth=1); // multi-buffering depth


		/**
		 * @param srcObj the object which describes the storage used by
		 * the source end of the conduit.
		 *
		 * @param depth the multi-buffering depth for the source end of
		 * the conduit. */
		void setup(DATATYPE& srcObj, // user supplied endpoint object
				int depth=1); // multi-buffering depth
		
		/**
		 * @param vecHandles Vector containing Handles pointing to allocated memory. 
		 *
		 */
		void setup(vector <DataTypePtr>& vecHandles); // user supplied endpoint object

		/**
		 * Queries whether the conduit is able to provide the buffer space
		 * needed for an outgoing transfer. 
		 *
		 * @return returns true if space is available 
		 * */
		inline bool available();

		/**
		 * Gives access to the data object which provides a 
		 * view of the next available buffer space. Note that getData()
		 * does not modify the contents of the returned data object
		 * therefore there is no guarantee that the initial contents of
		 * newly acquired buffer space has any particular value.  If
		 * necessary, the application should clear the data object before
		 * using it. 
		 *
		 * @return a reference to the conduit data object.
		 *
		 * */
		inline DATATYPE& getData();

		/**
		 * Puts back the data object in the conduit. 
		 *
		 * @return void
		 *
		 * */
		inline void releaseData();

		/**
		 * Gives access to the Data Handle to the object which provides a 
		 * view of the next available buffer space. Note that getHandle()
		 * does not modify the contents of the returned data object
		 * therefore there is no guarantee that the initial contents of
		 * newly acquired buffer space has any particular value.  If
		 * necessary, the application should clear the data object before
		 * using it. 
		 *
		 * @return a handle to the conduit data object.
		 *
		 * */
		inline Handle& getHandle();
		
		
		/**
		 * Releases the Handle. This may now be pointing to a different Data object
		 * than that supplied by the conduit 
		 *
		 * @return void
		 *
		 * */
		inline void releaseHandle();
		
		///Insert the EOC
		inline void insertEOC();

		///Clear the EOC
		inline void clearEOC();

	private:

		SharedConduit<DATATYPE> * m_conduit;
	};
	
	class ConduitExtractIf
	{
	public:

		// CONSTRUCTORS

		ConduitExtractIf();

		/**
		 * @param cdt the conduit for which this ConduitExtractIf is an interface
		 *
		 */
		ConduitExtractIf(SharedConduit<DATATYPE>& cdt);

		//Desructor
		~ConduitExtractIf();

		// ASSIGNMENT OPERATOR

		inline ConduitExtractIf& operator=(ConduitExtractIf& dif);

		// CONDUIT ACCESSORS

		// Setup methods
		/**
		 *
		 * @param par Argument for constructor of the destination object
		 * */
		template <class T> void setup(T par, // 
				int depth=1); // multi-buffering depth

		void setup(DATATYPE& dstObj, int depth = 1); // user supplied endpoint object

		/**
		 * @param vecHandles Vector containing Handles pointing to allocated memory. 
		 *
		 */
		void setup(vector <DataTypePtr>& vecHandles); // user supplied endpoint object
		
		
		void setup(int depth = 1); // user supplied endpoint object

		/**
		 * Queries whether the conduit has unread  data.
		 *
		 * @return Returns true if the next input buffer has data
		 * which has not yet been extracted.
		 */
		inline bool ready();

		/**
		 * Gives access to the data object Once
		 * extractReady() has returned true, the contents of the
		 * destination distributed data object is valid and repeated calls
		 * to getDestData() will return a view of the same buffer
		 * space.
		 *
		 * @return a reference to the conduit's data object.  */
		inline DATATYPE& getData();

		/**
		 * Releases the buffer space so that it can be re-used
		 */
		inline void releaseData();

		/**
		 * Returns the handle to the data object Repeated calls
		 * to getHandle() will return the same handle. Note that this handle 
		 * may not be pointing to the same buffer as the first call
		 *
		 * @return a handle to the conduit's data object.  */
		inline Handle& getHandle();
		
		/**
		 * Releases the handle
		 */
		inline void releaseHandle();
		
		///IsAtEOC
		inline bool isAtEOC();

	private:
		SharedConduit<DATATYPE> * m_conduit;
	};


	///////////////////////////////////////////////////////////////////////// 
	//Friends
	/////////////////////////////////////////////////////////////////////////

	friend class ConduitInsertIf;
	friend class ConduitExtractIf;

	/////////////////////////////////////////////////////////////////////////
	// PUBLIC TYPES FOR USING NESTED CLASSES
	/////////////////////////////////////////////////////////////////////////	  

	typedef ConduitInsertIf InsertIf;
	typedef ConduitExtractIf ExtractIf;

	typedef ConduitInsertIf Writer;
	typedef ConduitExtractIf Reader;

	//typedef typename BlockingCQueue<DataType>::value_type buffer_type;


	/////////////////////////////////////////////////////////////////////////
	// PUBLIC METHODS
	/////////////////////////////////////////////////////////////////////////

	// CONSTRUCTORS


	/**
	 * \brief    Constructor
	 * \param    name   name of the conduit
	 * 
	 * \return   return_type 
	 *
	 */
	SharedConduit(const string& name ="Conduit");

	///Destructor
	virtual ~SharedConduit() throw();

	///Get the Insertion interface (deprecated)
	InsertIf getInsertIf();
	
	///Get the Writer interface
	Writer getWriter();

	///Get the Extraction interface (deprecated)
	ExtractIf getExtractIf();

	///Get the Reader interface
	Reader getReader();
	
	///Get the name
	string getName();

	///Setup the source buffers using new DATATYPE
	void setupSrc(int depth = 1);

	//The setupXXXX methods can be templated on any number of  template params.
	///Setup the source buffers using new DATATYPE(par)
	template <class T> void setupSrc(T par, int depth = 1);

	///Setup the source buffers
	void setupSrc(DATATYPE & srcObj, int depth = 1);
	
	///Use user-supplied source  buffers
	void setupSrc(std::vector < DataTypePtr > & vecHandles);

	///Clear the Src buffers
	void clearSrc();

	///Setup the destination buffers (noop)
	void setupDest(int depth = 1);

	///Setup the destination buffers (noop)    
	template <class T> void setupDest(T par, int depth = 1);

	///Setup the destination buffers (noop)
	void setupDest(DATATYPE & srcObj, int depth = 1);

	///Setup the destination buffers (noop)
	void setupDest(vector<DataTypePtr>& vecHandles);
	
	///Clear the destination buffers (noop)
	void clearDest();

	///Can we write to the conduit
	inline bool insertAvailable();



	///Can we read from conduit?
	inline bool extractReady();


	//Finish setting up the conduit (noop)
	void setupComplete();

	/**
	 * Sends an end-of-cycle (EOC).
	 *
	 */
	inline void insertEOC();

	/**
	 *      clearEOC()
	 * Clears any end-of-cycle (EOC) signal.
	 *
	 * @return void
	 */
	inline void clearEOC();

	/**
	 * Queries whether the conduit has received an EOC signal.
	 */
	inline bool isAtEOC();

	/////////////////////////////////////////////////////////////////////////
	// PRIVATE METHODSs
	/////////////////////////////////////////////////////////////////////////
	
	
private:
	///Get the src data for buffer to write
	inline DATATYPE & getSrcData();

	///Release the source data
	inline void releaseSrcData();
	
	//Get the destination data for read
	inline DATATYPE & getDestData();

	//Release the data 
	inline void releaseDestData();
	
	///Get the src Handle for buffer to write
	inline Handle& getSrcHandle();

	///Release the source Handle
	inline void releaseSrcHandle();
	
	//Get the destination Handle for read
	inline Handle& getDestHandle();

	//Release the Handles 
	inline void releaseDestHandle();
	
	/////////////////////////////////////////////////////////////////////////
	// PRIVATE MEMBERS
	/////////////////////////////////////////////////////////////////////////
	
private:
	string m_name;
	int m_bufferLength;
	BlockingCQueue<DataType> m_src;
	Handle m_readData;
	Handle m_writeData;
	//BlockingCQueue<DataType>		m_dest;
	bool m_setupCompleteDone;
	bool m_srcsInitialized;
	bool m_dstsInitialized;
        pthread_mutex_t m_initMutex;

	bool m_EOC;
};

////////////////////////////////////////////////////////////////////
//                  I N L I N E        Methods
//--------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// ConduitInsertIf methods
/////////////////////////////////////////////////////////////////////////////
template <class DATATYPE> inline SharedConduit<DATATYPE>::ConduitInsertIf::ConduitInsertIf() :
	m_conduit(0)
{

}

template <class DATATYPE> inline SharedConduit<DATATYPE>::ConduitInsertIf::ConduitInsertIf(
		SharedConduit<DATATYPE>& cdt) :
	m_conduit(&cdt)
{
}

template <class DATATYPE> inline SharedConduit<DATATYPE>::ConduitInsertIf::~ConduitInsertIf()
{
	try
	{
		m_conduit->clearSrc();
	}
	catch(...)
	{}
}

template <class DATATYPE> inline
typename SharedConduit<DATATYPE>::ConduitInsertIf& 
SharedConduit<DATATYPE>::ConduitInsertIf::operator=
	(SharedConduit<DATATYPE>::ConduitInsertIf& sif)
{
	m_conduit = sif.m_conduit;
	return sif;
}

template <class DATATYPE>
template <class T>
inline
void SharedConduit<DATATYPE>::ConduitInsertIf::setup(
		T par,
		int depth
)
{
	m_conduit->setupSrc(par, depth);
}

template <class DATATYPE>
inline
void SharedConduit<DATATYPE>::ConduitInsertIf::setup(
		DATATYPE & srcObj,
		int depth)
{
	m_conduit->setupSrc(srcObj, depth);
}

template <class DATATYPE>
inline
void SharedConduit<DATATYPE>::ConduitInsertIf::setup(
		std::vector< DataTypePtr > & vecHandles)
{
	m_conduit->setupSrc(vecHandles);
}

template <class DATATYPE>
inline
void SharedConduit<DATATYPE>::ConduitInsertIf::setup(int depth)
{
	m_conduit->setupSrc(depth);
}

template <class DATATYPE>
inline bool SharedConduit<DATATYPE>::ConduitInsertIf::available()
{
	return m_conduit->insertAvailable();
}

template <class DATATYPE>
inline DATATYPE & SharedConduit<DATATYPE>::ConduitInsertIf::getData()
{
	return m_conduit->getSrcData();
}

template <class DATATYPE>
inline void SharedConduit<DATATYPE>::ConduitInsertIf::releaseData()
{
	m_conduit->releaseSrcData();
}

template <class DATATYPE>
inline typename SharedConduit<DATATYPE>::Handle& SharedConduit<DATATYPE>::ConduitInsertIf::getHandle()
{
	return m_conduit->getSrcHandle();
}

template <class DATATYPE>
inline void SharedConduit<DATATYPE>::ConduitInsertIf::releaseHandle()
{
	m_conduit->releaseSrcHandle();
}

template <class DATATYPE>
inline void SharedConduit<DATATYPE>::ConduitInsertIf::insertEOC()
{
	m_conduit->insertEOC();
}

template <class DATATYPE>
inline void SharedConduit<DATATYPE>::ConduitInsertIf::clearEOC()
{
	m_conduit->clearEOC();
}

/////////////////////////////////////////////////////////////////////////////
// ConduitExtractIf methods
/////////////////////////////////////////////////////////////////////////////

//Constructor
template <class DATATYPE>
inline
SharedConduit<DATATYPE>::ConduitExtractIf::ConduitExtractIf()
: m_conduit(0)
{
}

template <class DATATYPE>
inline
SharedConduit<DATATYPE>::ConduitExtractIf::ConduitExtractIf(
		SharedConduit<DATATYPE>& cdt)
: m_conduit(&cdt)
{
}

template <class DATATYPE>
inline
SharedConduit<DATATYPE>::ConduitExtractIf::~ConduitExtractIf()
{
	try
	{
		m_conduit->clearDest();
	}
	catch(...)
	{}
}

template <class DATATYPE>
inline 
typename SharedConduit<DATATYPE>::ConduitExtractIf &
SharedConduit<DATATYPE>::ConduitExtractIf::operator=
	(SharedConduit<DATATYPE>::ConduitExtractIf& dif)
{
	m_conduit = dif.m_conduit;
	return dif;
}

template <class DATATYPE>
inline
void SharedConduit<DATATYPE>::ConduitExtractIf::setup(int depth)
{
	m_conduit.setupSrc(depth);
}

template <class DATATYPE>
template <class T>
inline
void SharedConduit<DATATYPE>::ConduitExtractIf::setup( T par, int depth)
{
	m_conduit->setupSrc(par,depth);
}

template <class DATATYPE>
inline
void SharedConduit<DATATYPE>::ConduitExtractIf::setup(
		DATATYPE & srcObj,
		int depth)
{
	m_conduit->setupSrc(srcObj, depth);
}

template <class DATATYPE>
inline
void SharedConduit<DATATYPE>::ConduitExtractIf::setup(
		std::vector< DataTypePtr > & vecHandles)
{
	m_conduit->setupSrc(vecHandles);
}

template <class DATATYPE>
inline bool SharedConduit<DATATYPE>::ConduitExtractIf::ready()
{
	return m_conduit->extractReady();
}

template <class DATATYPE>
inline DATATYPE & SharedConduit<DATATYPE>::ConduitExtractIf::getData()
{
	return m_conduit->getDestData();
}

template <class DATATYPE>
inline void SharedConduit<DATATYPE>::ConduitExtractIf::releaseData()
{
	m_conduit->releaseDestData();
}


template <class DATATYPE>
inline typename SharedConduit<DATATYPE>::Handle& SharedConduit<DATATYPE>::ConduitExtractIf::getHandle()
{
	return m_conduit->getDestHandle();
}

template <class DATATYPE>
inline void SharedConduit<DATATYPE>::ConduitExtractIf::releaseHandle()
{
	m_conduit->releaseDestHandle();
}

template <class DATATYPE>
inline bool SharedConduit<DATATYPE>::ConduitExtractIf::isAtEOC()
{
	return m_conduit->isAtEOC();
}

/////////////////////////////////////////////////////////////////////////////
// SharedConduit methods
/////////////////////////////////////////////////////////////////////////////

//Constructor
template <class DATATYPE>
SharedConduit<DATATYPE>::SharedConduit(const string & name)
:m_name(name),
m_setupCompleteDone(false),
m_srcsInitialized(false),
m_dstsInitialized(false),
m_EOC(false)
{
   pthread_mutex_init(&m_initMutex, NULL);
}//end Cdt Construct()


//Destructor
template <class DATATYPE>
SharedConduit<DATATYPE>::~SharedConduit() throw()
{
	///@todo delete the contents of the source buffer m_src
	return;
}

template <class DATATYPE>
inline
typename SharedConduit<DATATYPE>::ConduitInsertIf
SharedConduit<DATATYPE>::getInsertIf()
{
	ConduitInsertIf iff(*this);
	return iff;
}

template <class DATATYPE>
inline
typename SharedConduit<DATATYPE>::Writer
SharedConduit<DATATYPE>::getWriter()
{
	ConduitInsertIf iff(*this);
	return iff;
}

template <class DATATYPE>
inline
typename SharedConduit<DATATYPE>::ConduitExtractIf
SharedConduit<DATATYPE>::getExtractIf()
{
	ConduitExtractIf eif( *this );
	return eif;
}

template <class DATATYPE>
inline
typename SharedConduit<DATATYPE>::Reader
SharedConduit<DATATYPE>::getReader()
{
	ConduitExtractIf eif( *this );
	return eif;
}

template <class DATATYPE>
inline bool SharedConduit<DATATYPE>::insertAvailable()
{
	return ! m_src.is_full();
}

template <class DATATYPE>
template <class T>
inline
void SharedConduit<DATATYPE>::setupSrc(
		T par,
		int depth
)
{
   pthread_mutex_lock(&m_initMutex);
   if(!m_srcsInitialized)
   {
	///@todo Allocate memory in the underlying buffer and reset
	m_bufferLength = depth;
	m_src.setCapacity(m_bufferLength);
	for(int i=0; i< m_bufferLength;++i)
	{
		DataTypePtr temp (new DATATYPE(par));
		m_src.insert(temp);
	}

	m_src.reset();
        m_srcsInitialized=true;
   }
   pthread_mutex_unlock(&m_initMutex);
}

template <class DATATYPE>
inline
void SharedConduit<DATATYPE>::setupSrc(
		DATATYPE & srcObj,
		int depth)
{
   pthread_mutex_lock(&m_initMutex);
   if(!m_srcsInitialized)
   {
	m_bufferLength = depth;
	m_src.setCapacity(m_bufferLength);
	for(int i=0; i< m_bufferLength;++i)
	{
		DataTypePtr temp(new DATATYPE(srcObj));
		m_src.insert(temp);
	}

	m_src.reset();
        m_srcsInitialized=true;
   }
   pthread_mutex_unlock(&m_initMutex);
}

template <class DATATYPE>
inline
void SharedConduit<DATATYPE>::setupSrc(
		std::vector< DataTypePtr > & vecHandles)
{
   pthread_mutex_lock(&m_initMutex);
   if(!m_srcsInitialized)
   {
	m_bufferLength = vecHandles.size();
	m_src.setCapacity(m_bufferLength);
	for(int i=0; i< m_bufferLength;++i)
	{
		m_src.insert(vecHandles[i]);
	}

	m_src.reset();
        m_srcsInitialized=true;
   } else {
      std::cerr << "SharedConduit setup: memory already allocated" <<std::endl;
   }
   pthread_mutex_unlock(&m_initMutex);
}

template <class DATATYPE>
inline
void SharedConduit<DATATYPE>::setupSrc(int depth)
{
   pthread_mutex_lock(&m_initMutex);
   if(!m_srcsInitialized)
   {
	m_bufferLength = depth;
	m_src.setCapacity(m_bufferLength);
	for(int i=0; i< m_bufferLength;++i)
	{
		DataTypePtr temp(new DATATYPE);
		m_src.insert(temp);
	}

	m_src.reset();
        m_srcsInitialized=true;
   }
   pthread_mutex_unlock(&m_initMutex);
}

template <class DATATYPE>
inline DATATYPE & SharedConduit<DATATYPE>::getSrcData()
{
	if (m_src.beginInsert(m_writeData)){
		return *m_writeData;
	}
	else{
		throw Exception("Conduit is not accepting more data. It is in Writelocked state");
	}
}

template <class DATATYPE>
inline void SharedConduit<DATATYPE>::releaseSrcData()
{
	if (m_src.endInsert(m_writeData))
	{
		m_writeData.reset();
	}
	
}

template <class DATATYPE>
inline typename SharedConduit<DATATYPE>::Handle& SharedConduit<DATATYPE>::getSrcHandle()
{
	if (m_src.beginInsert(m_writeData)){
		return m_writeData;
	}
	else{
		throw Exception("Conduit is not accepting more data. It is in Writelocked state");
	}
}

template <class DATATYPE>
inline void SharedConduit<DATATYPE>::releaseSrcHandle()
{
	if (m_src.endInsert(m_writeData))
	{
		m_writeData.reset();
	}
	
}

template <class DATATYPE>
inline bool SharedConduit<DATATYPE>::extractReady()
{
	return ! (m_src.is_empty());
}

template <class DATATYPE>
inline
void SharedConduit<DATATYPE>::setupDest(int depth)
{
	//do nothing for now. If we need dual ended conduits we can add stuff here
}

template <class DATATYPE>
template <class T>
inline
void SharedConduit<DATATYPE>::setupDest(
		T par,
		int depth
)
{
	//do nothing for now. If we need dual ended conduits we can add stuff here
}

template <class DATATYPE>
inline
void SharedConduit<DATATYPE>::setupDest(
		DATATYPE & srcObj,
		int depth)
{
	//do nothing for now. If we need dual ended conduits we can add stuff here
}

template <class DATATYPE>
inline
void SharedConduit<DATATYPE>::setupDest(vector<DataTypePtr>& vecHandles)
{
	//do nothing for now. If we need dual ended conduits we can add stuff here
}

template <class DATATYPE>
inline DATATYPE & SharedConduit<DATATYPE>::getDestData()
{
	m_src.beginRemove(m_readData);
	return *m_readData;
}

template <class DATATYPE>
inline void SharedConduit<DATATYPE>::releaseDestData()
{
	if (m_src.endRemove(m_readData))
		m_readData.reset();
}

template <class DATATYPE>
inline typename SharedConduit<DATATYPE>::Handle& SharedConduit<DATATYPE>::getDestHandle()
{
	m_src.beginRemove(m_readData);
	return m_readData;
}

template <class DATATYPE>
inline void SharedConduit<DATATYPE>::releaseDestHandle()
{
	if (m_src.endRemove(m_readData))
		m_readData.reset();
}

template <class DATATYPE>
inline void SharedConduit<DATATYPE>::setupComplete()
{
	//m_src.reset();
}

template <class DATATYPE>
inline void SharedConduit<DATATYPE>::insertEOC()
{
	m_EOC = true;
}

template <class DATATYPE>
inline void SharedConduit<DATATYPE>::clearEOC()
{
	m_EOC = false;
}

template <class DATATYPE>
inline bool SharedConduit<DATATYPE>::isAtEOC()
{
	return m_EOC;
}

template <class DATATYPE>
inline string SharedConduit<DATATYPE>::getName()
{
	return m_name;
}

template <class DATATYPE>
inline void SharedConduit<DATATYPE>::clearSrc()
{
	///@todo: 
	//If it is desired that the data is deleted when the ConduitXXIf dies then
	//uncomment the following

	//m_src.clear();
}

template <class DATATYPE>
inline void SharedConduit<DATATYPE>::clearDest()
{
	//noop
}
}//end namespace


#endif /*SHAREDCONDUIT_H_*/
