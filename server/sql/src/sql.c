/*****************************************************
copyright (C), 2014-2015, Lighting Studio. Co.,     Ltd. 
File name：
Author：Jerey_Jobs    Version:0.1    Date: 
Description：
Funcion List: 
*****************************************************/

#include "head.h"

void is_ok(int rc,char **errmsg)/*检测数据库函数调用结果是否正确*/
{
	if(rc != SQLITE_OK)
	{
		printf("error:rc=%d,%s\n",rc,*errmsg);
		exit(1);
	}
}

void open_db(sqlite3 **db)/*打开数据库*/
{
	int rc;

	rc =  sqlite3_open("server.db",db);
	is_ok(rc,NULL);
}

void close_db(sqlite3 *db)	/*关闭数据库*/
{
	int rc;

	rc = sqlite3_close(db);
	is_ok(rc,NULL);
}

/*创建数据表user，表中包含成员name（用户名），passwd（密码）,id*/
void creat_user_db(sqlite3 *db,char **errmsg)
{
	int rc;

	rc = sqlite3_exec(db,"CREATE TABLE user(id integer primary key autoincrement,name text,passwd text,user_id integer)",NULL,NULL,errmsg);
	is_ok(rc,errmsg);
}

/*创建数据表data，包含成员name（用户名），toname（接受者），msg（消息），time（发送消息的时间）*/
void creat_data_db(sqlite3 *db,char **errmsg)
{
	int rc;

	rc = sqlite3_exec(db,"CREATE TABLE data(id integer primary key autoincrement,name text,toname text,msg text,time text)",NULL,NULL,errmsg);
	is_ok(rc,errmsg);
}

/*online include name,user_id,sockfd*/
void creat_online_db(sqlite3 *db,char **errmsg)
{
	int rc;

	rc = sqlite3_exec(db,"CREATE TABLE online(id integer primary key autoincrement,name text,user_id integer,sockfd integer,speak_flag integer)",NULL,NULL,errmsg);
	is_ok(rc,errmsg);
}

void insert_data(sqlite3 *db,char **errmsg,char *name,char *toname,char *msg,char *time)	
{
	char sql[1024];
	int rc;
	
	sprintf(sql,"insert into data(name,toname,msg,time)values('%s','%s','%s','%s')",name,toname,msg,time);
	rc = sqlite3_exec(db,sql,NULL,NULL,errmsg);
	is_ok(rc,errmsg);
}

void insert_online(sqlite3 *db,char **errmsg,char *name,int id,int sockfd)	
{
	char sql[1024];
	int rc;
	
	sprintf(sql,"insert into online(name,user_id,sockfd,speak_flag)values('%s',%d,%d,0)",name,id,sockfd);
	rc = sqlite3_exec(db,sql,NULL,NULL,errmsg);
	is_ok(rc,errmsg);
}

void reg_db(sqlite3 *db,char **errmsg,char *name,char *passwd,int id)/*向服务器注册，向user表中插入name，passwd,id*/
{
	char sql[1024];
	int rc;

	sprintf(sql,"insert into user(name,passwd,user_id)values('%s','%s',%d);",name,passwd,id);
	rc = sqlite3_exec(db,sql,NULL,NULL,errmsg);
	is_ok(rc,errmsg);
}


int read_user_id(sqlite3 *db,char **errmsg,int id)  //check user id ,compare id have no same
{
	int rc;
	
	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare(db,"select * from user",-1,&stmt,0); 
    	is_ok(rc,errmsg);
	rc = sqlite3_step(stmt);
    	while(rc == SQLITE_ROW)
	{
	   if(id == atoi(sqlite3_column_text(stmt,3)))
	   {
			rc = sqlite3_finalize( stmt );
			is_ok(rc,errmsg);
			return IDFL;
	   }	
	   rc = sqlite3_step(stmt);
	}
	
	rc = sqlite3_finalize( stmt );
	is_ok(rc,errmsg);
	return IDOK;
}

int read_user_id_passwd(sqlite3 *db,char **errmsg,int id,char *passwd)  //login:check user id,password ,compare if their have same 
{
	int rc;
	
	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare(db,"select * from user",-1,&stmt,0); 
    	is_ok(rc,errmsg);
	rc = sqlite3_step(stmt);
    	while(rc == SQLITE_ROW)
	{
	   	if(id == atoi(sqlite3_column_text(stmt,3)))
	   	{
			if(0 == strcmp(sqlite3_column_text(stmt,2),passwd))
			{
				rc = sqlite3_finalize( stmt );
				is_ok(rc,errmsg);
				return LOGOK;
			}
	   	}	
	   	rc = sqlite3_step(stmt);
	}
	
	rc = sqlite3_finalize( stmt );
	is_ok(rc,errmsg);
	return PASSWDERROR;
}

char * return_user_name(sqlite3 *db,char **errmsg,int id)  //know id ,to find name and return it`s address
{
	int rc;
	char *temp = (char *)malloc(20);

	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare(db,"select * from user",-1,&stmt,0); 
    	is_ok(rc,errmsg);
	rc = sqlite3_step(stmt);
    	while(rc == SQLITE_ROW)
	{
	   if(id == atoi(sqlite3_column_text(stmt,3)))
	   {
			temp = (char *)sqlite3_column_text(stmt,1);
			rc = sqlite3_finalize( stmt );
			is_ok(rc,errmsg);
			return temp;
	   }	
	   rc = sqlite3_step(stmt);
	}
}

