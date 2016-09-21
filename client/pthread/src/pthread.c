/*****************************************************
copyright (C), 2014-2015, Lighting Studio. Co.,     Ltd. 
File name：
Author：Jerey_Jobs    Version:0.1    Date: 
Description：
Funcion List: 
*****************************************************/

#include "head.h"

void * read_recv_info(void * arg)
{	
	int server_sockfd = *((int *)arg);
	chat * recv_temp = (chat *)malloc(sizeof(chat));
	int i = 0;
	int j = 14;
	int k = 24;

	while(1)
	{
		memset(recv_temp,0,sizeof(chat));
		recv(server_sockfd,recv_temp,sizeof(chat),0);
		switch(recv_temp->cmd)
		{
			case BEGINREAD:
				{
					i += 3;
					printf("\033[1;63H--------------------------------");
					printf("\033[2;63H当前在线成员 |  ID  |  用户名   ");
					printf("\033[3;63H--------------------------------");
					break;
				}
			case READNEXT:
				{
					i++;
				//	printf("\033[1B");
				//	printf("\033[33D");
					printf("\033[%d;63H           | %d |     %s    ",i,recv_temp->member_id,recv_temp->member_name);

					break;
				}
			case ENDREAD:
				{
					i++;
				//	printf("\033[1B");
				//	printf("\033[33D");
					printf("\033[%d;63H---------------------------------",i);
				
					i = 0;
					break;
				}
			case READOK:
				{
				//	printf("\033[1B");
				//	printf("\033[33D");
					printf("接受完成\n");	
					
					break;
				}
			case NOREAD:
				{
					printf("\t\t错误：在线列表为空\n");
					
					break;
				}
			case CHATOK:
				{
					printf("\t\t成功连接！\n");
					
					break;
				}
			case CHATMSG:
				{
					j++;
					printf("\n\033[%d;63H%s",j,recv_temp->time);
					j++;
					printf("\033[%d;63H%s 对你说：%s\n",j,recv_temp->name,recv_temp->msg);
					if(j >= 22)
					{
						j = 14;
					}
					break;
				}
			case CHATOUT:
				{
					printf("\n\t\t%s 退出了会话\n",recv_temp->toname);
					
					break;
				}
			case CHATFL:
				{
					printf("\t\t该用户不在线!\n");
				
					break;
				}
			case ALL:
				{
					j++;
					printf("\n\033[%d;63H%s",j,recv_temp->time);
					j++;
					printf("\033[%d;63H%s 群发了：%s\n",j,recv_temp->name,recv_temp->msg);
					if(j >= 22)
					{
						j = 14;
					}
					break;
				}
			case ALLOK:
				{
					printf("\t\t群发消息成功！\n");

					break;
				}
			case ONLYYOU:
				{
					printf("\t\t抱歉：只有你一人在线！心疼摸摸头\n");
		
					break;
				}
			case SENDFIL:
				{
					int num;
					int file;
					char data[80];

					sprintf(data,"./temp/%s",recv_temp->filename);
					usleep(10000);
					file = open(data,O_CREAT|O_RDWR|O_NONBLOCK,S_IRWXU);//可读可写可执行
					if(file == -1 )
					{
						printf("\t\t文件打开失败！\n");
					}
					else
					{
					        lseek(file,0,SEEK_SET);
					        num =  write(file,recv_temp->msg,strlen(recv_temp->msg));
						if(num == -1)
						{
							   printf("\t\t文件写入失败\n");
						}
					}
					close(file);
					
					break;
				}
			case FILOK:
				{
					j++;
					printf("\033[%d;63H%s 向你发送了文件：%s\n",j,recv_temp->name,recv_temp->filename);
					j++;
					printf("\033[%d;63H请在temp目录下查找对方传过来的文件！\n",j);
					if(j >= 22)
					{
						j = 14;
					}
					
					break;
				}
			case DATA:
				{
					k++;
					printf("\n\033[%d;63H%s",k,recv_temp->time);
					k++;
					printf("\033[%d;63H%s 对 %s 说：%s\n",k,recv_temp->name,recv_temp->toname,recv_temp->msg);
					if(k >= 30)
					{
						k = 24;
					}
					
					
					break;
				}
			case ENDDATA:
				{
					printf("\t\t聊天记录显示完毕！\n");
				
					break;
				}
			case NODATA:
				{
					printf("\t\t没有聊天记录！\n");
					
					break;
				}
			case PASSWDOK:
				{
					printf("\t\t密码修改成功！\n");

					break;
				}
			case PASSWDFL:
				{
					printf("\t\t密码修改失败！\n");

					break;
				}
			case BAN:
				{
					printf("\t\t你被禁言啦！\n");

					break;
				}
			case BANOK:
				{
					printf("\t\t禁言成功！\n");

					break;
				}
			case BANFL:
				{
					printf("\t\t禁言失败！\n");
					
					break;	
				}
			case FREEOK:
				{
					printf("\t\t解除禁言成功！\n");
					
					break;	
				}
			case FREEFL:
				{
					printf("\t\t解除禁言失败！\n");
					
					break;	
				}
			case BREAK:
				{
					printf("\n\t\t你被管理员请出聊天室！\n\t\t请输入QUIT退出！\n");
					//printf("请输入QUIT退出！");
					sleep(2);
					//pthread_exit(0);	//被管理员踢出
					return (void *)222;			

					break;
				}
			case QUIT:
				{
					//pthread_exit((void *)1000);	//主动退出线程	
					return (void *)111;
					break;
				}
			
		}
	}
	free(recv_temp);
}
