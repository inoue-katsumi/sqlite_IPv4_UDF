#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "sqlite3.h"
#include "ntop.h"
#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT1

void
ntop(
  sqlite3_context *context
 ,int argc
 ,sqlite3_value **argv
){
  static unsigned char i4[] = {0x00,0x00,0x00,0x00
                              ,0x00,0x00,0x00,0x00
                              ,0x00,0x00,0xff,0xff};
  const unsigned char *b;
  char *r;
  int i;

  (void)argc;
  i = sqlite3_value_bytes(argv[0]);
  if ((i == 4 || i == 16)
   && (b = sqlite3_value_blob(argv[0]))
   && (r = sqlite3_malloc(40))) {
    if (i == 4)
      b = (const unsigned char *)inet_ntop(AF_INET, b, r, 40);
    else if (!memcmp(b, i4, sizeof(i4)))
      b = (const unsigned char *)inet_ntop(AF_INET, b + 12, r, 40);
    else
      b = (const unsigned char *)inet_ntop(AF_INET6, b, r, 40);
    if (b) {
      r[39] = '\0'; /* strlen safety */
      sqlite3_result_text(context, r, strlen(r), sqlite3_free);
    } else
      sqlite3_free(r);
  }
}

void
pton(
  sqlite3_context *context
 ,int argc
 ,sqlite3_value **argv
){
  const unsigned char *b;
  struct in_addr *r;
  int i;

  (void)argc;
  i = sqlite3_value_bytes(argv[0]);
  if ((i >= 7 || i <= 15)
   && (b = sqlite3_value_text(argv[0]))
   && (r = sqlite3_malloc(4))) {
    i = inet_pton(AF_INET, b, r);
    if (i) {
      //      sqlite3_result_blob(context, r, 4, sqlite3_free);
      //      sqlite3_result_int(context, 24341);
      sqlite3_result_int64(context, ntohl(r->s_addr));
      //sqlite3_result_int(context, (unsigned int)ntohl(r->s_addr));
    } else
      sqlite3_free(r);
  }
}

int sqlite3_extension_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
  SQLITE_EXTENSION_INIT2(pApi)
  sqlite3_create_function_v2(db, "ntop", 1, SQLITE_ANY, 0, ntop, NULL, NULL, NULL);
  sqlite3_create_function_v2(db, "pton", 1, SQLITE_ANY, 0, pton, NULL, NULL, NULL);
  return 0;
}
