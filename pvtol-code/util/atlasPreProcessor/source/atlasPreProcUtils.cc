/**
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Contains the functions that main() calls
 *
 *  $Id: atlasPreProcUtils.cc 938 2009-02-18 17:39:52Z ka21088 $
 *
 */

#include <atlasPreProc.h>
#include <atlasPreProcErrorHandler.h>
#include <atlasPreProcUtils.h>
#include <atlasStrings.h>
#include <ctype.h>
#include <string>
#include <sstream>
#include <fstream>

//using namespace std;

#define LOCAL_DEBUG 1
const int MATCH = 0;

static XercesDOMParser::ValSchemes gValScheme = XercesDOMParser::Val_Auto;

extern atlasStrings *g_atlasStr;

/**
 * \param argc Number of command line arguments.[in]
 * \param argv The command line arguments.[in]
 * \return A struct containing the information that was passed in on the
 *         command line
 *
 * Since there are lots of ways of running the Atlas PreProcessor, this
 * function encapsulates all of the available options into one struct that is
 * more friendly to other functions.
 * 
 */
userInputVars parseInputArgs(int argc, char *argv[])
{

   userInputVars userVar;
   std::string optionType;
   std::string inFileName;
   std::string outFileName;
   std::string defFileName;

   std::string Useage("AtlasPP -h\n"
         "\tAtlasPP -R inputFile\n"
         "\tAtlasPP -v inputFile outputFile\n"
         "\tAtlasPP -Rv inputFile\n"
         "\tAtlasPP inputFile outputFile\n"
         "\tAtlasPP -Rd defaultFile inputFile\n"
         "\tAtlasPP -vd defaultFile inputFile outputFile\n"
         "\tAtlasPP -Rvd defaultFile inputFile\n"
         "\tAtlasPP -d defaultFile inputFile outputFile\n");

   if(argc > 1 && argc < 6)
   {
      std::string options(argv[1]);
      if(options.size() > 1)
      {
         if(options[0] == '-' && options[1] == 'h'
               && options.size() == 2)
         {
            std::cout << "\nNAME" << std::endl << std::endl;
            std::cout << "AtlasPP - Atlas preprocessor"
               << std::endl << std::endl;
            std::cout << "SYNOPSIS" << std::endl << std::endl;
            std::cout << "atlasp [-hRv[d defaultFile] ] inputFile outputFile"
               << std::endl << std::endl;
            std::cout << "OPTIONS" << std::endl << std::endl;
            std::cout << "-h Show help." << std::endl;
            std::cout << "-R Report only Parse the file while checking"
               << std::endl;
            std::cout << "               syntax, print the contents of"
               << std::endl;
            std::cout << "               the map folder when done, but do"
               << std::endl;
            std::cout << "               not produce an output file."
               << std::endl;
            std::cout << "-v verbose     List map names and related information"
               << std::endl;
            std::cout << "               as each map is processed."
               << std::endl;
            std::cout << "-d defaultFile Path/name of file containing some"
               << std::endl;
            std::cout << "               default maps and map pieces."
               << std::endl;
            std::cout << "inputFile      Atlas file to be parsed."
               << std::endl;
            std::cout << "outputFile     Name of the binary output file."
               << std::endl << std::endl;
            exit(1);        
         }
         else 
         {
            if(options[0] == '-' &&
               options[1] == 'R' &&
               options.size() == 2)
            {
               if(argc == 3){
                  defFileName = "";
                  inFileName = argv[2];
                  outFileName = "";
                  optionType = "R";
               }
               else
               {
                  std::cout << "useage: " << Useage << std::endl;
                  exit(1);
               }
            }
            else 
            {
               if(options[0] == '-' &&
                  options[1] == 'R' &&
                  options[2] == 'v' &&
                  options.size() == 3)
               {
                  if(argc == 3){
                     defFileName = "";
                     inFileName = argv[2];
                     outFileName = "";
                     optionType = "Rv";
                  }
                  else
                  {
                     std::cout << "useage: " << Useage << std::endl;
                     exit(1);
                  }
               }
               else 
               {
                  if(options[0] == '-' &&
                     options[1] == 'v' &&
                     options.size() == 2)
                  {
                     if(argc == 4){
                        defFileName = "";
                        inFileName = argv[2];
                        outFileName = argv[3];
                        optionType = "v";
                     }
                     else
                     {
                        std::cout << "useage: " << Useage << std::endl;
                        exit(1);
                     }
                  }
                  else
                  {
                     if(options[0] == '-' &&
                        options[1] == 'R' &&
                        options[2] == 'd' &&
                        options.size() == 3)
                     {
                        if(argc == 4){
                           defFileName = argv[2];
                           inFileName = argv[3];
                           outFileName = "";
                           optionType = "Rd";
                        }
                        else
                        {
                           std::cout << "useage: " << Useage << std::endl;
                           exit(1);
                        }
                     }
                     else 
                     {
                        if(options[0] == '-' &&
                           options[1] == 'R' &&
                           options[2] == 'v' &&
                           options[3] == 'd' &&
                           options.size() == 4)
                        {
                           if(argc == 5){
                              defFileName = argv[2];
                              inFileName = argv[3];
                              outFileName = "";
                              optionType = "Rvd";
                           }
                           else
                           {
                              std::cout << "useage: " << Useage << std::endl;
                              exit(1);
                           }
                        }
                        else 
                        {
                           if(options[0] == '-' &&
                              options[1] == 'v' &&
                              options[2] == 'd' &&
                              options.size() == 3)
                           {
                              if(argc == 5){
                                 defFileName = argv[2];
                                 inFileName = argv[3];
                                 outFileName = argv[4];
                                 optionType = "vd";
                              }
                              else
                              {
                                 std::cout << "useage: " << Useage << std::endl;
                                 exit(1);
                              }
                           }
                           else
                           {
                              if(options[0] == '-' &&
                                    options[1] == 'd' &&
                                    options.size() == 2)
                              {
                                 if(argc == 5){
                                    defFileName = argv[2];
                                    inFileName = argv[3];
                                    outFileName = argv[4];
                                    optionType = "d";
                                 }
                                 else
                                 {
                                    std::cout << "useage: " << Useage
                                       << std::endl;
                                    exit(1);
                                 }
                              }
                              else
                              {
                                 if(argc == 3)
                                 {
                                    defFileName = "";
                                    inFileName = argv[1];
                                    outFileName = argv[2];
                                    optionType = "";
                                 }
                                 else
                                 {
                                    std::cout << "useage: " << Useage
                                       << std::endl;
                                    exit(1);
                                 }
                              }
                           }
                        }
                     }
                  }
               }
            }
         }
      }
      else
      { 
         //std::cout << "No arguments to parse" << std::endl;
         std::cout << "useage: " << Useage << std::endl; 
         exit(1);  
      }
   }
   else
   {  
      //std::cout << "argc out of range" << std::endl;
      std::cout << "useage: " << Useage << std::endl;
      exit(1);
   }

   userVar.defaultFile = defFileName;
   userVar.inputFile = inFileName;
   userVar.outputFile = outFileName;
   userVar.options = optionType;

   return userVar;
}


