/**
 * File: MulticastConduit.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *    January 2009: Ported from PVL by Jim Daly
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Class definition  & inline methods of MulticastConduit
 *            McInsertIf & McExtractIf
 *            the MulticastConduit class provides a means to multicast
 *              non-distributed data objects from a node in a source task
 *              to multiple nodes in a destination task.
 *
 *            The McInsertIf class provides a constrained API for the
 *            source end of a MulticastConduit.
 *            The McExtractIf class provides a constrained API for the
 *            destination end of a MulticastConduit.
 *
 *  $Id: MulticastConduit.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 * @author Eddie Rutledge
 *
 */

#ifndef PVTOL_MULTICASTCONDUIT_H
#define PVTOL_MULTICASTCONDUIT_H

#include <vector>
#include <string>

#include <Conduit.h>
#include <Vector.h>


namespace ipvtol
{
  using std::string;
  using std::vector;


  /* forward declarations needed for the MulticastConduit class */

  template <class DATATYPE> class McInsertIf;
  template <class DATATYPE> class McExtractIf;

  /**
   * The MulticastConduit class provides a means of multicasting the contents
   * of non-distributed data objects from a node of a source task to several
   * nodes of a destination task. The tasks may be co-mapped, partially 
   * co-mapped, or completely disjoint.
   *
   * The parameter type of a conduit will be a non-distributed object type 
   * that has no references, pointers, or virtual functions
   *
   * @author Eddie Rutledge
   *
   * */

  template <class DATATYPE>
  class MulticastConduit
  {
  public:
    typedef McInsertIf<DATATYPE> InsertIf;
    typedef McExtractIf<DATATYPE> ExtractIf;

  public:
    typedef DATATYPE DataType;

    // CONSTRUCTORS

    /**
     * @param name the name of the conduit
     *
     */
    MulticastConduit(const string& name);

    /**
     */
    virtual ~MulticastConduit() throw();

    // INITIALIZATION METHODS


    /**@name initialization methods
     * The purpose of the setup methods is to provide the information
     * needed to initialize a conduit. Each conduit creates
     * a source end object and a destination end object. The MulticastConduit
     * will internally create blocks based upon the 
     * information which is provided via these methods.
     *
     *Note that the methods in this section are used only during initialization.
     */
    //@{

  public:
    InsertIf getInsertIf(void);


  private:
    /**@name setupSrc() methods
     * The setupSrc() methods initialize the source end of a conduit.
     *
     * The MulticastConduit will internally create distributed blocks
     * based upon the maps which are given to setupSrc().
     *
     */
    //@{

    /**
     * @param srcMap the map that specifies which node will be the source
     * of the multicast.
     *
     * @param depth the multi-buffering depth for the source end of
     * the conduit
     */
    void
    setupSrc(const Map& srcMap, int depth);

    //@}

  public:
    ExtractIf getExtractIf(void);

  private:
    DataMap* makeDataMap(const Map& theMap);
    void mapToProcList(const Map& map, vector<int> & pl);

    /**@name setupDest() methods
     * The setupDest() methods initialize the destination end of a conduit.
     *
     * */
    //@{

    /**
     * @param destMap the map that specifies which nodes will be the 
     * destinations of the multicast.
     *
     * @param depth the multi-buffering depth for the source end of
     * the conduit
     *
     */
    void
    setupDest(const Map& destMap, int depth);

    //@}

  public:
    /**
     * setupComplete() must be called after setupSrc() and
     * setupDest() has been called. initComplete()
     * performs any communication which is necessary to distribute the
     * source and desitnation maps to the nodes where they are needed
     * and then initializes the conduit.
     *
     */
    void
    setupComplete();
    
    //@}

    // SOURCE END METHODS

    /**@name source end methods
     * the following methods are used only at the source end of the conduit.
     */
    //@{

  private:
    /**
     * Queries whether the conduit is able to provide the buffer space
     * needed for an outgoing transfer. 
     *
     * @return returns true when the source end
     * distributed data object has been set up in a free portion of
     * its underlying storage.
     * 
     */
    bool      insertAvailable();

