#!/bin/python

# Copyright 2017 Jens Pitkanen
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
# REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
# AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
# INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
# LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
# OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.

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
        ggb_xml += file.read()

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
