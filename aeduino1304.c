#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//*************************************declarations for serial com.******************************//

SoftwareSerial rpi(12,3);
SoftwareSerial gsm(13,2);
char incoming_char=0;
char incoming_chhar=0;

//********************************* User Defined Variables for ec sensing*****************************//
int R1= 1000;
int Ra=25; //Resistance of powering Pins
int ECPin= A0;
int ECGround=A1;
int ECPower =A4;
float PPMconversion=0.5;
//*Compensating for temperature**//
float TemperatureCoef = 0.019; 
//** Cell Constant For Ec Measurements **//
float K=-0.07;

//** Temp Probe Related **//
#define ONE_WIRE_BUS 10          // Data wire For Temp Probe is plugged into pin 10 on the Arduino
const int TempProbePossitive =8;  //Temp Probe power connected to pin 9
const int TempProbeNegative=9;    //Temp Probe Negative connected to pin 8
// END Of Recomended User Inputs **//
OneWire oneWire(ONE_WIRE_BUS);// Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature.
float Temperature=10;
float EC=0;
float EC25 =0;
int ppm =0;
 
float raw= 0;
float Vin= 5;
float Vdrop= 0;
float Rc= 0;
float buffer=0;
////*********************ec sensor declarations end here***********************//

void setup() {
  
  rpi.begin(9600);
  gsm.begin(9600);
 Serial.begin(9600);
 //******************************ec sensor related setup*************************//
 
 pinMode(TempProbeNegative , OUTPUT ); //seting ground pin as output for tmp probe
  digitalWrite(TempProbeNegative , LOW );//Seting it to ground so it can sink current
  pinMode(TempProbePossitive , OUTPUT );//ditto but for positive
  digitalWrite(TempProbePossitive , HIGH );
  pinMode(ECPin,INPUT);
  pinMode(ECPower,OUTPUT);//Setting pin for sourcing current
  pinMode(ECGround,OUTPUT);//setting pin for sinking current
  digitalWrite(ECGround,LOW);//We can leave the ground connected permanantly
 
  delay(100);// gives sensor time to settle
  sensors.begin();
  delay(100);
  R1=(R1+Ra);// Taking into acount Powering Pin Resitance
  // ****************************AT command to set SIM900 to SMS mode****************************//
  gsm.print("AT + CMGF=1\r"); 
  delay(3000);
  // Set module to send SMS data to serial out upon receipt 
  gsm.print("AT + CNMI=2,2,0,0,0\r");
  delay(1000);
}

void loop() {
////***********************************reciving message from gsm and sending to pi************************************//////
        if(gsm.available() >0) 
         {   incoming_char = gsm.read(); 
          Serial.print(incoming_char);
    //******************sending char recived from gsm to rpi********************//
    //in below loops we are just asking sensor status for light,moisture and temp//
    
    if(compchar("m",incoming_char)==0)
    {
      delay(1000);
      rpi.write("e");
    }

    if(compchar("t",incoming_char)==0)
    {
      delay(1000);
      rpi.write("g");
    }
    if(compchar("l",incoming_char)==0)
    {
      delay(1000);
      rpi.write("f");}
      
 //********************************in the below loop the value of analog sensoris being read*******************************// 
         if(compchar("g",incoming_char) == 0)
         {    GetEC();
             delay(5000);
             }

    //********************************message from gsm to drive outputs*******************************//
    if(compchar("t",incoming_char)==0)
    {
      delay(1000);
      rpi.write("t");
    }
    if(compchar("m",incoming_char)==0)
    {
      delay(1000);
      rpi.write("m");
    }
    if(compchar("l",incoming_char)==0)
    {
      delay(1000);
      rpi.write("l");
    }
    if(compchar("i",incoming_char)==0)
    {
      delay(1000);
      rpi.write("i");
    }
    if(compchar("h",incoming_char)==0)
    {
      delay(1000);
      rpi.write("h");
    }
      }
////***********************************receiving serial data from rpi and calling sta functions******************///
  if (rpi.available()>0)
    {incoming_chhar = rpi.read();
      Serial.write(incoming_chhar);
    delay(1000);
    //*************************************below if statements send messages to user's phone*****************************//
        
        if(comp("k",incoming_chhar) == 0)
{
    tempnor();
    }
    
if(comp("u",incoming_chhar) == 0)
{
    tempup();
    }
    if(comp("s",incoming_chhar) == 0)
{
    tempdn();
    }
    if(comp("r",incoming_chhar) == 0)
{
    moistup();
    }
    if(comp("q",incoming_chhar) == 0)
{
    moistdn();
    }
    if(comp("n",incoming_chhar) == 0)
{
    ldron();
    }
    if(comp("o",incoming_chhar) == 0)
{
    ldroff();
    }
    //********************************sending message to GSM/USER's phone for output's "on" status***************************//
    
    if(comp("z",incoming_chhar) == 0){
    text1();
    }
  if(comp("y",incoming_chhar) == 0){
    text2();
    }
  if(comp("x",incoming_chhar) == 0){
    text3();
    }
  if(comp("w",incoming_chhar) == 0){
    text4();
    }
  if(comp("v",incoming_chhar) == 0){
    text5();
    }

    
    
    //*****************************sending messages for "off" status of outputs *********************************///
     if(comp("M",incoming_chhar) == 0){
    text6();
    }
  if(comp("T",incoming_chhar) == 0){
    text7();
    }
  if(comp("L",incoming_chhar) == 0){
    text8();
    }
  if(comp("I",incoming_chhar) == 0){
    text9();
    }
  if(comp("H",incoming_chhar) == 0){
    text10();
    }
    }
    }
    
	//************ ec sense fun************************//
