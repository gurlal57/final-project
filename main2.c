/*PROGRAM SOURCE - https://www.github.com/gs369/mushroompi
/*LIBRARY*/
#include<stdio.h>
#include <dirent.h>
#include<wiringPi.h>
#include <fcntl.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
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
	float x,tempr;
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
	char *b = "t" , *d = "m" , *e = "l" , *f = "i" , *g = "p" ;
	char *txt1 = "z" , *txt2 = "y" , *txt3 = "x" , *txt4 = "w" , *txt5 = "v" ;
	char *stat1 = "u" , *stat2 = "s" , *stat3 = "r" , *stat4 = "q" , *stat5 = "n" , *stat6 = "o" ,*stat7 = "k" ;
	char *gsm1 = "e" , *gsm2 = "f" , *gsm3 = "g" ;
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
	//sensor reading*/
		//moisture reading*/
          if((a) == *gsm1){
	digitalread(SOIL_S,HIGH);
	delay(100);
	printf("no moisture");
	fflush(stdout);
	serialPutchar(fd, *stat4);
	}
	else{
	delay(100);
	printf("sufficient moisture");
	fflush(stdout);
	serialPutchar(fd, *stat3);
	}
		//light readings*/
		if((a) == *gsm2){
	digitalread(LIGHT_S,HIGH);
	delay(100);
	printf("switch off light");
	fflush(stdout);
	serialPutchar(fd, *stat5);
	}
	else{
	delay(100);
	printf("switch on light");
	fflush(stdout);
	serialPutchar(fd, *stat6);
	}
		//temp reading*/
		 if((a) == *gsm3){
	//call temp reading fun here*/
			 tempr = tempread(x);
                   if (tempr > 22){ 
	delay(100);
	printf("high temp");
	fflush(stdout);
	serialPutchar(fd, *stat1);
	}
	else if(tempr < 15){
	delay(100);
	printf("low temp");
	fflush(stdout);
	serialPutchar(fd, *stat2);
	}
	else if(15 < tempr < 22){
	delay(100);
	printf("normal temp");
	fflush(stdout);
	serialPutchar(fd, *stat7);		 
		 }
		//driving output relays*/
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
//function to read temperature from ds18b20*/

float tempread(float x)
{ float tempc;
DIR *dir;
 struct dirent *dirent;
 char dev[16];      // Dev ID
 char devPath[128]; // Path to device
 char buf[256];     // Data from device
 char tmpData[6];   // Temp C * 1000 reported by device 
 char path[] = "/sys/bus/w1/devices"; 
 ssize_t numRead;
 
 dir = opendir (path);
 if (dir != NULL)
 {
  while ((dirent = readdir (dir)))
   // 1-wire devices are links beginning with 28-
   if (dirent->d_type == DT_LNK && 
     strstr(dirent->d_name, "28-") != NULL) { 
    strcpy(dev, dirent->d_name);
    printf("\nDevice: %s\n", dev);
   }
        (void) closedir (dir);
        }
 else
 {
  perror ("Couldn't open the w1 devices directory");
  return 1;
 }

        // Assemble path to OneWire device
 sprintf(devPath, "%s/%s/w1_slave", path, dev);
 int fd = open(devPath, O_RDONLY);
  if(fd == -1)
  {
   perror ("Couldn't open the w1 device.");
   return 1;   
  }
  while((numRead = read(fd, buf, 256)) > 0) 
  {
   strncpy(tmpData, strstr(buf, "t=") + 2, 5); 
   float tempC = strtof(tmpData, NULL);
   
  }
  close(fd);
  return(tempc/1000);
  }
	
