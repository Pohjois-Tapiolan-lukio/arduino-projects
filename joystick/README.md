 Joystickin käyttö Arduinon kanssa
Tässä esimerkissä tehdään yksinkertainen ohjelma, joka lukee ohjaussauvan arvoja
ja tulostaa niitä sarjaliikennemonitoriin.

## Tarvikkeet
* Arduino
* USB-kaapeli
* Joystick
* Hyppykaapeleita (5kpl)

## Toimintaperiaate
Ohjaussauva kääntää kahta potentiometriä, jotka on asetettu kohtisuoraan
toisiinsa nähden. Potentiometreistä toinen seuraa sauvan sijaintia
vaaka-akselilla ja toinen pystyakselilla. Yhdistämällä nämä kaksi tietoa
voidaan esittää ohjaussauvan sijainti x,y-koordinaatistossa. Näiden lisäksi
joystickissä on painike, joka aktivoidaan painamalla ohjaussauvaa.

## Kytkentä
1. Liitetään joystickin pinnit Arduinoon hyppykaapeleilla seuraavasti:

|Arduino   |Joystick   |
|----------|-----------|
|GND       |GND        |
|2         |SEL        |
|A0        |HORZ       |
|A1        |VERT       |
|5V        |VCC        |

2. Kytketään Arduino USB-kaapelilla tietokoneeseen.
## Ohjelma
```c++
short x; // Joystickin x- ja y-koordinaatit
short y;
bool c; // Kertoo, onko joystickin nappi painettu

void setup() {
  pinMode(2, INPUT); // Asetetaan pinni 2 lukutilaan
  Serial.begin(9600); // Käynnistetään sarjaliikennne tietokoneen kanssa
}

void loop() {
  // Luetaan joystickin arvot ja sijoitetaan ne muuttujiin
  c = digitalRead(2);
  x = analogRead(A0);
  y = analogRead(A1);

  // Lähetetään luetut arvot tietokoneelle sarjaliikenteellä
  Serial.println(String(c) + ", X=" + String(x) + ", Y=" + String(y));
  delay(100); // Odotetaan 100ms
}
```

## Tuloste
Avaa sarjaliikennemonitori painamalla `ctrl`/`cmd` + `shift` + `M`
Ohjelma tulostaa luetut arvot 100ms välein.
Ohjaussauvaa pyörittäessä voivat arvot näyttää esimerkiksi seuraavanlaiselta:
```
1, X=1023, Y=508
1, X=1022, Y=0
1, X=529, Y=0
1, X=528, Y=0
1, X=0, Y=504
1, X=0, Y=858
1, X=150, Y=1022
1, X=1021, Y=1022
1, X=1023, Y=501
1, X=1022, Y=0
1, X=527, Y=0
```
Tulosteen ensimmäinen arvo kertoo, painetaanko joystickin painiketta.
Tämä on totuusarvo, eli sen arvo on joko 0 tai 1. Tämän jälkeiset arvot
ilmaisevat joysickin sijaintia x- ja y-akseleilla. Sijainti esitetään
lukuarvoilla 0-1023, joten joystickin ollessa keskiasennossa saa molemmat
muuttujat laitteen kalibroinnista ja laadusta riippuen suunnilleen arvon 512.
