#include <SoftwareSerial.h>
const int RX1 = 8;
const int TX1 = 9;
const int RX2 = 10;
const int TX2 = 11;
SoftwareSerial mySerial(RX2,TX2);
SoftwareSerial gsm(RX1,TX1);
char inData[20];
char inChar=19;
byte index = 0;
void setup() {
  
  mySerial.begin(115200);
  gsm.begin(9600);
 Serial.begin(115200);
}
void loop() {

  
  if (Serial.available())
    mySerial.write(Serial.read());
     if (mySerial.available())
    Serial.write(mySerial.read());
    if (gsm.available())
        mySerial.write(gsm.read());
        if(comp("k") == 0)
{
    tempnor();
    }
    
if(comp("u") == 0)
{
    tempup();
    }
    if(comp("s") == 0)
{
    tempdn();
    }
    if(comp("r") == 0)
{
    moistup();
    }
    if(comp("q") == 0)
{
    moistdn();
    }
    if(comp("p") == 0)
{
    ldron();
    }
    if(comp("o") == 0)
{
    ldroff();
    }
    if(comp("z") == 0){
    text1();
    }
  if(comp("y") == 0){
    text2();
    }
  if(comp("x") == 0){
    text3();
    }
  if(comp("w") == 0){
    text4();
    }
  if(comp("v") == 0){
    text5();
    }}
    //comparison function*/
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
//status functions*/
void tempnor(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("temperature is normal");
    delay(1000);
    Serial.println("every thing good");
    gsm.println((char)26);
    delay(2000);
}
void tempup(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("Switch On Cooler");
    delay(1000);
    Serial.println("COOLER");
    gsm.println((char)26);
    delay(2000);
}
void tempdn(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("Switch On heater");
    delay(1000);
    Serial.println("COOLER");
    gsm.println((char)26);
    delay(2000);
}
void moistup(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("Switch off pump");
    delay(1000);
    Serial.println("pump off");
    gsm.println((char)26);
    delay(2000);
}
void moistdn(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("Switch On pump");
    delay(1000);
    Serial.println("pump on");
    gsm.println((char)26);
    delay(2000);
}
void ldron(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("Switch off light");
    delay(1000);
    Serial.println("light off");
    gsm.println((char)26);
    delay(2000);
}
void ldroff(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("Switch On light");
    delay(1000);
    Serial.println("light on");
    gsm.println((char)26);
    delay(2000);
}
// output_relay_functions */
void text1(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("RELAY1");
    delay(1000);
    Serial.println("RELAY1");
    gsm.println((char)26);
    delay(2000);
}

void text2(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("RELAY2");
    delay(1000);
    Serial.println("RELAY2");
    gsm.println((char)26);
    delay(2000);
}

void text3(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("RELAY3");
    delay(1000);
    Serial.println("RELAY3");
    gsm.println((char)26);
    delay(2000);
}

void text4(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("RELAY4");
    delay(1000);
    Serial.println("RELAY4");
    gsm.println((char)26);
    delay(2000);
}

void text5(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("RELAY5");
    delay(1000);
    Serial.println("RELAY5");
    gsm.println((char)26);
    delay(2000);
}

