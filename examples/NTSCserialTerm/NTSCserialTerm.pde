#include <TVout.h>
#include <pollserial.h>

TVout TV;
pollserial pserial;

void setup()  {
  TV.begin(_NTSC,184,72);
  TV.select_font(_8X8);
  TV.println("Serial Terminal");
  TV.println("-- Version 0.1 --");
  pserial.begin(57600);
}

void loop() {
  if (pserial.available()) {
    TV.print((char)pserial.read());
  }
}
