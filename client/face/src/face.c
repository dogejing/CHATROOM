/*****************************************************
copyright (C), 2014-2015, Lighting Studio. Co.,     Ltd. 
File name：
Author：Jerey_Jobs    Version:0.1    Date: 
Description：
Funcion List: 
*****************************************************/

#include "head.h"
//#include "package.h"

extern int judge_cmd_inter(char * input_cmd);


int interface(int ID)  /*登录界面*/
{
	int cmd = 0;	/*指令对应的值*/
	char input_cmd[16];	/*存放键入的指令*/
	
	system("clear");/*清屏*/
	do
	{
		printf("\t\t*******************************欢迎使用痞子晶的聊天室**************************\n\033[0m");
		if(ID != 0)
		{
		printf("\t\t*  您的ID：%d                                                               *\n",ID);
		}
		printf("\t\t*                                                                             *\n");
		printf("\t\t*******************************************************************************\n\033[0m");
    	printf("\t\t*                                                                             *\n\033[0m");
    	printf("\t\t*                                 1、 LOG:  登录                              *\n\033[0m");
    	printf("\t\t*                                                                             *\n\033[0m");
    	printf("\t\t*                                 2、 REG:  注册                              *\n\033[0m");
    	printf("\t\t*                                                                             *\n\033[0m");
    	printf("\t\t*                                 3、 EXIT: 退出                              *\n\033[0m");
    	printf("\t\t*                                                                             *\n\033[0m");
    	printf("\t\t*******************************************************************************\n\033[0m");
		while(cmd == ERROR)
		{
			printf("\t\t请输入指令:");
			//ffulsh(stdin);	/*缓冲*/
		
			__fpurge(stdin);

			gets(input_cmd);		/*键入指令*/
//			getchar();	/*吸收回车*/
			cmd = judge_cmd_inter(input_cmd);	/*判断指令，得出对应的值*/
			if(cmd == ERROR)
			{
				printf("\t\t输入指令错误！\n");
			}
		}	
	}while((cmd != REG)&&(cmd != LOG)&&(cmd != EXIT));/*当指令输入正确，跳出循环*/

	return cmd;
}

int main_face(int id,char *name,int clear_flag) 	/*main face include chat ...*/ 
{
	int cmd = 0;
	char input_cmd[16];
	
	if(clear_flag == 1)
		system("clear");
	do
	{
	if(clear_flag == 1)	
	{
		printf("\t\t---------------------------------------------\n");
		printf("\t\t|您的ID:                                    |\n");
		printf("\t\t|用户名:                                    |\n");
		printf("\t\t---------------------------------------------\n");
		printf("\t\t|              CHAT   :私聊                 |\n");
		printf("\t\t|              ALL    :群聊                 |\n");
		printf("\t\t|              READ   :查看在线成员         |\n");
		printf("\t\t|              FIL    :文件                 |\n");
		printf("\t\t|              DATA   :聊天记录             |\n");
		printf("\t\t|              PASSWD :修改密码             |\n");
		printf("\t\t|              QUIT   :注销                 |\n");
		printf("\t\t|              CLE    :清屏                 |\n");
		printf("\t\t---------------------------------------------\n");
		printf("\033[2;25H%d",id);
		printf("\033[3;25H%s",name);
	}
		while(cmd == ERROR)
		{
			usleep(10000);
			printf("\n\033[14;17H请输入指令：");
			printf("\033[K");
//			__fpurge(stdin);	
			setbuf(stdin,NULL);
			gets(input_cmd);		/*键入指令*/
//			getchar();	/*吸收回车*/
			cmd = judge_cmd_main(input_cmd);	/*判断指令，得出对应的值*/
			if(cmd == ERROR)
			{
				printf("\t\t输入指令错误！\n");
			}
		}
	}while((cmd != CHAT)&&(cmd != ALL)&&(cmd != FIL)&&(cmd != QUIT)&&(cmd != READ)&&(cmd != CLE)&&(cmd != DATA)&&(cmd != PASSWD));

	return cmd;
}

int root_face(int id,char *name,int clear_flag) 	/*main face include chat ...*/ 
{
	int cmd = 0;
	char input_cmd[16];
	
	if(clear_flag == 1)
		system("clear");
	do
	{	
		printf("\t\t---------------------------------------------\n");
		printf("\t\t|您的ID:                                    |\n");
		printf("\t\t|用户名:                                    |\n");
		printf("\t\t---------------------------------------------\n");
		printf("\t\t|              CHAT   :私聊                 |\n");
		printf("\t\t|              ALL    :群聊                 |\n");
		printf("\t\t|              READ   :查看在线成员         |\n");
		printf("\t\t|              FIL    :文件                 |\n");
		printf("\t\t|              DATA   :聊天记录             |\n");
		printf("\t\t|              PASSWD :修改密码             |\n");
		printf("\t\t|              BAN    :禁言                 |\n");
		printf("\t\t|              FREE   :解除禁言             |\n");
		printf("\t\t|              BREAK  :踢人                 |\n");
		printf("\t\t|              QUIT   :注销                 |\n");
		printf("\t\t|              CLE    :清屏                 |\n");
		printf("\t\t---------------------------------------------\n");
		printf("\033[2;25H%d",id);
		printf("\033[3;25H%s",name);

		while(cmd == ERROR)
		{
			usleep(10000);
			printf("\n\033[18;17H请输入指令：");
			printf("\033[K");
			setbuf(stdin,NULL);
			gets(input_cmd);		/*键入指令*/
			cmd = judge_cmd_root(input_cmd);	/*判断指令，得出对应的值*/
			if(cmd == ERROR)
			{
				printf("\t\t输入指令错误！\n");
			}
		}
	}while((cmd != CHAT)&&(cmd != ALL)&&(cmd != FIL)&&(cmd != QUIT)&&(cmd != READ)&&(cmd != CLE)&&(cmd != DATA)&&(cmd != PASSWD)&&(cmd != BAN)&&(cmd != FREE)&&(cmd != BREAK));

	return cmd;
}
