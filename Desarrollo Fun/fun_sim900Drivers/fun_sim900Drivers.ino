/**
   Nombre Función:         fun_sim900Drivers
   Descripcion de Función:

   Configuracion y manejo de funciones principales de modulo SIM900 para ejecutar comandos AT y manejo de protocolo TCP
  

   Author (s)
   Proyectos Orbittas:          Alejandro Camacaro

   version                       1.1
   date                          21/04/2020


   Nombre del archivo :         fun_sim900Drivers.ino
   
   Dispositivos utilizables: sim900 GSM MODULE
   Dependencias:Requiere de estas librerias para trabajar :
*/    
   #include <PubSubClient.h> //     ---> Por Nick O'Leary version 2.7.0
#define USESIM900 ////comentar si no necesita usar modulo SIM900
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
/**** REGISTRO SIM900 ****/
#ifdef USESIM900
#ifdef esp32
HardwareSerial SIM900(2);              //Se define el puerto hardware uart 2 del esp32 para uso de modem SIM900
#endif

#ifdef arduino
#define  SIM900 Serial2              //Se define el puerto hardware uart 2 del arduino para uso de modem SIM900
#endif
#endif

/****Variables SIM900****/

char aux_str[100];                         //se utiliza como auxiliar en la formacion de cadenas para enviar comandos AT al sim900

char  __APN [120]     = "";                //APN de la red celular
char  __usrnm [60] = "";                   //User Name de la red celular
char __password[60] = "";                  //Password APN de la red celular
int intentosg = 0;                         //utilizada para controlar el numero de intentos en acceder a la red celular
char sim800lreset = 4;                     //utilizado para encendido y apagado el modulo sim900 con el pin GPIO4 del esp32 o pin 4 arduino
int start = 0;                             //utilizada para no repetir configuraciones del modulo sim900
int sim900 = 0;                            //utilizada para indicar si el mosulo sim900 esta activo o inactivo

#ifdef USESIM900
/***********************************************************************/ 
//Nombre de la funcion :Reiniciar()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada reiniciar el modem SIM900
/***********************************************************************/


void Reiniciar()
  {
  SIM900.begin(19200);
  DEBUG_PRINTLN("RESET SIM900...");
  sim900_enviarAT("AT+CPOF\r", "OK", 1000); 
  Power_off();
  delay(5000);
  Power_on();  
  delay(1000);
  Configurar_SIM();
 // MQTTProtocolNameLength = strlen(MQTTProtocolName); //only mqtt use
  start=0;                                     //secuencia de control de protocolo mqtt reiniciada
  }

/***********************************************************************/ 
//Nombre de la funcion :Power_off()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada apagar modem SIM900
/***********************************************************************/  
void Power_off()
  {while (sim900_enviarAT("AT\r", "OK", 1000)==1)
   { pinMode(sim800lreset, OUTPUT);             //configura como salida pin encendido y apagado del sim900
 digitalWrite( sim800lreset, HIGH);
 delay(1000);
 digitalWrite( sim800lreset, LOW);
 delay(2000);
 sim900_enviarAT("AT+CPOF\r", "OK", 1000);             // Software OFF
 }
 sim900=0;                                      //variable de sim900 inactivo
// mconnect=0;                                    //puede volver a reconectar con broquer mqtt a traves de sim900 only mqtt
 }
/***********************************************************************/ 
//Nombre de la funcion :Power_on()
//Entrada: Ninguna
//Salida :Booleana
//Descripcion:Esta funcion es la encargada encender modem SIM900
/***********************************************************************/
bool Power_on()
  {
  DEBUG_PRINTLN("STARTING SIM");
  int intentos=0;
  int intentos_salir=0;
  DEBUG_PRINTLN("STARTING GPRS...");
  pinMode(sim800lreset, OUTPUT);
  digitalWrite( sim800lreset, HIGH);
  delay(500);
  digitalWrite( sim800lreset, LOW);
  delay(1000);
  intentos=0;
  
  while ( ! sim900_enviarAT("AT", "OK",2000) )// 
    {pinMode(sim800lreset, OUTPUT);
  digitalWrite( sim800lreset, HIGH);
  delay(500);
  digitalWrite( sim800lreset, LOW);
  delay(1000);
    DEBUG_PRINT(".");
    //web_loop(); //only in TMP use or esp web server
    if (intentos_salir++>20)
      {
      DEBUG_PRINTLN("ERROR SIM");
      return false;
      }
    if (intentos++>5) 
      {   
        Power_off();
        DEBUG_PRINTLN("STARTING GPRS...");
        pinMode( sim800lreset, OUTPUT);
        digitalWrite( sim800lreset, HIGH);
        delay(500);
        digitalWrite( sim800lreset, LOW);
        delay(1000);
        intentos=0;
        break;
      }
    }
  if ( sim900_enviarAT("AT", "OK", 2000) )
    {
    intentos=0;
    DEBUG_PRINTLN("SIM STARTED, AT OK!");
    delay(1000);
    return true;
    }
  return false;
  }

