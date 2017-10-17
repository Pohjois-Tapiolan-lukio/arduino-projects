/***********************
 * Tweakable variables *
 ***********************/

/*
LedStrip variables:
*/

#define  STRIP_PIN D4    //Strip guidance.

#define NUM_LEDS 5     //Choose according to your strip.

#define BRIGHTNESS 250  // Values between 0 ... 255.

#define DEALAY_VAL 10   // Delay for leds.


#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>



#include "Adafruit_NeoPixel.h"


/*
Blynk
*/
char ssid[] = "your wifi";
char pass[] = "wifi passwd";
//Blynk token
char auth[] = "your Blynk secret key";

int delayTime = 1;
int charBreak = 2.1;

int red =255;
int green =0;
int blue =0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, STRIP_PIN, NEO_GRB + NEO_KHZ800);

WidgetTerminal teminal(V2);

BLYNK_WRITE(V2){

String str = param.asStr();
// Length (with one extra character for the null terminator)
int str_len =str.length()+1;
// Prepare the character array (the buffer)
char char_array[str_len];
// copy string to char
str.toCharArray(char_array,str_len);
// send to POV
displayString(char_array);

//terminal.flush();
  }

BLYNK_WRITE(V1) {

 red = param[0].asInt();
 green = param[1].asInt();
 blue = param[2].asInt();
/*
red = 255;
green = 0;
blue = 0;

  */
}

int a[] = {1, 6, 26, 6, 1};
int b[] = {31, 21, 21, 10, 0};
int c2[] = {14, 17, 17, 10, 0};
int d[] = {31, 17, 17, 14, 0};
int e[] = {31, 21, 21, 17, 0};
int f[] = {31, 20, 20, 16, 0};
int g[] = {14, 17, 19, 10, 0};
int h[] = {31, 4, 4, 4, 31};
int i[] = {0, 17, 31, 17, 0};
int j[] = {0, 17, 30, 16, 0};
int k[] = {31, 4, 10, 17, 0};
int l[] = {31, 1, 1, 1, 0};
int m[] = {31, 12, 3, 12, 31};
int n[] = {31, 12, 3, 31, 0};
int o[] = {14, 17, 17, 14, 0};
int p[] = {31, 20, 20, 8, 0};
int q[] = {14, 17, 19, 14, 2};
int r[] = {31, 20, 22, 9, 0};
int s[] = {8, 21, 21, 2, 0};
int t[] = {16, 16, 31, 16, 16};
int u[] = {30, 1, 1, 30, 0};
int v[] = {24, 6, 1, 6, 24};
int w[] = {28, 3, 12, 3, 28};
int x[] = {17, 10, 4, 10, 17};
int y[] = {17, 10, 4, 8, 16};
int z[] = {19, 21, 21, 25, 0};

int eos[] = {0, 1, 0, 0, 0};
int excl[] = {0, 29, 0, 0, 0};
int ques[] = {8, 19, 20, 8, 0};

void setup(){

Serial.begin(9600);
strip.begin();
displayString("Hello World");
Blynk.begin(auth, ssid, pass);

}


void displayLine(int line){

int myline;
myline = line;
if (myline>=16) {strip.setPixelColor(0, strip.Color(red,green,blue)); myline-=16;} else {strip.setPixelColor(0, strip.Color(0,0,0));}
if (myline>=8)  {strip.setPixelColor(1, strip.Color(red,green,blue)); myline-=8;}  else {strip.setPixelColor(1, strip.Color(0,0,0));}
if (myline>=4)  {strip.setPixelColor(2, strip.Color(red,green,blue)); myline-=4;}  else {strip.setPixelColor(2, strip.Color(0,0,0));}
if (myline>=2)  {strip.setPixelColor(3, strip.Color(red,green,blue)); myline-=2;}  else {strip.setPixelColor(3, strip.Color(0,0,0));}
if (myline>=1)  {strip.setPixelColor(4, strip.Color(red,green,blue)); myline-=1;}  else {strip.setPixelColor(4, strip.Color(0,0,0));}
strip.show();
}

