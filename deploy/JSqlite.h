#ifndef JSQLITE_H
#define JSQLITE_H
#include"sqlite3.h"

#define ERRORLEN 255



class JStmt{
public:
	JStmt();
	~JStmt();
	virtual bool Next();
	virtual const unsigned char *GetText(int i);
	virtual const void *GetBlob(int i);
	virtual double GetDouble(int i);
	virtual int GetInt(int i);
	virtual sqlite3_int64 GetInt64(int i);


    virtual sqlite3_stmt **Ptr();
private:
	sqlite3_stmt *p_Stmt;
	int m_Result;
};



class JSqlite
{
public:
	JSqlite();
	~JSqlite();

	virtual bool Open(const char *dbname);
	virtual bool Close();

	virtual bool ExecCmd(const char *cmd);

	virtual bool ExecStmt(const char *cmd,JStmt &stmt);

	virtual const char *GetLastError();
private:
	sqlite3 *p_Database;
	char m_ErrorMsg[ERRORLEN];
};



#endif

