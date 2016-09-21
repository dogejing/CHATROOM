/*****************************************************
copyright (C), 2014-2015, Lighting Studio. Co.,     Ltd. 
File name：
Author：Jerey_Jobs    Version:0.1    Date: 
Description：
Funcion List: 
*****************************************************/

#include "head.h"
//#include "package.h"


void display_temp(chat * temp)
{
	printf("\n");
	printf("name = %s\n",temp->name);
	printf("passwd = %s\n",temp->passwd);
	printf("sockfd = %d\n",temp->sockfd);
	printf("id = %d\n",temp->id);
	printf("ip = %s\n",temp->ip);
	printf("toname = %s\n",temp->toname);
	printf("toid = %d\n",temp->toid);
	printf("cmd = %d\n",temp->cmd);
	printf("msg = %s\n",temp->msg);
	printf("time = %s\n",temp->time);
	printf("\n");
}

char getch() //定义getch不回显函数
{
	char c=0;
	struct termios org_opts, new_opts;
	int res = 0;

	//-----  store old settings -----------
	res = tcgetattr(STDIN_FILENO, &org_opts); //获得键盘输入的初始值，赋给org_opts
	assert(res == 0);//断言
	//---- set new terminal parms --------
	memcpy(&new_opts, &org_opts, sizeof(new_opts));
	new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);//设置参数，不等数据传输完毕就立即改变属性
	c = getchar();
	//------  restore old settings ---------
	res = tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
	assert(res == 0);

	return c;
}


int cmd_reg(chat * temp)	/*注册*/
{
	chat * recv_temp = (chat *)malloc(sizeof(chat));	/*接受的数据缓存区*/
	char passwd[20];	/*密码*/
	char passwd_again[20];	/*再次确认密码*/
	int i;
	int flag = 1;
	int ID;

	printf("\t\t请输入用户名：");
	setbuf(stdin,NULL);
	gets(temp->name);	/*键入用户名*/
	while(flag)
	{
		printf("\t\t请输入密码：");
		for(i=0; i<20; i++)
		{
			passwd[i] = getch();	/*不回显得到密码*/
			if((passwd[i] != '\r')&&(passwd[i] != '\n'))
			{
				printf("*");	/*得到一个密码字符，输出一个＊代替*/
			}		
			if(passwd[i] == '\n')	/*回车后，密码输入完成*/
			{
				passwd[i] = '\0';
				break;
			}
			if(i == 19)		/*密码输入过多*/
			{
				printf("\n\t\t密码位数太多！\n");
				memset(passwd,0,sizeof(passwd));	/*清空密码*/
				i = 0;		/*重新输入*/
				printf("\t\t请重新输入！\n");
				printf("\t\t请输入密码：");
			}
		}
		printf("\n\t\t请再次确认：");
		for(i=0; i<20; i++)
		{
			passwd_again[i] = getch();
			if((passwd_again[i] != '\r')&&(passwd_again[i] != '\n'))
			{
				printf("*");
			}
			if(passwd_again[i] == '\n')
			{
				passwd_again[i] = '\0';
				break;
			}
			if(i == 19)
			{
				printf("\t\t密码位数太多!\n");
				memset(passwd_again,0,sizeof(passwd_again));
				i = 0;
				printf("\t\t请重新输入！\n");
				printf("\t\t请输入密码：");
			}
		}
		if(strcmp(passwd,passwd_again) == 0)	/*两次密码输入相同*/
		{
			printf("\n\t\t密码输入正确!\n");
			strcpy(temp->passwd,passwd);	/*将密码填入缓存区*/
			send(temp->sockfd,temp,sizeof(chat),0);	/*发送缓存数据*/
			printf("\t\t正在注册，请稍等……\n");
			sleep(1);
			memset(recv_temp,0,sizeof(chat));
			recv(temp->sockfd,recv_temp,sizeof(chat),0);	/*接受服务器传来的数据，存放到接受缓存区*/
		//	display_temp(recv_temp);
			if(recv_temp->cmd == REGOK)	/*返回值为注册成功*/
			{
				printf("\t\t注册成功！\n");
				printf("\t\t您的ID为: %d\n",recv_temp->id);	/*printf recv id*/ 
				flag = 0;	/*跳出while循环*/
				ID = recv_temp->id;
				free(recv_temp);
				sleep(1);
				return ID;
			}
			else if(recv_temp->cmd == REGFL)	/*返回值显示有重名*/
			{
				printf("\t\t注册失败：重名！\n");
				cmd_reg(temp);	/*递归调用*/
				break;
			}
			else
			{
				printf("\t\t注册失败:接受服务器消息错误\n");
				exit(1);
			}
		}
		else	/*两次密码输入不相同，再次循环直到输入正确为止*/
		{
			printf("\n\t\t密码输入一致！\n");
			printf("\t\t重新输入密码！\n");
			flag = 1;
		}
	}
}

