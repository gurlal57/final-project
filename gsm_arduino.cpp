#include<OneWire.h>
#include<DallasTemperature.h>

float CEc=1.38;
int R1= 1000;
int Ra=25; //Resistance of powering Pins
int ECPin= A0;
int ECGround=A1;
int ECPower =A4;

float TemperatureCoef = 0.019; //compensation for temp
//temp related//
#define ONE_WIRE_BUS 10          // Data wire plugged into pin 10 
const int TempProbePossitive =8;  //connected to pin 9
const int TempProbeNegative=9;    //connected to pin 8
OneWire oneWire(ONE_WIRE_BUS);// Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature.
float TempFinish=0;
float TempStart=0;
float EC=0;
int ppm =0;
 
 
float raw= 0;
float Vin= 5;
float Vdrop= 0;
float Rc= 0;
float K=0;
 int i=0;
float buffer=0;
*******************************
 #include <SoftwareSerial.h>
SoftwareSerial gsmSerial(2,3); //tx,rx

void setup() {
    gsmSerial.begin(9600);
    Serial.begin(9600);
	
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
	//** Adding Digital Pin Resistance to [25 ohm] to the static Resistor *********//
	// Consule Read-Me for Why, or just accept it as true
	R1=(R1+Ra);
	
	*************************
    delay(100);
    gsmSerial.println("AT+CMGF=1");
    delay(100);
    gsmSerial.println("AT+CMGS=\"+16479175767\"\r");
    delay(100);
    gsmSerial.println("hello");
    delay(1000);
    gsmSerial.println((char)26);
    Serial.println("Message Sent!");
    delay(2000);
    
}

void loop() {

i=1;
  buffer=0;
sensors.requestTemperatures();// Send the command to get temperatures
TempStart=sensors.getTempCByIndex(0); //Stores Value in Variable
//estimate resistance of liquid//
while(i<=10){
 
digitalWrite(ECPower,HIGH);
raw= analogRead(ECPin);
raw= analogRead(ECPin);// This is not a mistake, First reading will be low
digitalWrite(ECPower,LOW);
buffer=buffer+raw;
i++;
delay(500);
};
raw=(buffer/10);
sensors.requestTemperatures();// Send the command to get temperatures
TempFinish=sensors.getTempCByIndex(0); //Stores Value in Variable
//compensation for temp//
EC =CEc*(1+(TemperatureCoef*(TempFinish-25.0))) ;
 
//***************** Calculates R relating to Calibration fluid **************************//
Vdrop= (((Vin)*(raw))/1024.0);
Rc=(Vdrop*R1)/(Vin-Vdrop);
Rc=Rc-Ra;
K= 1000/(Rc*EC);
 
Serial.print("Calibration Fluid EC: ");
Serial.print(EC);
Serial.print(" S  ");  //add units here
Serial.print("Cell Constant K");
Serial.print(K);
    
}
