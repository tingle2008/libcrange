#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libpq-fe.h>
#include <apr_strings.h>

#include "set.h"
#include "libcrange.h"
#include "range.h"

const char** functions_provided(libcrange* lr)
{
  static const char* functions[] = { "mt5grp", "mt5ugrp", "mt5posuser", "mt5possym", "vn",
                                     "mt5sym", 0 };
  return functions;
}

PGconn * _open_db(range_request* rr)
{
  PGconn* conn = NULL;
  libcrange* lr = range_request_lr( rr );
  apr_pool_t* pool = range_request_pool(rr);

  int errors = 0;

  if (!(conn = (PGconn *)libcrange_get_cache(lr, "pgsql:conn"))) {
    const char* pgsql_user = libcrange_getcfg(lr, "mt5_pgsql_user");
    const char* pgsql_db = libcrange_getcfg(lr, "mt5_pgsql_db");
    const char* pgsql_passwd = libcrange_getcfg(lr, "mt5_pgsql_passwd");
    const char* pgsql_host = libcrange_getcfg(lr, "mt5_pgsql_host");
    const char* pgsql_port = libcrange_getcfg(lr, "mt5_pgsql_port");

    if (!pgsql_user) {
      range_request_warn(rr, "pgsql no user specified");
      errors++;
    }

    if (!pgsql_db) {
      range_request_warn(rr, "pgsql no db specified");
      errors++;
    }

    if (!pgsql_passwd) {
      range_request_warn(rr, "pgsql no passwd specified");
      errors++;
    }

    if (!pgsql_host)
      pgsql_host = "localhost";

    if (!pgsql_port)
      pgsql_port = "5432";

    if (errors)
      return conn;

    char* conninfo = apr_psprintf
      (pool, "host=%s port=%s user=%s password=%s dbname=%s",
       pgsql_host, pgsql_port, pgsql_user, pgsql_passwd,
       pgsql_db);

    if (!(conn = PQconnectdb(conninfo))) {
      range_request_warn(rr, "pgsql dbname=%s: can't connect",
                         pgsql_db);
      return conn;
    }

    if (PQstatus(conn) != CONNECTION_OK) {
      range_request_warn(rr, "pgsql connection: %s",
                         PQerrorMessage(conn));
      return conn;
    }
    libcrange_set_cache(lr, "pgsql:conn", conn);
  }

  return conn;
}

range* rangefunc_mt5sym(range_request* rr, range** r)
{
    range* ret;
    PGconn *conn;

    apr_pool_t* pool = range_request_pool(rr);

    PGresult* result;
    int row, rows;
    const char* query;


    ret = range_new(rr);

    conn = _open_db( rr );

    if (! conn ) {
      return ret;
    }

    query = apr_psprintf(pool, "select \"Symbol\" from mt5_symbols;");

    result = PQexec(conn, query);

    if (!result) {
      range_request_warn(rr, "pgsql_group: %s",
                         PQerrorMessage(conn));
      return ret;
    }

    if (( PQresultStatus(result) != PGRES_COMMAND_OK ) &&
        ( PQresultStatus(result) != PGRES_TUPLES_OK ))
      {
        range_request_warn(rr, "pgsql_group: %s",
                           PQerrorMessage(conn));
        return ret;
      }

    rows = PQntuples(result);
    for (row=0; row < rows; ++row) {
      const char* gg = PQgetvalue(result, row, 0);
      range_add(ret, gg);
    }

    PQclear(result);
    return ret;
}

range* rangefunc_mt5possym(range_request* rr, range** r)
{
    range* ret;
    PGconn *conn;

    apr_pool_t* pool = range_request_pool(rr);

    PGresult* result;
    int row, rows;
    const char* query;


    ret = range_new(rr);

    conn = _open_db( rr );

    if (! conn ) {
      return ret;
    }

    query = apr_psprintf(pool, "select distinct(\"Symbol\") from mt5_positions");

    result = PQexec(conn, query);

    if (!result) {
      range_request_warn(rr, "pgsql_group: %s",
                         PQerrorMessage(conn));
      return ret;
    }

    if (( PQresultStatus(result) != PGRES_COMMAND_OK ) &&
        ( PQresultStatus(result) != PGRES_TUPLES_OK ))
      {
        range_request_warn(rr, "pgsql_group: %s",
                           PQerrorMessage(conn));
        return ret;
      }

    rows = PQntuples(result);
    for (row=0; row < rows; ++row) {
      const char* gg = PQgetvalue(result, row, 0);
      range_add(ret, gg);
    }

    PQclear(result);
    return ret;
}

