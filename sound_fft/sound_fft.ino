#include "arduinoFFT.h"

arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
/*
These values can be changed in order to evaluate the functions
*/
#define CHANNEL A1
const uint16_t samples = 1024; //This value MUST ALWAYS be a power of 2
const double samplingFrequency = 2000; //Hz, must be less than 10000 due to ADC
// This value divided by 2 is the maximum frequency we can analyze

unsigned int sampling_period_us;
unsigned long microseconds;
double vReal[samples];
double vImag[samples];

void setup()
{
  sampling_period_us = round(1000000*(1.0/samplingFrequency));
  Serial.begin(115200);
}

void loop()
{
  /*SAMPLING*/
  microseconds = micros();
  for(int i=0; i<samples; i++)
  {
      vReal[i] = analogRead(CHANNEL);
      vImag[i] = 0;
      while(micros() - microseconds < sampling_period_us){
        //empty loop
      }
      microseconds += sampling_period_us;
  }

  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
  // PrintVector(vReal, vImag, samples >> 1);

  double x = FFT.MajorPeak(vReal, samples, samplingFrequency);
  Serial.println(x, 1); //Print out what frequency is the most dominant.
}

void PrintVector(double *ReData, double *ImData, uint16_t bufferSize)
{
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    double abscissa = ((i * 1.0 * samplingFrequency) / samples);
    Serial.print(abscissa, 6);
    Serial.print("Hz \t");
    Serial.println(ReData[i]*ReData[i] + ImData[i]*ImData[i], 4);
  }
  Serial.println("---");
}
