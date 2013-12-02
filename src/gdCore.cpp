/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/lexical_cast.hpp>

#include "gdCore.h"

using namespace boost::archive::iterators;

// http://stackoverflow.com/questions/7053538/how-do-i-encode-a-string-to-base64-using-only-boost
//insert line breaks every 72 characters, convert binary values ot base64 characters, retrieve 6 bit integers from a sequence of 8 bit bytes
typedef insert_linebreaks< base64_from_binary< transform_width< const char*, 6, 8 > > , 72 > b64enc_bin2text72;

// Without line break
typedef                    base64_from_binary< transform_width< const char*, 6, 8 > >        b64enc_bin2text;




float  gdcore_rounddouble(const float  theDouble, const unsigned precision)
{
  // On appelle la version long double
  const long double n = theDouble;
  return (float) gdcore_rounddouble(n, precision);
}

double gdcore_rounddouble(const double theDouble, const unsigned precision)
{
  // On appelle la version long double
  const long double n = theDouble;
  return (double) gdcore_rounddouble(n, precision);
}

long double gdcore_rounddouble(const long double theDouble, const unsigned precision)
{
  if ( !precision ) return roundl(theDouble); // Pas de soucis si 0 décimale, roundl arrondi a l'entier
  // Cette version risque un débordement dans le cas ou la valeur entiere est trop élevée.
  // Origine : http://bytes.com/topic/c/answers/739971-rounding-double
  if ( 0 ) {
    const long double ld = (long double) precision; // powl prend deux long double
    return (floorl(theDouble * powl(10.0L, ld) + 0.5L) / powl(10.0L, ld));
  }
  // Cette version risque aussi un débordement dans le cas ou la valeur entiere est trop élevée.
  if ( 0 ) {
    const long double ld = (long double) precision; // powl prend deux long double
    long double oper      = powl(10.0L, ld);           // 10 ^ ld (nombre de decimale), (^ = exposant)
    return (round(theDouble * oper) / oper);
  }
  // Celle ci par contre evite ce problème
  if ( 1 ) {
    const long double ld  = (long double) precision;   // powl prend deux long double
    long double intpart   = 0.0L;
    long double oper      = powl(10.0L, ld);           // 10 ^ ld (nombre de decimale), (^ = exposant)
    long double fractpart = modfl(theDouble, &intpart); // modfl retourne la partie entiere dans intpart et la mantisse (partie fractionnée) dans fractpart
    return ( (theDouble - fractpart) + (round(fractpart * oper) / oper) );
  }
  // Ancienne version avec sprintf, donne des résultats bizarre en 64bits
  char           buffer[ 64 + 1 ];
  int            i1;
  for (i1 = 0; i1 < 65; i1++) buffer[i1] = '\0';
  (void) sprintf(buffer, "%#32.*Lf", precision, theDouble);
  return ((long double) atof(buffer));
}

std::string gdcore_double2string(double theDouble, int precision)
{
  char    buffer[64 + 1];
  for (int i1 = 0 ; i1 < 65 ; i1++) buffer[i1] = 0;
  sprintf(buffer, "%#32.*f", precision, theDouble);
  char* pbuf = buffer;
  while ( *pbuf == ' ' ) pbuf++;
  return pbuf;
}

long gdcore_str2long(const std::string& str)
{
  return atol(str.c_str());
}

std::string gdcore_long2str(const long& l)
{
  return boost::lexical_cast<std::string>(l);
}

/* Split a string into two parts */
void gdcore_string_split(const std::string& str, std::string& left, std::string& right, char delimiter)
{
  std::string::size_type pos = str.find(delimiter);
  if ( pos != std::string::npos ) {
    left  = str.substr(0, pos);
    right = str.substr(pos + 1);
  } else {
    left = str;
    right = "";
  }
}

void gd_core_string2vector(std::vector<std::string>& vect, const char* str, const char* delim)
{
  char* str2 = strdup(str);
  char* token = strtok(str2, delim);
  while (token != NULL) {
    vect.push_back(token);
    token = strtok(NULL, delim);
  }
  free(str2);
}