void GetEC(){
//*********Reading Temperature Of Solution *******************//
sensors.requestTemperatures();// Send the command to get temperatures
Temperature=sensors.getTempCByIndex(0); //Stores Value in Variable

//************Estimates Resistance of Liquid ****************//
digitalWrite(ECPower,HIGH);
raw= analogRead(ECPin);
raw= analogRead(ECPin);// This is not a mistake, First reading will be low beause if charged a capacitor
digitalWrite(ECPower,LOW);
 
//***************** Converts to EC **************************//
Vdrop= (Vin*raw)/1024.0;
Rc=(Vdrop*R1)/(Vin-Vdrop);
Rc=Rc-Ra; //acounting for Digital Pin Resitance
EC = 1000/(Rc*K);
 
//*************Compensating For Temperaure********************//
EC25  =  EC/ (1+ TemperatureCoef*(Temperature-25.0));
ppm=(EC25)*(PPMconversion*1000);
gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
gsm.println("Rc: ");
delay(500);
gsm.println(Rc);
delay(500);
gsm.println(" EC: ");
delay(500);
gsm.println(EC25);
delay(500);
gsm.println(" Simens  ");
delay(500);
gsm.println(ppm);
delay(500);
gsm.println(" ppm  ");
delay(500);
gsm.println(Temperature);
delay(500);
gsm.println(" *C ");
 
}
//************************** End OF EC Function ***************************//
 
 //***************** output_relay_functions FOR switch on status********************** */
void text1(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("RELAY1(water pump) is on");
    delay(1000);
    Serial.println("RELAY1(motor) is on");
    gsm.println((char)26);
    delay(2000);
}

void text2(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("RELAY2(fan) is on");
    delay(1000);
    Serial.println("RELAY2(fan) is on");
    gsm.println((char)26);
    delay(2000);
}

void text3(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("RELAY3(bulb) is on");
    delay(1000);
    Serial.println("RELAY3(bulb) is on");
    gsm.println((char)26);
    delay(2000);
}

void text4(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("RELAY4(N pump) is on");
    delay(1000);
    Serial.println("RELAY4(N pump) is on");
    gsm.println((char)26);
    delay(2000);
}

void text5(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("RELAY5(heater)is on");
    delay(1000);
    Serial.println("RELAY5(heater)is on");
    gsm.println((char)26);
    delay(2000);
}
//************** output_relay_functions FOR switch off status of relay output******* */
void text6(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("RELAY1(water pump) is off");
    delay(1000);
    Serial.println("RELAY1(water pump) is off");
    gsm.println((char)26);
    delay(2000);
}

void text7(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("RELAY2(fan) is off");
    delay(1000);
    Serial.println("RELAY2(fan) is off");
    gsm.println((char)26);
    delay(2000);
}

void text8(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("RELAY3(bulb) is off");
    delay(1000);
    Serial.println("RELAY3(bulb) is off");
    gsm.println((char)26);
    delay(2000);
}

void text9(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("RELAY4(N pump) is off");
    delay(1000);
    Serial.println("RELAY4(N pump) is off");
    gsm.println((char)26);
    delay(2000);
}

void text10(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("RELAY5(heater)is off");
    delay(1000);
    Serial.println("RELAY5(heater)is off");
    gsm.println((char)26);
    delay(2000);
}

//***********sensors status functions*********************/
void tempnor(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("temperature is normal 15<temp<22");
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
    gsm.println("Switch On Cooler temp >22");
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
    gsm.println("Switch On heater temp <15");
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
    gsm.println("sufficient moisture in soil");
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
    gsm.println("soil is dry");
    delay(1000);
    Serial.println("switch pump on");
    gsm.println((char)26);
    delay(2000);
}
void ldron(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("no light in mushroom zone");
    delay(1000);
    Serial.println("light is off");
    gsm.println((char)26);
    delay(2000);
}
void ldroff(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("light bulb is on");
    delay(1000);
    Serial.println("light on");
    gsm.println((char)26);
    delay(2000);
}
//********************compareing char coming from gsm to put char on rpi.serial*****************//
char compchar(char a,char b){
  if(a==b){
    return 0;
  }
  else
    return -1;
}
//****************compareing char coming from gsm to put char on rpi.serial****************//
char comp(char c,char d){
  if(c==d){
    return 0;
  }
  else
    return -1;
}
