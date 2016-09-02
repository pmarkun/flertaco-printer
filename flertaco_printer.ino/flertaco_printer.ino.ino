#include <Adafruit_Thermal.h>
#include "logohacker.h"

#include "SoftwareSerial.h"
#define TX_PIN 2 // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN 3 // Arduino receive   GREEN WIRE   labeled TX on printer

int pressed = 0;

char[][140] = {
"cada vereador pode contratar até 30 funcionários para seu gabinete?",
"cada gabinete dispõe de 130 mil reais mensais para pagamento de salários de funcionários?",
"cada gabinete pode gastar até 239 mil reais por ano com aluguel de veículos, assinaturas de jornais, serviços gráficos e material de escritório?"
}

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);     // Pass addr to printer constructor

void setup() {
  mySerial.begin(19200);
  printer.begin();
  pinMode(8, INPUT_PULLUP);
  rockandroll();
}

void loop() {
  pressed = digitalRead(8);
  if (!pressed) {
    rockandroll();
    delay(500);
  }
}


void rockandroll() {
  printer.justify('C'); 
  printer.printBitmap(logohacker_width, logohacker_height, logohacker_data);
  printer.feed(1);
  printer.println("== Politica Hacker ==");
  printer.println("Vote Pedro Markun");
  printer.setSize('L');      // setting the size adds a linefeed
  printer.println("18007");
  printer.setSize('S');      // setting the size adds a linefeed
  printer.println("fb.com/politicahacker");
  printer.println("(11) 994837732");
  printer.println("CNPJ: 25.864.934/0001-00");
  printer.feed(3);
}