int read_online_id(sqlite3 *db,char **errmsg,int id)  //check online id ,compare id have no same
{
	int rc;
	
	printf("into read online id...\n");
	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare(db,"select * from online",-1,&stmt,0); 
    	is_ok(rc,errmsg);
	rc = sqlite3_step(stmt);
	printf("ddddddd\n");
    	while(rc == SQLITE_ROW)
	{
	   	if(id == atoi(sqlite3_column_text(stmt,2)))
	   	{
			rc = sqlite3_finalize( stmt );
			is_ok(rc,errmsg);
			return IDIN;
	   	}	
	  	rc = sqlite3_step(stmt);
	}
	
	rc = sqlite3_finalize( stmt );
	is_ok(rc,errmsg);
	return IDOUT;
}

int read_online_name(sqlite3 *db,char **errmsg,int id)  //check online id ,compare id have no same
{
	int rc;
	
	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare(db,"select * from online",-1,&stmt,0); 
	is_ok(rc,errmsg);
	rc = sqlite3_step(stmt);
	while(rc == SQLITE_ROW)
	{
	   if(id == atoi(sqlite3_column_text(stmt,2)))
	   {
			rc = sqlite3_finalize( stmt );
			is_ok(rc,errmsg);
			return LOGFL;
	   }	
	   rc = sqlite3_step(stmt);
	}
	
	rc = sqlite3_finalize( stmt );
	is_ok(rc,errmsg);
	return LOGOK;
}

int read_user_name(sqlite3 *db,char **errmsg,char *name)  //check online id ,compare id have no same
{
	int rc;
	
	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare(db,"select * from user",-1,&stmt,0); 
	is_ok(rc,errmsg);
	rc = sqlite3_step(stmt);
	while(rc == SQLITE_ROW)
	{
	   if(0 == strcmp(name,sqlite3_column_text(stmt,1)))
	   {
			rc = sqlite3_finalize( stmt );
			is_ok(rc,errmsg);
			return USERIN;
	   }	
	   rc = sqlite3_step(stmt);
	}
	
	rc = sqlite3_finalize( stmt );
	is_ok(rc,errmsg);
	return USEROUT;
}

char * return_online_name(sqlite3 *db,char **errmsg,int n)  //返回online数据表中第n+1个name
{
	int rc;
	char *temp = (char *)malloc(20);

	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare(db,"select * from online",-1,&stmt,0); 
    	is_ok(rc,errmsg);
	rc = sqlite3_step(stmt);
    	while(rc == SQLITE_ROW)
	{
	   	if(n == 0)
	   	{
			temp = (char *)sqlite3_column_text(stmt,1);
			rc = sqlite3_finalize( stmt );
			is_ok(rc,errmsg);
			return temp;
	   	}
		n--;	
	   	rc = sqlite3_step(stmt);
	}
}

int return_online_id(sqlite3 *db,char **errmsg,int n)  //返回online数据表中第n+1个id
{
	int rc;
	int id = 0;

	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare(db,"select * from online",-1,&stmt,0); 
    	is_ok(rc,errmsg);
	rc = sqlite3_step(stmt);
    	while(rc == SQLITE_ROW)
	{
	   	if(n == 0)
	   	{
			id = atoi(sqlite3_column_text(stmt,2));
			rc = sqlite3_finalize( stmt );
			is_ok(rc,errmsg);
			return id;
	   	}
		n--;	
	   	rc = sqlite3_step(stmt);
	}
}

int return_online_member_num(sqlite3 *db,char **errmsg)  //返回online数据表中有n成员
{
	int rc;
	int n = 0;

	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare(db,"select * from online",-1,&stmt,0); 
    	is_ok(rc,errmsg);
	rc = sqlite3_step(stmt);
    	while(rc == SQLITE_ROW)
	{
		n++;
	   	rc = sqlite3_step(stmt);
	}
	rc = sqlite3_finalize( stmt );
	is_ok(rc,errmsg);

	return n;
}

int return_online_sockfd(sqlite3 *db,char **errmsg,int id)  //know id ,to find name and return it`s address
{
	int rc;
	char tempsockfd[16];

	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare(db,"select * from online",-1,&stmt,0); 
    	is_ok(rc,errmsg);
	rc = sqlite3_step(stmt);
    	while(rc == SQLITE_ROW)
	{
	   if(id == atoi(sqlite3_column_text(stmt,2)))
	   {
			strcpy(tempsockfd,sqlite3_column_text(stmt,3));
			rc = sqlite3_finalize( stmt );
			is_ok(rc,errmsg);
			return atoi(tempsockfd);
	   }	
	   rc = sqlite3_step(stmt);
	}
}

