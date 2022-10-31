#include<file.h>
#include "GetOptAndPath.h"
void rm_r(char *path)
{
    int count;
    int j=0;
    char item[160];
    char pathbuf[100];
    int fd;
    struct st_inode inode;

    fd=open(path,0111);  
    if(fd==-1)
    {
         printf("Wrong file %s!\n",path); 
      //   (*flag)=1;
         return;
    }        
    if(stat(path,&inode)==-1)
    {
        printf("Inode of %s cannot be read!\n",path);
      //  (*flag)=1;
        return;
    }
 
      
    if((inode.st_mode&0x4000)==0)
    {    
          
       // printf("%s unlink:%d\n",path,unlink(path));
          unlink(path);
          return;
    }   
    else
   { 
          count=read(fd,item,32);
        

          while(count!=0)                     
          {  
        	  	  if(*((int*)item)==0)
                 {
                      for(j=0;j<160;j++)
                      item[j]='\0';
                      int addition_esize=*((int*)(item+28));
                      //printf("addition_esize: %d\n" ,addition_esize);
                      seek(fd,addition_esize*32,1);
                      count=read(fd,item,32);
                 }    
                  else
                 {   
					   int addition_esize=*((int*)(item+32-4));
					   //printf("addition_esize: %d\n" ,addition_esize);
					   int k;
					   for(k=1;k<=addition_esize;k++)
					   {
						   count=read(fd,item+28+(k-1)*32,32);
						   if(count!=32)
						   {
							   printf("error reading long name" ,item+4);
						   }
					   }

                      strcpy(pathbuf,path);//Combine parent path 
                                         //and current item path
                      strcat(pathbuf,"/");
                      strcat(pathbuf,item+4);//to get real path

                      rm_r(pathbuf);
                      //clear pathbuf
                      for(j=0;j<100;j++)
                      pathbuf[j]='\0';
                      //clear item: 
                      for(j=0;j<160;j++)
                      item[j]='\0';      
                      count=read(fd,item,32);
                 }
         }//while
    
         unlink(path);

         return;//Important! Don't forget that!
    
   }//else
}

void usage()
{
     printf("Usage:rm [options] filename1 filename2 ...\n");
     printf("Options:\n");
     printf("-r:Delete recursively all the files under a directory\n");
     printf("-f:Compulsory delete file\n");
     printf("--help:show usage\n");
     
}

void main1(int argc, char **argv)
{
 //int *rm_r_flag_fail;
 //int val=0;
 char answer[10];
  //  rm_r_flag_fail=&val;
  //  rm_r(argv[1],rm_r_flag_fail);

 int fd;
 struct st_inode inode;//Initially "struct st_inode *inode" is wrong!
// char item[32];//store directory infos. 
// char pathbuf[100];
// int i=0;
// int j=0;
// int count;
// int ret;

 struct option options[]=
{
 {"-r","",0,'r'},
 {"-f","",0,'f'},
 {"","--help",0,'h'}
};

int flag=0;
int flag_file=0;
int flagf=0;
int flagr=0;
int getret;
char optarg[10];
char path[10][100];
int *pathi;
int *optind;
int a=0;
int b=1;
int i=0;

pathi=&a;
optind=&b;
//rm_r_flag_fail=&val;


while((getret=GetOptAndPath(argc,argv,optind,pathi,optarg,path,options,3)))
{
   switch(getret)
   {
       case 1: 
       flag_file=1;
       break;
       case 2:
       printf("The option is not right.\n");
       flag=1;
       break;
       case 3:
       printf("Certain options lacks argument\n");
       flag=1;
       break;
       case 'h':
       flag=1;
       break;
       case 'r':
       flagr=1;
       break; 
       case 'f':
       flagf=1;
       break;
   }
}

if(flag_file==0)
{
  printf("No path has been input!\n");
  flag=1;
}

if(argc==1)
{
   printf("Too few options!\n");
   flag=1;
}


if(flag==1)
{
  printf("\n");
  usage();
  return;
}
 
     for(i=0;i<(*pathi);i++)    
     {
          if(flagf==0)
          {
           printf("remove %s? \'y\' for Yes. Press anykey to quit.",path[i]);
           gets(answer);  
           printf("\n");
           if((answer[0]!='y')&&(answer[1]!='Y'))
           return;
              
               if(flagr==1)
                {
                     rm_r(path[i]);
                     
                }
                else
                {     int stat_ret=stat(path[i],&inode);
                     if(stat_ret==-1)
                       {
                         //printf("stat_ret: %d\n",stat_ret);
                    	 printf("Wrong file \'%s\'!\n",path[i]);
                       } 
                     else
                       {
                           if((inode.st_mode&0x4000)==0)
                              unlink(path[i]);
                           else
                              printf("\'%s\' is a directory!\n",path[i]);
                                 
                       }                   

                }

          }
          else
          {

                
               if(flagr==1)
                {
                     rm_r(path[i]);
                     
                }
                else
                {     
                     if(stat(path[i],&inode)==-1)
                       {
                         printf("Wrong file \'%s\'!\n",path[i]);
                         
                       } 
                     else
                       {
                           if((inode.st_mode&0x4000)==0)
                              unlink(path[i]);
                           else
                              printf("\'%s\' is a directory!\n",path[i]);
                                 
                       }                   

                }


          }          

      }//for

 }
