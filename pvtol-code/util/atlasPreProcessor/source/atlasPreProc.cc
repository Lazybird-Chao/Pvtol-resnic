/**
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   main() for the Atlas PreProcessor application
 *
 *  $Id: atlasPreProc.cc 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#include <PvtolObjects.h>
#include <atlasPreProc.h>
#include <atlasPreProcUtils.h>
#include <atlasStrings.h>

#include <vector>

/// Contains all of the strings needed by the Atlas PreProcessor
atlasStrings *g_atlasStr;

/**
 * \brief The main function for the Atlas PreProcessor
 * \param argc The number of command line arguments.[in]
 * \param argv An array of C strings of the arguments*.[in]
 * \return The return value for the function
 */
int main(int argc, char* argv[])
{
  /// Initialize the Xerces XML system
  try
  {
     XMLPlatformUtils::Initialize();
  }

  catch (const XMLException& toCatch)
  {
     XERCES_STD_QUALIFIER cerr << "Error during initialization! :\n"
        << XMLString::transcode(toCatch.getMessage())
        << XERCES_STD_QUALIFIER endl;
     exit(1);
  }

  g_atlasStr = new atlasStrings();

  /// All of the useful info from the input arguments is put here
  userInputVars userVar;

  /// Since multiple input files are possible, we create a STL vector
  /// to hold each one
  std::vector<DOMDocument *> DOMDocVec;

  /// pObjects will be the object that is serialized to the Atlas
  PvtolObjects *pObjects = NULL;

  /// Parse the input arguments and extract the useful information
  userVar = parseInputArgs(argc,argv);

  /// Open and process the default XML document, if there is one
  if(userVar.defaultFile != "")
  {
     DOMDocVec.push_back(openXMLDocument(userVar.defaultFile));
  }

  /// Open remaining XML document
  /// TODO: only processes the first one; need to add loop here
  DOMDocVec.push_back(openXMLDocument(userVar.inputFile));

  /// Parse the XML DOM tree and generate the Pvtol Objects
  pObjects = generatePvtolObjects(DOMDocVec);

  /// Write the objects out to a file
  if(userVar.outputFile != "")
  {
     serializePvtolObjects(*pObjects, userVar.outputFile);
  }

  /// Call the termination method
  XMLPlatformUtils::Terminate();

  return 0;
} /* end main() */
