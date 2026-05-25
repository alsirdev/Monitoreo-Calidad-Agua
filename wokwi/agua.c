// Exportacion de librerias
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Definicion de la pantalla
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, 
                          &Wire, OLED_RESET);

// Sensores Pines de conexion en el ESP32
#define CONDUCTIVIDAD 34
#define ACIDEZ        35
#define TURBIEDAD     32

int condADC;
int phADC;
int turbADC;

// Variables convertidas
int EC;
float pH;
float NTU;

//-------------------------
// CONDUCTIVIDAD
//-------------------------
String estadoConductividad(int valorEC){

if(valorEC < 300)
 return "Excelente";

else if(valorEC <=600)
 return "Adecuado";

else
 return "ALERTA";

}

//-------------------------
// PH
//-------------------------
String estadoPH(float valorPH){

if(valorPH < 5.0)
 return "No apto";

else if(valorPH <=9.0)
 return "Permitido";

else
 return "ALERTA";

}

//-------------------------
// TURBIDEZ
//-------------------------
String estadoTurbidez(float valorNTU){

if(valorNTU <1.0)
 return "Ideal";

else if(valorNTU <=4.0)
 return "Permitido";

else
 return "ALERTA";

}

void setup(){

Serial.begin(115200);

Wire.begin(21,22);

if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
Serial.println("OLED no encontrada");
while(true);
}

pinMode(CONDUCTIVIDAD,INPUT);
pinMode(ACIDEZ,INPUT);
pinMode(TURBIEDAD,INPUT);

// Pantalla inicio
display.clearDisplay();

display.setTextSize(1);
display.setTextColor(SSD1306_WHITE);

display.setCursor(8,15);
display.println("Estacion");

display.setCursor(8,40);
display.println("de Medicion");

display.display();

delay(2500);

}

void loop(){

// Lecturas ADC
condADC = analogRead(CONDUCTIVIDAD);
phADC   = analogRead(ACIDEZ);
turbADC = analogRead(TURBIEDAD);

// Conversiones
EC  = map(condADC,0,4095,0,1000);

pH  = (phADC * 14.0)/4095.0;

NTU = (turbADC * 10.0)/4095.0;

// Estados
String estadoEC   = estadoConductividad(EC);
String estadoPh   = estadoPH(pH);
String estadoTurb = estadoTurbidez(NTU);

// SERIAL
Serial.print("EC:");
Serial.print(EC);
Serial.print(" ");
Serial.print(estadoEC);

Serial.print(" | pH:");
Serial.print(pH,1);
Serial.print(" ");
Serial.print(estadoPh);

Serial.print(" | NTU:");
Serial.print(NTU,1);
Serial.print(" ");
Serial.println(estadoTurb);

// OLED
display.clearDisplay();
display.setTextSize(1);

// Titulo (sin linea separadora)
display.setCursor(20,2);
display.println("Monitoreo Agua");

// Distribucion mejorada
display.setCursor(0,14);
display.print("EC:");
display.print(EC);
display.print(" ");
display.print(estadoEC);

display.setCursor(0,32);
display.print("pH:");
display.print(pH,1);
display.print(" ");
display.print(estadoPh);

display.setCursor(0,50);
display.print("NTU:");
display.print(NTU,1);
display.print(" ");
display.print(estadoTurb);

display.display();

delay(300);

}