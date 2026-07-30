#ifndef OLED_H_
#define OLED_H_

#define OLED_ADDRESS    0x3C
#define X_WIDTH         128
#define Y_WIDTH_        8

#define OLED_CTRL_SH1106   0
#define OLED_CTRL_ST7315   1
#define OLED_CONTROLLER    OLED_CTRL_SH1106

#if   (OLED_CONTROLLER == OLED_CTRL_SH1106)
  #define OLED_COL_OFFSET   2
#elif (OLED_CONTROLLER == OLED_CTRL_ST7315)
  #define OLED_COL_OFFSET   0
#else
  #error "unknown OLED_CONTROLLER"
#endif

void oled_cmd(unsigned char IIC_Command);
void oled_dat(unsigned char IIC_Data);
void oled_set_pos(unsigned char x, unsigned char y);
void oled_init_dram(char a);
void oled_set_lum(unsigned char a);
void oled_init(void);

#endif