/***********************************************************************/ 
//Nombre de la funcion :Configurar_SIM()
//Entrada: Ninguna
//Salida :Booleana
//Descripcion:Esta funcion es la encargada de Configurar parametros de modem SIM900
/***********************************************************************/
  
void Configurar_SIM()
  {
  DEBUG_PRINTLN("SETTING...");
  pinMode(sim800lreset, OUTPUT);
  Power_off();
  delay(2000);
  Power_on();
  delay(1000);    
  delay (100);
  int intentos=0;
  sim900_enviarAT("ATE0\r", "OK", 100);  
  sim900_enviarAT("AT+CREG=0\r", "OK", 100); 
  sim900_enviarAT("AT+CFUN=1\r", "OK", 100); 
  sim900_enviarAT("AT+CLTS=1\r", "OK", 100); 
  sim900_enviarAT("AT+CMGF=1\r", "OK", 100); 
  sim900_enviarAT("AT+CPMS=\"SM\",\"SM\",\"SM\"\r", "OK", 1000);
  sim900_enviarAT("AT+CNMI=0,0,0,0,0\r", "OK", 100); 
  sim900_enviarAT("AT&W\r", "OK", 100); 
  sim900_enviarAT("AT+CCLK?\r", "OK", 100); 
  DEBUG_PRINTLN("NETWORK CONNECTING...");
  delay (1000);

  while ( sim900==0 &&intentosg<=10)  
    {
      //web_loop(); //only in TMP use or esp web server
    if (sim900_enviarAT2("AT+CREG?", "+CREG: 0,1", "+CREG: 0,5", 3000)==1)
      {
       sim900=1;                                       //coloca el estado del sim900 como activo
       intentosg=0;                                    //reinicia el control de intentos de conexion a la red con sim900
      break;
      }
    else
     {
      //web_loop(); //only in TMP use or esp web server
      if (intentosg++>10)                             //si la cantidad de intentos es mayor a 10 arroja error de red reinicia el modem y sale del bucle        
        {
         DEBUG_PRINTLN("ERROR RED O SIMCARD");  
         Power_off();
         intentosg=0;
         delay(1000);
         Power_on();
         sim900_enviarAT("ATE0\r", "OK", 100); 
         sim900_enviarAT("AT+CREG=0\r", "OK", 100); 
         sim900_enviarAT("AT+CFUN=1\r", "OK", 100); 
         sim900_enviarAT("AT+CLTS=1\r", "OK", 100); 
         sim900_enviarAT("AT+CMGF=1\r", "OK", 100); 
         sim900_enviarAT("AT+CNMI=0,0,0,0,0\r", "OK", 100); //
         sim900_enviarAT("AT&W\r", "OK", 100); 
         sim900_enviarAT("AT+CCLK?\r", "OK", 100); // get time
         DEBUG_PRINTLN("NETWORK CONNECTING...");
         delay (1000);
         sim900=0;
         break;
         }
     }
  }
  if (sim900==1)
  {DEBUG_PRINTLN("NETWORK CONNECTED.");
  DEBUG_PRINTLN("READY.");}
 
  }
/***********************************************************************/ 
//Nombre de la funcion :sim900_enviarAT()
//Entrada: String ATCommand,String resp_correcta,unsigned long tiempo
//Salida :int
//Descripcion:Esta funcion es la encargada de enviar comandos AT via serial a SIM900
/***********************************************************************/
int sim900_enviarAT(String ATcommand, String resp_correcta, unsigned long tiempo)
  {
  int x = 0;
  bool correcto = false;
  char respuesta[240];
  char respuesta1[240];
  unsigned long anterior;
  DEBUG_PRINTLN("--->"+ ATcommand);
  memset(respuesta, '\0', 240);                  
  memset(respuesta1, '\0', 240); 
  resp_correcta.toCharArray(respuesta1,resp_correcta.length()+1);
  delay(100);
  while ( SIM900.available() > 0)
    { 
     SIM900.read();
     } 
  SIM900.println(ATcommand);                     
  
  anterior = millis();
  while ((correcto == 0) && (abs(millis() - anterior) < tiempo))  
      {
      if (SIM900.available() != 0)                               // verifica si hay respuesta del comando enviado
          {if(x<240)
           {respuesta[x++] = SIM900.read();}
           if(x>239)
           {DEBUG_PRINT(SIM900.read());}
           if (strstr(respuesta, respuesta1) != NULL)           // compara respuestas
              {
               correcto = 1;
               break;
               }
          }  
     // web_loop(); //only in TMP use or esp web server
      }
  if (respuesta[0]!='\0') 
      DEBUG_PRINTLN("<---"+String(respuesta));
      return correcto;
  }
