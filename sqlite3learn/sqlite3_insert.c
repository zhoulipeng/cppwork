/*
http://www.sqlite.org/quickstart.html
*/
#include <stdio.h>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

int main(int argc, char **argv[]){
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  if( argc!=2 ){
    fprintf(stderr, "Usage: %s DATABASE\n", argv[0]);
    return(1);
  }
  rc = sqlite3_open(argv[1], &db);
  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return(1);
  }
  //rc = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg);
  rc = sqlite3_exec(db,"insert into tbl1(one,two) values('first', 'y Liu')",NULL,NULL,&zErrMsg);
  printf("Insert a record %s\n",rc == SQLITE_OK ? "OK":"FAIL");

  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
  sqlite3_close(db);
  return 0;
}
