/*PROGRAM SOURCE - https://www.github.com/gs369/mushroompi
/*LIBRARY*/
#include<stdio.h>
#include<wiringPi.h>
#include<string.h>
#include<wiringSerial.h>
/*DECLARAION OF RASPBERRYPI PINS*/
#define RELAY 9 	//SOIL
#define RELAY2 11 	//TEMPERATURE
#define RELAY3 0 	//LDR
#define RELAY4 5 	//ION
#define RELAY5 6 	//HEATER
/////////////////////////////////
#define SOIL_S 17
#define TEMP_S 22
#define LIGHT_S 27
#define ION_S 10
/*FOR LINES*/
char *dash(char *buf, int len ) {
   memset(buf, '-', len);
   buf[len] = 0;
   return buf;
}
/*DISPLAY - PROJECT,NAME,STUDENT_ID,INSTRUCTORS*/
int main ()
{
	char buf[60];
	printf("%s\n", dash(buf,60));
	printf("PROJECT     : EMBEDDING EMBEDDED IN MUSHROOM GROWING\n");
	printf("%s\n", dash(buf,60));
	printf("BY          : NAME\t\t\tSTUDENT ID\n");
	printf("              GURLAL SINGH\t\tC0717098\n");
	printf("              SANDEEP KAUR\t\tC0722494\n");
	printf("              HARPREET SINGH\t\tC0719364\n");
	printf("              HARJAP KAUR\t\tC0721913\n");
	printf("%s\n", dash(buf,60));
	printf("INSTRUCTORS : PROF. TAKIS ZOURNTOS\n\t      PROF. MIKE ALESHAMS\n");
	printf("%s\n", dash(buf,60));
	/*SERIAL DISPLAY*/
	int fd, a; //PREDEFINED INT
	char *b = "t" , *d = "m" , *e = "l" , *f = "i" , *g = "p" , *txt1 = "z" , *txt2 = "y" , *txt3 = "x" , *txt4 = "w" , *txt5 = "v";
	fd = serialOpen("/dev/ttyAMA0",115200); //UART at 115200, ttyAMA0.
	/*GPIO*/
	wiringPiSetupGpio();
	/*WiringPi GPIO*/
	/*DECLARATION OF OUTPUTS*/
	pinMode(RELAY,OUTPUT); //SOIL MOISTURE
	pinMode(RELAY2,OUTPUT);//TEMPERATURE
	pinMode(RELAY3,OUTPUT);//LIGHT
	pinMode(RELAY4,OUTPUT);//ION
	pinMode(RELAY5,OUTPUT);//HEATER
	/*DECLARATION OF INPUTS*/
	pinMode(SOIL_S,INPUT);
	pinMode(TEMP_S,INPUT);
	pinMode(LIGHT_S,INPUT);
	pinMode(ION_S,INPUT);
	/*SENSOR FUNCTIONS*/
	for(;;){
	a = serialGetchar(fd);
	//printf("%c",a);
	if((a) == *b){
	digitalWrite(RELAY,HIGH);
	delay(100);
	printf("RELAY1\n");
	fflush(stdout);
	serialPutchar(fd, *txt1);
	}
	else{
	digitalWrite(RELAY,LOW);
	}
	if((a) == *d){
	digitalWrite(RELAY2,HIGH);
	delay(100);
	printf("RELAY2\n");
	fflush(stdout);
	serialPutchar(fd, *txt2);
	}
	else{
	digitalWrite(RELAY2,LOW);
	}
	if((a) == *e){
	digitalWrite(RELAY3,HIGH);
	printf("RELAY3\n");
	fflush(stdout);
	serialPutchar(fd, *txt3);
	}
	else{
	digitalWrite(RELAY3,LOW);
	}
	if((a) == *f){
	digitalWrite(RELAY4,HIGH);
	printf("RELAY4\n");
	fflush(stdout);
	serialPutchar(fd, *txt4);
	}
	else{
	digitalWrite(RELAY4,LOW);
	}
	if((a) == *g){
	digitalWrite(RELAY5,HIGH);
	printf("RELAY5\n");
	fflush(stdout);
	serialPutchar(fd, *txt5);
	}
	else{
	digitalWrite(RELAY5,LOW);
	}
}
}

