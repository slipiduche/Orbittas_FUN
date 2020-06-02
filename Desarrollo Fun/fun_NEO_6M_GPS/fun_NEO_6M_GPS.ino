/**
   Nombre Función:         fun_NEO_6M_GPS
   Descripcion de Función:

   Configuracion y manejo de funciones de adquisicion de datos del GPS UBLOX NEO GPSNEO6VM2
  

   Author (s)
   Proyectos Orbittas:          Alejandro Camacaro

   version                      1.1
   date                          20/04/2020


   Nombre del archivo :         fun_NEO_6M_GPS.ino
   
   Dispositivos utilizables: GPSNEO6VM2
   
   Dependencias:Requiere de estas librerias para trabajar :
        TinyGPS++.h,        ---> Por Mikal Hart Version 1.0   <--  https://github.com/mikalhart/TinyGPSPlus
        */
#include <TinyGPS++.h>
TinyGPSPlus gps;                       //utilizado para acceder a funciones del GPS

#define COMPILE //utilizado para compilar comenta si va a utilizar la función en otro sketch
#define USEGPS //comentar si no necesita usar GPS
#ifdef USEGPS

///DEBUG PORT
#ifndef DEBUG
#define DEBUG
#endif
#ifdef  DEBUG
#define DEBUG_PORT         Serial      //al usar la clase Serial se esta usando el puerto uart 0 ubicado por defecto en GPIO1/RX y GPIO3/TX del esp32

#define DEBUG_PRINT(x)     Serial.print (x)
#define DEBUG_WRITE(x)     Serial.write (x)
#define DEBUG_PRINTx(x,y)  Serial.print (x,y)
#define DEBUG_PRINTDEC(x)  Serial.print (x, DEC)
#define DEBUG_PRINTLN(x)   Serial.println (x)
#else
#define DEBUG_PORT
#define DEBUG_WRITE(x) 
#define DEBUG_PRINT(x)
#define DEBUG_PRINTx(x,y) 
#define DEBUG_PRINTDEC(x)
#define DEBUG_PRINTLN(x)
#endif
////variables 
String lt, ln, dt, dtsd, tm;                    //latitud, longitud, fecha, hora que se optiene del gps
/****Variable  BREAK gps fail****/

unsigned int count = 0;                            //utilizado para el control de intentos de recepcion de data gps
/***********************************************************************/ 
//Nombre de la funcion :ini_gps()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada de inicializar variables de latitud,longitud,fecha, y hora
/***********************************************************************/

void ini_gps()
{
lt=String(gps.location.lat(),6); 
ln=String(gps.location.lng(),6);
dt=String(gps.date.value());
tm=String(gps.time.value());  
}
/***********************************************************************/ 
//Nombre de la funcion :get_gpsdata()
//Entrada: String target
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada de decodificar la trama NMEA recibida por el puerto serial del GPS
/***********************************************************************/
void get_gpsdata()

