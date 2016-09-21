/*****************************************************
copyright (C), 2014-2015, Lighting Studio. Co.,     Ltd. 
File name：
Author：Jerey_Jobs    Version:0.1    Date: 
Description：
Funcion List: 
*****************************************************/

#include "head.h"
//#include "package.h"

void cmd_reg(chat * temp,sqlite3 *db,char **errmsg,int new_fd)	/*执行注册指令*/
{
	int id = 0;
	int rv;		/*save return value*/ 
	
	do
	{
		srand((unsigned)time(NULL)); //be used to make it sured is a randnum
    		id = rand()%10000;  //creat a rand num		
	}while(((rv = read_user_id(db,errmsg,id)) != IDOK) || (id < 1000));
	reg_db(db,errmsg,temp->name,temp->passwd,id);	/*向数据库user表中插入name，passwd,id*/
	temp->id = id;
}

int cmd_log(chat * temp,sqlite3 *db,char **errmsg,int new_fd)	/*login func*/
{
	int rv;		/*save return value*/ 
	char tempname[30] = {0};		/*save return name from user table*/
	int id = 0;
	
	rv = read_user_id_passwd(db,errmsg,temp->id,temp->passwd);	/*check id and password are right or false*/ 
	if(rv == LOGOK)		/*when id and pw is right*/ 
	{
		strcpy(tempname,return_user_name(db,errmsg,temp->id));	//返回该id的用户名
		id = temp->id;
		memset(temp,0,sizeof(chat));	/*clear temp*/ 
		strcpy(temp->name,tempname);
		insert_online(db,errmsg,tempname,id,new_fd);		/*put name and id to online table*/ 

		return LOGOK;
	}
	if(rv == PASSWDERROR)
	{
		return PASSWDERROR;
	}
}

void cmd_chat(chat * temp,sqlite3 *db,char **errmsg,int new_fd)		/*私聊处理函数*/
{
	char *wday[]={"周日","周一","周二","周三","周四","周五","周六"};
	time_t timep;
	struct tm *p;
	char temptime[128];
	int tempsockfd;

	time(&timep);
	p=localtime(&timep); /*取得当地时间*/
	sprintf(temptime,"%d/%d/%d %s%d:%d:%d\n",(1900+p->tm_year),( 1+p->tm_mon), p->tm_mday,
	wday[p->tm_wday],p->tm_hour, p->tm_min, p->tm_sec);

	strcpy(temp->time,temptime);
	strcpy(temp->toname,return_user_name(db,errmsg,temp->toid));	//由id得到接受人的name
	insert_data(db,errmsg,temp->name,temp->toname,temp->msg,temptime);	//插入data数据库
	tempsockfd = return_online_sockfd(db,errmsg,temp->toid);	//由toid得到收件人的sockfd
	temp->cmd = CHATMSG;
	temp->sockfd = tempsockfd;
//	display_temp(temp);	
//	send(tempsockfd,temp,sizeof(chat),0);
}

int cmd_read(chat * temp,sqlite3 *db,char **errmsg,int new_fd)	//发送在线成员
{
	int member_num = 0;	//在线成员总人数
	chat * send_temp = (chat *)malloc(sizeof(chat));
	
	memset(send_temp,0,sizeof(chat));
	send_temp->cmd = BEGINREAD;
	send(new_fd,send_temp,sizeof(chat),0);
	display_temp(temp);
	memset(send_temp,0,sizeof(chat));
	member_num = return_online_member_num(db,errmsg);
	send_temp->member_num = member_num;
	while(member_num)
	{
		member_num--;
		memset(send_temp->member_name,0,sizeof(20));
		send_temp->cmd = READNEXT;
		send_temp->member_id = return_online_id(db,errmsg,member_num);
		printf("____%d_____\n",return_online_id(db,errmsg,member_num));
		strcpy(send_temp->member_name,return_online_name(db,errmsg,member_num));
		send(new_fd,send_temp,sizeof(chat),0);
		if(member_num == 0)	//发送完成，没有更多的成员
		{
			return ENDREAD;//完成查看
		}
	}
	
	return NOREAD;
}

