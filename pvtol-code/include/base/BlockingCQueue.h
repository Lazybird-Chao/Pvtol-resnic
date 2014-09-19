/**
 *    File: BlockingCQueue.h
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the Blocking Circular Queue class.
 *
 *  Author: Sanjeev Mohindra
 *
 *  $Id: BlockingCQueue.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#ifndef BLOCKINGCQUEUE_H_
#define BLOCKINGCQUEUE_H_

#include <ThreadManager.h>
#include <shared_ptr.hpp>
#include <vector>

using boost::shared_ptr;

namespace ipvtol
{
typedef unsigned int size_type;

template <class T>
class BlockingCQueue
{
public:
	typedef shared_ptr<T> value_type;
	typedef shared_ptr<T> DataTypePtr;
	typedef std::vector< value_type > container_type;
	//typedef container_type::iterator iterator;
	typedef unsigned int size_type;

	///Constructor
	explicit BlockingCQueue(size_type capacity = 1);

	///Discard all items in queue and change capacity
	bool setCapacity(size_type capacity);


	///Get the circular queue capacity
	size_type getCapacity();


	/**
	 * \brief    Insert an item to the queue.
	 * 			 This inserts the item and moves the write location index.
	 * 			 It blocks if the queue is empty, unless the queue is wite locked.
	 *           If the queue is writelocked, it returns false immediately
	 * \param    value_type&   This is the value type which the user has modified in place
	 *
	 * \return   false if remove failed, true otherwise
	 *
	 */
	bool insert(value_type& item);

	/**
	 * \brief    Begin the process of two phase insertion.
	 * \param    value_type&   This is the value type which is returned to the user to modify
	 *
	 * \return   false if the queue is locked, true otherwise
	 *
	 */
	bool beginInsert(value_type& item);


	/**
	 * \brief    Finish the process of two phase insertion.
	 * 			 This inserts the item and moves the location index
	 * \param    value_type&   This is the value type which the user has modified in place
	 *
	 * \return   false if the queue is locked, true otherwise
	 *
	 */
	bool endInsert(value_type& item);


	/**
	 * \brief    Remove an item from the queue.
	 * 			 This removes the item and moves the read location index.
	 * 			 It blocks if the queue is empty, unless the queue is wite locked.
	 * \param    value_type&   This is the value type which the user has modified in place
	 *
	 * \return   false if remove failed, true otherwise
	 *
	 */
	bool remove(value_type& item);


	/**
	 * \brief    Begin the process of two phase removal.
	 * 			 It blocks if the queue is empty, and is not writelocked
	 *
	 * \param    value_type&   item. The item to be removed
	 *
	 * \return   false if removal fails, true otherwise
	 *
	 */
	bool beginRemove(value_type& item);

	/**
	 * \brief    Finish the process of two phase removal.
	 * \param    value_type&   item. The item to be removed
	 *
	 * \return   false if removal fails, true otherwise
	 *
	 */
	bool endRemove(value_type& item);

	///Wait for the queue to drain, and then delete all elements
	void clear();

	///Is the queue empty?
	bool is_empty() const;


	///Is the queue full?
	bool is_full() const;

	///Reset the Queue and discard all content
	void reset();


	///Write protect the queue. Prevents adding of new items to queue
	void writeLock();

	///Unlock the queue. The queue is open for writing again
	void writeUnlock();


	///Query the writable state of the circular queue
	bool isWriteLocked();


private:
	BlockingCQueue(const BlockingCQueue&); // Disabled copy constructor
	BlockingCQueue& operator =(const BlockingCQueue&); // Disabled assign operator

	size_type m_unread;
	container_type m_container;
	pthread_mutex_t m_mutex;
	pthread_mutex_t m_readMutex;
	pthread_mutex_t m_writeMutex;
	pthread_cond_t m_not_empty;
	pthread_cond_t m_empty;
	pthread_cond_t m_not_full;
	size_type m_readIndex;
	size_type m_writeIndex;
	size_type m_capacity;
	bool m_writeLock;
};


////////////////////////IMPLEMENTATION/////////////////////////////////
template <class T>
BlockingCQueue<T>::BlockingCQueue(size_type capacity) :
	m_unread(0), m_readIndex(0), m_writeIndex(0), m_capacity(capacity), m_writeLock(false)
{
	pthread_mutex_init(&m_mutex, NULL);
	pthread_mutex_init(&m_readMutex, NULL);
	pthread_mutex_init(&m_writeMutex, NULL);
	pthread_cond_init(&m_not_empty, NULL);
	pthread_cond_init(&m_empty, NULL);
	pthread_cond_init(&m_not_full, NULL);
}

