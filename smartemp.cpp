#include "smartemp.h"

#include <Arduino.h>
#include <Thread.h>
#include <ThreadController.h>
#include <DHT.h>
#include "cli.h"
#include "smartemp.h"


float temp_min=16;
float temp_max=25;
float temp_current=0;
unsigned int period=1000;

float hum_min;
float hum_max;
float hum_current;

ThreadController controll = new ThreadController();
Thread *thread_Cli = new Thread();
Thread *thread_CheckTemp = new Thread();
DHT dht(DHTPIN, DHTTYPE);

void checktemp(){

	  float h = 60; //dht.readHumidity();
	  float t = 20; //dht.readTemperature();

	  // testa se retorno é valido, caso contrário algo está errado.
	  if (isnan(t) || isnan(h))
	  {
	    Serial.println("Failed to read from DHT");
	  }
	  else{

		temp_current = t;
		hum_current = h;

		if(temp_current<temp_min)
				Serial.println("ajustar");
   	    if(temp_current>temp_max)
				Serial.println("ligar ar");

	  }

	  delay(period);
}

void setup(){
	Serial.begin(9600);
	dht.begin();

	pinMode(13,OUTPUT);

	thread_Cli->onRun(checktemp);
	thread_Cli->setInterval(0);

	thread_CheckTemp->onRun(cli_init);
	thread_CheckTemp->setInterval(0);

	controll.add(thread_Cli);
	controll.add(thread_CheckTemp);
}

void loop() {

	controll.run();

}
