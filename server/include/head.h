#ifndef _HEAD_H_
#define _HEAD_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sqlite3.h>
#include <pthread.h>
#include <assert.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORTNUMBER 8888	/*端口号*/
#define LISTEN_MAX 20 /*最大监听人数*/

typedef struct ch
{
	char name[20];		/*1用户名*/
	char passwd[20];	/*2密码*/
	int  sockfd;		/*3套接字*/
	int	 id;			/*4ID*/
	char ip[64];		/*5IP*/
	char toname[20];	/*6接受消息的人*/
	int  cmd;			/*7指令*/
	char msg[1024];		/*8发送的消息*/
	int  flag;			/*9禁言标志*/
	char filename[64];	/*10传输的文件名*/
	int toid;
	int member_num;		//在线总人数
	int member_id;		//在线成员id
	char member_name[20]; 	//在线成员用户名
	char time[32];
}chat;

typedef struct p
{
	int sockfd;
	sqlite3 * db;
}ph;

enum cmd 
{
	ERROR = 0, 
	REG	  = 1,   	/*注册*/
	REGOK = 100, 	/*注册成功*/
	REGFL = -100,	/*注册失败*/
	LOG	  =	2,		/*登录*/
	LOGOK = 200,		/*登录成功*/
	LOGFL =	-200,	/*登录失败*/
	EXIT  =	3,		/*退出*/
//	NEXIT =	-3,		/*退出失败*/
	IDOK  = 400,
	IDFL  = -400,
	PASSWDERROR  =  -500, 
	CHAT  = 4,		/*chat to one*/
	ENDCHAT = -4,	//私聊结束
	CHATOK = 600,
	CHATFL = -600,
	ALL   = 5,		/*chat to all user*/
	ENDALL = -5,
	FIL  = 6,		/*传输文件*/
	QUIT  = 7,		/*用户注销*/
	CHATMSG = 8,		/*私聊消息*/
	CHATOUT = -8,
	USERIN = 9,
	USEROUT = -9,
	IDIN = 10,
	IDOUT = -10,
	READ = 11,	//查看在线成员
	NOREAD = -11,	//没有在线成员
	READNEXT = 12,	//接受下一个在线成员信息
	ENDREAD = -12,	//没有更多的在线成员
	READOK = 13,	//完成查看
	CLE = 14,
	BEGINREAD = 15,	//开始
	ALLOK = 16,  	//群发成功
	ONLYYOU = 17,	//心疼只有你一人
	FILOK = 18,	//文件接受完成
	SENDFIL = 19,	//正在发送文件
	DATA = 20,	//查看聊天记录
	ENDDATA = 21,	//发送聊天记录完成
	NODATA = -21,	//没有聊天记录
	PASSWD = 22,	//修改密码
	PASSWDOK = 23,	//修改密码成功
	PASSWDFL = -23,	//修改密码失败
	BAN = 24,	//禁言
	FREE = -24,	//解除禁言
	BREAK = 25,	//踢人
	BANOK = 26,
	BANFL = -26,
	FREEOK = 27,
	FREEFL = -27,
};

#endif
