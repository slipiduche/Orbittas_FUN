
/**
*  Nombre de la Funcion:         Funcion Debug 
* Descripcion de la Funcion:
* 
* Este proyecto consiste en un ESP32 como elemento principal, esta funcion esta basada en el Main de 
* alejandro camacaro para ser incluida dentro del set de funciones, para hacer mas facil su llamado y 
* pueda ser utilizada en otras partes de codigo 
* 
*  Author (s)
*  Proyectos Orbittas:          Alejandro Camacaro 
*                               Kevin Mijares
*   
*  version                       1.0.1   
*  date                          05/07/2019
*                                         
*  
*  Nombre del archivo :         DEBUG 
*
*
*    
*  Dependencias:Requiere de estas librerias para trabajar :
*  Este template incluye algunas dependecias de archivos .h (headers) que estan disponibles en la base de dato y contienen la documentacion
*  sientete libre de agregar nuevas si son necesarias dentro de tu proyecto, recuerda documentar como se hace aqui 
*      
*  *  Dependencias:Requiere de estas librerias para trabajar :
*     
*     NO REQUIERE LIBRERIAS
*       
*  Requisitos para compilar el codigo:
*  
* -Tener instalado arduino ide-1.8.9
* -Tener instalado Arduino core for ESP32 WiFi chip ver instrucciones en el siguiente link https://github.com/espressif/arduino-esp32
* -Descomprimir todos los archivos de la carpeta librerias en el directorio de librerías de Arduino.
*      
*      
********************************************************************************/

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
