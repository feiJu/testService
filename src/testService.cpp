//============================================================================
// Name        : testService.cpp
// Author      : chengfuju@shijingvr.com
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <ctime>
#include <string.h>
#include  <direct.h>

using namespace std;

#define SLEEP_TIME 5000 //间隔时间

string FILE_PATH = "C:\\log.txt"; //信息输出文件

bool brun=false;

SERVICE_STATUS servicestatus;

SERVICE_STATUS_HANDLE hstatus;



int WriteToLog(char* str);

void WINAPI ServiceMain(int argc, char** argv);

void WINAPI CtrlHandler(DWORD request);

int InitService();



int WriteToLog(char* str)

{

    FILE* pfile = fopen(FILE_PATH.data(),"a+");


    //fopen_s(&pfile,FILE_PATH,"a+");

    if (pfile==NULL)

    {

        return -1;

    }

   // time_t now_time;
  //  now_time = time(NULL);
  //  cout<<now_time;

    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    int year,month,day,hour,min,sec;
    year = 1900+timeinfo->tm_year;
    month = 1+timeinfo->tm_mon;
    day = timeinfo->tm_mday;
    hour = timeinfo->tm_hour;
    min = timeinfo->tm_min;
    sec = timeinfo->tm_sec;
//    printf ( "当前时间:%4d-%02d-%02d %02d:%02d:%02d\n\n",year, month,day,hour,min,sec);

    char now_time[100];
    sprintf(now_time,"[%4d-%02d-%02d %02d:%02d:%02d]",year, month,day,hour,min,sec);


    char str_log[1024];
    sprintf(str_log,"%s : %s",now_time,str);

    fprintf(pfile, "%s\n",str_log);
    //fwrite(str, sizeof(char), 1, pfile);
    //fprintf_s(pfile,"%s\n",str);

    fclose(pfile);

    return 0;

}



void WINAPI ServiceMain(int argc, char** argv)

{

    servicestatus.dwServiceType = SERVICE_WIN32;

    servicestatus.dwCurrentState = SERVICE_START_PENDING;

    servicestatus.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN|SERVICE_ACCEPT_STOP;//在本例中只接受系统关机和停止服务两种控制命令

    servicestatus.dwWin32ExitCode = 0;

    servicestatus.dwServiceSpecificExitCode = 0;

    servicestatus.dwCheckPoint = 0;

    servicestatus.dwWaitHint = 0;

    hstatus = ::RegisterServiceCtrlHandler("testservice", CtrlHandler);

    if (hstatus==0)

    {

        WriteToLog("RegisterServiceCtrlHandler failed");

        return;

    }

    WriteToLog("RegisterServiceCtrlHandler success");

    //向SCM 报告运行状态

    servicestatus.dwCurrentState = SERVICE_RUNNING;

    SetServiceStatus (hstatus, &servicestatus);

    //下面就开始任务循环了，你可以添加你自己希望服务做的工作

    brun=true;

    MEMORYSTATUS memstatus;

    char str[100];

    memset(str,'\0',100);

    while (brun)

    {

        GlobalMemoryStatus(&memstatus);

        int availmb=memstatus.dwAvailPhys/1024/1024;

        sprintf(str,"available memory is %dMB",availmb);

        //sprintf_s(str,100,"available memory is %dMB",availmb);

        WriteToLog(str);

        Sleep(SLEEP_TIME);

    }

    WriteToLog("service stopped");

}



void WINAPI CtrlHandler(DWORD request)

{

    switch (request)

    {

        case SERVICE_CONTROL_STOP:

            brun=false;

            servicestatus.dwCurrentState = SERVICE_STOPPED;

            break;

        case SERVICE_CONTROL_SHUTDOWN:

            brun=false;

            servicestatus.dwCurrentState = SERVICE_STOPPED;

            break;

        default:

            break;

    }

    SetServiceStatus (hstatus, &servicestatus);
}

int main(){
//	WriteToLog("------------ test service -------------");
//	char   crnPath[MAX_PATH];
//	getcwd(crnPath, MAX_PATH);
//	//printf( "The   current   directory   is:   %s ",   crnPath);	// 获取当前工程路径
//	WriteToLog(crnPath);
//	// 获取exe路径
//	char chpath[MAX_PATH];
//	GetModuleFileName(NULL,(LPSTR)chpath,sizeof(chpath));
//	std::cout<<chpath<<std::endl;
//	WriteToLog("------------ exe ----------------");
//	WriteToLog(chpath);

	//获取当前模块所在路径
	char szFilePath[MAX_PATH];//szExePath[MAX_PATH];
	if(GetModuleFileName(NULL,szFilePath,MAX_PATH)>0)
	{
	        (*strrchr(szFilePath,'\\'))='\0';//丢掉文件名，得到路径
	}

	FILE_PATH = (string) szFilePath+"\\"+"log.txt";

	WriteToLog("------------ service start -------------");

    SERVICE_TABLE_ENTRY entrytable[2];

    entrytable[0].lpServiceName="testservice";

    entrytable[0].lpServiceProc=(LPSERVICE_MAIN_FUNCTION)ServiceMain;

    entrytable[1].lpServiceName=NULL;

    entrytable[1].lpServiceProc=NULL;

    StartServiceCtrlDispatcher(entrytable);

    return 0;
}
