#!/bin/python

import sys
import zipfile
import os

def print_help():
    print("GeoGebra Printer utility script for .txt to .ggb conversion.\n")
    print("See README.md for more information about how to create the .txt file.\n")
    print("USAGE:\n  python makeggb.py [<serial.txt> <output.ggb>]");

def convert():
    # Declare the filenames (defaults if none were given)
    txt_file = "serial.txt"
    if len(sys.argv) >= 2:
        txt_file = sys.argv[1]
    ggb_file = "output.ggb"
    if len(sys.argv) >= 3:
        ggb_file = sys.argv[2]
    xml_file = "geogebra.xml"

    # Inform the user of the status of the application
    print("Converting " + txt_file + " to " + ggb_file + "...")

    # Write the start of the ggb file
    ggb_xml = """<?xml version="1.0" encoding="utf-8"?>"""
    ggb_xml += """<geogebra format="5.0" version="5.0.390.0" app="classic" platform="d" id="a0d10cb1-ae9f-41c3-adcc-574dd33863ba"  xsi:noNamespaceSchemaLocation="http://www.geogebra.org/ggb.xsd" xmlns="" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" ><construction title="" author="" date="">"""

    # Write the <element> declarations from the given file
    with open(txt_file, "r") as file:
        index = 0
        for line in file.readlines():
            line_ = line.split(" ");
            ggb_xml += "<element type=\"point\" label=\"DataPoint" + str(index) + "\"><coords x=\"" + line_[0] + "\" y=\"" + line_[1] + "\" z=\"1.0\"/><show object=\"true\" label=\"false\"/><objColor r=\"77\" g=\"77\" b=\"255\" alpha=\"0.0\"/><layer val=\"0\"/><labelMode val=\"0\"/><animation step=\"1\" speed=\"1\" type=\"1\" playing=\"false\"/><isShape val=\"false\"/><pointSize val=\"5\"/><pointStyle val=\"0\"/></element>"
            index += 1

    # Write the closing tags for the xml
    ggb_xml += "</construction></geogebra>"

    # Write the xml to a temporary file
    with open(xml_file, "w") as file:
        file.write(ggb_xml)

    # Zip up the file
    zip = zipfile.ZipFile(ggb_file, "w")
    zip.write(xml_file)
    zip.close()

    # Remove the temporary xml file
    os.remove(xml_file)

    # Inform the user that we're done and no errors happened
    print("Done!")

# Print help if it's asked for, otherwise do the .txt -> .ggb conversion
if len(sys.argv) == 2 and ("help" in sys.argv[1] != -1 or sys.argv[1].equals("-h")):
    print_help();
else:
    convert();
