#include <TVout.h>

TVout TV;
unsigned char x,y;

void setup()  {
  x=0;
  y=0;
  TV.start_render(_PAL);	//for devices with only 1k sram(m168) use TV.begin(_PAL,128,56)
}

void loop() {
  TV.clear_screen();
  x=0;
  y=0;
  for (char i = 32; i < 127; i++) {
    TV.print_char(x*6,y*8,i);
    x++;
    if (x > TV.char_line()) {
      y++;
      x=0;
    }
  }
  TV.delay_frame(50);
  TV.clear_screen();
  TV.print_str(0,0,"fill screen pixel");
  TV.print_str(0,8,"by pixel");
  TV.delay_frame(50);
  TV.clear_screen();
  for(x=0;x<TV.horz_res();x++){
    for(y=0;y<TV.vert_res();y++){
      TV.set_pixel(x,y,1);
    }
  }
  TV.delay_frame(50);
  TV.clear_screen();
  TV.print_str(0,0,"draw some lines");
  TV.delay_frame(50);
  for(y=0;y<TV.vert_res();y++){
    delay(10);
    TV.draw_line(0,y,x-y,y,2);
  }
  TV.delay_frame(50);
}
