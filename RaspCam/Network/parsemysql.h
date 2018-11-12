#ifndef PARSEMYSQL_H_
#define PARSEMYSQL_H_


#include <iostream>
#include <mysql.h> // I added include /usr/include/mysql/ to ld.so.conf which is why that works


#define HOST "192.168.1.137" // you must keep the quotes on all four items,
#define USER "root" // the function "mysql_real_connect" is looking for a char datatype,
#define PASSWD "root" // without the quotes they're just an int.
#define DB "k595np"
#define DBPORT 3306

extern int mysql_connect(void);
extern void mysql_disconnect(void);

extern int getProcessSeqFromDB(const char *ordernum, const char *process);
extern unsigned int getProcNumFromDB(const char *procnum);
extern int updateProcAccuracyFromDB(int accuracy, int item);



#endif // PARSEMYSQL_H_