    /**
     * Gives access to the data object which is mapped onto the next 
     * available buffer space. Note that getHandle() does not modify the 
     * contents of the returned data object therefore there is no guarantee 
     * that the initial contents of newly acquired buffer space has any 
     * particular value.  If necessary, the application should clear the 
     * data object before using it. This behavior is helpful in cases where 
     * the data which will be inserted into the conduit is being accumulated
     * over multiple executions of a task's run() method. As the data
     * is being accumulated, each execution of the run() method can
     * call getHandle() to obtain access to the data object.
     *
     * @return a reference to the conduit's data object.
     */
    DATATYPE& getInsertHandle();

    /**
     * Sends the current source buffer (the view of which was
     * returned by getHandle()) to the current destination buffer. The
     * buffer space which is being sent will be unavailable until the
     * source buffer is transferred to the destination buffer. Note
     * that this method kicks off any communications which are needed
     * to send the data to the destination.
     *
     */
    void      insert();

    //@}

    // DESTINATION END METHODS

    /**@name destination end methods
     * the following methods are used only at the source end of the conduit.
     */
    //@{

    /**
     * Queries whether the conduit has received new data.
     *
     * @return Returns true if the next input buffer has data
     * which has not yet been extracted.
     */
    bool      extractReady();

    /**
     * Gives access to the data object which provides a
     * view of the next available destination buffer space. Once
     * extractReady() has returned true, the contents of the
     * destination data object is valid and repeated calls
     * to getExtractHandle() will return a view of the same buffer
     * space.
     *
     * @return a reference to the conduit data object.  
     */
    DATATYPE& getExtractHandle();

    /**
     * Releases the buffer space so that it can be re-used by another
     * transfer.
     *
     */
    void release();

    //@}

  private:
    Conduit<Vector<int>, DATATYPE, false> m_internalCond;

  public:

    friend class McInsertIf<DATATYPE>;
    friend class McExtractIf<DATATYPE>;

  };//end class MulticastConduit


  /**
   * The McExtractIf class provides a constrained API for the
   * destination end of a conduit.
   *
   * The parameter type of a McExtractIf must be the same as the
   * parameter type of the MulticastConduit class which it is used with.
   *
   * @see MulticastConduit
   *
   * @author Eddie Rutledge
   *
   * */

  template < class DATATYPE >
  class McExtractIf 
  {
  private:
    MulticastConduit<DATATYPE>* m_conduit;

  public:

    // CONSTRUCTORS

    /**
     * Default constructor.
     *
     */
    McExtractIf(void)
    {
      m_conduit = NULL;
    };

    /**
     * @param cdt the conduit for which this McExtractIf is an 
     * interface
     */
    McExtractIf(MulticastConduit<DATATYPE>& cdt)
    {
      m_conduit = &cdt;
    };

    // ASSIGNMENT OPERATOR

    /**
     */
    McExtractIf& operator=(McExtractIf& dif)
    {
      m_conduit = dif.m_conduit;
      return dif;
    };

    /**@name initialization methods
     * The purpose of the setup methods is to provide the information
     * needed to initialize a conduit. Each conduit creates
     * a source end object and a destination end object.
     *
     *Note that the methods in this section are used only during initialization.
     */
    //@{

  public:

    /**
     * @param dstMap the map which specifies which node will be the 
     * destination of the multicast
     *
     * @param depth the multi-buffering depth for the source end of
     * the conduit
     */
    void
    setup(const Map& destMap, int depth)
    {
      m_conduit->setupDest(destMap, depth);
    };

    //@}


    /**@name runtime methods
     * The purpose of the runtime methods is to provide the means
     * to set up and perform transfers via the MulticastConduit.
     *
     *Note that the methods in this section are used only after
     *initialization is complete.
     */
    //@{

  public:

