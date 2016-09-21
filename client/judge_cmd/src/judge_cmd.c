/*****************************************************
copyright (C), 2014-2015, Lighting Studio. Co.,     Ltd. 
File name：
Author：Jerey_Jobs    Version:0.1    Date: 
Description：
Funcion List: 
*****************************************************/

#include "head.h"
//#include "package.h"

int judge_cmd_inter(char * input_cmd)	/*判断登录界面指令，得出对应的值*/
{
	if( strcmp(input_cmd,"REG") == 0)	/*注册*/
	{
		return REG;
	}
	else if(strcmp(input_cmd,"LOG") == 0)	/*登录*/
	{
		return LOG;
	}
	else if(strcmp(input_cmd,"EXIT") == 0)	/*退出*/
	{
		return EXIT;
	}
	else
	{
		return ERROR;
	}
}

int judge_cmd_main(char * input_cmd)	/*判断主界面指令，返回相应值*/
{
	if( strcmp(input_cmd,"CHAT") == 0)	/*私聊*/
	{
		return CHAT;
	}
	else if(strcmp(input_cmd,"ALL") == 0)	/*群聊*/
	{
		return ALL;
	}
	else if(strcmp(input_cmd,"FIL") == 0)	/*文件传输*/
	{
		return FIL;
	}
	else if(strcmp(input_cmd,"READ") == 0)	/*查看在线帐户*/
	{
		return READ;
	}
	else if(strcmp(input_cmd,"DATA") == 0)	/*查看聊天记录*/
	{
		return DATA;
	}
	else if(strcmp(input_cmd,"PASSWD") == 0)/*修改密码*/
	{
		return PASSWD;
	}
	else if(strcmp(input_cmd,"QUIT") == 0)	/*注销帐户*/
	{
		return QUIT;
	}
	else if(strcmp(input_cmd,"CLE") == 0)	/*清屏*/
	{
		return CLE;
	}
	else
	{
		return ERROR;
	}
}

int judge_cmd_root(char * input_cmd)	/*判断管理员指令，返回相应值*/
{
	if( strcmp(input_cmd,"CHAT") == 0)	/*私聊*/
	{
		return CHAT;
	}
	else if(strcmp(input_cmd,"ALL") == 0)	/*群聊*/
	{
		return ALL;
	}
	else if(strcmp(input_cmd,"FIL") == 0)	/*文件传输*/
	{
		return FIL;
	}
	else if(strcmp(input_cmd,"READ") == 0)	/*查看在线帐户*/
	{
		return READ;
	}
	else if(strcmp(input_cmd,"DATA") == 0)	/*查看聊天记录*/
	{
		return DATA;
	}
	else if(strcmp(input_cmd,"PASSWD") == 0)/*修改密码*/
	{
		return PASSWD;
	}
	else if(strcmp(input_cmd,"BAN") == 0)	/*禁言*/
	{
		return BAN;
	}
	else if(strcmp(input_cmd,"FREE") == 0)	/*禁言*/
	{
		return FREE;
	}
	else if(strcmp(input_cmd,"BREAK") == 0)	/*禁言*/
	{
		return BREAK;
	}
	else if(strcmp(input_cmd,"QUIT") == 0)	/*注销帐户*/
	{
		return QUIT;
	}
	else if(strcmp(input_cmd,"CLE") == 0)	/*清屏*/
	{
		return CLE;
	}
	else
	{
		return ERROR;
	}
}





