# Feather M0 käyttöönotto
Tässä ohjeessa käyttöönotetaan Adafruit Feather M0 toimimaan
Arduino IDEn kanssa.

1. [Lataa ja asenna Arduino IDE](https://www.arduino.cc/en/Main/Software)
2. Avaa Arduino > Preferences ja syötä Additional Boards Manager URL -kenttään
   `https://adafruit.github.io/arduino-board-index/package_adafruit_index.json`
3. Avaa valikosta Tools > Board > Boards Manager
4. Etsi `Adafruit SAMD Boards` ja asenna Arduino SAMD Boards
   ja Adafruit SAMD Boards
5. Avaa Arduino IDE uudelleen
6. Valitse oikea board valikosta Tools > Board > Adafruit Feather M0
7. Kytke Feather M0 tietokoneeseen USB-kaapelilla

Laitteen pitäisi ilmestyä valikkoon Tools > Port. Jos näin ei tapahdu,
tarvitset mahdollisesti Windows-käyttöjärjestelmällä ajurin.
Lataa ajuri [täältä](https://github.com/adafruit/Adafruit_Windows_Drivers/releases/latest) ja yritä uudelleen.
