//打开文件，读取文件的内容
 
#include<stdio.h>
 
// #include<io.h>
 
#include<fcntl.h>
#include <unistd.h>
 
int main(void){
 
   // int fd=open("sperf_tmo.out",O_RDONLY);
   int fd=open("sperf_tmp.output",O_RDONLY);
   if(fd==-1){
 
      printf("can not open the file\n");
   
      return 1;
 
   }
 
   char buf[1024]={"\0"};
 
   int len=read(fd,buf,1024);
 
   printf("%s\nlen=%d\n",buf,len);
 
   close(fd);
 
   return 0;
 
}