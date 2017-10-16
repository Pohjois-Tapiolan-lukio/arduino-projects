# GeoGebra Printer
Want to get some sensor data to the GeoGebra (.ggb) format? Follow these 6 easy steps!
1. Copy the `geogebra_printer.ino` to your project folder.
2. Make sure you've enabled Serial printing. (Eg. by calling `Serial.begin(9600)` in the `void setup()`)
3. Use the `printGeoGebraPoint(float x, float y)` function to record a sample. (Eg. `printGeoGebraPoint(millis() / 1000.0, 1.0)` would output a data point with the x value of the current time (in seconds) and the y value of 1.0)
4. Run your Arduino for a while, gather the samples.
5. Copy the Serial Monitor contents to a text file, eg. `serial.txt`.
6. Run the `makeggb.py` Python script with the name of the text file and the name of the output .ggb file as arguments. (Eg. `python makeggb.py serial.txt output.ggb`)
   <small>Note: You could just name your file `serial.txt` and run the script without any arguments, the defaults are `serial.txt` and `output.ggb`.</small>
7. Done! You can open the `output.ggb` in GeoGebra and do whatever analysis you'd like.
