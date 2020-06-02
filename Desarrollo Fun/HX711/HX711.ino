/**
   Nombre Función:         HX711
   Descripcion de Función:

   Configuracion y manejo de funciones de adquisicion de datos del HX711 
  

   Author (s)
   Proyectos Orbittas:          Alejandro Camacaro

   version                      1.1
   date                          21/04/2020


   Nombre del archivo :         HX711.ino
   
   Dispositivos utilizables: HX711 conversor analógico digittal para balanzas o puentes de wheastone
   
   Dependencias:Requiere de estas librerias para trabajar :
*/  
  #include <Adafruit_Sensor.h> // - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

  #include <HX711.h> //by bodgan necula version 0.7.4 https://github.com/bogde/HX711

#define USEHX711 ////comentar si no necesita usar HX711
#define COMPILE //utilizado para compilar comenta si va a utilizar la función en otro sketch

/***DEBUG_PORT***/
#define DEBUG
#ifdef  DEBUG
#define DEBUG_PORT         Serial      //

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
#ifdef USEHX711
// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 3;
const int LOADCELL_SCK_PIN = 4;
const int LOADCELL_DOUT_PIN2 = 5;
const int LOADCELL_SCK_PIN2 = 6;
const int LOADCELL_DOUT_PIN3 = 7;
const int LOADCELL_SCK_PIN3 = 8;
const int LOADCELL_DOUT_PIN4 = 9;
const int LOADCELL_SCK_PIN4 = 10;
const int LOADCELL_DOUT_PIN5 = 11;
const int LOADCELL_SCK_PIN5 = 12;
const int LOADCELL_DOUT_PIN6 = 13;
const int LOADCELL_SCK_PIN6 = 14;
HX711 scale;
HX711 scale2;
HX711 scale3;
HX711 scale4;
HX711 scale5;
HX711 scale6;
String eeprom_cmg1;                   // valor dc de calibración
String eeprom_cmg2;               //
String eeprom_cmg3;                 //
String eeprom_cmg4;               //
String eeprom_cmg5;                 //
String eeprom_cmg6;               //
float weight1, weight2, weight3, weight4, weight5, weight6;// weight global variables