/**
 * \param DOMDocList std::vector of DOM Documents.[in]
 * \return PvtolObjects object containing all of the PVTOL objects specified
 *         in all of the DOM Documents
 *
 * Creates a new XML parser and parses the file specified by the filename.
 * One parser object is created for each file that parsed and is never
 * destroyed. Technically a memory leak, but not too big of a problem because
 * of the short duration of the program.
 */
DOMDocument* openXMLDocument(std::string fileName)
{

   const char *XMLFile = 0;

   XMLFile = fileName.c_str();

   /// Instantiate the DOM parser.
   XercesDOMParser *parser = new XercesDOMParser;
   parser->setValidationScheme(gValScheme);

   /// And create our error handler and install it
   preproc preprocErrorHandler;
   parser->setErrorHandler(&preprocErrorHandler);

   try
   {
      std::cout << "\nTrying to parse file: " << XMLFile << std::endl << std::endl;
      parser->parse(XMLFile);
   }
   catch (const XMLException& toCatch)
   {
      std::cerr << "\nError during parsing: '" << XMLFile << "'\n"
         << "Exception message is:  \n"
         << XMLString::transcode(toCatch.getMessage()) << "\n" << std::endl;
      exit(1);
   }
   catch (const DOMException& toCatch)
   {
      const unsigned int maxChars = 2047;
      XMLCh errText[maxChars + 1];

      std::cerr << "\nDOM Error during parsing: '" << XMLFile << "'\n"
         << "DOMException code is:  " << toCatch.code << std::endl;

      if (DOMImplementation::loadDOMExceptionMsg(toCatch.code, errText, maxChars))
         std::cerr << "Message is: " << XMLString::transcode(errText) << std::endl;

      XMLPlatformUtils::Terminate();
      exit(1);
   }
   catch (...)
   {
      std::cerr << "\nUnexpected exception during parsing: '" << XMLFile << "'\n";
      XMLPlatformUtils::Terminate();
      exit(1);
   }

   DOMDocument *mapSpecs = parser->getDocument();

   /// Make sure that this file contains PVTOL maps
   DOMDocumentType *docType = mapSpecs->getDoctype();
   if(!XMLString::equals(docType->getName(), g_atlasStr->PVTOL_MAP_ROOT))
   {
      std::cout << "Expected a PvtolMap or DefaultPvtolMap file, received a " << docType->getName() << " file." << std::endl;
      exit(1);
   }

   return mapSpecs;
}

