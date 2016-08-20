// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\

// https://forum.arduino.cc/index.php?topic=38107.msg282342#msg282342
void PrintHex8(uint8_t *data, uint8_t length) {
   char tmp[length*3+1];
   memset(tmp, 0x20, length * 3);
   byte first;
   byte second;
   for (int i=0; i<length; i++) {
     first = (data[i] >> 4) & 0x0f;
     second = data[i] & 0x0f;
     // base for converting single digit numbers to ASCII is 48
     // base for 10-16 to become lower-case characters a-f is 87
     // note: difference is 39
     tmp[i*3] = first+48;
     tmp[i*3+1] = second+48;
     if (first > 9) tmp[i*3] += 39;
     if (second > 9) tmp[i*3+1] += 39;
   }
   tmp[length*3] = 0;
   Serial.println(tmp);
}

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\

void setup() {
  // Debugging out.
  Serial.begin(74880);
  // In from host processor.
  Serial1.begin(74880);  

  while (!Serial) { }

  Serial.println("Device started!");
}


byte buf[16];

void loop() {
  if (Serial1.available()) {
    /*
    Serial1.readBytes(buf, 2);
    if (buf[0] != 0xD0 || buf[1] != 0x0D) {
      while (Serial1.available()) Serial1.read();
    }
    */

    byte len = Serial1.read();
    Serial.print("Reading bytes:");
    Serial.println(len, DEC);

    if (len) {
      Serial1.readBytes(buf, len);
      PrintHex8(buf, len);
    }
  }
}

