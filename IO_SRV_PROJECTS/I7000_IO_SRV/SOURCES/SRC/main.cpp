#include <stdlib.h>
#include <stdio.h>
#include <I7000_io_srv.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void Bunner()
{
  printf("*******************************************************************\nInput-Output server for I7041 and I7042 DIO modules and STM.\n\t\t Version 1.0a. REST ltd, 2001.\n*******************************************************************\n");
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void Usage()
{
  printf("Usage:\n I7000_IO_SRV -port <port_number> -cfg <config_file_name>");
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void main(int argc,char **argv)
{
  unsigned port;
  Bunner();
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
      I7000_IoSrv io_srv;
      io_srv.Init(port,argv[4],"kp_dump.conf","kp_dump.log");      
      if(io_srv.Ok())
      {
        while(true)
        {
          io_srv.Step();
        }
      }
      else
      {
        puts("Server starting failed! See logfile for detailes...");
        exit(-1);
      }
    }
  }
  Usage();
  exit(-1);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%