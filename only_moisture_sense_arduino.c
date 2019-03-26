#include <SoftwareSerial.h>
//declarations for serial com.//
const int RX2 = 10;
const int TX2 = 11;
SoftwareSerial mySerial(RX2,TX2);

char inData[20];
char inChar=0;
byte index = 0;
void setup() {
  
  mySerial.begin(115200);
  Serial.begin(9600);
}
void loop() {

  if (Serial.available())
    mySerial.write(Serial.read());
     if (mySerial.available())
    Serial.write(mySerial.read());

if(comp("r") == 0)
{
    moistup();
    }
    if(comp("q") == 0)
{
    moistdn();
    }
    if(comp("z") == 0){
    text1();
    }
     if(comp("M") == 0){
    text2();
    }
}
    
///comparison//
//comparison function for pi characters*/
    char comp(char* This){
 while(mySerial.available() > 0)
 {
   if(index < 19) // One less than the size of the array
   {
     inChar = mySerial.read(); // Read a character
     inData[index] = inChar; // Store it
     index++; // Increment where to write next
     inData[index] = '\0'; // Null terminate the string
   }
 }
 if(strcmp(inData,This)  == 0){
   for(int i=0;i<19;i++){
     inData[i]=0;
   }
   index=0;
   return(0);

 }
 else{
   return(1);
}}
////out_put_repal/////
// output_relay_functions */
void text1(){
   
    Serial.println("RELAY2(motor) is on");
    
}
// output_relay_functions */
void text2(){
    
    Serial.println("RELAY2(motor) is off and moisture is good");
    
}
////stat_fun///////
void moistup(){
    
    Serial.println("good moisture condition");
    
    delay(2000);
}
void moistdn(){
    
    Serial.println("conditions are dry");
    
    delay(2000);
}