//TODO: uhhh this function originally only returned false
template <class T>
bool BlockingCQueue<T>::setCapacity(size_type capacity)
{
	if (capacity < 1)
	{
		return false;
	}
	else
	{
		m_capacity = capacity;
		reset();
	}
	return true;
}


template <class T>
size_type BlockingCQueue<T>::getCapacity()
{
	return m_capacity;
}

template <class T>
bool BlockingCQueue<T>::insert(value_type& item)
{
	if (m_writeLock)
		return false;
	pthread_mutex_lock(&m_mutex);
	while (is_full())
		pthread_cond_wait(&m_not_full, &m_mutex);
	//Insert the item
	if (m_container.size() < m_capacity)
	{
		m_container.push_back(item);
	}
	else
	{
		m_container[m_writeIndex] = item;
	}
	m_writeIndex = (m_writeIndex+1) % m_capacity;
	++m_unread;
	pthread_mutex_unlock(&m_mutex);
	pthread_cond_signal(&m_not_empty);
	return true;
}


template <class T>
bool BlockingCQueue<T>::beginInsert(value_type& item)
{
	if (m_writeLock)
		return false;
	pthread_mutex_lock(&m_mutex);
	while (is_full())
		pthread_cond_wait(&m_not_full, &m_mutex);
	//Insert the item
	item = m_container[m_writeIndex];
	pthread_mutex_unlock(&m_mutex);
	return true;
}

template <class T>
bool BlockingCQueue<T>::endInsert(value_type& item)
{
	if (m_writeLock)
		return false;
	//Insert the item
	m_container[m_writeIndex] = item;
	m_writeIndex = (m_writeIndex+1) % m_capacity;
	++m_unread;
	pthread_cond_signal(&m_not_empty);
	return true;
}

template <class T>
bool BlockingCQueue<T>::remove(value_type& item)
{
	pthread_mutex_lock(&m_mutex);
	bool result = false;
	while (is_empty() && !m_writeLock)
		pthread_cond_wait(&m_not_empty, &m_mutex);
	//remove the item
	if (!is_empty())
	{
		item = m_container[m_readIndex];
		m_readIndex = (m_readIndex + 1) % m_capacity;
		--m_unread;
		result = true;
	}

	pthread_mutex_unlock(&m_mutex);

	if (m_unread == 0){
		pthread_cond_signal(&m_empty);
	}
	pthread_cond_signal(&m_not_full);
	return result;
}

template <class T>
bool BlockingCQueue<T>::beginRemove(value_type& item)
{
	pthread_mutex_lock(&m_mutex);
	bool result = false;
	while (is_empty() && !m_writeLock)
		pthread_cond_wait(&m_not_empty, &m_mutex);
	//remove the item
	if (!is_empty())
	{
		item = m_container[m_readIndex];
		//m_readIndex = (m_readIndex + 1) % m_capacity;
		result = true;
	}

	pthread_mutex_unlock(&m_mutex);
	//pthread_cond_signal(&m_not_full);
	return result;
}

template <class T>
bool BlockingCQueue<T>::endRemove(value_type& item)
{
	bool result = false;

	//remove the item
	if (!is_empty())
	{
		// Put the item back into the queue, in case the
		// data being pointed to by the handle was swapped
		// by the application using shared_ptr.swap()
		m_container[m_readIndex] = item;

		m_readIndex = (m_readIndex + 1) % m_capacity;
		--m_unread;
		result = true;
	}

	if (m_unread == 0){
		pthread_cond_signal(&m_empty);
	}

	pthread_cond_signal(&m_not_full);
	return result;
}

template <class T>
void BlockingCQueue<T>::clear()
{
//@todo
}

template <class T>
bool BlockingCQueue<T>::is_empty() const
{
	return m_unread == 0;
}


template <class T>
bool BlockingCQueue<T>::is_full() const
{
	return m_unread == m_capacity;
}

template <class T>
void BlockingCQueue<T>::reset()
{
	m_unread = m_readIndex = m_writeIndex = 0 ;
}

template <class T>
void BlockingCQueue<T>::writeLock()
{
	m_writeLock = true;
}

template <class T>
void BlockingCQueue<T>::writeUnlock()
{
	m_writeLock = false;
}

template <class T>
bool BlockingCQueue<T>::isWriteLocked()
{
	return m_writeLock;
}

///////////////////////////////////////////////////////////////////////
}//end namespace
#endif /*BLOCKINGCQUEUE_H_*/
