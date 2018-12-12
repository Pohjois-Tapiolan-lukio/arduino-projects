int liipaisin=7;
int kaiku=6; //
long kesto=0;
long etaisyys=0;

void setup()
{
Serial.begin (9600);
pinMode(liipaisin, OUTPUT); // liipaisin-Pin
pinMode(kaiku, INPUT); // kaiku-Pin
}

void loop()
{
digitalWrite(liipaisin, LOW);
delay(5); //kesto: 5
digitalWrite(liipaisin, HIGH);
delay(10);
digitalWrite(liipaisin, LOW);
kesto = pulseIn(kaiku, HIGH);
etaisyys = (kesto/2) * 0.03432;
if (etaisyys >= 500 || etaisyys <= 0)
{
Serial.println("Ei mittausarvoa..");
}
else
{
Serial.print(etaisyys);
Serial.println(" cm");
}
delay(1000); 
}