/**
 * \param DOMDocList std::vector of DOM Documents.[in]
 * \return PvtolObjects object containing all of the PVTOL objects specified
 *         in all of the DOM Documents
 *
 * Parses each DOM Document and places the information into a PvtolObjects
 * object.
 */
PvtolObjects* generatePvtolObjects(std::vector<DOMDocument *> DOMDocList)
{
   std::vector<DOMDocument *>::iterator DOMDocIter;
   PvtolObjects *objects = new PvtolObjects();
   DOMNodeList *nodeList = NULL;
   //DOMNodeList *childList = NULL;
   DOMNode *rootNode = NULL;
   DOMElement *currentElement = NULL;
   //DOMText *textNode = NULL;
   unsigned int ii;
   //unsigned int jj;
   std::string name;
   std::string dimString;
   std::istringstream str2num;
   pvtol::Grid *tempGrid;
   pvtol::RankList *tempRankList;
   pvtol::TaskMap *tempTaskMap;
   taskMapInfoType tempTaskMapInfo;
   vector<int> tempList;
   vector<taskMapInfoType> taskMapInfoList;
   //unsigned int childListLen;
   //int tempInt;

   // parse each input file and create the fake objects
   for(DOMDocIter = DOMDocList.begin(); DOMDocIter != DOMDocList.end(); DOMDocIter++)
   {
      std::cout << "parsing a file" << std::endl;

      // find the root node for the XML file -- should be "PvtolMaps"
      rootNode = (*DOMDocIter)->getElementsByTagName(g_atlasStr->PVTOL_MAP_ROOT)->item(0);
#if LOCAL_DEBUG
      std::cout << "rootNode: " << XMLString::transcode(rootNode->getNodeName()) << std::endl;
#endif

      // get a list of all of the children
      nodeList = rootNode->getChildNodes();
#if LOCAL_DEBUG
      for(ii = 0; ii < nodeList->getLength(); ii++)
      {
         std::cout << "Child Nodes and type #: " << XMLString::transcode(nodeList->item(ii)->getNodeName()) << " " << nodeList->item(ii)->getNodeType() << std::endl;
      }
#endif

      for(ii = 0; ii < nodeList->getLength(); ii++)
      {
         // we only want to work with the elements (not comments or anything else)
         if(nodeList->item(ii)->getNodeType() == DOMNode::ELEMENT_NODE)
         {
#if LOCAL_DEBUG
            std::cout << "element Nodes and type #: " << XMLString::transcode(nodeList->item(ii)->getNodeName()) << " " << nodeList->item(ii)->getNodeType() << std::endl;
#endif
            currentElement = dynamic_cast<DOMElement *> (nodeList->item(ii));
#if LOCAL_DEBUG
            std::cout << "Current element name: " << XMLString::transcode(currentElement->getTagName()) << std::endl;
#endif
            /// Grid
            /// Grid
            /// Grid
            if(XMLString::compareIString(currentElement->getTagName(), g_atlasStr->GRID) == MATCH)
            {
               processGrid(currentElement, name, &tempGrid);
               objects->storeNewGrid(name, tempGrid);
               objects->printGrid();
            }

            /// RankList
            /// RankList
            /// RankList
            if(XMLString::compareIString(currentElement->getTagName(), g_atlasStr->RANK_LIST) == MATCH)
            {
               processRankList(currentElement, name, &tempRankList);
               objects->storeNewRankList(name, tempRankList);
               objects->printRankList();
            }

            /// TaskMap
            /// TaskMap
            /// TaskMap
            if(XMLString::compareIString(currentElement->getTagName(), g_atlasStr->TASK_MAP) == MATCH)
            {
               processTaskMap(currentElement, tempTaskMapInfo);
               taskMapInfoList.push_back(tempTaskMapInfo);
#if 0
#if LOCAL_DEBUG
               std::cout << "Processing element name: " << XMLString::transcode(currentElement->getTagName()) << std::endl;
#endif
               // clear the vector
               tempTaskMapInfo.rankListString = "";
               tempTaskMapInfo.gridString = "";
               tempTaskMapInfo.taskDistDescString = "";

               // get the name of the RankList element
               tempTaskMapInfo.taskMapName = XMLString::transcode(currentElement->getAttribute(g_atlasStr->NAME));

#if LOCAL_DEBUG
               std::cout << "TaskMap name: " << name << std::endl;
#endif

               // get a list of the children
               childList = currentElement->getElementsByTagName(g_atlasStr->ALL);
               childListLen = childList->getLength();
#if LOCAL_DEBUG
               for(jj = 0; jj < childListLen; jj++)
               {
                  std::cout << "TaskMap child Nodes and type #: " << XMLString::transcode(childList->item(jj)->getNodeName()) << " " << childList->item(jj)->getNodeType() << std::endl;
               }
#endif

               for(jj = 0; jj < childListLen; jj++)
               {
                  if(XMLString::compareIString(childList->item(jj)->getNodeName(), g_atlasStr->GRID) == MATCH)
                  {
                     textNode = dynamic_cast<DOMText *> (childList->item(jj)->getFirstChild());
                     tempTaskMapInfo.gridString = XMLString::transcode(textNode->getData());
                  }
                  else
                  {
                     if(XMLString::compareIString(childList->item(jj)->getNodeName(), g_atlasStr->RANK_LIST) == MATCH)
                     {
                        textNode = dynamic_cast<DOMText *> (childList->item(jj)->getFirstChild());
                        tempTaskMapInfo.rankListString = XMLString::transcode(textNode->getData());
                     }
                     else
                     {
                        if(XMLString::compareIString(childList->item(jj)->getNodeName(), g_atlasStr->TASK_DIST_DESC) == MATCH)
                        {
                           textNode = dynamic_cast<DOMText *> (childList->item(jj)->getFirstChild());
                           tempTaskMapInfo.taskDistDescString = XMLString::transcode(textNode->getData());
                        }
                        else
                        {
                        }
                     }
                  }
#if LOCAL_DEBUG
                  std::cout << "TaskMap : " << XMLString::transcode(textNode->getData()) << std::endl;
#endif
               }
               taskMapInfoList.push_back(tempTaskMapInfo);
#endif
            }
         }
      }
   }

   // fully populate and store off the objects

   std::cout << "now consolidating TaskMap" << std::endl;

   /// TaskMap
   /// TaskMap
   /// TaskMap
   vector<taskMapInfoType>::iterator taskMapInfoListIter;

   for(taskMapInfoListIter = taskMapInfoList.begin();
       taskMapInfoListIter != taskMapInfoList.end();
       taskMapInfoListIter++)
   {
      if(taskMapInfoListIter->taskDistDescString != "")
      {
         //tempTaskMap = new pvtol::TaskMap(objects->getRankList(taskMapInfoListIter->rankListString),
               //objects->getTaskDistDesc(taskMapInfoListIter->taskDistDescString));
         //objects->storeNewTaskMap(taskMapInfoListIter->taskMapName, tempTaskMap);
         //objects->printTaskMap();
      }
      else
      {
         if(taskMapInfoListIter->rankListString == "")
         {
            tempTaskMap = new pvtol::TaskMap(taskMapInfoListIter->rankList);
            objects->storeNewTaskMap(taskMapInfoListIter->taskMapName, tempTaskMap);
            objects->printTaskMap();
         }
         else
         {
            tempTaskMap = new pvtol::TaskMap(*(objects->getRankList(taskMapInfoListIter->rankListString)));
            objects->storeNewTaskMap(taskMapInfoListIter->taskMapName, tempTaskMap);
            objects->printTaskMap();
         }
      }
   }

   return(objects);
}

