/* Copyright 2017 Jens Pitkanen
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

int geoGebraPointCount = 0;

void printGeoGebraPoint(float x, float y) {
  Serial.print("<element type=\"point\" label=\"DataPoint");
  Serial.print(geoGebraPointCount++);
  Serial.print("\"><coords x=\"");
  Serial.print(x);
  Serial.print("\" y=\"");
  Serial.print(y);
  Serial.println("\" z=\"1.0\"/><show object=\"true\" label=\"false\"/><objColor r=\"77\" g=\"77\" b=\"255\" alpha=\"0.0\"/><layer val=\"0\"/><labelMode val=\"0\"/><animation step=\"1\" speed=\"1\" type=\"1\" playing=\"false\"/><isShape val=\"false\"/><pointSize val=\"5\"/><pointStyle val=\"0\"/></element>");
}
