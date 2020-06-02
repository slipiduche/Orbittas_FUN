/**
   Nombre Función:         fun_b707c.ino
   Descripcion de Función:

   Configuracion y manejo de funciones correspondientes al sensor b707c (telemetro)
  

   Author (s)
   Proyectos Orbittas:          Alejandro Camacaro

   version                      1.1
   date                          20/04/2020


   Nombre del archivo :         fun_b707c.ino
   
   Dispositivos utilizables: telemetro b707c
   
   Dependencias:No requiere
 */



#define COMPILE //utilizado para compilar comenta si va a utilizar la función en otro sketch
//#define esp32 //descomentar si usa esp32
#define arduino //comentar si usa esp32
#define USEb707c 

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
#ifdef esp32
#define RXD1 25                        //Se mapea el puerto de recepcion serial al pin GPIO25 del esp32 puesto que el uart 1 por defecto comparte pines con bus spi
#define TXD1 26                        //Se mapea el puerto de transmision serial al pin GPIO26 del esp32 puesto que el uart 1 por defecto comparte pines con bus spi

HardwareSerial Sensor(1); // Defni puerto serial para el telemetro //comentar si se usa arduino
#endif
#ifdef arduino
#define Sensor Serial2

#endif

/***********************************************************************/ 
//Nombre de la funcion :b707c_setup()
//Entrada:Ninguna
//Salida :Ninguna
//Descripcion:Configura el puerto serial para sensor b707c
/***********************************************************************/
#ifdef USEb707c
void b707c_setup() {
  // initialize both serial ports:
  //Serial.begin(115200);
 #ifdef esp32
  Sensor.begin(19200,SERIAL_8N1,RXD1,TXD1);
  #endif
  #ifdef arduino
  Sensor.begin(19200);
  #endif 
  //Sensor.write(0x55);

}

