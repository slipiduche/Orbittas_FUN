/**
   Nombre Función:         fun_dht
   Descripcion de Función:

   Configuracion y manejo de funciones de adquisicion de datos del dht22 y dht11 (temperatura y humedad) 
  

   Author (s)
   Proyectos Orbittas:          Alejandro Camacaro

   version                      1.1
   date                          22/04/2020


   Nombre del archivo :        fun_dht.ino
   
   Dispositivos utilizables:sensor de humedad y temperatura DHT11 y DHT12
   
   Dependencias:Requiere de estas librerias para trabajar :
*/ 
#define COMPILE //utilizado para compilar comenta si va a utilizar la función en otro sketch
#define USEDHT  ////comentar si no necesita usar DHT
#ifdef USEDHT 
  #include <Adafruit_Sensor.h> // - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
/*#include <DHT.h>   //by adafruit version 1.3.8 // - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
  #include <DHT_U.h>*/
  #include "DHT.h" // in project folder


//BASED in Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain



#define DHTPIN 2     // what pin we're connected to

#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor for normal 16mhz Arduino
// **** NOTE: the Proteus virtual board runs at 8MHz so the threshols parameter should be passed is 4 ****
DHT dht(DHTPIN, DHTTYPE,4);
// NOTE: For working with a faster chip, like an Arduino Due or Teensy, you
// might need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// Example to initialize DHT sensor for Arduino Due:
//DHT dht(DHTPIN, DHTTYPE, 30);


//Variables
float temp = 0, moisture = 0; //temp and moisture global variables

/***********************************************************************/ 
//Nombre de la funcion :dht_setup() 
//Entrada:Ninguna
//Salida :Ninguna
//Descripcion:Configura sensor dht
/***********************************************************************/
void dht_setup() {
  Serial.begin(9600); 
  Serial.println("DHTxx test!");
 
  dht.begin();
}
/***********************************************************************/ 
//Nombre de la funcion :sdht_loop() 
//Entrada:Ninguna
//Salida :Ninguna
//Descripcion:Asigna lectura de humedad y temperatura a variables globales temp and moisture e impime al puerto serial para monitoreo
/***********************************************************************/
void dht_loop() {
  // Wait a few seconds between measurements.
  //delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  moisture=h;

  temp=t;
  // Compute heat index
  // Must send in temp in Fahrenheit!
  float hi = dht.computeHeatIndex(f, h);
 /* Serial.println();
  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hi);
  Serial.println(" *F");*/
}
#ifdef  COMPILE
void setup()
{}
void loop()
{}
#endif 
#else
void dht_setup(){}
void dht_loop(){}

#endif
