/*LIBRARY*/
#include<stdio.h>
#include<wiringPi.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<wiringSerial.h>
/*DECLARAION OF RASPBERRYPI PINS*/
#define RELAY 9         //Moisture Pump
#define SOIL_S 17       //SOIL SENSOR
int main (void)
{/*SERIAL DISPLAY*/
        int fd, a; //PREDEFINED INT
char *b = "m"; //switch on motor from serial
char *txt1 = "z";//message to serial that pump is on
char *gsm1 = "e";//ask for status of moisture
char *stat4 = "q",*stat3 = "r";//call moistdn and moistup fun on arduino
char *txt2 = "M";
fd = serialOpen("/dev/ttyAMA0",115200); //UART at 115200, ttyAMA0.
        /*GPIO*/
        wiringPiSetupGpio();
        /*WiringPi GPIO*/
        /*DECLARATION OF OUTPUTS*/
        pinMode(RELAY,OUTPUT); //SOIL MOISTURE
/*DECLARATION OF INPUTS*/
        pinMode(SOIL_S,INPUT);
/*SENSOR FUNCTIONS*/
        for(;;){
        a = serialGetchar(fd);
        //printf("%c",a);
        //sensor reading*/
                //moisture reading*/
          if((a) == *gsm1)
          { if(digitalRead(SOIL_S) == 1)
                  { delay(100);
        printf("no moisture");
        fflush(stdout);
        serialPutchar(fd, *stat4);
                }
else if(digitalRead(SOIL_S) == 0){
        delay(100);
        printf("sufficient moisture");
        fflush(stdout);
        serialPutchar(fd, *stat3);
        }}
//driving output relays*/
        if((a) == *b){
        digitalWrite(RELAY,HIGH);
        delay(100);
        printf("RELAY1\n");
        fflush(stdout);
        serialPutchar(fd, *txt1);
while(digitalRead(SOIL_S) == 0)
{ digitalWrite(RELAY,LOW);
delay(100);
printf("relay off");
fflush(stdout);
serialPutchar(fd,*txt2);
}}}}
        



