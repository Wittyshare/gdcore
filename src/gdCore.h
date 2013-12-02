/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef GD_CORE_H__
#define GD_CORE_H__    1

/*!
   \file gdCore.h
   \brief Basic functions.

   double to string and string manipulation.
 */

#include <string>
#include <vector>

#include <boost/filesystem/path.hpp>


/*! \brief round a double, force the precision to 'precision'.

 */
float  gdcore_rounddouble(const      float  theDouble, const unsigned precision);
double gdcore_rounddouble(const      double theDouble, const unsigned precision);
long double gdcore_rounddouble(const long double theDouble, const unsigned precision);

/*! \brief Convert a double to a string, force the precision to 'precision'.

 */
std::string gdcore_double2string(double theDouble, int precision);

/*! \brief Convert a string to a long.

 */
long gdcore_str2long(const std::string& str);


/*! \brief Convert a long to a string

 */
std::string gdcore_long2str(const long& l);


/*! \brief Split a string into 2 parts.

 */
void        gdcore_string_split(const std::string& str, std::string& left, std::string& right, char delimiter);


/*! \brief Split a string delimited into a vector.

 */
void        gd_core_string2vector(std::vector<std::string>& vect, const char* str, const char* delim);

/*! \brief Return a string without all left characters 'delimiter'.

 */
std::string gdcore_string_left_of(const std::string strSrc, char delimiter);

/*! \brief Erase all left characters 'delimiter' into the string given as parameter.

 */
void        gdcore_string_erase_left(std::string& strSrc, char delimiter);

/*! \brief Return a string without all right characters 'delimiter'.

 */
std::string gdcore_string_right_of(const std::string strSrc, char delimiter);

/*! \brief Erase all right characters 'delimiter' into the string given as parameter.

 */
void        gdcore_string_erase_right(std::string& strSrc, char delimiter);

/*! \brief Normalize a string that contains special characters like < > for html rendering  */
std::string gdcore_string2xhtml(const std::string& src);

/*! \brief Normalize a string that contains special characters like &gt; &lt; for html rendering  */
std::string gdcore_xhtml2string(const std::string& src);

/*! \brief test if path is hidden */
bool gdcore_isPathHidden(const boost::filesystem::path& fsPath);

/*! \brief test if path exist */
bool gdcore_isPathExist(const boost::filesystem::path& fsPath);

/*! \brief test if path is a directory */
bool gdcore_isPathDirectory(const boost::filesystem::path& fsPath);

/*! \brief test if path is a regular file */
bool gdcore_isPathFile(const boost::filesystem::path& fsPath);

/*! \brief test if path is a symbolic link */
bool gdcore_isPathSymLink(const boost::filesystem::path& fsPath);

/*! \brief convert binary data to an encoded base 64 std::string, new line after 72 characters */
std::string gdcore_b64enc_bin2text72(const char* pData, const size_t length);

/*! \brief convert binary data to an encoded base 64 std::string */
std::string gdcore_b64enc_bin2text(const char* pData, const size_t length);

/*! \brief load a file in memory */
int gdcore_file_content2string(const char* path, std::string& content);

#endif // ifndef GD_CORE_H__

