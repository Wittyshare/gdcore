/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef GD_XML_H__
#define GD_XML_H__    1

/*!
   \file gdXml.h
   \brief Xml facilities functions

   xmlNode to double, long and int conversions.
 */

#include <string>

#include <libxml/parser.h>
#include <libxml/tree.h>

/*! \brief Convert a xmlNode* to a boolean. If failure return the default value.

 */
bool   getXmlBool(xmlNode* xmlNode, bool defVal);

/*! \brief Convert a xmlNode properties to a boolean. If failure return the default value.

 */
bool   getXmlBoolProp(xmlNode* xmlNode, const char* xmlLabel, bool defVal);

/*! \brief Convert a xmlNode* to a double. If failure return the default value.

 */
double getXmlDouble(xmlNode* xmlNode, double defVal);

/*! \brief Convert a xmlNode properties to a double. If failure return the default value.

 */
double getXmlDoubleProp(xmlNode* xmlNode, const char* xmlLabel, double defVal);

/*! \brief Convert a xmlNode* to a long. If failure return the default value.

 */
long   getXmlLong(xmlNode* xmlNode, long defVal);

/*! \brief Convert a xmlNode properties to a long. If failure return the default value.

 */
long getXmlLongProp(xmlNode* xmlNode, const char* xmlLabel, long defVal);

/*! \brief Convert a xmlNode* to an integer. If failure return the default value.

 */
int    getXmlInt (xmlNode* xmlNode, int defVal);

/*! \brief Convert a xmlNode properties to a int. If failure return the default value.

 */
int getXmlIntProp(xmlNode* xmlNode, const char* xmlLabel, int defVal);

/*! \brief Convert a xmlNode* to a char*. If failure return the default value.

 */
std::string getXmlStr(xmlNode* xmlNode, const char* defVal);

/*! \brief Convert a xmlNode properties to a string. If failure return the default value.

 */
std::string getXmlStringProp(xmlNode* xmlNode, const char* xmlLabel, std::string defVal);


#endif // ifndef GD_XML_H__