/***********************************************************************/ 
//Nombre de la funcion :sim900_enviarAT2()
//Entrada: String ATCommand,char* resp_correcta,char* resp_correcta2,unsigned int tiempo
//Salida :int
//Descripcion:Esta funcion es la encargada de enviar comandos AT via serial a SIM900 con 2 posibles respuestas
/***********************************************************************/
int sim900_enviarAT2(String ATcommand, char* resp_correcta,char* resp_correcta2, unsigned int tiempo)
  {
  int x = 0;  
  int correcto = 0;
  char respuesta[240];
  unsigned long anterior;
  DEBUG_PRINTLN("--->"+ ATcommand);
  memset(respuesta, '\0', 100); 
  delay(100);
  while ( SIM900.available() > 0)
      { 
       SIM900.read();
       SIM900.flush();
      }
  SIM900.println(ATcommand);
 
  anterior = millis();
  
  while ((correcto == 0) && (abs(millis() - anterior) < tiempo))
    {
    if (SIM900.available() != 0)                            // verifica si hay respuesta del modem
      {
      if(x<240)
          { respuesta[x++] = SIM900.read();}
           if(x>239)
          { DEBUG_PRINT(SIM900.read());}
      if (strstr(respuesta, resp_correcta) != NULL)         // compara respuesta 1
        {
        correcto = 1;
        break;
        }
      else if (strstr(respuesta, resp_correcta2) != NULL)  // compara respuesta 2
        {
        correcto = 1;
        break;
        }
      }
  //web_loop(); //only in TMP use or esp web server
    }
  if (respuesta[0]!='\0')   
    {
    DEBUG_PRINT("<---");
    DEBUG_PRINTLN(respuesta);
    }
  DEBUG_PRINTLN(correcto);
   
  if (correcto==0) 
    {
     if (intentosg++>10)                          //utilizado en el intento de conexion a la red para reiniciar el modem sim900 si supera los 10 intentos       
        {
         DEBUG_PRINTLN("ERROR RED O SIMCARD");  
         Power_off();
         intentosg=0;
         delay(1000);
         Power_on();
          return correcto;
         }
     }
  return correcto;
  }
/***********************************************************************/ 
//Nombre de la funcion :readSeverResponse()
//Entrada: char* ATcommand,char* expected_answer1,char* expected_answer2 ,unsigned int timeout
//Salida :int8_t
//Descripcion:Esta funcion es la encargada verificar recepcion de datos a traves de SIM900 e imprimirlos al puerto serial
/***********************************************************************/
int8_t readServerResponse(char* ATcommand, char* expected_answer1, char* expected_answer2, unsigned int timeout) 
  {char trama[512];
  int index=0,comas=0;
  
  unsigned long nowMillis = millis();
  memset(trama, 0, 512);
  SIM900.println(ATcommand);
  delay(3000);
  if (SIM900.available()) 
    { while (SIM900.available()) 
    {trama[index++]=byte(SIM900.read());             //recepcion de la respuesta del modem
   
    DEBUG_PRINT(trama[index-1]);
    if(trama[index-1]==0x00)                         //si hay caracteres vacíos reinicia el indice de escritura
    {index=0;
    memset(trama, 0, 512);
   
    }
if(trama[index-1]==',')                              //cuenta la cantidad de comas recibidas para descartar comandos no deseados enviados por el modem
    {comas++;
    }
    if (comas>8)
    {if(trama[index-1]==0x0D)
     {DEBUG_PRINTLN();
      break;}
      }
    }
    
    index=0;
    //loadmqttconfig(trama);                          //solo tmp ubicada en fun_loadconfig.ino --> verifica si es un comando de configuracion válido para cargarlo y almacenarlo en la memoria micro SD
                                                    
                                                    //Se descartan datos residuales del modem
    while (char(SIM900.read()) != 0x24)             
          {
      if (SIM900.available())
        {
         DEBUG_PRINT(char(SIM900.read()));
         }
      if (abs(millis() - nowMillis) > timeout) 
        {
          //web_loop(); //only in TMP use or esp web server
        DEBUG_PRINTLN("NO DATA RECEIVED FROM REMOTE");
        if(sim900_enviarAT("AT+CIPSERVER=0", "OK", 1000)==1){DEBUG_PRINTLN("close connection");};
        break;
        }
       }
    nowMillis=(millis());
    /*while (SIM900.available()) 
    {
    DEBUG_PRINT(char(SIM900.read()));
    }
    */
  }

  }
