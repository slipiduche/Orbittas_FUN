/* Nombre Función:         fun_Watchdog_deepsleep
   Descripcion de Función:

   Configuracion y manejo de funciones correspondientes al watchdog y deepsleep del 
   ESP32
  

   Author (s)
   Proyectos Orbittas:          Kevin Mijares
                                Alejandro Camacaro (edición)

   version                      1.1
   date                         20/04/2020


   Nombre del archivo :         fun_Watchdog_deepsleep.ino
   
   Dispositivos utilizables: ESP32
   
   Dependencias:NO REQUIERE
   Requisitos:
  -Tener instalado arduino ide-1.8.9
  -Tener instalado Arduino core for ESP32 WiFi chip ver instrucciones en el siguiente link https://github.com/espressif/arduino-esp32
        
*/ 
#define COMPILE //utilizado para compilar comenta si va a utilizar la función en otro sketch
#define WATCHDOG //comentar si no necesita usar WATCHDOG
#define DEEPSLEEP //comentar si no necesita usar Deep Sleep
#define gpioin1 1 //comment or change if you need a diferent pin for enable or disable deepsleep
#ifdef WATCHDOG
hw_timer_t *watchdogTimer = NULL; //llamamos al watchdog y usamos un puntero para asignarlo hw_timer
long looptime = 0; // funcion long que nos dara el tiempo de loop para el experimento
long timetosleep=60000;// tiempo en milisegundos para configurar el tiempo dormido
#endif
#ifdef DEEPSLEEP
#define uS_TO_S_FACTOR 1000000  // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP  10        // Time ESP32 will go to sleep (in seconds) 
RTC_DATA_ATTR int bootCount = 0;
#endif


/*
Simple Deep Sleep with Timer Wake Up
=====================================
ESP32 offers a deep sleep mode for effective power
saving as power is an important factor for IoT
applications. In this mode CPUs, most of the RAM,
and all the digital peripherals which are clocked
from APB_CLK are powered off. The only parts of
the chip which can still be powered on are:
RTC controller, RTC peripherals ,and RTC memories

This code displays the most basic deep sleep with
a timer to wake it up and how to store data in
RTC memory to use it over reboots

This code is under Public Domain License.

Author:
Pranav Cherukupalli <cherukupallip@gmail.com>
*/
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

#ifdef DEEPSLEEP
//////
/***********************************************************************/ 
//Nombre de la funcion :print_wakeup_reason()
//Entrada:Ninguna
//Salida :Ninguna
//Descripcion:Imprime la razon por lacual el esp32 sale de modo sleep
/***********************************************************************/
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case 1  : DEBUG_PRINTLN("Wakeup caused by external signal using RTC_IO"); break;
    case 2  : DEBUG_PRINTLN("Wakeup caused by external signal using RTC_CNTL"); break;
    case 3  : DEBUG_PRINTLN("Wakeup caused by touchpad"); break;
    case 4  : DEBUG_PRINTLN("Wakeup caused by timer"); break;
    case 5  : DEBUG_PRINTLN("Wakeup caused by ULP program"); break;
    default : DEBUG_PRINTLN("Wakeup was not caused by deep sleep"); break;
  }
}
/***********************************************************************/ 
//Nombre de la funcion :deepsleep_setup()
//Entrada:Ninguna
//Salida :Ninguna
//Descripcion:Configura modo sleep
/***********************************************************************/
void deepsleep_setup()
{
  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  /*
  First we configure the wake up source
  We set our ESP32 to wake up for an external trigger.
  There are two types for ESP32, ext0 and ext1 .
  ext0 uses RTC_IO to wakeup thus requires RTC peripherals
  to be on while ext1 uses RTC Controller so doesnt need
  peripherals to be powered on.
  Note that using internal pullups/pulldowns also requires
  RTC peripherals to be turned on.
  */
 ///esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,0); /*1 = High, 0 = Low */
 /*
  First we configure the wake up source
  We set our ESP32 to wake up every 60 seconds
  */
 esp_sleep_enable_timer_wakeup(timetosleep* 1000);
 DEBUG_PRINTLN("Setup ESP32 to sleep for every " + String(timetosleep/1000) +
  " Seconds");

  /*
  Next we decide what all peripherals to shut down/keep on
  By default, ESP32 will automatically power down the peripherals
  not needed by the wakeup source, but if you want to be a poweruser
  this is for you. Read in detail at the API docs
  http://esp-idf.readthedocs.io/en/latest/api-reference/system/deep_sleep.html
  Left the line commented as an example of how to configure peripherals.
  The line below turns off all RTC peripherals in deep sleep.
  */
  //esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  //DEBUG_PRINTLN("Configured all RTC Peripherals to be powered down in sleep");

  /*
  Now that we have setup a wake cause and if needed setup the
  peripherals state in deep sleep, we can now start going to
  deep sleep.
  In the case that no wake up sources were provided but deep
  sleep was started, it will sleep forever unless hardware
  reset occurs.
  */
  ////
}
#else
void print_wakeup_reason(){}
void deepsleep_setup(){}
#endif

#ifdef WATCHDOG
/***********************************************************************/ 
//Nombre de la funcion :isr()
//Entrada:Ninguna
//Salida :Ninguna
//Descripcion:reinicia el esp32 al alcanzar el tiempo de watchdog
/***********************************************************************/
void IRAM_ATTR isr() //Funcion que se dispara si el timer supera el tiempo 
{
 // DEBUG_PRINT("Rebooting2 .. \n\n");
  ESP.restart();
}
/***********************************************************************/ 
//Nombre de la funcion :Watchdog_setup()
//Entrada:Ninguna
//Salida :Ninguna
//Descripcion:Configura watchdog
/***********************************************************************/

void Watchdog_setup()
{
  
  DEBUG_PRINT("Setting timer in setup");
  watchdogTimer = timerBegin(0, 80, true); //timer 0 divisor 80
    /* 
   *  Se asigna un tiempo en uS debemos colocar un valor que consideremos este demtro del tiempo que nuestro 
   *  programa termina un ciclo, por eso se recomienda usar la funcion millis() para medir este tiempo
  */ 
  timerAlarmWrite(watchdogTimer, (timetosleep+5)*1000, false); // set time in uS must be fed within this time or reboot
  timerAttachInterrupt(watchdogTimer, isr, true);
  timerAlarmEnable(watchdogTimer);  // Activa la alarma y la interrupcion

  /////

  
  
}
/***********************************************************************/ 
//Nombre de la funcion :Watchdog_loop()
//Entrada:Ninguna
//Salida :Ninguna
//Descripcion:reinicia el temporizador de watchdog e imprime informacion de monitoreo
/***********************************************************************/
void Watchdog_loop()
{
  timerWrite(watchdogTimer, 0); // Reseteamos el timer al principio de cada loop para no disparar la alarma
 
  
  looptime = millis();
  #ifdef DEEPSLEEP
   if (digitalRead(gpioin1))  
   {
  DEBUG_PRINTLN("Main Loop Running"); //simplemente avisa cada vez que arrancamos 
   }
  #endif
  looptime = millis() - looptime; // se usa para medir el tiempo 
  #ifdef DEEPSLEEP
   if (digitalRead(gpioin1))  
   {
  DEBUG_PRINT("Tiempo en loop: "); //Imprime el mensaje
  DEBUG_PRINT(looptime);
  DEBUG_PRINTLN("ms \n\n");
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
void Watchdog_setup(){}
void Watchdog_loop(){}
void IRAM_ATTR isr(){}
#endif