{
char c;
count=0;

while (Serial.available() > 0)
  {
   c=Serial.read();
   DEBUG_PRINT(c);
   gps.encode(c);
    
  if (gps.location.isUpdated())
    {
     ini_gps();
     DEBUG_PRINT("Latitude= ");         // Latitud en grados (doble)
     DEBUG_PRINTx(gps.location.lat(), 6); 
          
     DEBUG_PRINT(" Longitude= ");      //Longitud en grados (doble)
     DEBUG_PRINTx(gps.location.lng(), 6 ); 
      DEBUG_PRINTLN();
       
     
     DEBUG_PRINT("Raw latitude = "); // Latitud bruta en grados enteros
     DEBUG_PRINT(gps.location.rawLat().negative ? "-" : "+");
     DEBUG_PRINTLN(gps.location.rawLat().deg); 
 
     DEBUG_PRINTLN(gps.location.rawLat().billionths);// ... y billonésimas (u16 / u32)
       
     DEBUG_PRINT("Raw longitude = ");   // Longitud cruda en grados enteros
     DEBUG_PRINT(gps.location.rawLng().negative ? "-" : "+");
     DEBUG_PRINTLN(gps.location.rawLng().deg);      
     DEBUG_PRINTLN(gps.location.rawLng().billionths);// ... y billonésimas (u16 / u32)
   
     DEBUG_PRINT("Raw date DDMMYY = ");// Fecha sin formato en formato DDMMYY (u32)
     DEBUG_PRINTLN(gps.date.value()); 

     DEBUG_PRINT("Year = ");         // Año (2000+) (u16)
     DEBUG_PRINTLN(gps.date.year()); 
     
     DEBUG_PRINT("Month = ");       // Mes (1-12) (u8)
     DEBUG_PRINTLN(gps.date.month()); 
      
     DEBUG_PRINT("Day = ");        // Día (1-31) (u8)
     DEBUG_PRINTLN(gps.date.day()); 

     DEBUG_PRINT("Raw time in HHMMSSCC = "); // Tiempo sin formato en formato HHMMSSCC (u32)
     DEBUG_PRINTLN(gps.time.value()); 

     DEBUG_PRINT("Hour = ");                // Hora (0-23) (u8)
     DEBUG_PRINTLN(gps.time.hour()); 

     DEBUG_PRINT("Minute = ");             // Minuto (0-59) (u8)
     DEBUG_PRINTLN(gps.time.minute()); 

     DEBUG_PRINT("Second = ");              // Segundo (0-59) (u8)
     DEBUG_PRINTLN(gps.time.second()); 
      
     DEBUG_PRINT("Centisecond = ");          // centésimas de segundo (0-99) (u8)
     DEBUG_PRINTLN(gps.time.centisecond()); 

     DEBUG_PRINT("Raw speed in 100ths/knot = ");// Velocidad bruta en centésimas de nudo (i32)
     DEBUG_PRINTLN(gps.speed.value()); 

     DEBUG_PRINT("Speed in knots/h = ");     // Velocidad en nudos (doble)
     DEBUG_PRINTLN(gps.speed.knots()); 
 
     DEBUG_PRINT("Speed in miles/h = ");     // Velocidad en millas por hora (doble)
     DEBUG_PRINTLN(gps.speed.mph()); 

     DEBUG_PRINT("Speed in m/s = ");         // Velocidad en metros por segundo (doble)
     DEBUG_PRINTLN(gps.speed.mps()); 

     DEBUG_PRINT("Speed in km/h = ");         // Velocidad en kilómetros por hora (doble)
     DEBUG_PRINTLN(gps.speed.kmph()); 

     DEBUG_PRINT("Raw course in degrees = ");  // Curso crudo en centésimas de grado (i32)
     DEBUG_PRINTLN(gps.course.value()); 

     DEBUG_PRINT("Course in degrees = ");      // Curso en grados (doble)
     DEBUG_PRINTLN(gps.course.deg()); 

     DEBUG_PRINT("Raw altitude in centimeters = "); // Altitud cruda en centímetros (i32)
     DEBUG_PRINTLN(gps.altitude.value()); 

     DEBUG_PRINT("Altitude in meters = ");        // Altitud en metros (doble)
     DEBUG_PRINTLN(gps.altitude.meters()); 

     DEBUG_PRINT("Altitude in miles = ");        // Altitud en millas (doble)
     DEBUG_PRINTLN(gps.altitude.miles()); 

     DEBUG_PRINT("Altitude in kilometers = ");    // Altitud en kilómetros (doble)
     DEBUG_PRINTLN(gps.altitude.kilometers()); 

     DEBUG_PRINT("Altitude in feet = ");         // Altitud en pies (doble)
     DEBUG_PRINTLN(gps.altitude.feet()); 


     DEBUG_PRINT("Number os satellites in use = "); // Número de satélites en uso (u32)
     DEBUG_PRINTLN(gps.satellites.value()); 

     DEBUG_PRINT("HDOP = ");                       // Horizontal dim. de Precisión (100ths-i32)
     DEBUG_PRINTLN(gps.hdop.value()); 
     count=0;
     break;      
    }
  else
  {
    count++;                                      //Si no hay datos GPS sale del bucle de recepcion serial
    if (count>2500)
      {
      DEBUG_PRINTLN("NO GPS DATA"); 
      ini_gps();
      count=0;
      break;     
      }
    
  }
  
  }
ini_gps();
}
#ifdef  COMPILE
void setup()
{}
void loop()
{}
#endif 
#else
void get_gpsdata(){}
void ini_gps(){}

#endif
