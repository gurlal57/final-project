#include<stdio.h>
#include<wiringPi.h>
#include<wiringSerial.h>
int main(){
        int fd;
        fd = serialOpen("/dev/ttyAMA0",115200);
        char c,b[10];
char *stat = b;
        char *a = "t";
char *f = "z";
        wiringPiSetupGpio();
while(1)
        {
        //putchar(serialGetchar(fd));
        //fflush(stdout);
        c = serialGetchar(fd);
        printf("%c",c);
        if((c) == *a){
    //printf("enter value of b\n");
      //scanf("%c",b);
  fflush(stdout);
        serialPrintf(fd,f);
        }
}
}
