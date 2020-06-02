/**
   Nombre Función:         fun_pms
   Descripcion de Función:

   Configuracion y manejo de funciones de adquisicion de datos del pms1003
  

   Author (s)
   Proyectos Orbittas:          Alejandro Camacaro

   version                      1.1
   date                          21/04/2020


   Nombre del archivo :         fun_pms.ino
   
   Dispositivos utilizables: pms1003 sensor de particulas
   
   Dependencias:Requiere de estas librerias para trabajar :
*/      
   #include "PMS.h" //---> Version Original Por Mariusz Kacki version 1.1.0 <--Se modifico ligeramente para usar habilitacion del ESP32 en pin GPIO32 (usar la proporcionada en carpeta libraries)
#define USEPMS ////comentar si no necesita usar PMS1003
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
                       //Utilizado para aceder funciones de librería PMS
/**** REGISTRO SENSOR DE PMS ***/

#define RXD1 25                        //Se mapea el puerto de recepcion serial al pin GPIO25 del esp32 puesto que el uart 1 por defecto comparte pines con bus spi
#define TXD1 26                        //Se mapea el puerto de transmision serial al pin GPIO26 del esp32 puesto que el uart 1 por defecto comparte pines con bus spi
#ifdef USEPMS
#ifdef esp32
HardwareSerial pms1003(1);             //Se define el puerto hardware uart 1 del esp32 para uso de sensor PMS
#endif
#ifdef arduino
#define pms1003 Serial2
#endif
#define pmsen 32                       //Modificar tambien en PMS.h
PMS pms(pms1003);                      //Utilizado para aceder funciones de librería PMS
#endif
PMS::DATA data; 
#ifdef USEPMS
/****Variables****/

uint32_t PMS_READ_INTERVAL = 30000;                //intervalo en milisegundos de adquisicion de datos del PMS, Debe ser mayor a PMS_READ_DELAY 
uint32_t PMS_READ_DELAY = 20000;                   //intervalo en milisegundos Utilizado para esperar un tiempo y obtener mediciones estables del PMS
uint32_t timerInterval = PMS_READ_DELAY;           //utilizada para el temporizador del PMS

/***********************************************************************/ 
//Nombre de la funcion :pms_setup()
//Entrada:ninguna
//Salida :Ninguna
//Descripcion://Iniciaización de funciones de librería PMS
/***********************************************************************/
void pms_setup()
{
  
  #ifdef esp32
pms1003.begin(PMS::BAUD_RATE, SERIAL_8N1, RXD1, TXD1); 
#endif
#ifdef arduino
pms1003.begin(PMS::BAUD_RATE); 
#endif
  
  }
/***********************************************************************/ 
//Nombre de la funcion :timerCallback()
//Entrada: String datar
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada de gestionar el intervalo de medicion PMS
/***********************************************************************/ 

void timerCallback() 
  {
  if (timerInterval == PMS_READ_DELAY)
    {
    readData();                                             //ubicada en fun_pms.ino

    DEBUG_PRINTLN("Going to sleep.");
    pms.sleep();                                            //ubicada en libreria PMS.h
    
    }
  else
    {
    DEBUG_PRINTLN("Waking up.");
    pms.wakeUp();                                           //ubicada en librería PMS.h
    }
  }

/***********************************************************************/ 
//Nombre de la funcion :readData()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada de leer y decodificar la data de PMS1003
//funciones internas ubicadas en librería PMS.h y requieren de la creacion de los objetos
//pms y pms1003
/***********************************************************************/ 
void readData()
  { 
    #ifdef USEPMS

  while (pms1003.available()) 
    { pms1003.read(); }
  
     pms.requestRead();
  if (pms.readUntil(data))
    {
    DEBUG_PRINTLN();

    DEBUG_PRINT("PM 1.0 (ug/m3): ");
    DEBUG_PRINTLN(data.PM_AE_UG_1_0);

    DEBUG_PRINT("PM 2.5 (ug/m3): ");
    DEBUG_PRINTLN(data.PM_AE_UG_2_5);

    DEBUG_PRINT("PM 10.0 (ug/m3): ");
    DEBUG_PRINTLN(data.PM_AE_UG_10_0);

    DEBUG_PRINTLN();    
    }
  else
    {
    DEBUG_PRINTLN("No data.");
    }
  #endif
  }
#ifdef  COMPILE
void setup()
{}
void loop()
{}
#endif 
#else
void timerCallback(){}
void readData(){}
 
#endif
