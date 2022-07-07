#include "JSqlite.h"
#include<iostream>


void PrintError(char *buf,int len,const char *msg){
	memset(buf,0,len);

	sprintf_s(buf,len,"%s",msg);
}

void PrintError1(char *buf,int len,const char *fmt,const char *msg){
	memset(buf,0,len);

	sprintf_s(buf,len,fmt,msg);
}




JSqlite::JSqlite()
{
	p_Database=NULL;
	memset(m_ErrorMsg,0,ERRORLEN);

}

JSqlite::~JSqlite()
{

	if(p_Database){
		Close();
	}
}

bool JSqlite::Open( const char *dbname )
{
	if(p_Database){
		Close();
	}

	
	if(SQLITE_OK==sqlite3_open(dbname,&p_Database))
	{
		return true;

	}else{
	
		PrintError(m_ErrorMsg,ERRORLEN,"open database error");
	}

	return false;
}

bool JSqlite::ExecCmd( const char *cmd )
{
	if(!p_Database) {
		PrintError(m_ErrorMsg,ERRORLEN,"database not open");
		return false;
	}

	char *errmsg=NULL;
	if(SQLITE_OK==sqlite3_exec(p_Database,cmd,NULL,NULL,&errmsg))
	{

		return true;
	}else{
		PrintError1(m_ErrorMsg,ERRORLEN,"exec error:%s",errmsg);
		sqlite3_free(errmsg);
	}

	return false;
	
}

bool JSqlite::ExecStmt( const char *cmd,JStmt &stmt )
{
	if(!p_Database) {
		PrintError(m_ErrorMsg,ERRORLEN,"database not open");
		return false;
	}

	int result = sqlite3_prepare_v2(p_Database, cmd, -1,stmt.Ptr(),NULL);

	if (result == SQLITE_OK) {

		return true;
	}else{
		PrintError(m_ErrorMsg,ERRORLEN,"prepare sql error");
		return false;
	}

}

const char * JSqlite::GetLastError()
{
	return m_ErrorMsg;
}

bool JSqlite::Close()
{
	if(p_Database){
		bool r=SQLITE_OK==sqlite3_close(p_Database);
		
		p_Database=NULL;

		return r;
	}

	return true;
}

bool JSqlite::IsOpen()
{
	return p_Database!=NULL;
}

int JSqlite::ExecCount( const char *cmd )
{
	if(!p_Database) {
		PrintError(m_ErrorMsg,ERRORLEN,"database not open");
		return -1;
	}

	JStmt stmt;

	if(ExecStmt(cmd,stmt))
	{
		while(stmt.Next())
		{
			return stmt.GetInt(0);
		}

	}else{

		return -2;
	}

}

JStmt::JStmt()
{
	m_Result=-1;
	p_Stmt=NULL;
}

JStmt::~JStmt()
{
	if(p_Stmt){
		sqlite3_finalize(p_Stmt);
		p_Stmt=NULL;
	}
}

bool JStmt::Next()
{
	m_Result=sqlite3_step(p_Stmt);
	return SQLITE_ROW==m_Result;
}



sqlite3_stmt ** JStmt::Ptr()
{
	return &p_Stmt;
}

const unsigned char * JStmt::GetText( int i )
{
	return  sqlite3_column_text(p_Stmt,i);
}

const void * JStmt::GetBlob( int i )
{
	return sqlite3_column_blob(p_Stmt,i);
}

double JStmt::GetDouble( int i )
{
	return sqlite3_column_double(p_Stmt,i);
}

int JStmt::GetInt( int i )
{
	return sqlite3_column_int(p_Stmt,i);
}

sqlite3_int64 JStmt::GetInt64( int i )
{
	return sqlite3_column_int64(p_Stmt,i);
}
