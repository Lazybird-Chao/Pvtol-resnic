/**
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Contains code for the error handler for Xerces. Not really sure
 *           why, but it uses SAX stuff to handle DOM parsing errors. The
 *           sample code that came with Xerces does it this way, so that is
 *           what is done here.
 *
 *  $Id: atlasPreProcErrorHandler.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */

#ifndef __atlasPreProcErrorHandler_h__
#define __atlasPreProcErrorHandler_h__

#include <atlasPreProc.h>

class preproc : public ErrorHandler
{
public:
    preproc();
    ~preproc();

    // Accessors
    bool getSawErrors() const;

    void warning(const SAXParseException& e);
    void error(const SAXParseException& e);
    void fatalError(const SAXParseException& e);
    void resetErrors();

private :
    preproc(const preproc&);
    void operator=(const preproc&);
    bool    m_fSawErrors;
};

inline bool preproc::getSawErrors() const
{
    return m_fSawErrors;
}

#endif
