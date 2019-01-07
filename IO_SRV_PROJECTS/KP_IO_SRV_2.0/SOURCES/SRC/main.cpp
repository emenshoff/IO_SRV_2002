#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <kp_io_srv.h>
#include <io_srv_dump.h>

#include <time_utils.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void Bunner()
{
  printf("*******************************************************************\nInput-Output server for \"Granit\" telemetric complex and STM.\n\t\t Version 1.2. REST ltd, 17.04.02.\n*******************************************************************\n");
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void Usage()
{
  printf("Usage:\n KP_IO_SRV -port <port_number> -cfg <config_file_name>\n");
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void (*old_handler)(int);
void (*SIGTERM_old)(int);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void sig_fn(int i)
{
  char sg_name[80];
  switch(i)
  {
    case SIGABRT:
      sprintf(sg_name,"SIGABRT - ��������� ���������� ���������");
      break;
    case SIGINT:
      sprintf(sg_name,"SIGINT - ���������� ��������");
      break;
    case SIGTERM:
      sprintf(sg_name,"SIGTERM - ���������� ��������");
//      signal(SIGSEGV,SIGTERM_old); //��� ����������� ���������� ��������.
      exit(2);
      break;
    case SIGFPE:
      sprintf(sg_name,"SIGFPE - ������������ �������������� ��������");
      break;
    case SIGILL:
      sprintf(sg_name,"SIGILL - ������� ��������� ������������ �������� �������");
      break;
    case SIGSEGV:
      sprintf(sg_name,"SIGSEGV - ������ �����������");
      break;    
#ifndef WIN32
    case SIGSTOP:
      sprintf(sg_name,"SIGSTOP - ������� ��������");
      break;
    case SIGKILL:
      sprintf(sg_name,"SIGKILL - ����������� ��������");
      break;
    case SIGPIPE:
      sprintf(sg_name,"SIGPIPE - ����� ������ (�����)");
      break;
#endif
    default:
      break;
  }
  dump.Print(D_MSG,"������� ������� ������ %d (%s)",i,sg_name);
  if(i==SIGSEGV)
    signal(SIGSEGV,old_handler);//����� core �����...
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int main(int argc,char **argv)
{
  unsigned port;
  Bunner();
  signal(SIGABRT,sig_fn);
  signal(SIGINT,sig_fn);
  signal(SIGTERM,sig_fn);
  signal(SIGFPE,sig_fn);
  signal(SIGILL,sig_fn);
  old_handler = signal(SIGSEGV,sig_fn); 
  SIGTERM_old;
#ifndef WIN32
  signal(SIGSTOP,sig_fn);
  signal(SIGKILL,sig_fn);
  signal(SIGPIPE,SIG_IGN);
#endif
  if(argc==5)
  {
    if(!strcmp(argv[1],"-port") && !strcmp(argv[3],"-cfg"))
    {
      char *stop_smb;
      port=(unsigned)strtoul(argv[2],&stop_smb,10);
      if(strlen(argv[2])!=stop_smb-argv[2])
      {
        Usage();
        exit(-2);
      }
      dump.ResetDump("kp_dump.log");
      dump.Print(D_ERR, "��� ������������: %s",argv[4]);
      KpIoSrv io_srv;
      io_srv.Init(port,argv[4],"kp_dump.conf","kp_dump.log");
      if(io_srv.Ok())
      {
        double time;
        struct timeval t1,t2,t3;
        while(true)
        {
          //GetCurrTime(t1);
          io_srv.Step();
          //GetCurrTime(t2);
          //dump.Print(D_MSG,"����� ������ : %f �����������",Tv2Dbl(t2-t1)/1000);
        }
      }
      else
      {
        puts("Server starting failed! See logfile for detailes...");
        dump.Print(D_ERR, "������ ������� �������!...");
        exit(-1);
      }
    }
  }
  Usage();
  exit(-1);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