range* rangefunc_mt5posuser(range_request* rr, range** r)
{
    range* ret;
    PGconn *conn;

    apr_pool_t* pool = range_request_pool(rr);

    PGresult* result;
    int row, rows;
    const char* query;


    ret = range_new(rr);

    conn = _open_db( rr );

    if (! conn ) {
      return ret;
    }

    query = apr_psprintf(pool, "select distinct(\"Login\") from mt5_positions_prod");

    result = PQexec(conn, query);

    if (!result) {
      range_request_warn(rr, "pgsql_group: %s",
                         PQerrorMessage(conn));
      return ret;
    }

    if (( PQresultStatus(result) != PGRES_COMMAND_OK ) &&
        ( PQresultStatus(result) != PGRES_TUPLES_OK ))
      {
        range_request_warn(rr, "pgsql_group: %s",
                           PQerrorMessage(conn));
        return ret;
      }

    rows = PQntuples(result);
    for (row=0; row < rows; ++row) {
      const char* gg = PQgetvalue(result, row, 0);
      range_add(ret, gg);
    }

    PQclear(result);
    return ret;
}



range* rangefunc_mt5grp(range_request* rr, range** r)
{
    range* ret;
    PGconn *conn;

    apr_pool_t* pool = range_request_pool(rr);

    PGresult* result;
    int row, rows;
    const char* query;


    ret = range_new(rr);

    conn = _open_db( rr );

    if (! conn ) {
      return ret;
    }

    query = apr_psprintf(pool, "select \"Group\" from mt5_groups_prod");

    result = PQexec(conn, query);

    if (!result) {
      range_request_warn(rr, "pgsql_group: %s",
                         PQerrorMessage(conn));
      return ret;
    }

    if (( PQresultStatus(result) != PGRES_COMMAND_OK ) &&
        ( PQresultStatus(result) != PGRES_TUPLES_OK ))
      {
        range_request_warn(rr, "pgsql_group: %s",
                           PQerrorMessage(conn));
        return ret;
      }

    rows = PQntuples(result);
    for (row=0; row < rows; ++row) {
      const char* gg = PQgetvalue(result, row, 0);
      range_add(ret, gg);
    }

    PQclear(result);
    return ret;
}

range* rangefunc_mt5ugrp(range_request* rr, range** r)
{
  range* ret;
  PGconn* conn;

  apr_pool_t* pool = range_request_pool( rr );

  int i;
  const char** members;
  members = range_get_hostnames(pool, r[0]);

  ret = range_new( rr );
  conn = _open_db( rr );

  if ( ! conn ) {
    return ret;
  }

  for (i = 0; members[i]; i++) {

    PGresult* result;
    int row, rows;
    const char* query;
    char* m;

    if(members[i][0] == '\"') {
      query = apr_psprintf(pool,
                           "select \"Login\" from mt5_users_prod where \"Group\" = trim(both '\"' from '%s')",
                           members[i]);
    } else {
      query = apr_psprintf(pool,
                           "select \"Login\" from mt5_users_prod where \"Group\" = '%s'",
                           members[i]);
    }

    result = PQexec(conn, query);

    if( !result ) {
      range_request_warn(rr, "mt5_user_by_group: %s",
                         PQerrorMessage(conn));
      return ret;
    }

    rows = PQntuples( result );
    for (row = 0; row < rows; ++row) {
      const char* element = PQgetvalue(result, row, 0);
      range_add(ret, element);
    }

    PQclear(result);
  }

  return ret;
}

range* rangefunc_vn(range_request* rr, range** r)
{
  range* ret;
  PGconn* conn;

  apr_pool_t* pool = range_request_pool( rr );

  int i;
  const char** members;
  members = range_get_hostnames(pool, r[0]);

  ret = range_new( rr );
  conn = _open_db( rr );

  if ( ! conn ) {
    return ret;
  }

  for (i = 0; members[i]; i++) {

    PGresult* result;
    int row, rows;
    const char* query;
    char* m;

    query = apr_psprintf(pool,"select node from GROUPS.%s",members[i]);

    result = PQexec(conn, query);

    if( !result ) {
      range_request_warn(rr, "mt5 groups tag: %s",
                         PQerrorMessage(conn));
      return ret;
    }

    rows = PQntuples( result );
    for (row = 0; row < rows; ++row) {
      const char* element = PQgetvalue(result, row, 0);
      range_add(ret, element);
    }

    PQclear(result);
  }

  return ret;
}