int delete_online_name(sqlite3 *db,char **errmsg,char *name)	//注销用户
{
        int rc;
        char sql[1024];
	 
	sprintf(sql,"delete from online where name = '%s'",name);
	rc = sqlite3_exec(db,sql,NULL,NULL,errmsg);
        if(rc == SQLITE_OK)
	{
		return 1;
	}
	else
	{
		 return 0;
   	}
}

int return_data_num(sqlite3 *db,char **errmsg)  //返回data数据表中有n条记录
{
	int rc;
	int n = 0;

	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare(db,"select * from data",-1,&stmt,0); 
    	is_ok(rc,errmsg);
	rc = sqlite3_step(stmt);
    	while(rc == SQLITE_ROW)
	{
		n++;
	   	rc = sqlite3_step(stmt);
	}
	rc = sqlite3_finalize( stmt );
	is_ok(rc,errmsg);

	return n;
}

char * return_data_name(sqlite3 *db,char **errmsg,int n)	//返回data中的name
{
	int rc;
	char *temp = (char *)malloc(20);

	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare(db,"select * from data",-1,&stmt,0); 
    	is_ok(rc,errmsg);
	rc = sqlite3_step(stmt);
    	while(rc == SQLITE_ROW)
	{
	   	if(n == 0)
	   	{
			temp = (char *)sqlite3_column_text(stmt,1);
			rc = sqlite3_finalize( stmt );
			is_ok(rc,errmsg);
			return temp;
	   	}
		n--;	
	   	rc = sqlite3_step(stmt);
	}
}

char * return_data_toname(sqlite3 *db,char **errmsg,int n)	//返回data中的toname
{
	int rc;
	char *temp = (char *)malloc(20);

	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare(db,"select * from data",-1,&stmt,0); 
    	is_ok(rc,errmsg);
	rc = sqlite3_step(stmt);
    	while(rc == SQLITE_ROW)
	{
	   	if(n == 0)
	   	{
			temp = (char *)sqlite3_column_text(stmt,2);
			rc = sqlite3_finalize( stmt );
			is_ok(rc,errmsg);
			return temp;
	   	}
		n--;	
	   	rc = sqlite3_step(stmt);
	}
}

char * return_data_msg(sqlite3 *db,char **errmsg,int n)		//返回data中的msg
{
	int rc;
	char *temp = (char *)malloc(1024);

	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare(db,"select * from data",-1,&stmt,0); 
    	is_ok(rc,errmsg);
	rc = sqlite3_step(stmt);
    	while(rc == SQLITE_ROW)
	{
	   	if(n == 0)
	   	{
			temp = (char *)sqlite3_column_text(stmt,3);
			rc = sqlite3_finalize( stmt );
			is_ok(rc,errmsg);
			return temp;
	   	}
		n--;	
	   	rc = sqlite3_step(stmt);
	}
}

char * return_data_time(sqlite3 *db,char **errmsg,int n)	//返回data中的时间
{
	int rc;
	char *temp = (char *)malloc(1024);

	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare(db,"select * from data",-1,&stmt,0); 
    	is_ok(rc,errmsg);
	rc = sqlite3_step(stmt);
    	while(rc == SQLITE_ROW)
	{
	   	if(n == 0)
	   	{
			temp = (char *)sqlite3_column_text(stmt,4);
			rc = sqlite3_finalize( stmt );
			is_ok(rc,errmsg);
			return temp;
	   	}
		n--;	
	   	rc = sqlite3_step(stmt);
	}
}

int update_user_passwd(sqlite3 *db,char **errmsg,char *name,char *passwd)//修改密码
{
        int rc;
        char sql[1024];

	sprintf(sql,"update user set passwd = '%s'where name = '%s'",passwd,name);
	rc = sqlite3_exec(db,sql,NULL,NULL,errmsg);
        if(rc == SQLITE_OK)
	{
		 return PASSWDOK;
	}
	else
	{
		 return PASSWDFL;
   	}
}

int update_online_speak_flag(sqlite3 *db,char **errmsg,int user_id,int speak_flag)//修改密码
{
        int rc;
        char sql[1024];

	sprintf(sql,"update online set speak_flag = %d where user_id = %d",speak_flag,user_id);
	rc = sqlite3_exec(db,sql,NULL,NULL,errmsg);
        if(rc == SQLITE_OK)
	{
		 return BANOK;
	}
	else
	{
		 return BANFL;
   	}
}

int read_online_speak_flag(sqlite3 *db,char **errmsg,int id)  //返回是否禁言 0正常 1禁言
{
	int rc;
	int speak_flag;
	
	sqlite3_stmt *stmt = NULL;
	rc = sqlite3_prepare(db,"select * from online",-1,&stmt,0); 
    	is_ok(rc,errmsg);
	rc = sqlite3_step(stmt);
    	while(rc == SQLITE_ROW)
	{
	   	if(id == atoi(sqlite3_column_text(stmt,2)))
	   	{
			speak_flag = atoi(sqlite3_column_text(stmt,4));
			rc = sqlite3_finalize( stmt );
			is_ok(rc,errmsg);
			printf("\n\n*********%d**********\n\n",speak_flag);

			return speak_flag;
	   	}	
	  	rc = sqlite3_step(stmt);
	}
}
