/**
 * \param pObjects The Pvtol Objects to serialize.[in]
 * \param fileName The name of the output file.[in]
 *
 * This function serializes the Pvtol Objects that are passed into it
 */
void serializePvtolObjects(const PvtolObjects &pObjects, std::string fileName)
{
   std::ofstream outFile;

   outFile.open(fileName.c_str());

   if(outFile == NULL)
   {
      std::cout << "couldn't open file for writing: " << fileName << std::endl;
      exit(1);
   }

   boost::archive::text_oarchive oa(outFile);

   /// now save the std::map containers
   oa << pObjects;

   /// outFile is closed when destructors are called.

   std::cout << "objects have been serialized to: " << fileName << std::endl;
}

void processGrid(DOMElement * element,
      std::string &name,
      pvtol::Grid **tempGrid)
{
   vector<int> tempList;
   DOMNodeList *childList = NULL;
   unsigned int childListLen;
   unsigned int ii;
   DOMText *textNode = NULL;
   std::string dimString;
   std::istringstream str2num;
   int tempInt;

#if LOCAL_DEBUG
   std::cout << "Processing element name: " << XMLString::transcode(element->getTagName()) << std::endl;
#endif
   // get the name of the Grid element
   name = XMLString::transcode(element->getAttribute(g_atlasStr->NAME));

#if LOCAL_DEBUG
   std::cout << "Grid name: " << name << std::endl;
#endif

   // get a list of the children
   childList = element->getElementsByTagName(g_atlasStr->ALL);
   childListLen = childList->getLength();

#if LOCAL_DEBUG
   for(ii = 0; ii < childListLen; ii++)
   {
      std::cout << "Grid child Nodes and type #: " << XMLString::transcode(childList->item(ii)->getNodeName()) << " " << childList->item(ii)->getNodeType() << std::endl;
   }
#endif

   if(childListLen > 3)
   {
      std::cout << "Grid does not support " << childListLen << " dimensions" << std::endl;
   }

   for(ii = 0; ii < childListLen; ii++)
   {
      textNode = dynamic_cast<DOMText *> (childList->item(ii)->getFirstChild());
      dimString = XMLString::transcode(textNode->getData());
      str2num.str(dimString);
      str2num >> tempInt;
      str2num.clear();
      tempList.push_back(tempInt);
#if LOCAL_DEBUG
      std::cout << "Grid Dimension: " << tempInt << std::endl;
#endif
   }

   // a Grid must be fully specified, so we can just create the object
   // now that we have all of the information
   if(childListLen == 1)
   {
      *tempGrid = new pvtol::Grid(tempList[0]);
   }
   else
   {
      if(childListLen == 2)
      {
         *tempGrid = new pvtol::Grid(tempList[0], tempList[1]);
      }
      else
      {
         if(childListLen == 3)
         {
            *tempGrid = new pvtol::Grid(tempList[0], tempList[1], tempList[2]);
         }
         else
         {
            std::cout << "Grid does not support " << childListLen << " dimensions" << std::endl;
         }
      }
   }
}

