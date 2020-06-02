/**
   Nombre Función:         fun_vl53l1x
   Descripcion de Función:

   Configuracion y manejo de funciones de adquisicion de datos del VL53L1X
  

   Author (s)
   Proyectos Orbittas:          Alejandro Camacaro

   version                      1.1
   date                          21/04/2020


   Nombre del archivo :         fun_vl53l1x.ino
   
   Dispositivos utilizables: VL53L1X telémetro 4 metros
   
   Dependencias:Requiere de estas librerias para trabajar :
*/      
  #include <Wire.h>     //Libreria que funciona por defecto al instalar Arduino core for ESP32 o Arduino ide
  #include <VL53L1X.h>  //---> Por www.pololu.com Version 1.0.1 <-- https://github.com/pololu/vl53l1x-arduino


#define USEvl53l1x ////comentar si no necesita usar telemetro VL53L1X
#define COMPILE //utilizado para compilar comenta si va a utilizar la función en otro sketch
#define arduino //comentar si se usa esp32
//#define esp32 //descomentar si se usa esp32
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
#ifdef USEvl53l1x
VL53L1X sensor;
/***********************************************************************/ 
//Nombre de la funcion :VL53L1X_setup() 
//Entrada:Ninguna
//Salida :Ninguna
//Descripcion:Configura sensor vl53l1x
/***********************************************************************/

void VL53L1X_setup()
{
 
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    DEBUG_PRINTLN("Failed to detect and initialize sensor!");
    //POWERRESET();
    return;
  }
  
  // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
  // You can change these settings to adjust the performance of the sensor, but
  // the minimum timing budget is 20 ms for short distance mode and 33 ms for
  // medium and long distance modes. See the VL53L1X datasheet for more
  // information on range and timing limits.
  //sensor.setDistanceMode(VL53L1X::Long);
  sensor.setDistanceMode(VL53L1X::Short);
  sensor.setMeasurementTimingBudget(140000);//minimum 50000

  // Start continuous readings at a rate of one measurement every 50 ms (the
  // inter-measurement period). This period should be at least as long as the
  // timing budget.
  sensor.startContinuous(140);//minimum 50
}
/***********************************************************************/ 
//Nombre de la funcion :VL53L1X_loop() 
//Entrada:Ninguna
//Salida :Ninguna
//Descripcion:Lee los datos del sensor vl53l1x
/***********************************************************************/
void VL53L1X_loop()
{
  sensor.read();
  /*
  DEBUG_PRINT("range: ");
  DEBUG_PRINT(sensor.ranging_data.range_mm);
  DEBUG_PRINT("\tstatus: ");
  DEBUG_PRINT(VL53L1X::rangeStatusToString(sensor.ranging_data.range_status));
  DEBUG_PRINT("\tpeak signal: ");
  DEBUG_PRINT(sensor.ranging_data.peak_signal_count_rate_MCPS);
  DEBUG_PRINT("\tambient: ");
  DEBUG_PRINT(sensor.ranging_data.ambient_count_rate_MCPS);
  
  DEBUG_PRINTLN();
  */
}
#ifdef  COMPILE
void setup()
{}
void loop()
{}
#endif 
#else
void VL53L1X_setup(){}
void VL53L1X_loop(){}
#endif
