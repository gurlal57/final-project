///***************LIBRARIES*****************///
#include<stdio.h>
#include <dirent.h>
#include<wiringPi.h>
#include <fcntl.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include<wiringSerial.h>
//************function to read the temperature from ds18b20*************/

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
   //************1-wire devices are links beginning with 28-**********//
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
/*************DECLARAION OF RASPBERRYPI PINS***************/
#define RELAY 9         //WATER PUMP
#define RELAY2 11       //COOLING FAN
#define RELAY3 0        //LIGHT BULB
#define RELAY4 5        //NITROGEN PUMP
#define RELAY5 6        //HEATER

//********DEFINING INPUT PINS**********///
#define SOIL_S 17
#define TEMP_S 22
#define LIGHT_S 27

/*************LINES GROUP AND PROJECT INFO************/
char *dash(char *buf, int len ) {
   memset(buf, '-', len);
   buf[len] = 0;
   return buf;
}
/***************MAIN BEGINS*************/
int main ()
{        
/***************DISPLAY - PROJECT,NAME,STUDENT_ID,INSTRUCTORS*************/
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
        printf("INSTRUCTORS : PROF. TAKIS ZOURNTOS\n\t      PROF. MIKE ALESHAMS$");
        printf("%s\n", dash(buf,60));
        /*******************SERIAL DISPLAY****************/
        int fd, a; //PREDEFINED INT
        char *b = "m" , *d = "t" , *e = "l" , *f = "i" , *g = "h" ;
        char *B = "M" , *D = "T" , *E = "L" , *F = "I" , *G = "H" ;
	char *txt6 = "Z" , *txt7 = "Y" , *txt8 = "X" , *txt9 = "W" , *txt10 = "V" ;
        char *txt1 = "z" , *txt2 = "y" , *txt3 = "x" , *txt4 = "w" , *txt5 = "v";
        char *stat1 = "u" , *stat2 = "s" , *stat3 = "r" , *stat4 = "q" , *stat5 ="p" , *stat7 ="k" , *stat6 ="o";
        char *gsm1 = "e" , *gsm2 = "f" , *gsm3 = "a" ;
		
		//***************OPENING PI SERIAL*************//
                fd = serialOpen("/dev/ttyAMA0",9600); //UART at 115200, ttyAMA0.
				
        /************SETTING UP GPIO**************/
 wiringPiSetupGpio();
 
        /**************WiringPi GPIO************/
        /*******************SETTING UP PIN MODES****************/
        pinMode(RELAY,OUTPUT); //WATER PUMP
        pinMode(RELAY2,OUTPUT);//COOLING FAN
        pinMode(RELAY3,OUTPUT);//LIGHT BULB
        pinMode(RELAY4,OUTPUT);//NITROGEN PUMP
        pinMode(RELAY5,OUTPUT);//HEATER
		
                /**************DECLARATION OF INPUTS PINMODES***********/
        pinMode(SOIL_S,INPUT);
        pinMode(TEMP_S,INPUT);
        pinMode(LIGHT_S,INPUT);
    
		
        /************INFINITE LOOP TO READ/WRITE SERIAL AND I/Os**********/
        for(;;){
        a = serialGetchar(fd);
        
         //***************moisture reading*******************/
          if((a) == *gsm1)
          { if(digitalRead(SOIL_S) == 1)
                  { delay(100);
        printf("no moisture");
        fflush(stdout);
        serialPutchar(fd, *stat4);
        }
        else if(digitalRead(SOIL_S) == 0){
        delay(100);
               printf("sufficient moisture");
        fflush(stdout);
        serialPutchar(fd, *stat3);
        }}
           //**********************light readings********************/
                if((a) == *gsm2)
                { if(digitalRead(LIGHT_S) == 1)
                { delay(100);
        printf("no light");
        fflush(stdout);
                serialPutchar(fd, *stat5);
        }

 else if(digitalRead(LIGHT_S) == 0){
        delay(100);
        printf(" light is on");
        fflush(stdout);
        serialPutchar(fd, *stat6);
        }}
        //******************************temp reading*********************/
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
                 }}
				 
     //************************driving output relays*******************/
	 //*********WATER PUMP*****************////
        if((a) == *b)
	{
          digitalWrite(RELAY,HIGH);
        delay(100);
        printf("RELAY1 is on\n");
        fflush(stdout);
        serialPutchar(fd, *txt1);
        }
        else if((a) == *B)
	{
        digitalWrite(RELAY,LOW);
                delay(100);
        printf("RELAY1 is off\n");
        fflush(stdout);
        serialPutchar(fd, *txt6);
        }
		
		///************COOLING FAN**************///
        if((a) == *d){
        digitalWrite(RELAY2,HIGH);
        delay(100);
        printf("RELAY2 is on\n");
        fflush(stdout);
                serialPutchar(fd, *txt2);
        }
        else ((a) == *D)
	{
        digitalWrite(RELAY2,LOW);
         delay(100);
        printf("RELAY2 is off\n");
        fflush(stdout);
        serialPutchar(fd, *txt7);
        }
		
		///********************LIGHT BULB**********//
        if((a) == *e){
        digitalWrite(RELAY3,HIGH);
        printf("RELAY3 is on\n");
               fflush(stdout);
        serialPutchar(fd, *txt3);
        }
        else if((a) == *E){
        digitalWrite(RELAY3,LOW);
         delay(100);
        printf("RELAY3 is off\n");
        fflush(stdout);
        serialPutchar(fd, *txt8);
        }
		
	//*****************NITROGEN PUMP**************//
        if((a) == *f){
        digitalWrite(RELAY4,HIGH);
        printf("RELAY4 is on\n");
        fflush(stdout);
        serialPutchar(fd, *txt4);
        }
        else if ((a) == *F)
	{
        digitalWrite(RELAY4,LOW);
         delay(100);
        printf("RELAY4 is off\n");
          fflush(stdout);
        serialPutchar(fd, *txt9);
        }
		
		///************HEATER**********///
        if((a) == *g)
	{
        digitalWrite(RELAY5,HIGH);
        printf("RELAY5 is on\n");
        fflush(stdout);
        serialPutchar(fd, *txt5);
        }
        else if((a) == *G)
	{        digitalWrite(RELAY5,LOW);
             delay(100);
        printf("RELAY5 is off\n");
        fflush(stdout);
        serialPutchar(fd, *txt10);
        }
}
}
                       
