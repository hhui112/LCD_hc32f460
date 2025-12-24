#ifndef GC9A01_DRV_H_
#define GC9A01_DRV_H_

#include "stdint.h"
#include "lcd_font.h"
#include "lcd_pic.h"


typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

void Add_set(unsigned char x_start,unsigned char y_start,unsigned char x_end,unsigned char y_end);
void TFT_clear(void);
void TFT_full(unsigned int color);
void TFT_dispoint(unsigned char x,unsigned char y,unsigned int color);
void Picture_display(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const unsigned char *ptr_pic);
void Logo_display(const unsigned char *ptr_pic,unsigned int Bcolor,unsigned int Ccolor);
void Lcd_init(void);
void SPI_Config(void);
void TFT_full_dma(uint16_t color);

void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);//指定区域填充颜色
void LCD_DrawPoint(u16 x,u16 y,u16 color);//在指定位置画一个点

void LCD_ShowChinese(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示汉字串
void LCD_ShowChinese12x12(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个12x12汉字
void LCD_ShowChinese16x16(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个16x16汉字
void LCD_ShowChinese24x24(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个24x24汉字
void LCD_ShowChinese32x32(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个32x32汉字
void LCD_ShowChinese40x40(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);
void LCD_ShowChinese48x48(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);


void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode);//显示一个字符
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode);//显示字符串
u32 mypow(u8 m,u8 n);//求幂
void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey);//显示整数变量
void LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey);//显示两位小数变量

void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[]);//显示图片
void LCD_ShowPicture_DMA(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t *pic);
void bg_display(uint16_t x,uint16_t y,uint16_t length,uint16_t width,unsigned int color);
void Set_BLK_PORT(uint8_t blk);
#endif 
