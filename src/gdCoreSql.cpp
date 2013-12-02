/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <iostream>
#include <sstream>

#include "gdCoreSql.h"

gdCoreSql::gdCoreSql() : pConn(0), nConn(CPSNotConnected), pRes(0), m_bDebug(0)
{ }

void gdCoreSql::setConnInfo(std::string connInfo)
{
  m_connInfo = connInfo;
}

void gdCoreSql::setDebugging(bool bDebug)
{
  m_bDebug = bDebug;
}

gdCoreSql::~gdCoreSql()
{
  closeDB();
}

int gdCoreSql::connectDB()
{
  if ( nConn == CPSConnected ) return CPSqlErrOK;
  pRes = 0;
  if ( m_bDebug ) fprintf(stderr, "Starting db connection ...\n");
  pConn = PQconnectdb(m_connInfo.c_str());
  if ( m_bDebug ) fprintf(stderr, "End connection process\n");
  if ( PQstatus(pConn) == CONNECTION_BAD ) {
    if ( m_bDebug ) fprintf(stderr, "Error when trying connection : %s\n", PQerrorMessage(pConn));
    closeDB();
    pRes = 0;
    pConn = 0;
    return CPSqlErrBadConnection;
  }
  if ( m_bDebug ) fprintf(stderr, "Database opened \n");
  nConn = CPSConnected;
  return CPSqlErrOK;
}

void gdCoreSql::closeDB()
{
  if ( m_bDebug ) fprintf(stderr, "closing database\n");
  if ( nConn != CPSNotConnected ) {
    nConn = CPSNotConnected;
    PQfinish(pConn);
  }
  pRes = 0;
  pConn = 0;
}

int gdCoreSql::execQuery(const char* pQuery)
{
  if ( connectDB() ) {
    if ( m_bDebug ) fprintf(stderr, "Error when trying connection : %s\n", PQerrorMessage(pConn));
    return 0;
  }
  if ( m_bDebug )
    fprintf(stderr, "gdCoreSql - Execute :\n%s\n", pQuery);
  pRes = PQexec(pConn, pQuery);
  if ( !pRes || ((PQresultStatus(pRes) != PGRES_TUPLES_OK) && (PQresultStatus(pRes) != PGRES_COMMAND_OK)) ) {
    if ( m_bDebug ) fprintf(stderr, "Error when exec : %s\n", PQerrorMessage(pConn));
    return 0;
  }
  //  if ( !getRowCount() )               return 0;
  return 1;
}

void gdCoreSql::releaseQuery()
{
  PQclear(pRes);
  pRes = 0;
  if ( m_bDebug ) fprintf(stderr, "release query\n");
}

long gdCoreSql::getRowCount()
{
  return PQntuples(pRes);
}

long gdCoreSql::getColumnCount()
{
  return PQnfields(pRes);
}

char* gdCoreSql::getColumnName(int nCol)
{
  return PQfname(pRes, nCol);
}

int gdCoreSql::getColumnIndex(const char* columnName)
{
  for (int idx = 0; idx < getColumnCount(); idx++)
    if ( strcmp(columnName, getColumnName(idx)) == 0 ) return idx;
  return -1;
}

gdCoreSql::sqlTypes    gdCoreSql::getColumnType(int nCol)
{
  return (sqlTypes) PQftype(pRes, nCol);
}

char* gdCoreSql::getValueAsStr(int row, int column)
{
  return PQgetvalue(pRes, row, column);
}

long gdCoreSql::getValueAsLong(int row, int column)
{
  return atoi(PQgetvalue(pRes, row, column));
}

double gdCoreSql::getValueAsDouble(int row, int column)
{
  return atof(PQgetvalue(pRes, row, column));
}

char gdCoreSql::getValueAsChar(int row, int column)
{
  char leChar;
  char* pChar = PQgetvalue(pRes, row, column);
  leChar = pChar[0];
  return leChar;
}

std::string gdCoreSql::convertString2Sql(const std::string strSource)
{
  std::string strNew = strSource;
  boost::algorithm::replace_all(strNew, "'", "''");
  return strNew;
}