int cmd_log(chat * temp)	/*login*/
{
	chat * recv_temp = (chat *)malloc(sizeof(chat));	/*接受的数据缓存区*/
	char passwd[20];
	int i;
	int flag = 1;	/*是否成功的标志位*/
	int ID;

	memset(recv_temp,0,sizeof(chat));
	while(flag)
	{
		printf("\t\t请输入ID :");
		scanf("%d",&temp->id);
		printf("\t\t请输入密码：");
		getchar();
		for(i=0; i<20; i++)
		{
			passwd[i] = getch();	/*不回显得到密码*/
			if((passwd[i] != '\r')&&(passwd[i] != '\n'))
			{
				printf("*");	/*得到一个密码字符，输出一个＊代替*/
			}		
			if(passwd[i] == '\n')	/*回车后，密码输入完成*/
			{
				passwd[i] = '\0';
				break;
			}
			if(i == 19)		/*密码输入过多*/
			{
				printf("\n\t\t密码位数太多！\n");
				memset(passwd,0,sizeof(passwd));	/*清空密码*/
				i = 0;		/*重新输入*/
				printf("\t\t请重新输入！\n");
				printf("\t\t请输入密码：");
			}
		}
		strcpy(temp->passwd,passwd);	/*将密码填入缓存区*/
		send(temp->sockfd,temp,sizeof(chat),0);	/*发送缓存数据*/
		printf("\n\t\t正在登录，请稍等……\n");
		sleep(1);
		recv(temp->sockfd,recv_temp,sizeof(chat),0);	/*接受服务器传来的数据，存放到接受缓存区*/
	//	display_temp(recv_temp);
		if(recv_temp->cmd == LOGOK)	/*返回值为登录成功*/
		{
			printf("\t\t登录成功！\n");
			flag = 0;	/*跳出while循环*/
			ID = temp->id;
			strcpy(temp->name,recv_temp->name);
			free(recv_temp);
			sleep(1);

			return ID;
			break;
		}
		else if(recv_temp->cmd == IDIN)	/*返回值显示该用户已经登录*/
		{
			printf("\t\t登录失败：用户已经登录！\n");
			sleep(2);
			break;
		}
		else if(recv_temp->cmd == PASSWDERROR)	/*received information i*/ 
		{
			printf("\t\t登录失败：密码错误!\n");
			sleep(2);
			break;
		}
		else
		{
			printf("\t\t登录失败：未知错误！\n");
			exit(1);
		}		
	}
}

void cmd_exit(chat * temp)	/*exit func*/
{
	temp->cmd = EXIT;
	send(temp->sockfd,temp,sizeof(chat),0);
	printf("\t\t你离开了聊天室！\n");
	close(temp->sockfd);
	exit(1);
}

