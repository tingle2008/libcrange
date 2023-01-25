#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <apr_strings.h>

#include "set.h"
#include "libcrange.h"
#include "range.h"

const char** functions_provided(libcrange* lr)
{
  static const char* functions[] = {"mt5ug","mt5group", 0};
  return functions;
}

MYSQL* _open_db(range_request* rr)
{
    MYSQL* conn;
    libcrange* lr = range_request_lr(rr);

    if(!(conn = (MYSQL *)libcrange_get_cache(lr, "mysql:nodes"))) {
      const char* mysql_user = libcrange_getcfg(lr, "mysqluser");
      const char* mysql_db = libcrange_getcfg(lr, "mysqldb");
      const char* mysql_passwd = libcrange_getcfg(lr, "mysqlpasswd");

      conn = mysql_init(NULL);
      mysql_real_connect(conn, "127.0.0.1", mysql_user, mysql_passwd,
                         mysql_db, 3316, NULL, 0);
      libcrange_set_cache(lr, "mysql:nodes", conn);
    }
    return conn;
}

range* rangefunc_mt5ug(range_request* rr, range** r)
{
  range *ret;
  int i;
  MYSQL *conn;
  conn = _open_db( rr );
  MYSQL_ROW row;
  MYSQL_RES *res;
  apr_pool_t* pool = range_request_pool(rr);
  libcrange* lr = range_request_lr(rr);

  ret = range_new(rr);

  const char * query = "select `Group` from mt5_groups";
  if (mysql_query( conn, query )) {
    fprintf(stderr, "query: %s failed: %s\n",
            query, mysql_error(conn));
    return range_new(rr);
  }

  res = mysql_store_result(conn);
  assert(res);

  while ((row = mysql_fetch_row(res)) != NULL) {
    range* this_group;
    const char* result = row[0];
    range_add(ret, result);
  }
  mysql_free_result(res);
  return ret;
}


range* rangefunc_mt5group(range_request* rr, range** r)
{
  range *ret;
  int i;
  MYSQL *conn;
  conn = _open_db( rr );
  MYSQL_ROW row;
  MYSQL_RES *res;
  apr_pool_t* pool = range_request_pool(rr);
  libcrange* lr = range_request_lr(rr);

  ret = range_new(rr);

  const char * query = "select `Group` from mt5_groups";
  if (mysql_query( conn, query )) {
    fprintf(stderr, "query: %s failed: %s\n",
            query, mysql_error(conn));
    return range_new(rr);
  }

  res = mysql_store_result(conn);
  assert(res);

  while ((row = mysql_fetch_row(res)) != NULL) {
    range* this_group;
    const char* result = row[0];
    range_add(ret, result);
  }
  mysql_free_result(res);
  return ret;
}

