/*****************************************************
copyright (C), 2014-2015, Lighting Studio. Co.,     Ltd. 
File name：
Author：Jerey_Jobs    Version:0.1    Date: 
Description：
Funcion List: 
*****************************************************/

#include "head.h"

// void display_temp(chat * temp);
// void cmd_reg(chat * temp,sqlite3 *db,char **errmsg,int new_fd);
// int cmd_log(chat * temp,sqlite3 *db,char **errmsg,int new_fd);
// void cmd_chat(chat *temp,sqlite3 *db,char **errmsg,int new_fd);
// void cmd_file(chat * temp,sqlite3 *db,char **errmsg,int new_fd);

int user_cmd(chat * temp,sqlite3 *db,char ** errmsg,int new_fd)
{
	int flag;

	printf("cmd = %d\n",temp->cmd);
	switch (temp->cmd)
	{
		case REG:
			{
				flag = read_user_name(db,errmsg,temp->name);	//检测用户名是否被注册过
				if(flag == USEROUT)
				{
					cmd_reg(temp,db,errmsg,new_fd);	/*执行注册指令*/
					return REGOK;
				}
				else
				{
					return REGFL;
				}
				break;
			}
		case LOG:
			{
				flag = read_online_id(db,errmsg,temp->id);
				if(flag == IDOUT)
				{
					flag = cmd_log(temp,db,errmsg,new_fd); 
					return flag;
				}
				else if(flag == IDIN)
				{
					return IDIN;	
				}
				else
				{
					return LOGFL;
				}
				break;
			}
		case EXIT:
			{
				printf("%s 用户断开连接！\n",temp->ip);
				pthread_exit(0);

				break;
			}
		case CHAT:
			{
				flag = read_online_speak_flag(db,errmsg,temp->id);
				if(flag)	//1是禁言 0正常
				{
					return BAN;
				}
				flag = read_online_id(db,errmsg,temp->toid);
				if(flag == IDOUT)
				{
					return CHATFL;	//用户不在线
				}
				if(flag == IDIN)
				{	
					return CHATOK;	//用户在线
				}
				break;
			}
		case CHATMSG:
			{
				flag = read_online_speak_flag(db,errmsg,temp->id);
				if(flag)	//1是禁言 0正常
				{
					return BAN;
				}
				cmd_chat(temp,db,errmsg,new_fd);
				return CHATMSG;

				break;
			}
		case READ:
			{
				flag = cmd_read(temp,db,errmsg,new_fd);
				return flag;
				
				break;
			}
		case ALL:
			{
				flag = read_online_speak_flag(db,errmsg,temp->id);
				if(flag)	//1是禁言 0正常
				{
					return BAN;
				}
				flag = cmd_all(temp,db,errmsg,new_fd);
				return flag;

				break;
			}
		case FIL:
		   	{
              			flag = read_online_id(db,errmsg,temp->toid);
			  	if(flag == IDOUT)
				{
					return CHATFL;	//用户不在线
				}
				if(flag == IDIN)
				{	
					return CHATOK;	//用户在线
				}
				break;
		   	}
		case SENDFIL:
			{
				cmd_file(temp,db,errmsg,new_fd);
				return SENDFIL;

				break;
			}
		case FILOK:
			{
				temp->sockfd= return_online_sockfd(db,errmsg,temp->toid);
				return FILOK;
				
				break;
			}
		case QUIT:
			{
				delete_online_name(db,errmsg,temp->name);
				return QUIT;

				break;
			}	
		case DATA:
			{
				flag = cmd_data(temp,db,errmsg,new_fd);
				return flag;	
		
				break;	
			}
		case PASSWD:
			{
				flag = update_user_passwd(db,errmsg,temp->name,temp->passwd);
				return flag;

				break;
			}
		case BAN:
			{
				flag = update_online_speak_flag(db,errmsg,temp->toid,1);
				return flag;
				
				break;
			}
		case FREE:
			{
				flag = update_online_speak_flag(db,errmsg,temp->toid,0);
				if(flag == BANOK)
				{
					return FREEOK;
				}
				else
				{
					return FREEFL;
				}
				
				break;
			}
		case BREAK:
			{
				char tempname[20];
				temp->sockfd = return_online_sockfd(db,errmsg,temp->toid);
				strcpy(tempname,return_user_name(db,errmsg,temp->toid));
				delete_online_name(db,errmsg,tempname);
				

				return BREAK;
			}
		default :
			{
				printf("user_cmd recv error CMD");
				sleep(1);	//TODO :完成后删除
				break;
			}
	}
}