void displayChar(char c){
if (c == 'a'){for (int i = 0; i <5; i++){displayLine(a[i]);delay(delayTime);}displayLine(0);}
if (c == 'b'){for (int i = 0; i <5; i++){displayLine(b[i]);delay(delayTime);}displayLine(0);}
if (c == 'c2'){for (int i = 0; i <5; i++){displayLine(c2[i]);delay(delayTime);}displayLine(0);}
if (c == 'd'){for (int i = 0; i <5; i++){displayLine(d[i]);delay(delayTime);}displayLine(0);}
if (c == 'e'){for (int i = 0; i <5; i++){displayLine(e[i]);delay(delayTime);}displayLine(0);}
if (c == 'f'){for (int i = 0; i <5; i++){displayLine(f[i]);delay(delayTime);}displayLine(0);}
if (c == 'g'){for (int i = 0; i <5; i++){displayLine(g[i]);delay(delayTime);}displayLine(0);}
if (c == 'h'){for (int i = 0; i <5; i++){displayLine(h[i]);delay(delayTime);}displayLine(0);}
if (c == 'i'){for (int it = 0; it <5; it++){displayLine(i[it]);delay(delayTime);}displayLine(0);}
if (c == 'j'){for (int i = 0; i <5; i++){displayLine(j[i]);delay(delayTime);}displayLine(0);}
if (c == 'k'){for (int i = 0; i <5; i++){displayLine(k[i]);delay(delayTime);}displayLine(0);}
if (c == 'l'){for (int i = 0; i <5; i++){displayLine(l[i]);delay(delayTime);}displayLine(0);}
if (c == 'm'){for (int i = 0; i <5; i++){displayLine(m[i]);delay(delayTime);}displayLine(0);}
if (c == 'n'){for (int i = 0; i <5; i++){displayLine(n[i]);delay(delayTime);}displayLine(0);}
if (c == 'o'){for (int i = 0; i <5; i++){displayLine(o[i]);delay(delayTime);}displayLine(0);}
if (c == 'p'){for (int i = 0; i <5; i++){displayLine(p[i]);delay(delayTime);}displayLine(0);}
if (c == 'q'){for (int i = 0; i <5; i++){displayLine(q[i]);delay(delayTime);}displayLine(0);}
if (c == 'r'){for (int i = 0; i <5; i++){displayLine(r[i]);delay(delayTime);}displayLine(0);}
if (c == 's'){for (int i = 0; i <5; i++){displayLine(s[i]);delay(delayTime);}displayLine(0);}
if (c == 't'){for (int i = 0; i <5; i++){displayLine(t[i]);delay(delayTime);}displayLine(0);}
if (c == 'u'){for (int i = 0; i <5; i++){displayLine(u[i]);delay(delayTime);}displayLine(0);}
if (c == 'v'){for (int i = 0; i <5; i++){displayLine(v[i]);delay(delayTime);}displayLine(0);}
if (c == 'w'){for (int i = 0; i <5; i++){displayLine(w[i]);delay(delayTime);}displayLine(0);}
if (c == 'x'){for (int i = 0; i <5; i++){displayLine(x[i]);delay(delayTime);}displayLine(0);}
if (c == 'y'){for (int i = 0; i <5; i++){displayLine(y[i]);delay(delayTime);}displayLine(0);}
if (c == 'z'){for (int i = 0; i <5; i++){displayLine(z[i]);delay(delayTime);}displayLine(0);}
if (c == '!'){for (int i = 0; i <5; i++){displayLine(excl[i]);delay(delayTime);}displayLine(0);}
if (c == '?'){for (int i = 0; i <5; i++){displayLine(ques[i]);delay(delayTime);}displayLine(0);}
if (c == '.'){for (int i = 0; i <5; i++){displayLine(eos[i]);delay(delayTime);}displayLine(0);}
delay(charBreak);
}

void displayString(char* s){
for (int i = 0; i<=strlen(s); i++){
displayChar(s[i]);
}
}

void loop(){
//displayString("Your Text");
Blynk.run();
}
