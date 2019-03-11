#include<stdio.h>
#include<wiringPi.h>
#include<string.h>
#define LIGHT_S 27
#define RELAY4 25
int main(void)
{ wiringPiSetupGpio();
pinMode(RELAY4,OUTPUT);
pinMode(LIGHT_S,INPUT);
while(1){
if(digitalRead(LIGHT_S)==0)
{ digitalWrite(RELAY4,HIGH);}
else
{ digitalWrite(RELAY4,LOW);}}
return 0;
}

