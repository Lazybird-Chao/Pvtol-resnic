/**
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Contains the error handlers for the XML parser
 *
 *  $Id: atlasPreProcErrorHandler.cc 938 2009-02-18 17:39:52Z ka21088 $
 *
 */

#ifndef CSP_NO_NAMESPACE
using std::cerr;
#endif /* CSP_NO_NAMESPACE */

#include <xercesc/sax/SAXParseException.hpp>
#include <atlasPreProcErrorHandler.h>

#include <stdlib.h>
#include <string.h>
#include <iostream>

preproc::preproc() : m_fSawErrors(false)
{
}

preproc::~preproc()
{
}

// ---------------------------------------------------------------------------
//  preprocHandlers: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
void preproc::error(const SAXParseException& e)
{
   m_fSawErrors = true;
   std::cerr << "\nError at file " << XMLString::transcode(e.getSystemId())
      << ", line " << e.getLineNumber() << ", char " << e.getColumnNumber()
      << "\n  Message: " << XMLString::transcode(e.getMessage()) << std::endl;
   exit(1);
}

void preproc::fatalError(const SAXParseException& e)
{
   m_fSawErrors = true;
   std::cerr << "\nFatal Error at file "
      << XMLString::transcode(e.getSystemId()) << ", line " << e.getLineNumber()
      << ", char " << e.getColumnNumber()
      << "\n  Message: " << XMLString::transcode(e.getMessage()) << std::endl;
   exit(1);
}

void preproc::warning(const SAXParseException& e)
{
   std::cerr << "\nWarning at file " << XMLString::transcode(e.getSystemId())
      << ", line " << e.getLineNumber() << ", char " << e.getColumnNumber()
      << "\n  Message: " << XMLString::transcode(e.getMessage()) << std::endl;
   exit(1);
}

void preproc::resetErrors()
{
}

