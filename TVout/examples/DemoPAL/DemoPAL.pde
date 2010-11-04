#include <TVout.h>
#include <fontALL.h>

TVout TV;
unsigned char x,y;

void setup()  {
  x=0;
  y=0;
  TV.begin(PAL);	//for devices with only 1k sram(m168) use TV.begin(_NTSC,128,56)
  TV.select_font(font6x8);
}

void loop() {
  TV.clear_screen();
  x=0;
  y=0;
  for (char i = 32; i < 127; i++) {
    TV.print_char(x*6,y*8,i);
    x++;
    if (x >= TV.char_line()) {
      y++;
      x=0;
    }
  }
  TV.delay(1000);
  TV.clear_screen();
  TV.println("Fill the Screen\nPixel by Pixel");
  TV.delay(1000);
  TV.clear_screen();
  for(x=0;x<TV.hres();x++){
    for(y=0;y<TV.vres();y++){
      TV.set_pixel(x,y,1);
    }
  }
  TV.delay(1000);
  TV.clear_screen();
  TV.print("Draw some lines");
  TV.delay(1000);
  x = TV.hres() - 1;
  for(y=0;y<TV.vres();y++){
    TV.draw_line(0,y,x-y,y,2);
  }
  TV.delay(1000);
}