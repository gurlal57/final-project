#include <SoftwareSerial.h>
#include<stdio.h>
#include<string.h>

SoftwareSerial gsm(13,2);
String incoming_char;

void setup() {
  gsm.begin(9600);
 Serial.begin(9600);
 gsm.print("AT + CMGF=1\r"); 
  delay(2000);
  // Set module to send SMS data to serial out upon receipt 
  gsm.print("AT + CNMI=1,2,0,0,0\r");
  delay(1000);
}
void loop() {
////***********************************reciving message from gsm and sending to pi************************************//////
        if(gsm.available() >0) 
         {   
         incoming_char = gsm.readString();
         delay(50);
        Serial.print(incoming_char);
        delay(50);
        Serial.print(incoming_char.length());}
                   for(int x=0;x!=incoming_char.length();x++)
                   {if (incoming_char[x]=='a')
                   {Serial.print("good");}}
         }
