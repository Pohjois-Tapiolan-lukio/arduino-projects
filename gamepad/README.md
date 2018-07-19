# Gamepad
Raspberry Pi-osuuden löydät [täältä]()!

## Yhteenveto
Tämä on Arduino-peliohjaimen Arduino-osuus ja dokumentaatio sen
rakentamiselle.

Tämän projektin tarkoitus on opettaa yksinkertaisen sarjaliikenteen perusteet.

Tässä kansiossa on vain ohjeet ja valmiit arduino-koodit.
Tarvitset myös [sarjaliikenteen kuuntelijan]().

## Tarvittavat osat
- Arduino & USB-johto
- Kytkentäalusta
- Jotain seuraavista:
    - Nappeja
    - Joystickeja
- 1KΩ – 1MΩ vastuksia
- Hyppylankoja

## Ohjeet
> **Huomaa**: ohjeet eivät ole tarkasti tiettyä konfiguraatiota varten,
> vaan ohjeet pääosin selittävät miten osia yhdistetään
> Hyödynnä ohjeita osiesi ja osaamisesi mukaan

### Näin kytket ja ohjelmoit napin
![](https://github.com/Pohjois-Tapiolan-lukio/arduino-projects/tree/master/gamepad/nappeja.jpg)
> Digitaalisia kaksiasentoisia nappeja

![](https://www.shallowsky.com/arduino/class/buttonlegsdiag.jpg)
> 4-jalkainen nappi
- kun painat napista, niin kaikki napin muodostavat yhteisen kytkennän

![](https://github.com/Pohjois-Tapiolan-lukio/arduino-projects/tree/master/gamepad/4jalkapiiri.png)
> 4-jalkaisen napin kytkentäkaavio
- nappi kytketään alustaan keskelle

- 2-jalkaisten nappien jalat muodostavat kytkennän napin painalluksesta.

Napeille on oleellista olla *alasvetovastus*, jotta napin mittauskannan
(jossa kaaviossa on vaaleansininen johto) potentiaali on aina joko 5V (päällä)
tai 0V (pois päältä). Jos vastus on liian pieni tai vastuksen tilalla on johto,
niin napin painallus aiheuttaa oikosulun Arduinon virtalähteeseen.

> Tällainen oikosulku ei vaurioita itse Arduinoa

##### Arduino
4-jalkaiset ja 2-jalkaiset napit käyttäytyvät identtisesti koodin näkökulmasta

Määritellään napin mittauskannan pinni
```cpp
#define NAPPI1 [napin pinnin numero]
```

> Rivi ei ollut arduino-koodia vaan macro-määritelmä, sen takia rivin loppuun
> ei tule `;`-merkkiä

Määritellään napin pinnin numero Arduinon `setup`-*rutiinissa* syötteeksi
```cpp
pinMode(NAPPI1, INPUT);
```

Syötettä voidaan hakea jatkuvasti `loop`-*rutiinin* sisällä
```cpp
digitalRead(NAPPI1);
```

> Arduino-koodissa määriteltiin kokonaisluku-muuttuja avainsanalla `int`

``` cpp
int arvo1 = digitalRead(NAPPI1);
```

### Näin kytket joystickin
![](https://github.com/Pohjois-Tapiolan-lukio/arduino-projects/tree/master/gamepad/joystick.png)
> Joystick, jossa on kaksi potentiometriä ja yksi nappi

## Esimerkki
