#include <Adafruit_Thermal.h>
#include "logohacker.h"

#include "SoftwareSerial.h"
#define TX_PIN 2 // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN 3 // Arduino receive   GREEN WIRE   labeled TX on printer
#define BT_PIN 8 // press button to print

int pressed = 0;

#define NFRASES 3
char* frases[] = {
"cada vereador pode contratar até 30 funcionários para seu gabinete?",
"cada gabinete dispõe de 130 mil reais mensais para pagamento de salários de funcionários?",
"cada gabinete pode gastar até 239 mil reais por ano com aluguel de veículos, assinaturas de jornais, serviços gráficos e material de escritório?"
};

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);     // Pass addr to printer constructor

// ****** UTF8-Decoder: convert UTF8-string to extended ASCII *******
static byte c1;  // Last character buffer

// Convert a single Character from UTF8 to Extended ASCII
// Return "0" if a byte has to be ignored
byte utf8ascii(byte ascii) {
  if ( ascii<128 ) {  // Standard ASCII-set 0..0x7F handling  
    c1=0;
    return( ascii );
  }

  // get previous input
  byte last = c1;   // get last char
  c1=ascii;         // remember actual character

  switch (last) {   // conversion depending on first UTF8-character
    case 0xC2: return  (ascii);  break;
    case 0xC3: return  (ascii | 0xC0);  break;
    case 0x82: if(ascii==0xAC) return(0x80);       // special case Euro-symbol
  }

  return  (0);                                     // otherwise: return zero, if character has to be ignored
}

// convert String object from UTF8 String to Extended ASCII
String utf8ascii(String s) {      
  String r="";
  char c;
  for (int i=0; i<s.length(); i++) {
    c = utf8ascii(s.charAt(i));
    if (c!=0) r+=c;
  }
  return r;
}

// In Place conversion UTF8-string to Extended ASCII (ASCII is shorter!)
void utf8ascii(char* s) {      
  int k=0;
  char c;
  for (int i=0; i<strlen(s); i++) {
    c = utf8ascii(s[i]);
    if (c!=0)
      s[k++]=c;
  }
  s[k]=0;
}

void setup() {
  for(byte i = 0; i < NFRASES; i++)
    utf8ascii(frases[i]);
  
  randomSeed(analogRead(0));
  byte devnull = random(256);
  devnull = random(256);
  devnull = random(256);
  
  mySerial.begin(19200);
  printer.begin();
  pinMode(BT_PIN, INPUT_PULLUP);
  rockandroll();
}

void loop() {
  pressed = digitalRead(BT_PIN);
  if (!pressed) {
    rockandroll();
    delay(500);
  }
}

void rockandroll() {
  printer.setCodePage(CODEPAGE_WCP1252);
  printer.justify('C');
  printer.printBitmap(logohacker_width, logohacker_height, logohacker_data);
  printer.feed(1);
  printer.println(F("== Pol\xEDtica Hacker =="));
  printer.println(F("Vote Pedro Markun"));
  printer.setSize('L');      // setting the size adds a linefeed
  printer.println(F("18007"));
  printer.setSize('S');      // setting the size adds a linefeed
  printer.println(F("fb.com/politicahacker"));
  printer.println(F("(11) 994837732"));
  printer.println(F("CNPJ: 25.864.934/0001-00"));
  printer.feed(1);
  printer.println(F("==== bit pol\xEDtico ===="));
  printer.print(F("Voc\xEA sabia que "));
  printer.println(frases[random(NFRASES)]);
  printer.println(F("==== bit pol\xEDtico ===="));
  printer.feed(3);
}