void cmd_chat(chat * temp)	/*私聊*/
{
	chat * send_temp = (chat *)malloc(sizeof(chat));	/*发送的数据缓存区*/
	int TOID;
	
	memset(send_temp,0,sizeof(chat));
	printf("\t\t********启动私聊功能**********\n");
	printf("\t\t输入他的ID：");
	scanf("%d",&(send_temp->toid));	/*输入被聊天人的id*/
	send_temp->cmd = CHAT;
	TOID = send_temp->toid;
	send_temp->id = temp->id; 
	printf("\n");
	send(temp->sockfd,send_temp,sizeof(chat),0);	
	printf("\t\t正在发起会话...\n");
	sleep(1);
	printf("\t\t******输入 QUIT 退出会话******\n\n");	
	strcpy(send_temp->name,temp->name);	
	send_temp->id = temp->id;
	while(1)
	{	
		usleep(500000);
		printf("\t\t输入：");
		setbuf(stdin,NULL);
		gets(send_temp->msg);
		if(strcmp("QUIT",send_temp->msg) == 0)
		{
			break;
		}
		send_temp->toid = TOID;
		send_temp->cmd = CHATMSG;	//发送私聊消息
		send(temp->sockfd,send_temp,sizeof(chat),0);
		memset(send_temp->msg,0,sizeof(send_temp->msg));
	}
	send_temp->cmd = ENDCHAT;
	send(temp->sockfd,send_temp,sizeof(chat),0);	
	printf("\n\t\t********你退出了会话********\n");
}
	
void cmd_read(chat * temp)
{		
	send(temp->sockfd,temp,sizeof(chat),0);	
}

void cmd_all(chat * temp)
{
	chat * send_temp = (chat *)malloc(sizeof(chat));	/*发送的数据缓存区*/
	int TOID;
	
	memset(send_temp,0,sizeof(chat));
	printf("\t\t********启动群聊功能**********\n");
	printf("\n");
	printf("\t\t正在发起会话...\n");
	sleep(1);
	printf("\t\t******输入 QUIT 退出会话******\n\n");	
	strcpy(send_temp->name,temp->name);	
	send_temp->id = temp->id;	
	while(1)
	{	
		usleep(500000);
		printf("\t\t输入：");
		setbuf(stdin,NULL);
		gets(send_temp->msg);
		if(strcmp("QUIT",send_temp->msg) == 0)
		{
			break;
		}
		send_temp->cmd = ALL;	//发送私聊消息
		send(temp->sockfd,send_temp,sizeof(chat),0);
		memset(send_temp->msg,0,sizeof(send_temp->msg));
	}
	send_temp->cmd = ENDALL;
	send(temp->sockfd,send_temp,sizeof(chat),0);	
	printf("\n\t\t********你退出了会话********\n");
}

int cmd_file(chat * temp)	//发文件
{
	FILE * fp = NULL;

	char filename[64];
	printf("\t\t*************文件发送***************\n");
	printf("\t\t请输入您要发送文件的对象：\n");
	scanf("%d",&(temp->toid));
	temp->cmd = FIL;
	send(temp->sockfd,temp,sizeof(chat),0);	
	usleep(500000);
	printf("\t\t如果对方不在线，请输入QUIT退出\n");
	printf("\t\t请输入您要发送的文件名：(小于64字节)\n");
	scanf("%s",filename);
	if(strcmp("QUIT",filename) == 0)
	{
		return 0;
	}
	if((fp = fopen(filename,"r")) == NULL)
	{
		printf("\t\t您要发送的文件不存在\n");
	}
	else
	{
		strcpy(temp->filename,filename);
		int len = 0;
		FILE * file = fopen(filename,"r");
		if(file == NULL)
		{
			printf("fopen error\n");
			exit(1);
		}
		temp->cmd = SENDFIL;
		do
		{
			len = fread(temp->msg,1,sizeof(temp->msg),file);
			printf("\t\t服务器发送文件：%s ,长度为：%d \n",filename,len);
			send(temp->sockfd,temp,sizeof(chat),0);
			memset(temp->msg,0,sizeof(temp->msg));
			usleep(10000);
		}while(len == 1024);
		fclose(fp);

		temp->cmd = FILOK;
		send(temp->sockfd,temp,sizeof(chat),0);
		printf("\t\t************发送成功**************\n");
	}
}

