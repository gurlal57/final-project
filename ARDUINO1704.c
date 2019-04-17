#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//*************************************declarations for serial com.******************************//

//SoftwareSerial rpi(12,3);
SoftwareSerial gsm(13,2);
char zone;
char pi;

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
  
  Serial.begin(9600);
  gsm.begin(9600);
 Serial3.begin(9600);
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
  delay(2000);
  // Set module to send SMS data to serial out upon receipt 
  gsm.print("AT + CNMI=1,2,0,0,0\r");
  delay(1000);
}

void loop() {
////***********************************reciving message from gsm and sending to pi************************************//////
        if(gsm.available()) 
         { 
         zone = gsm.read();
         delay(10);
          Serial.print(zone);
    //******************sending char recived from gsm to rpi********************//
    //in below loops we are just asking sensor status for light,moisture and temp//
    
    if(zone == 'e')
    {
      //delay(1000);
      Serial3.write("e");
    }

    if(zone == 'f')
    {
      //delay(1000);
      Serial3.write("f");
    }
    if(zone == 'a')
    {
      //delay(1000);
      Serial3.write("a");
    }
      
 //********************************in the below loop the value of analog sensoris being read*******************************// 
         if(zone == 'g')
    {
      //delay(1000);
      Serial3.write("g");
    }

    //********************************message from gsm to drive outputs*******************************//
    if(zone == 't')
    {
      //delay(1000);
      Serial3.write("t");
    }
    if(zone == 'm')
    {
      //delay(1000);
      Serial3.write("m");
    }
    if(zone == 'l')
    {
      //delay(1000);
      Serial3.write("l");
    }
    if(zone == 'i')
    {
      //delay(1000);
      Serial3.write("i");
    }
    if(zone == 'h')
    {
      //delay(1000);
      Serial3.write("h");
    }
    //***********************gsm message to drive outputs in off state*********************//
    if(zone == 'D')
    {
      //delay(1000);
      Serial3.write("D");
    }
    if(zone == 'W')
    {
      //delay(1000);
      Serial3.write("W");
    }
    if(zone == 'L')
    {
      //delay(1000);
      Serial3.write("L");
    }
    if(zone == 'E')
    {
      //delay(1000);
      Serial3.write("I");
    }
    if(zone == 'H')
    {
      //delay(1000);
      Serial3.write("H");
    }
      
      }
////***********************************receiving serial data from rpi and calling sta functions******************///
  if (Serial3.available()>0)
    {pi = Serial3.read();
      Serial.write(pi);
    delay(1000);
    //*************************************below if statements send messages to user's phone*****************************//
        
        if(pi == 'k')
{
    tempnor();
    }
    
if(pi == 'u')
{
    tempup();
    }
    if(pi == 's')
{
    tempdn();
    }
    if(pi == 'r')
{
    moistup();
    }
    if(pi == 'q')
{
    moistdn();
    }
    if(pi == 'p')
{
    ldron();
    }
    if(pi == 'o')
{
    ldroff();
    }
    //********************************sending message to GSM/USER's phone for output's "on" status***************************//
    
    if(pi == 'z'){
    text1();
    }
  if(pi == 'y'){
    text2();
    }
  if(pi == 'x'){
    text3();
    }
  if(pi == 'w'){
    text4();
    }
  if(pi == 'v'){
    text5();
    }

    
    
    //*****************************sending messages for "off" status of outputs *********************************///
     if(pi == 'Z'){
    text6();
    }
  if(pi == 'Y'){
    text7();
    }
  if(pi == 'X'){
    text8();
    }
  if(pi == 'W'){
    text9();
    }
  if(pi == 'V'){
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
    gsm.write("w PUMP run");
    delay(5000);
    //Serial.println("RELAY1(motor) is on");
    gsm.println((char)26);
    delay(2000);
}

void text2(){
   gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("FAN runS");
    delay(1000);
    //Serial.println("RELAY2(fan) is on");
    gsm.println((char)26);
    delay(2000);
}

void text3(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("R3(BUB) ON");
    delay(1000);
    //Serial.println("RELAY3(bulb) is on");
    gsm.println((char)26);
    delay(2000);
}

void text4(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("N PUMP RUNS");
    delay(1000);
    //Serial.println("RELAY4(N pump) is on");
    gsm.println((char)26);
    delay(2000);
}

void text5(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("wARMAR ON");
    delay(1000);
    //Serial.println("RELAY5(heater)is on");
    gsm.println((char)26);
    delay(2000);
}
//************** output_relay_functions FOR switch off status of relay output******* */
void text6(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("w PUMP OFF");
    delay(1000);
   // Serial.println("RELAY1(water pu Off");
    gsm.println((char)26);
    delay(2000);
}

void text7(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("FAN OFF");
    delay(1000);
    //Serial.println("RELAY2(fan) is off");
    gsm.println((char)26);
    delay(2000);
}

void text8(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("R3 BUB OFF");
    delay(1000);
   // Serial.println("RELAY3(bulb) is off");
    gsm.println((char)26);
    delay(2000);
}

void text9(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("N PUMP OFF");
    delay(1000);
   // Serial.println("RELAY4(N pump) is off");
    gsm.println((char)26);
    delay(2000);
}

void text10(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("wARMAR OFF");
    delay(1000);
    //Serial.println("RELAY5(heater)is off");
    gsm.println((char)26);
    delay(2000);
}

//***********sensors status functions*********************/
void tempnor(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("TMP B/w 15<TMP<22");
    delay(1000);
    //Serial.println("every thing good");
    gsm.println((char)26);
    delay(2000);
}
void tempup(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("TURN FAN ON TMP >22");
    delay(1000);
   // Serial.println("COOLER");
    gsm.println((char)26);
    delay(2000);
}
void tempdn(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("TURN wARMAR ON TMP<15");
    delay(1000);
    //Serial.println("COOLER");
    gsm.println((char)26);
    delay(2000);
}
void moistup(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("MOSTUR GOOd");
    delay(1000);
    //Serial.println("pump off");
    gsm.println((char)26);
    delay(2000);
}
void moistdn(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("DRY CONd.");
    delay(1000);
   // Serial.println("switch pump on");
    gsm.println((char)26);
    delay(2000);
}
void ldron(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("BUB OFF");
    delay(1000);
    //Serial.println("light is off");
    gsm.println((char)26);
    delay(2000);
}
void ldroff(){
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+16479175767\"\r");
    delay(1000);
    gsm.println("BUB ON");
    delay(1000);
    //Serial.println("light on");
    gsm.println((char)26);
    delay(2000);
}
