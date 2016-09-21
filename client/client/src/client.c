/*****************************************************
copyright (C), 2014-2015, Lighting Studio. Co.,     Ltd. 
File name：
Author：Jerey_Jobs    Version:0.1    Date: 
Description：
Funcion List: 
*****************************************************/

#include "head.h"
//#include "package.h"

extern void read_recv_info(void * arge);

int main(int argc,char *argv[])
{
	char ip[64];

	int sockfd;	/*创建socket描述符*/
	struct sockaddr_in s_addr;	

	chat temp;	/*建立结构体缓存*/

	int ID = 0;
	char NAME[20];

	int clear_flag = 1;
	int root_flag = 0;
	int login_flag = 0;		/*judge id login or not*/

	pthread_t c_thread;	/*线程ID指针*/
	
	strncpy(ip,argv[1],sizeof(ip));/*将输入的ip复制到ip[64]中*/
	if(argc != 2)
	{
		fprintf(stderr,"Usage:%s IP \a\n",argv[0]);
		exit(1);
	}
	sockfd = mysocket();	/*创建socket套接口*/
	myconnect(sockfd,&s_addr,ip);	/*将sockfd连接至服务器*/
	memset(&temp,0,sizeof(temp));	/*初始化缓存区*/
	while(1)
	{	while((temp.cmd != EXIT)&&(login_flag != 1)&&(root_flag != 1))
		{
			memset(&temp,0,sizeof(temp));	/*初始化缓存区*/
			temp.sockfd = sockfd;	/*填写服务器套接字*/
			temp.cmd = interface(ID);	/*显示登录界面,返回指令*/
			strcpy(temp.ip,ip);		/*填写客户端IP*/
			switch (temp.cmd)
			{
				case REG:ID = 0;
					ID = cmd_reg(&temp);
					memset(&temp,0,sizeof(temp));
						 break;
				case LOG:ID = cmd_log(&temp);
					strcpy(NAME,temp.name);
					if((ID != 0)&&(ID != 10000))
					{
						login_flag = 1;
						clear_flag = 1;
					}
					if(ID == 10000)
					{
						root_flag = 1;
						clear_flag = 1;
					}
						 break;
				case EXIT:cmd_exit(&temp);
						 break;
				default:printf("error!\n");
						 exit(1);
			}
		}

		void *tret;

		int pth_flag = pthread_create(&c_thread,NULL,read_recv_info,(void *)(&sockfd));		/*接受消息的线程*/
		

		while(login_flag)	//普通用户界面
		{
			memset(&temp,0,sizeof(temp));
			temp.sockfd = sockfd;
			temp.cmd = main_face(ID,NAME,clear_flag);	/*显示主界面，返回指令*/
			clear_flag = 0;
			temp.id = ID;
			strcpy(temp.ip,ip);
			strcpy(temp.name,NAME);	
			switch	(temp.cmd)
			{
				case CHAT:	strcpy(temp.name,NAME);
						cmd_chat(&temp);
						break;
				case ALL:	cmd_all(&temp);
						break;
				case FIL:	cmd_file(&temp);
						break;		
				case READ:	cmd_read(&temp);
						break;
				case DATA:	printf("聊天记录：\n");
						cmd_data(&temp);
						sleep(2);
						break;
				case PASSWD:	cmd_passwd(&temp);
						break;
				case CLE :	clear_flag = 1;
						break;
				case QUIT:	login_flag = 0;
						cmd_quit(&temp);
						printf("注销完成！\n");
						sleep(1);
						break;
				default:
						break;
			}	
		}
		
		while(root_flag)	//管理员界面
		{
			memset(&temp,0,sizeof(temp));
			temp.sockfd = sockfd;
			temp.cmd = root_face(ID,NAME,clear_flag);	/*显示主界面，返回指令*/
			clear_flag = 0;
			temp.id = ID;
			strcpy(temp.ip,ip);
			strcpy(temp.name,NAME);	
			switch	(temp.cmd)
			{
				case CHAT:	strcpy(temp.name,NAME);
						cmd_chat(&temp);
						break;
				case ALL:	cmd_all(&temp);
						break;
				case FIL:	cmd_file(&temp);
						break;		
				case READ:	cmd_read(&temp);
						break;
				case DATA:	printf("聊天记录：\n");
						cmd_data(&temp);
						sleep(2);
						break;
				case PASSWD:	cmd_passwd(&temp);
						break;
				case CLE :	clear_flag = 1;
						break;
				case QUIT:	root_flag = 0;
						cmd_quit(&temp);
						printf("注销完成！\n");
						sleep(1);
						break;
				case BAN:	cmd_ban(&temp);
						break;
				case FREE:	cmd_free(&temp);
						break;
				case BREAK:	cmd_break(&temp);
						break;
				default:printf("error!\n");
						exit(1);
			}		
		}
		
		login_flag = 0;
		root_flag = 0;

		int pt_jo;
		
		pt_jo = pthread_join(c_thread,&tret);		/*释放线程*/
		//printf("thread exit code: %d ", (int)tret);
 		if(((int)tret == 222))
 		{
			close(sockfd);
 			exit(1);
 		}
	}
}
