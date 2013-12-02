/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#include <string.h>

#include "gdXml.h"

double getXmlDouble(xmlNode* xmlNode, double defVal)
{
  double          ret;
  xmlChar*        content = xmlNodeGetContent(xmlNode);
  if ( content ) ret = atof((char*) content);
  else          ret = defVal;
  if ( content ) xmlFree(content);
  return ret;
}

double getXmlDoubleProp(xmlNode* xmlNode, const char* xmlLabel, double defVal)
{
  double          value = defVal;
  if ( xmlHasProp(xmlNode, (xmlChar*) xmlLabel) ) {
    xmlChar* xc = xmlGetProp(xmlNode, (xmlChar*) xmlLabel);
    value = atof((const char*) xc);
    xmlFree(xc);
  }
  return value;
}

long getXmlLong(xmlNode* xmlNode, long defVal)
{
  return (long) getXmlDouble(xmlNode, (double) defVal);
}

long getXmlLongProp(xmlNode* xmlNode, const char* xmlLabel, long defVal)
{
  return (long) getXmlDoubleProp(xmlNode, xmlLabel, (double) defVal);
}

int getXmlInt(xmlNode* xmlNode, int  defVal)
{
  return (int)  getXmlLong  (xmlNode, (long)   defVal);
}

int getXmlIntProp(xmlNode* xmlNode, const char* xmlLabel, int  defVal)
{
  return (int)  getXmlLongProp  (xmlNode, xmlLabel, (long)   defVal);
}

bool getXmlBool(xmlNode* xmlNode, bool defVal)
{
  bool            ret = defVal;
  xmlChar*        content = xmlNodeGetContent(xmlNode);
  if ( content ) {
    if ( !strcmp((const char*) content, "1") || !strcmp((const char*) content, "true") )
      ret = true;
    xmlFree(content);
  }
  return ret;
}

bool getXmlBoolProp(xmlNode* xmlNode, const char* xmlLabel, bool defVal)
{
  bool            value = defVal;
  if ( xmlHasProp(xmlNode, (xmlChar*) xmlLabel) ) {
    xmlChar* xc = xmlGetProp(xmlNode, (xmlChar*) xmlLabel);
    if ( !strcmp((const char*) xc, "1") || !strcmp((const char*) xc, "true") )
      value = true;
    xmlFree(xc);
  }
  return value;
}

std::string getXmlStr(xmlNode* xmlNode, const char* defVal)
{
  std::string str(defVal);
  xmlChar* content = xmlNodeGetContent(xmlNode);
  if ( content ) {
    str = (char*) content;
    xmlFree(content);
  }
  return str;
}

std::string getXmlStringProp(xmlNode* xmlNode, const char* xmlLabel, std::string defVal)
{
  std::string     value = defVal;
  if ( xmlHasProp(xmlNode, (xmlChar*) xmlLabel) ) {
    xmlChar* xc = xmlGetProp(xmlNode, (xmlChar*) xmlLabel);
    value = (char*) xc;
    xmlFree(xc);
  }
  return value;
}

