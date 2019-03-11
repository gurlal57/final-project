#include <wiringPi.h>
#include <stdio.h>
#include <string.h> 
#define RELAY 9
#define RELAY2 11
#define RELAY3 0
#define RELAY4 25
#define RELAY5 6
 
int main(void)
{
wiringPiSetupGpio();
pinMode(RELAY, OUTPUT);
pinMode(RELAY2,OUTPUT);
pinMode(RELAY3,OUTPUT);
pinMode(RELAY4,OUTPUT);
pinMode(RELAY5,OUTPUT); 
while(1){
digitalWrite(RELAY, LOW);
delay(1000);
digitalWrite(RELAY, HIGH);
delay(1000);
digitalWrite(RELAY2, LOW);
delay(1000);
digitalWrite(RELAY2, HIGH);
delay(1000);
digitalWrite(RELAY3, LOW);
delay(1000);
digitalWrite(RELAY3, HIGH);
delay(1000);
digitalWrite(RELAY4, LOW);
delay(1000);
digitalWrite(RELAY4, HIGH);
delay(1000);
digitalWrite(RELAY5, LOW);
delay(1000);
digitalWrite(RELAY5, HIGH);
delay(1000);
}
 
return 0;
}