int cmd_all(chat * temp,sqlite3 *db,char **errmsg,int *new_fd)		/*群聊处理函数*/
{
	char *wday[]={"周日","周一","周二","周三","周四","周五","周六"};
	time_t timep;
	struct tm *p;
	char temptime[128];
	chat * send_temp = (chat *)malloc(sizeof(chat));
	int member_num = 0;	//在线成员总人数

	time(&timep);
	p=localtime(&timep); /*取得当地时间*/
	sprintf(temptime,"%d/%d/%d %s%d:%d:%d\n",(1900+p->tm_year),( 1+p->tm_mon), p->tm_mday,
	wday[p->tm_wday],p->tm_hour, p->tm_min, p->tm_sec);

	strcpy(temp->time,temptime);
	insert_data(db,errmsg,temp->name,"ALL",temp->msg,temptime);	//插入data数据库
	
	memset(send_temp,0,sizeof(chat));
	member_num = return_online_member_num(db,errmsg);
	send_temp->member_num = member_num;
	send_temp->cmd = ALL;
	strcpy(send_temp->name,temp->name);
	strcpy(send_temp->msg,temp->msg);
	if(member_num == 1)	
	{
		return ONLYYOU;
	}
	while(member_num)
	{
		member_num--;
		memset(send_temp->toname,0,sizeof(20));
		send_temp->toid = return_online_id(db,errmsg,member_num);
		if(send_temp->toid != temp->id)
		{
			send_temp->sockfd = return_online_sockfd(db,errmsg,send_temp->toid);
		//	printf("____%d_____\n",return_online_id(db,errmsg,member_num));
			strcpy(send_temp->toname,return_online_name(db,errmsg,member_num));
		//	display_temp(temp);
			send(send_temp->sockfd,send_temp,sizeof(chat),0);
			if(member_num == 0)	//发送完成，没有更多的成员
			{
				return ALLOK;//完成查看
			}
		}
	}
}

void cmd_file(chat * temp,sqlite3 *db,char **errmsg,int new_fd)		/*文件发送函数*/
{	
	strcpy(temp->toname,return_user_name(db,errmsg,temp->toid));	//由id得到接受人的name
	temp->sockfd= return_online_sockfd(db,errmsg,temp->toid);	//由toid得到收件人的sockfd
	temp->cmd = SENDFIL;		
}

int cmd_data(chat * temp,sqlite3 *db,char **errmsg,int new_fd)		//查看聊天记录
{
	int data_num = 0;	//聊天记录总条数
	chat * send_temp = (chat *)malloc(sizeof(chat));
	char tempname[20];
	char temptoname[20];
	
	memset(send_temp,0,sizeof(chat));
	data_num = return_data_num(db,errmsg);
	while(data_num)
	{
		memset(tempname,0,sizeof(20));
		memset(temptoname,0,sizeof(20));
		data_num--;
		strcpy(tempname,return_data_name(db,errmsg,data_num));
		strcpy(temptoname,return_data_toname(db,errmsg,data_num));
		
		if((strcmp(tempname,temp->name) == 0) || ((strcmp(temptoname,temp->name) == 0)))
		{
			send_temp->cmd = DATA;
			strcpy(send_temp->msg,return_data_msg(db,errmsg,data_num));
			strcpy(send_temp->time,return_data_time(db,errmsg,data_num));
			strcpy(send_temp->name,tempname);
			strcpy(send_temp->toname,temptoname);
			send(new_fd,send_temp,sizeof(chat),0);
			memset(send_temp,0,sizeof(chat));
		}
		if(data_num == 0)	//发送完成，没有更多的成员
		{
			return ENDDATA;//完成查看
		}
	}
	
	return NODATA;
}













