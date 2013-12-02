/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef GD_CORESQL_H__
#define GD_CORESQL_H__

/*!
   \file gdCoreSql.h
   \brief PostgreSQL database access.

   Database query and data manipulation.
 */

#include "postgresql/libpq-fe.h"

#include <string>
#include <vector>

/*!
    \class gdCoreSql
    \brief Base class to access the postgres database system.
*/
class gdCoreSql {
public :
  /*! \brief ConnectionStatus enumerator.  */
  enum ConnectionStatus {
    CPSNotConnected, //!< Not connected.
    CPSConnected,    //!< Connected
  };

  /*! \brief Operation result enumerator.  */
  enum OperationResult {
    CPSqlErrOK,                  /*!< OK */
    CPSqlErrBadConnection = 401, /*!< Postgresql connection error. */
    CPSqlErrSelectError,         /*!< Postgresql select error. */
  };

  /*! \brief Sql types enumerator.  SELECT * FROM pg_type ; */
  enum sqlTypes {
    sqlType_int8    =   20, //!< Type sql int8 or bigint
    sqlType_int4    =   23, //!< Type sql int4
    sqlType_text    =   25, //!< Type sql text
    sqlType_date    = 1082, //!< Type sql date
    sqlType_numeric = 1700, //!< Type sql numeric
    sqlType_end
  };

public :
  /*! \brief Constructor.  */
  gdCoreSql();

  /*! \brief Destructor.

    Close the connection and cleanup the database handle.
  */
  ~gdCoreSql();

  /*! \brief Enable or disable the debugging output.  */
  void                      setDebugging(bool bDebug = false); //!< Print the error on the stdout.

  /*! \brief Set the connection information.

    Example : setConnInfo("host=localhost port=5432 dbname=dbname user=postgres password=xxxxxxxx");
  */
  void                      setConnInfo(std::string connInfo);

  /*! \brief Start the connection.

    Connect to the database return OperationResult.
  */
  int                       connectDB();

  /*! \brief Close the database connection.

  */
  void                      closeDB();

  /*! \brief Execute a query.

    Example : execQuery("SELECT * FROM mytable;");
  */
  int                       execQuery(const char* pQuery);

  /*! \brief Cleanup the memory allowed pending an execQuery.

  */
  void                      releaseQuery();

  /*! \brief The number of rows returned by an execQuery.

  */
  long                      getRowCount();

  /*! \brief The number of columns returned by an execQuery.

  */
  long                      getColumnCount();

  /*! \brief Return the column name from a query.

  */
  char*                     getColumnName(int nCol);

  /*! \brief Return the column index when the column name = 'columnName'.

  */
  int                       getColumnIndex(const char* columnName);

  /*! \brief Return the column type from a query.

  */
  sqlTypes                  getColumnType(int nCol);

  /*! \brief Return a value from a query as char*.

  */
  char*                     getValueAsStr(int row, int column);

  /*! \brief Return a value from a query as long.

  */
  long                      getValueAsLong(int row, int column);

  /*! \brief Return a value from a query as double.

  */
  double                    getValueAsDouble(int row, int column);

  /*! \brief Return a value from a query as char.

  */
  char                      getValueAsChar(int row, int column);

  /*! \brief Convert a sql query string containing special characters to a string with the correct adaptations.

    Actually double the character '.
  */
  std::string               convertString2Sql(const std::string strSource);

private :
  PGconn*         pConn;
  int             nConn;
  PGresult*       pRes;
  bool            m_bDebug;
  std::string     m_connInfo;
};

#endif // ifdef GD_CORESQL_H__