void processRankList(DOMElement * element,
      std::string &name,
      pvtol::RankList **tempRankList)
{
   vector<int> tempList;
   DOMNodeList *childList = NULL;
   unsigned int childListLen;
   unsigned int ii;
   DOMText *textNode = NULL;
   std::string dimString;
   std::istringstream str2num;
   int tempInt;

#if LOCAL_DEBUG
   std::cout << "Processing element name: " << XMLString::transcode(element->getTagName()) << std::endl;
#endif
   // get the name of the RankList element
   name = XMLString::transcode(element->getAttribute(g_atlasStr->NAME));

   // clear the vector
   tempList.clear();

#if LOCAL_DEBUG
   std::cout << "RankList name: " << name << std::endl;
#endif

   // get a list of the children
   childList = element->getElementsByTagName(g_atlasStr->ALL);
   childListLen = childList->getLength();
#if LOCAL_DEBUG
   for(ii = 0; ii < childListLen; ii++)
   {
      std::cout << "RankList child Nodes and type #: " << XMLString::transcode(childList->item(ii)->getNodeName()) << " " << childList->item(ii)->getNodeType() << std::endl;
   }
#endif

   for(ii = 0; ii < childListLen; ii++)
   {
      textNode = dynamic_cast<DOMText *> (childList->item(ii)->getFirstChild());
      dimString = XMLString::transcode(textNode->getData());
      str2num.str(dimString);
      str2num >> tempInt;
      str2num.clear();
      tempList.push_back(tempInt);
#if LOCAL_DEBUG
      std::cout << "RankList Dimension: " << tempInt << std::endl;
#endif
   }

   // a RankList must be fully specified,
   // so we can just create the object
   // now that we have all of the information
   *tempRankList = new pvtol::RankList(tempList);
}

