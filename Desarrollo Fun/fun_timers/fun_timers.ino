/* Nombre Función:         fun_Watchdog_deepsleep
   Descripcion de Función:

   Funcion de temporizador general para ser llamada desde el loop genera un retardo de duracion multiple si lo desea
  

   Author (s)
   Proyectos Orbittas:          Alejandro Camacaro 

   version                      1.1
   date                         28/04/2020


   Nombre del archivo :         fun_timers.ino
   
   Dispositivos utilizables: ESP32, Arduino, Stm32
   
   Dependencias:NO REQUIERE
   Requisitos:
  -Tener instalado arduino ide-1.8.9
  -Tener instalado Arduino core for ESP32 WiFi chip ver instrucciones en el siguiente link https://github.com/espressif/arduino-esp32
        
*/ 
#define COMPILE //utilizado para compilar comenta si va a utilizar la función en otro sketch

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
////////timers/////////
uint32_t timerLast = 0;                             //last value of timer 
uint32_t timerInterval=60000;                       // Setear para la primera vez de encendido
/***********************************************************************/ 
//Nombre de la funcion :timer_fun(uint32_t timer_intervals, uint32_t fun_delay, String fun)
//Entrada:uint32_t timer_intervals_1: tiempo de retardo de ejecución, siempre necesario para generar el retardo
//        uint32_t timer_intervals_2: 2do retardo de ejecución, colocar 0 si no va a usarse
//        String fun: Para utilizar varias veces y distinguir entre funciones. "ALGO"
//Salida :Ninguna
//Descripcion:Configura modo sleep
/***********************************************************************/

void timer_fun(uint32_t timer_intervals_1, uint32_t timer_intervals_2, String fun)
{
  uint32_t timerNow = millis();
  
  if (fun == "ALGO") {
    if (abs(timerNow - timerLast) >= timerInterval) {
      timerLast = timerNow;
      
      // do something
      if (timer_intervals_2==0)
      {timerInterval = timer_intervals_1; //Si necesita un solo retardo
      }
      else{timerInterval = timerInterval == timer_intervals_2 ? timer_intervals_1 : timer_intervals_2;//  si necesitas agregar intervalos diferentes sucesivos
      }
      DEBUG_PRINT("something");
    }
  }

}
#ifdef  COMPILE
void setup()
{}
void loop()
{}
#endif 