    /**
     * Queries whether the conduit has received new data.
     *
     * @return Returns true if the next input buffer has data
     * which has not yet been extracted.
     */
    bool ready()
    {
      return m_conduit->extractReady();
    };

    /**
     * Gives access to the data object which provides a
     * view of the next available destination buffer space. Once
     * extractReady() has returned true, the contents of the
     * destination data object is valid and repeated calls
     * to getExtractHandle() will return a view of the same buffer
     * space.
     *
     * @return a reference to the conduit distributed data object.  */
    DATATYPE& getHandle()
    {
      return m_conduit->getExtractHandle();
    };

    /**
     * Releases the buffer space so that it can be re-used by another
     * transfer.
     *
     */
    void release()
    {
      m_conduit->release();
    };
    //@}

  };//end class McExtractIf


  /**
   * The McInsertIf class provides a constrained API for the
   * source end of a MulticastConduit.
   *
   * The parameter type of a McInsertIf must be the same as the
   * parameter type of the MulticastConduit class which it is used with.
   *
   * @see MulticastConduit
   *
   * @author Eddie Rutledge
   */

  template <class DATATYPE>
  class McInsertIf 
  {
  private:
    MulticastConduit<DATATYPE>* m_conduit;

  public:
    // INITIALIZATION METHODS

    /**
     * Default constructor.
     *
     */
    McInsertIf(void)
    {
      m_conduit = NULL;
    };

    /**
     * @param cdt the conduit for which this McExtractIf is an 
     * interface
     */
    McInsertIf(MulticastConduit<DATATYPE>& cdt)
    {
      m_conduit = &cdt;
    };

    /**
     */
    McInsertIf& operator=(McInsertIf& sif) 
    {
      m_conduit = sif.m_conduit;
      return sif;
    };

    /**@name initialization methods
     * The purpose of the setup methods is to provide the information
     * needed to initialize a conduit. Each conduit creates
     * a source end object and a destination end object.
     *
     *Note that the methods in this section are used only during initialization.
     */
    //@{
  public:

    /**
     * @param srcMap the map which defines the distribution of the source
     * end of the conduit.
     *
     * @param depth the multi-buffering depth for the source end of
     * the conduit
     */
    void setup(const Map& srcMap, int depth)
    {
      m_conduit->setupSrc(srcMap, depth);
    };

    //@}


    /**@name runtime methods
     * The purpose of the runtime methods is to provide the means
     * to set up and perform transfers via the Conduit.
     *
     *Note that the methods in this section are used only after
     *initialization is complete.
     */
    //@{
  public:

    /**
     * Queries whether the conduit is able to provide the buffer space
     * needed for an outgoing transfer. 
     *
     * @return returns true when the source end data object has been set
     *     up in a free portion of its underlying storage.
     */
    bool available()
    {
      return m_conduit->insertAvailable();
    };

    /**
     * Gives access to the data object which provides a 
     * view of the next available buffer space. Note that getHandle()
     * does not modify the contents of the returned data object
     * therefore there is no guarantee that the initial contents of
     * newly acquired buffer space has any particular value.  If
     * necessary, the application should clear the data object before
     * using it. This behavior is helpful in cases where the data
     * which will be inserted into the conduit is being accumulated
     * over multiple executions of a task's run() method. As the data
     * is being accumulated, each execution of the run() method can
     * call getHandle() to obtain access to the data object.
     *
     * @return a reference to the conduit data object.
     */
    DATATYPE& getHandle()
    {
      return m_conduit->getInsertHandle();
    };

    /**
     * Sends the current source buffer (the view of which was
     * returned by getHandle()) to the current destination buffer. The
     * buffer space which is being sent will be unavailable until the
     * source buffer is transferred to the destination buffer. Note
     * that this method kicks off any communications which are needed
     * to send the data to the destination.
     * */
    void insert()
    {
      m_conduit->insert();
    };

    //@}
  };//end class McInsertIf

}//end namespace

#include <MulticastConduit.inl> 

#endif // PVTOL_MULTICASTCONDUIT_H
// End of MulticastConduit.h