// remove space before text content and return a new string, source still unchanged
std::string gdcore_string_left_of(const std::string strSrc, char delimiter)
{
  return strSrc.substr(strSrc.find_first_not_of(delimiter));
}

// remove space before text content into the string given in parameter
void gdcore_string_erase_left(std::string& strSrc, char delimiter)
{
  size_t pos = strSrc.find_first_not_of(delimiter);
  strSrc.erase(0, pos);
}

// remove space after text content and return a new string, source still unchanged
std::string gdcore_string_right_of(const std::string strSrc, char delimiter)
{
  std::string strDest = strSrc;
  size_t pos = strDest.find_last_not_of(delimiter);
  return strDest.erase(pos + 1, strDest.length());
}

// remove space after text content into the string given in parameter
void gdcore_string_erase_right(std::string& strSrc, char delimiter)
{
  size_t pos = strSrc.find_last_not_of(delimiter);
  strSrc.erase(pos + 1, strSrc.length());
}

std::string gdcore_string2xhtml(const std::string& src)
{
  std::string str = src;
  boost::algorithm::replace_all(str, "&",  "&amp;");
  boost::algorithm::replace_all(str, "<",  "&lt;");
  boost::algorithm::replace_all(str, ">",  "&gt;");
  boost::algorithm::replace_all(str, "\"", "&quot;");
  boost::algorithm::replace_all(str, "=",  "&#61;");
  return str;
}

std::string gdcore_xhtml2string(const std::string& src)
{
  std::string str = src;
  boost::algorithm::replace_all(str, "&lt;",   "<");
  boost::algorithm::replace_all(str, "&gt;",   ">");
  boost::algorithm::replace_all(str, "&amp;",  "&");
  boost::algorithm::replace_all(str, "&quot;", "\"");
  boost::algorithm::replace_all(str, "&#61;",  "=");
  return str;
}

bool gdcore_isPathHidden(const boost::filesystem::path& fsPath)
{
  std::string str = fsPath.filename().string();
  if ( str.size() ) {
    if ( str    == ".." ) return false;
    if ( str[0] == '.'  ) return true;
  }
  return false;
}

bool gdcore_isPathExist(const boost::filesystem::path& fsPath)
{
  return boost::filesystem::exists(fsPath);
}

bool gdcore_isPathDirectory(const boost::filesystem::path& fsPath)
{
  bool bOk;
  try {
    bOk = boost::filesystem::is_directory(fsPath);
  } catch (boost::filesystem::filesystem_error& e) {
    bOk = false;
  }
  return bOk;
}

bool gdcore_isPathFile(const boost::filesystem::path& fsPath)
{
  bool bOk;
  try {
    bOk = boost::filesystem::is_regular_file(fsPath);
  } catch (boost::filesystem::filesystem_error& e) {
    bOk = false;
  }
  return bOk;
}

bool gdcore_isPathSymLink(const boost::filesystem::path& fsPath)
{
  bool bOk;
  try {
    bOk = boost::filesystem::is_symlink(fsPath);
  } catch (boost::filesystem::filesystem_error& e) {
    bOk = false;
  }
  return bOk;
}

std::string gdcore_b64enc_bin2text72(const char* pData, const size_t length)
{
  std::stringstream os;
  std::copy(b64enc_bin2text72(pData), b64enc_bin2text72(pData + length), ostream_iterator<char>(os));
  return os.str();
}

std::string gdcore_b64enc_bin2text(const char* pData, const size_t length)
{
  std::stringstream os;
  std::copy(b64enc_bin2text(pData), b64enc_bin2text(pData + length), ostream_iterator<char>(os));
  return os.str();
}

int gdcore_file_content2string(const char* path, std::string& content)
{
  std::stringstream       buffer;
  std::ifstream           in(path);
  if ( !in ) {
    return 0;
  }
  buffer << in.rdbuf();
  in.close();
  content = buffer.str();
  return 1;
}