/***********************************************************************/ 
//Nombre de la funcion :get_distanciaD()
//Entrada:Ninguna
//Salida :String
//Descripcion:Devuelve el valor medido de la distancia en metros xx.xxx metros
/***********************************************************************/
String get_distanciaD()
{String distancia="";
 long int breakout=0;
 char inByte;
    Sensor.write('D');
  while (Sensor.available()==0){
    if (breakout++==10000)
    { distancia="";
      break;}
    }
    breakout=0;
  while (Sensor.available()||inByte!='m') {
    if (inByte==' '||inByte==':')
    {distancia="";}
     inByte = Sensor.read();
    if (inByte=='m')
    {break;}
    else if (inByte!='¿')
    {distancia=distancia+inByte;}
    if (breakout++==10000)
    {distancia="";break;}

    
  }
 breakout=0;
 return distancia;
 
}
/***********************************************************************/ 
//Nombre de la funcion :get_distanciaF()
//Entrada:Ninguna
//Salida :String
//Descripcion:Devuelve el valor medido de la distancia en metros xx.xxx metros
/***********************************************************************/
String get_distanciaF()
{String distancia="0";
 long int breakout=0;
 float distf;
 char inByte;
    Sensor.write('F');
    DEBUG_PRINTLN("distancia:");
  while (Sensor.available()==0){
    if (breakout++==10000)
    { distancia="";
      break;}
    }
    breakout=0;
  while (Sensor.available()||inByte!='m') {
    if (inByte==' '||inByte==':')
    {distancia="";}
     inByte = Sensor.read();
    if (inByte=='m')
    {break;}
    else if (inByte!='¿')
    {distancia=distancia+inByte;}
    if (breakout++==20000)
    {DEBUG_PRINTLN(distancia);
      distancia="0";break;}

    
  }
  
 breakout=0;
 distf=distancia.toFloat();
 DEBUG_PRINTLN(distancia);
 DEBUG_PRINTLN(distf);
 if (distf>100.00)
 {distancia="100.000";
 
 //POWERRESET(); only in tmp use
 b707c_setup();
 }
 return distancia;
 
}
/***********************************************************************/ 
//Nombre de la funcion :get_distanciaM()
//Entrada:Ninguna
//Salida :String
//Descripcion:Devuelve el valor medido de la distancia en metros xx.xxx metros
/***********************************************************************/
String get_distanciaM()
{String distancia="";
 long int breakout=0;
 char inByte;
    Sensor.write('M');
  while (Sensor.available()==0){
    if (breakout++==20000)
    { distancia="";
      break;}
    }
    breakout=0;
  while (Sensor.available()||inByte!='m') {
    if (inByte==' '||inByte==':')
    {distancia="";}
     inByte = Sensor.read();
    if (inByte=='m')
    {break;}
    else if (inByte!='¿')
    {distancia=distancia+inByte;}
    if (breakout++==20000)
    {distancia="";break;}

    
  }
 breakout=0;
 return distancia;
 
}
/***********************************************************************/ 
//Nombre de la funcion :laser_on()
//Entrada:Ninguna
//Salida :String
//Descripcion:Devuelve OK si enciende laser
/***********************************************************************/
String laser_on()
{String respuesta="";
 long int breakout=0;
 char inByte;
    Sensor.write('O');
  while (Sensor.available()==0){
    if (breakout++==10000)
    { respuesta="";
      break;}
    }
    breakout=0;
  while (Sensor.available()||inByte!='!') {
    if (inByte==',')
    {respuesta="";}
     inByte = Sensor.read();
    if (inByte=='!')
    {break;}
    else if (inByte!='¿')
    {respuesta=respuesta+inByte;}
    if (breakout++==10000)
    {respuesta="";break;}

    
  }
 breakout=0;
 return respuesta;
 
}
/***********************************************************************/ 
//Nombre de la funcion :laser_off()
//Entrada:Ninguna
//Salida :String
//Descripcion:Devuelve ok si apaga el laser
/***********************************************************************/
String laser_off()
{String respuesta="";
 long int breakout=0;
 char inByte;
    Sensor.write('C');
  while (Sensor.available()==0){
    if (breakout++==10000)
    { respuesta="";
      break;}
    }
    breakout=0;
  while (Sensor.available()||inByte!='!') {
    if (inByte==',')
    {respuesta="";}
     inByte = Sensor.read();
    if (inByte=='!')
    {break;}
    else if (inByte!='¿')
    {respuesta=respuesta+inByte;}
    if (breakout++==10000)
    {respuesta="";break;}

    
  }
 breakout=0;
 return respuesta;
 
}
/***********************************************************************/ 
//Nombre de la funcion :temp_volt()
//Entrada:Ninguna
//Salida :String
//Descripcion:Devuelve temperatura y voltaje
/***********************************************************************/
String temp_volt()
{String respuesta="";
 long int breakout=0;
 char inByte;
    Sensor.write('S');
  while (Sensor.available()==0){
    if (breakout++==10000)
    { respuesta="";
      break;}
    }
    breakout=0;
  while (Sensor.available()||inByte!='V') {
    if (inByte==' ')
    {respuesta="";}
     inByte = Sensor.read();
    if (inByte=='V')
    {respuesta=respuesta+inByte;break;}
    else if (inByte!='¿')
    {respuesta=respuesta+inByte;}
    if (breakout++==10000)
    {respuesta="";break;}

    
  }
 breakout=0;
 return respuesta;
 
}
/***********************************************************************/ 
//Nombre de la funcion :versionh()
//Entrada:Ninguna
//Salida :String
//Descripcion:Devuelve version del dispositivo
/***********************************************************************/
String versionh()
{String respuesta="";
 long int breakout=0;
 char inByte;
    Sensor.write('V');
  while (Sensor.available()==0){
    if (breakout++==10000)
    { respuesta="";
      break;}
    }
    breakout=0;
  while (Sensor.available()||inByte!=',') {
    if (inByte==':')
    {respuesta="";}
     inByte = Sensor.read();
    
    if (inByte!='¿')
    {respuesta=respuesta+inByte;
   }
   
    if (breakout++==10000)
    {respuesta="";break;}
if (inByte==0x0D&&respuesta.length()>8)
   {break;}
    
  }
 breakout=0;
 return respuesta;
 
}
/***********************************************************************/ 
//Nombre de la funcion :b707c_loop()
//Entrada:Ninguna
//Salida :Ninguna
//Descripcion: Recibe caracteres por puerto serial para ejecutar funciones del sensor
/***********************************************************************/
void b707c_loop() {
 
  
///DEBUG_PRINTLN();
  // read from port 0, send to port 1:
  if (Serial.available()) {
    int inByte = Serial.read();
    DEBUG_PRINTLN(inByte);
    if (inByte=='D'){
      String c;
      c=get_distanciaD();
      DEBUG_PRINTLN(c);
      }
       if (inByte=='F'){
      String c;
      c=get_distanciaF();
            DEBUG_PRINTLN("distancia:");
      DEBUG_PRINTLN(c);
      }
      if (inByte=='M'){
      String c;
      c=get_distanciaM();
      DEBUG_PRINTLN(c);
      }
      if (inByte=='O'){
      String c;
      c=laser_on();
      DEBUG_PRINTLN(c);
      }
      if (inByte=='S'){
      String c;
      c=temp_volt();
      DEBUG_PRINTLN(c);
      }
       if (inByte=='V'){
      String c;
      c=versionh();
      DEBUG_PRINTLN(c);
      }
       if (inByte=='C'){
      String c;
      c=laser_off();
      DEBUG_PRINTLN(c);
      }
  }
}
#ifdef  COMPILE
void setup()
{}
void loop()
{}
#endif 
#else
void b707c_setup(){}
String get_distanciaD(){}
String get_distanciaF(){}
String get_distanciaM(){}
String laser_on(){}
String laser_off(){}
String temp_volt(){}
String versionh(){}
void b707c_loop(){}
#endif
