#include <TVout.h>
#include <TVoutfonts/fontALL.h>
#include "pollserial.h"

TVout TV;
pollserial pserial;

void setup()  {
  TV.begin(NTSC,184,72);
  TV.select_font(font6x8);
  TV.println("Serial Terminal");
  TV.println("-- Version 0.1 --");
  TV.set_hbi_hook(pserial.begin(57600));
}

void loop() {
  if (pserial.available()) {
    TV.print((char)pserial.read());
  }
}