void cmd_quit(chat * temp)	//注销当前的账号
{
	temp->cmd = QUIT;
	send(temp->sockfd,temp,sizeof(chat),0);
}

void cmd_data(chat * temp)	//查看聊天记录
{
	temp->cmd = DATA;
	send(temp->sockfd,temp,sizeof(chat),0);
}

void cmd_passwd(chat * temp)	//修改密码
{
	int flag = 1;
	char passwd[20];	/*密码*/
	char passwd_again[20];	/*再次确认密码*/
	int i;

	temp->cmd = PASSWD;
	while(flag)
	{
		printf("\t\t请修改密码：");
		for(i=0; i<20; i++)
		{
			passwd[i] = getch();	/*不回显得到密码*/
			if((passwd[i] != '\r')&&(passwd[i] != '\n'))
			{
				printf("*");	/*得到一个密码字符，输出一个＊代替*/
			}		
			if(passwd[i] == '\n')	/*回车后，密码输入完成*/
			{
				passwd[i] = '\0';
				break;
			}
			if(i == 19)		/*密码输入过多*/
			{
				printf("\n\t\t密码位数太多！\n");
				memset(passwd,0,sizeof(passwd));	/*清空密码*/
				i = 0;		/*重新输入*/
				printf("\t\t请重新输入！\n");
				printf("\t\t请输入密码：");
			}
		}
		printf("\n\t\t请再次确认：");
		for(i=0; i<20; i++)
		{
			passwd_again[i] = getch();
			if((passwd_again[i] != '\r')&&(passwd_again[i] != '\n'))
			{
				printf("*");
			}
			if(passwd_again[i] == '\n')
			{
				passwd_again[i] = '\0';
				break;
			}
			if(i == 19)
			{
				printf("\t\t密码位数太多!\n");
				memset(passwd_again,0,sizeof(passwd_again));
				i = 0;
				printf("\t\t请重新输入！\n");
				printf("\t\t请输入密码：");
			}
		}
		if(strcmp(passwd,passwd_again) == 0)	/*两次密码输入相同*/
		{
			printf("\n\t\t密码输入正确!\n");
			strcpy(temp->passwd,passwd);	/*将密码填入缓存区*/
			send(temp->sockfd,temp,sizeof(chat),0);	/*发送缓存数据*/
			printf("\t\t正在修改，请稍等……\n");
			sleep(1);
			flag = 0;
		}
		else	/*两次密码输入不相同，再次循环直到输入正确为止*/
		{
			printf("\n\t\t密码输入一致！\n");
			printf("\t\t重新输入密码！\n");
			flag = 1;
		}
	}
	
}

void cmd_ban(chat * temp)
{
	printf("\n\t\t********启动禁言功能**********\n");
	printf("\t\t输入他的ID：");
	scanf("%d",&(temp->toid));	/*输入被禁言人的id*/
	temp->cmd = BAN;
	printf("\n");
	send(temp->sockfd,temp,sizeof(chat),0);
}

void cmd_free(chat * temp)
{
	printf("\n\t\t*******关闭禁言功能**********\n");
	printf("\t\t输入他的ID：");
	scanf("%d",&(temp->toid));	/*输入解除禁言人的id*/
	temp->cmd = FREE;
	printf("\n");
	send(temp->sockfd,temp,sizeof(chat),0);
}

void cmd_break(chat * temp)
{
	printf("\n\t\t*******启动踢人功能**********\n");
	printf("\t\t输入他的ID：");
	scanf("%d",&(temp->toid));	/*输入被踢人的id*/
	temp->cmd = BREAK;
	printf("\n");
	send(temp->sockfd,temp,sizeof(chat),0);
}