/***********************************************************************/ 
//Nombre de la funcion :scale_setup() 
//Entrada:Ninguna
//Salida :Ninguna
//Descripcion:Configura sensor HX711
/***********************************************************************/
void scale_setup() {
  

  // Initialize library with data output pin, clock input pin and gain factor.
  // Channel selection is made by passing the appropriate gain:
  // - With a gain factor of 64 or 128, channel A is selected
  // - With a gain factor of 32, channel B is selected
  // By omitting the gain factor parameter, the library
  // default "128" (Channel A) is used here.
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale2.begin(LOADCELL_DOUT_PIN2, LOADCELL_SCK_PIN2);
  scale3.begin(LOADCELL_DOUT_PIN3, LOADCELL_SCK_PIN3);
  scale4.begin(LOADCELL_DOUT_PIN4, LOADCELL_SCK_PIN4);
  scale5.begin(LOADCELL_DOUT_PIN5, LOADCELL_SCK_PIN5);
  scale6.begin(LOADCELL_DOUT_PIN6, LOADCELL_SCK_PIN6);
  /*
  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());			// print a raw reading from the ADC
  
  Serial.print("read2: \t\t");
  Serial.println(scale2.read());      // print a raw reading from the ADC
  Serial.print("read3: \t\t");
  Serial.println(scale3.read());      // print a raw reading from the ADC
  Serial.print("read4: \t\t");
  Serial.println(scale4.read());      // print a raw reading from the ADC
  Serial.print("read5: \t\t");
  Serial.println(scale5.read());     // print a raw reading from the ADC
  Serial.print("read6: \t\t");
  Serial.println(scale6.read());      // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));  	// print the average of 20 readings from the ADC
 
  Serial.print("read average2: \t\t");
  Serial.println(scale2.read_average(20));    // print the average of 20 readings from the ADC
 
  Serial.print("read average3: \t\t");
  Serial.println(scale3.read_average(20));    // print the average of 20 readings from the ADC
  Serial.print("read average4: \t\t");
  Serial.println(scale4.read_average(20));    // print the average of 20 readings from the ADC
  Serial.print("read average5: \t\t");
  Serial.println(scale5.read_average(20));    // print the average of 20 readings from the ADC
  Serial.print("read average6: \t\t");
  Serial.println(scale6.read_average(20));    // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight (not set yet)
 
  Serial.print("get value2: \t\t");
  Serial.println(scale2.get_value(5));    // print the average of 5 readings from the ADC minus the tare weight (not set yet)
  Serial.print("get value3: \t\t");
  Serial.println(scale3.get_value(5));    // print the average of 5 readings from the ADC minus the tare weight (not set yet)
  Serial.print("get value4: \t\t");
  Serial.println(scale4.get_value(5));    // print the average of 5 readings from the ADC minus the tare weight (not set yet)
  Serial.print("get value5: \t\t");
  Serial.println(scale5.get_value(5));    // print the average of 5 readings from the ADC minus the tare weight (not set yet)
  Serial.print("get value6: \t\t");
  Serial.println(scale6.get_value(5));    // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);	// print the average of 5 readings from the ADC minus tare weight (not set) divided
  
  Serial.print("get units2: \t\t");
  Serial.println(scale2.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
  Serial.print("get units3: \t\t");
  Serial.println(scale3.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
  Serial.print("get units4: \t\t");
  Serial.println(scale4.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
  Serial.print("get units5: \t\t");
  Serial.println(scale5.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
  Serial.print("get units6: \t\t");
  Serial.println(scale6.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
  
  // by the SCALE parameter (not set yet)

  scale.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();				        // reset the scale to 0
  
  scale2.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale2.tare();                // reset the scale to 0
  scale3.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale3.tare();                // reset the scale to 0
  scale4.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale4.tare();                // reset the scale to 0
  scale5.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale5.tare();                // reset the scale to 0
  scale6.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale6.tare();                // reset the scale to 0
  
  /*scale.set_scale(); //La escala por defecto es 1
    scale.tare(20); //El peso actual es considerado Tara.*/
  /*Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC
  
  Serial.print("read2: \t\t");
  Serial.println(scale2.read());                 // print a raw reading from the ADC
  Serial.print("read3: \t\t");
  Serial.println(scale3.read());                 // print a raw reading from the ADC
  Serial.print("read4: \t\t");
  Serial.println(scale4.read());                 // print a raw reading from the ADC
  Serial.print("read5: \t\t");
  Serial.println(scale5.read());                 // print a raw reading from the ADC
  Serial.print("read6: \t\t");
  Serial.println(scale6.read());                 // print a raw reading from the ADC

  
  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC
  
  Serial.print("read average2: \t\t");
  Serial.println(scale2.read_average(20));       // print the average of 20 readings from the ADC
  Serial.print("read average3: \t\t");
  Serial.println(scale3.read_average(20));       // print the average of 20 readings from the ADC
  Serial.print("read average4: \t\t");
  Serial.println(scale4.read_average(20));       // print the average of 20 readings from the ADC
  Serial.print("read average5: \t\t");
  Serial.println(scale5.read_average(20));       // print the average of 20 readings from the ADC
  Serial.print("read average6: \t\t");
  Serial.println(scale6.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight, set with tare()
  
  Serial.print("get value2: \t\t");
  Serial.println(scale2.get_value(5));    // print the average of 5 readings from the ADC minus the tare weight, set with tare()
  Serial.print("get value3: \t\t");
  Serial.println(scale3.get_value(5));    // print the average of 5 readings from the ADC minus the tare weight, set with tare()
  Serial.print("get value4: \t\t");
  Serial.println(scale4.get_value(5));    // print the average of 5 readings from the ADC minus the tare weight, set with tare()
  Serial.print("get value5: \t\t");
  Serial.println(scale5.get_value(5));    // print the average of 5 readings from the ADC minus the tare weight, set with tare()
  Serial.print("get value6: \t\t");
  Serial.println(scale6.get_value(5));    // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
  
  Serial.print("get units2: \t\t");
  Serial.println(scale2.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
  Serial.print("get units3: \t\t");
  Serial.println(scale3.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
  Serial.print("get units4: \t\t");
  Serial.println(scale4.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
  Serial.print("get units5: \t\t");
  Serial.println(scale5.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
  Serial.print("get units6: \t\t");
  Serial.println(scale6.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
  
  // by the SCALE parameter set with set_scale
*/
  Serial.println("Readings:");
}
/***********************************************************************/ 
//Nombre de la funcion :scale_calibrate(int scalen, int scalek) 
//Entrada:int scalen (numero de balanza a configurar), int scalek (factor de scala para calibrar)
//Salida :Ninguna
//Descripcion:Caalibración de balanzas
/***********************************************************************/
void scale_calibrate(int scalen, int scalek)
{ if (scalen == 1)
  {
    scale.set_scale(scalek);                      // this value is obtained by calibrating the scale with known weights; see the README for details
    scale.tare();                // reset the scale to 0
  }
  else if (scalen == 2)
  {
    scale2.set_scale(scalek);                      // this value is obtained by calibrating the scale with known weights; see the README for details
    scale2.tare();                // reset the scale to 0
  }
  else if (scalen == 3)
  {
    scale3.set_scale(scalek);                      // this value is obtained by calibrating the scale with known weights; see the README for details
    scale3.tare();                // reset the scale to 0
  }
  else if (scalen == 4)
  {
    scale4.set_scale(scalek);                      // this value is obtained by calibrating the scale with known weights; see the README for details
    scale4.tare();                // reset the scale to 0
  }
  else if (scalen == 5)
  {
    scale5.set_scale(scalek);                      // this value is obtained by calibrating the scale with known weights; see the README for details
    scale5.tare();                // reset the scale to 0
  }
  else if (scalen == 6)
  {
    scale6.set_scale(scalek);                      // this value is obtained by calibrating the scale with known weights; see the README for details
    scale6.tare();                // reset the scale to 0
  }
}
/***********************************************************************/ 
//Nombre de la funcion :scale_loop() 
//Entrada:Ninguna
//Salida :Ninguna
//Descripcion:Asigna lectura de pesos a variables globales weightN e impime al puerto serial para monitoreo
/***********************************************************************/
void scale_loop() {
 // scale.power_up();
  /*
  scale2.power_up();
  scale3.power_up();
  scale4.power_up();
  scale5.power_up();
  scale6.power_up();
  */
  Serial.print("one reading:\t");
  Serial.print(scale.get_units(), 1);
  Serial.print("\t| average:\t");
  weight1 = scale.get_units(10)+eeprom_cmg1.toInt();
  Serial.println(weight1, 1);
  Serial.print("one reading2:\t");
  Serial.print(scale2.get_units(), 1);
  Serial.print("\t| average2:\t");
  weight2 = scale2.get_units(10)+eeprom_cmg2.toInt();
  Serial.println(weight2, 1);
  Serial.print("one reading3:\t");
  Serial.print(scale3.get_units(), 1);
  Serial.print("\t| average3:\t");
  weight3 = scale3.get_units(10)+eeprom_cmg3.toInt();
  Serial.println(weight3, 1);
  Serial.print("one reading4:\t");
  Serial.print(scale4.get_units(), 1);
  Serial.print("\t| average4:\t");
  weight4 = scale4.get_units(10)+eeprom_cmg4.toInt();
  Serial.println(weight4, 1);
  Serial.print("one reading5:\t");
  Serial.print(scale5.get_units(), 1);
  Serial.print("\t| average5:\t");
  weight5 = scale5.get_units(10)+eeprom_cmg5.toInt();
  Serial.println(weight5, 1);
  Serial.print("one reading6:\t");
  Serial.print(scale6.get_units(), 1);
  Serial.print("\t| average6:\t");
  weight6 = scale6.get_units(10)+eeprom_cmg6.toInt();
  Serial.println(weight6, 1);
  
 // scale.power_down();			        // put the ADC in sleep mode
  /*scale2.power_down();              // put the ADC in sleep mode
  scale3.power_down();              // put the ADC in sleep mode
  scale4.power_down();              // put the ADC in sleep mode
  scale5.power_down();              // put the ADC in sleep mode
  scale6.power_down();              // put the ADC in sleep mode

*/
}
#ifdef  COMPILE
void setup()
{}
void loop()
{}
#endif 
#else
void scale_setup(){}
void scale_loop(){}
void scale_calibrate(){}
#endif