/***********************************************************************/ 
//Nombre de la funcion :initTCP()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada de iniciar la comunicacion TCP/IP
/***********************************************************************/
int initTCP() {
//web_loop(); //only in TMP use or esp web server
  if ( sim900==1 )
    {  
     delay(2000);
     if (start<1)
        { 
        if (sim900_enviarAT("AT+CIPMUX?", "+CIPMUX: 0", 300)==0)
        sim900_enviarAT("AT+CIPMUX=0", "OK", 100);
        }
   
      if (sim900_enviarAT("AT+CIPMUX?", "+CIPMUX: 0", 300)==1|| start>1)  
        {
        if (start<1)
          {
          if (sim900_enviarAT("AT+CIPRXGET?", "+CIPRXGET: 1",300)==0 )
          sim900_enviarAT("AT+CIPRXGET=1", "OK", 100);
          }
        if (sim900_enviarAT2("AT+CIPRXGET?", "+CIPRXGET: 0","+CIPRXGET: 1",300)==1 ) 
          { 
          int8_t correcto = 0;
          int8_t correcto1 = 0;
          if (start<1)
            {
            delay(1000);
            if ((sim900_enviarAT("AT+CIPMODE?", "+CIPMODE: 0", 300) )==0)
                sim900_enviarAT("AT+CIPMODE=0", "OK", 100);
            if ((sim900_enviarAT("AT+CIPMODE?", "+CIPMODE: 0", 300) )==0)
                return 0;
            delay(500);
            if ((sim900_enviarAT("AT+CIPSRIP?", "+CIPSRIP: 0", 300) )==0)
                sim900_enviarAT("AT+CIPSRIP=0", "OK", 1000);
            if ((sim900_enviarAT("AT+CIPSRIP?", "+CIPSRIP: 0", 300) )==0)
                return 0;
            delay(500);
            while (sim900_enviarAT("AT+CGATT?", "+CGATT: 1", 300) == 0 );
              delay(1000);
           delay(1000);
           snprintf(aux_str, sizeof(aux_str), "AT+CSTT=\"%s\",\"%s\",\"%s\"", __APN,"","");// __usrnm, __password);
            if (sim900_enviarAT2("AT+CIPSTATUS", "INITIAL","CONNECTED", 1000) == 1 )
              {
              if(sim900_enviarAT(aux_str, "OK",  30000)){start=1;}
              }
            }
           if(start>=1)
               {  
                if (start<2)
                    {
                    sim900_enviarAT("AT+CIICR", "OK", 3000);
                    if(sim900_enviarAT("AT+CIPSTATUS", "GPRSACT", 500)==1)
                        { 
                        start=2;
                        }
                     }
                if(start>1)
                    {
                    if (start<3)
                        {
                        if (sim900_enviarAT("AT+CIFSR", ".", 10000)==1)
                            {start=3;}
                        }
                    if (start>2)
                        {
                        delay(5000);
                        DEBUG_PRINTLN(F("Opening TCP"));
                        //snprintf(aux_str, sizeof(aux_str), "AT+CIPSTART=\"TCP\",\"%s\",\"%s\"", MQTTHost, MQTTPort);  //if you need a mqtt or tcp conection
                       if (start<4)
                       {
                        if (sim900_enviarAT2(aux_str, "OK\r\n\r\nCONNECT","ALREADY CONNECT", 30000) )
                           {start=4;
                           DEBUG_PRINTLN(F("Connected"));
                           return 1;
                            }
                        else
                            {
                            DEBUG_PRINTLN(F("Error opening the connection"));
                            DEBUG_PRINTLN(F("UNABLE TO CONNECT TO SERVER "));
                            start=0;
                            return 0;
                            }
                        }
                            if (start==4){
                              DEBUG_PRINTLN(F("Still Connected"));
                           return 1;
                              }

                        }
                    else
                        {
                        DEBUG_PRINTLN(F("ERROR GETTING IP ADDRESS "));
                         start=0;
                        return 0;
                       }
                    }
                else
                    {
                    DEBUG_PRINTLN(F("ERROR BRINGING UP WIRELESS CONNECTION"));
                     start=0;
                    return 0;
                    }
                }
        else 
        {
        DEBUG_PRINTLN(F("Error setting the APN"));
         start=0;
        Reiniciar();
        return 0;
        }


      }
      else
      { start=0;
      DEBUG_PRINTLN(F("Error setting CIPRXGET"));
      return 0;
      } 


      }
  else{ start=0;Reiniciar();
  }
  }
 else{ start=0;Reiniciar();
 }
}
#ifdef  COMPILE
void setup()
{}
void loop()
{}
#endif 
#else
void Reiniciar(){}
bool Power_off(){}
bool Power_on(){}
void Configurar_SIM(){}
int sim900_enviarAT(String ATcommand, String resp_correcta, unsigned long tiempo){}
int sim900_enviarAT2(String ATcommand, char* resp_correcta,char* resp_correcta2, unsigned int tiempo){}
int8_t readServerResponse(char* ATcommand, char* expected_answer1, char* expected_answer2, unsigned int timeout) {}
int initTCP(){}

#endif