void processTaskMap(DOMElement * element,
      taskMapInfoType &taskMapInfo)
{
   std::string name;
   vector<int> tempList;
   DOMNodeList *childList = NULL;
   DOMNodeList *gridList = NULL;
   DOMNodeList *rankListList = NULL;
   DOMElement *gridElement = NULL;
   DOMElement *rankListElement = NULL;
   unsigned int childListLen;
   unsigned int gridListLen;
   unsigned int rankListListLen;
   unsigned int ii;
   unsigned int jj;
   DOMText *textNode = NULL;
   std::string dimString;
   std::istringstream str2num;
   //int tempInt;
   pvtol::RankList *tempRankList;
   pvtol::Grid *tempGrid;

#if LOCAL_DEBUG
   std::cout << "Processing element name: " << XMLString::transcode(element->getTagName()) << std::endl;
#endif
   // clear the vector
   taskMapInfo.rankListString = "";
   taskMapInfo.gridString = "";
   taskMapInfo.taskDistDescString = "";

   // get the name of the RankList element
   taskMapInfo.taskMapName = XMLString::transcode(element->getAttribute(g_atlasStr->NAME));

#if LOCAL_DEBUG
   std::cout << "TaskMap name: " << taskMapInfo.taskMapName << std::endl;
#endif

   // get a list of the children
   childList = element->getElementsByTagName(g_atlasStr->ALL);
   childListLen = childList->getLength();
#if LOCAL_DEBUG
   for(ii = 0; ii < childListLen; ii++)
   {
      std::cout << "TaskMap child Nodes and type #: " << XMLString::transcode(childList->item(ii)->getNodeName()) << " " << childList->item(ii)->getNodeType() << std::endl;
   }
#endif

   // for each child under the TaskMap
   for(ii = 0; ii < childListLen; ii++)
   {
      if(XMLString::compareIString(childList->item(ii)->getNodeName(), g_atlasStr->GRID) == MATCH)
      {
         gridElement = dynamic_cast<DOMElement *> (childList->item(ii));
         gridList = gridElement->getElementsByTagName(g_atlasStr->ALL);
         gridListLen = gridList->getLength();
#if LOCAL_DEBUG
         for(jj = 0; jj < gridListLen; jj++)
         {
            std::cout << "Grid child Nodes and type #: " << XMLString::transcode(gridList->item(jj)->getNodeName()) << " " << gridList->item(jj)->getNodeType() << std::endl;
         }
#endif
         if(gridListLen > 0)
         {
            taskMapInfo.gridString = "";
            processGrid(gridElement, taskMapInfo.gridString, &tempGrid);
            taskMapInfo.grid = *tempGrid;
         }
         else
         {
            textNode = dynamic_cast<DOMText *> (childList->item(ii)->getFirstChild());
            taskMapInfo.gridString = XMLString::transcode(textNode->getData());
         }
      }
      else
      {
         if(XMLString::compareIString(childList->item(ii)->getNodeName(), g_atlasStr->RANK_LIST) == MATCH)
         {
            rankListElement = dynamic_cast<DOMElement *> (childList->item(ii));
            rankListList = rankListElement->getElementsByTagName(g_atlasStr->ALL);
            rankListListLen = rankListList->getLength();
#if LOCAL_DEBUG
            for(jj = 0; jj < rankListListLen; jj++)
            {
               std::cout << "RankList child Nodes and type #: " << XMLString::transcode(rankListList->item(jj)->getNodeName()) << " " << rankListList->item(jj)->getNodeType() << std::endl;
            }
#endif
            if(rankListListLen > 0)
            {
               taskMapInfo.rankListString = "";
               processRankList(rankListElement, taskMapInfo.rankListString, &tempRankList);
               taskMapInfo.rankList = *tempRankList;
            }
            else
            {
               textNode = dynamic_cast<DOMText *> (childList->item(ii)->getFirstChild());
               taskMapInfo.rankListString = XMLString::transcode(textNode->getData());
            }
         }
         else
         {
            if(XMLString::compareIString(childList->item(ii)->getNodeName(), g_atlasStr->TASK_DIST_DESC) == MATCH)
            {
               textNode = dynamic_cast<DOMText *> (childList->item(ii)->getFirstChild());
               taskMapInfo.taskDistDescString = XMLString::transcode(textNode->getData());
            }
            else
            {
            }
         }
      }
   }
}

