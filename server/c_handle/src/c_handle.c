/*****************************************************
copyright (C), 2014-2015, Lighting Studio. Co.,     Ltd. 
File name：
Author：Jerey_Jobs    Version:0.1    Date: 
Description：
Funcion List: 
*****************************************************/

#include "head.h"

void display_temp(chat * temp)
{
	printf("\n");
	printf("name = %s\n",temp->name);
//	printf("passwd = %s\n",temp->passwd);
	printf("sockfd = %d\n",temp->sockfd);
	printf("id = %d\n",temp->id);
//	printf("ip = %s\n",temp->ip);
	printf("toname = %s\n",temp->toname);
	printf("toid = %d\n",temp->toid);
	printf("cmd = %d\n",temp->cmd);
	printf("msg = %s\n",temp->msg);
//	printf("time = %s\n",temp->time);
//	printf("member_num = %d\n",temp->member_num);
//	printf("member_id = %d\n",temp->member_id);
	printf("\n");
}

void * c_handle(void * arg)
{
	ph * foo = (ph *)malloc(sizeof(ph));	//将入参内容存放入结构体
	memcpy(foo,(char *)arg,sizeof(ph));

	int cmd;	/*接受的指令*/
	chat * temp = (chat *)malloc(sizeof(chat));	/*接受的数据缓存区*/
	sqlite3 * db = foo->db;	/*定义数据库句柄*/
	int new_fd = foo->sockfd;	//套接字
	char * errmsg;	/*存放数据库函数调用返回的错误信息*/
		
	while(1)
	{
		memset(temp,0,sizeof(chat));
		usleep(100);
		if(recv(new_fd,temp,sizeof(chat),0) == -1)/*接受客户端传来的数据，存入缓存区temp*/
		{
			perror("recv:");
		//	close(new_fd);
			pthread_exit(0);
		}
		printf("接受自客户端：");
		temp->sockfd = new_fd;
		display_temp(temp);
		temp->cmd = user_cmd(temp,db,&errmsg,new_fd);
		usleep(100);
		printf("发送往客户端：");
		display_temp(temp);
		printf("new_fd = %d\n",new_fd);
		if((temp->cmd == CHATMSG)||(temp->cmd == SENDFIL)||(temp->cmd == FILOK)||(temp->cmd == BREAK))
		{
			if(send(temp->sockfd,temp,sizeof(chat),0) == -1)
			{
				perror("send:");
			//	close(new_fd);
				pthread_exit(0);
			}
			if(temp->cmd == BREAK)	
			{
				close(temp->sockfd);
			}
		}
		else
		{
			if(send(new_fd,temp,sizeof(chat),0) == -1)
			{
				perror("send:");
			//	close(new_fd);
				pthread_exit(0);
			}
		}
		if(temp->cmd == LOGOK)
		{
			sleep(2);
			memset(temp,0,sizeof(chat));
			temp->cmd = cmd_read(temp,db,errmsg,new_fd);
			if(send(new_fd,temp,sizeof(chat),0) == -1)
			{
				perror("send:");
				pthread_exit(0);
			}
			
		}
	}
	free(temp);

    pthread_exit(0);
}
