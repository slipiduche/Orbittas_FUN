/**
   Nombre Función:         fun_ds18b20
   Descripcion de Función:

   Configuracion y manejo de funciones de adquisicion de datos del ds18b20 one wire temperature sensor
  

   Author (s)
   Proyectos Orbittas:          Alejandro Camacaro

   version                      1.1
   date                          20/04/2020


   Nombre del archivo :         fun_ds18b20.ino
   
   Dispositivos utilizables: ds18b20 one wire temperature sensor

   Dependencias:Requiere de estas librerias para trabajar :  **/ 
       
#define USEds18b20 //comentar si no necesita usar ds18b20
#define COMPILE //utilizado para compilar comenta si va a utilizar la función en otro sketch
#include <DallasTemperature.h> //---> Por Miles Burton Version 3.8.0 <--Instalada desde incluir librerias Info https://github.com/milesburton/Arduino-Temperature-Control-Library
#include <OneWire.h>  //---> Por Paul Stoffregen version 2.3.4 <--Instalada desde incluir librerias https://github.com/PaulStoffregen/OneWire
/* agregar en main*/
       




/****REGISTRO SENSOR DS18B20****/
#ifdef USEds18b20
#define PIN_DATOS_DS18B20 32  //cambiar el numero del pin al que se requiera segun el microcontrolador por default esp32 gpio32
OneWire oneWireObjeto(PIN_DATOS_DS18B20 );//Instancia a las clases OneWire 
DallasTemperature sensorDS18B20(&oneWireObjeto);//Instancia a las clases  DallasTemperature
#endif
/***Puerto de Monitoreo***/
#ifdef  DEBUG
#define DEBUG_PORT         Serial      //al usar la clase Serial se esta usando el puerto uart 0 ubicado por defecto en GPIO1/RX y GPIO3/TX del esp32

#define DEBUG_PRINT(x)     Serial.print (x)
#define DEBUG_WRITE(x)     Serial.write (x)
#define DEBUG_PRINTx(x,y)  Serial.print (x,y)
#define DEBUG_PRINTDEC(x)  Serial.print (x, DEC)
#define DEBUG_PRINTLN(x)   Serial.println (x)
#define DEBUG_PRINTLNx(x,y)   Serial.println (x,y)
#else
#define DEBUG_PORT
#define DEBUG_WRITE(x)
#define DEBUG_PRINT(x)
#define DEBUG_PRINTx(x,y)
#define DEBUG_PRINTDEC(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTLNx(x,y)
#endif
#ifdef USEds18b20

/****Variables DS18B20****/
unsigned long             tiempo_adquisicion_Temp         = 0; //tiempo de adquisicion de temperatura con sensor ds18b20
float TempActual = 0;

/**************************************************************************
Nombre de la Funcion :Inicializacion_DS18B20()
Entrada:Ninguna
Salida:Ninguna
Descripcion:Esta funcion se encarga de inicializar el bus 1-Wire
*************************************************************************/
void Inicializacion_DS18B20()
{
  sensorDS18B20.begin();
  //pinMode(PIN_DATOS_DS18B20, INPUT_PULLUP);  
}
/************************************************************************ 
Nombre de la Funcion :Adquirir_Temperatura_DS18B20()
Entrada:Ninguna
Salida:float Temp_Actual
Descripcion: Esta funcion  adquiere la temperatura actual del sensor
DS18B20 via ONEWIRE y es usada para actualizar la temperatura.

/***********************************************************************/
float Adquirir_Temperatura_DS18B20()
{
   delay(10);
   //DEBUG_PRINTLN("Adquirir_Temperatura_DS18B20("+String(millis())+")");
   float Temp_Actual =0;                         // Se limpia temperatura actual 
   sensorDS18B20.requestTemperatures();          // Se prepara adquisicion de temperatura
   delay(10);
   Temp_Actual=sensorDS18B20.getTempCByIndex(0); // se actualiza temperatura actual con el sensor
   //DEBUG_PRINTLN("FIN Adquirir_Temperatura_DS18B20("+String(millis())+")");
   return Temp_Actual;
}
/************************************************************************ 
Nombre de la Funcion :Adquirir_Temperatura_DS18B201()
Entrada:Ninguna
Salida:float Temp_Actual1
Descripcion: Esta funcion  adquiere la temperatura actual del sensor
DS18B20 via ONEWIRE y es usada para actualizar la temperatura.
***********************************************************************/
float Adquirir_Temperatura_DS18B201()
{
    //DEBUG_PRINTLN("Adquirir_Temperatura_DS18B20("+String(millis())+")");
    float Temp_Actual1 =0;                       // Se limpia temperatura actual
    sensorDS18B20.requestTemperatures();         // Se prepara adquisicion de temperatura
    Temp_Actual1=sensorDS18B20.getTempCByIndex(0);// se actualiza temperatura actual con el sensor
    //DEBUG_PRINTLN("FIN Adquirir_Temperatura_DS18B20("+String(millis())+")");
    return Temp_Actual1;
}
#ifdef  COMPILE
void setup()
{}
void loop()
{}
#endif 
#else
void Inicializacion_DS18B20(){}
float Adquirir_Temperatura_DS18B20(){}
float Adquirir_Temperatura_DS18B201(){}
#endif
