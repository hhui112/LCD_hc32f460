#include "main.h"
#include "bsp_spi.h"
#include "GC9a01_drv.h"



#define TFT_COLUMN_NUMBER 240
#define TFT_LINE_NUMBER 240

/* LCD_DC = PB6*/
#define LCD_DC_PORT                    	 	(GPIO_PORT_B)
#define LCD_DC_PIN                      	(GPIO_PIN_06)
//#define LCD_DC_FUNC                   	(GPIO_FUNC_42)

/* LCD_BLK = PA15*/
#define LCD_BLK_PORT                     	(GPIO_PORT_B)
#define LCD_BLK_PIN                      	(GPIO_PIN_05)
//#define LCD_BLK_FUNC                   	(GPIO_FUNC_42)


#define LCD_BLK_PORT_0()      (GPIO_ResetPins(LCD_BLK_PORT, LCD_BLK_PIN))
#define LCD_BLK_PORT_1()      (GPIO_SetPins(LCD_BLK_PORT, LCD_BLK_PIN))

#define LCD_DC_0()      			(GPIO_ResetPins(LCD_DC_PORT, LCD_DC_PIN))
#define LCD_DC_1()      			(GPIO_SetPins(LCD_DC_PORT, LCD_DC_PIN))


static void delay_ms(unsigned int _ms_time)
{
	SysTick_Delay(_ms_time);
}
 

void SPI_SendByte(uint8_t byte)
{
	LCD_CS_0();
	SPI_WriteData(SPI_UNIT, byte);
	LCD_CS_1();
}

void TFT_send_cmd(unsigned char o_command)
{
	LCD_DC_0();
	SPI_SendByte(o_command);
	LCD_DC_1();
}
	
void TFT_send_data(unsigned char o_data)
{ 
	SPI_SendByte(o_data);	  
}

void TFT_init(void)	//GC9A01
  {
		// SPI_RST_0;//??? 
	  //delay_ms(100); 
	  //SPI_RST_1; 
		LCD_CS_0();
	  delay_ms(100); 
	  // LCD_BLK_PORT_1();
		
	  TFT_send_cmd(0xEF); 		
	  TFT_send_cmd(0xEB); 		
	  TFT_send_data(0x14);
	  
	  TFT_send_cmd(0xFE); 		
	  TFT_send_cmd(0xEF); 	
	  
	  TFT_send_cmd(0xEB); 
	  TFT_send_data(0x14); 
	  
	  TFT_send_cmd(0x84); 
	  TFT_send_data(0x40); 
	  TFT_send_cmd(0x85); 
	  TFT_send_data(0xFF); 
	  TFT_send_cmd(0x86); 
	  TFT_send_data(0xFF); 
	  TFT_send_cmd(0x87); 
	  TFT_send_data(0xFF); 
	  TFT_send_cmd(0x88); 
	  TFT_send_data(0x0A); 
	  TFT_send_cmd(0x89); 
	  TFT_send_data(0x21);
	  
	  TFT_send_cmd(0x8A); 
	  TFT_send_data(0x00);
	  
	  TFT_send_cmd(0x8B); 
	  TFT_send_data(0x80);
	  
	  TFT_send_cmd(0x8C); 
	  TFT_send_data(0x01);
	  
	  TFT_send_cmd(0x8D); 
	  TFT_send_data(0x01);
	  
	  TFT_send_cmd(0x8E); 
	  TFT_send_data(0xFF);
	  
	  TFT_send_cmd(0x8F); 
	  TFT_send_data(0xFF);
	  
	  TFT_send_cmd(0xB6); 	//?????  ??????????
	  TFT_send_data(0x00); 		//????????S1-S360 G1-32
	  TFT_send_data(0x20); 		
	  
	  TFT_send_cmd(0x36); 	//??????? ?????? Memory Access Control
	  TFT_send_data(0xC8); 		//MY MX MV ML BGR MH X X
	  
	  TFT_send_cmd(0x3A); 	//??????? 	Pixel Format Set
	  TFT_send_data(0x05); 			//16bit MCU????g?
	  
	  TFT_send_cmd(0x90); 	//xxx
	  TFT_send_data(0x08); 
	  TFT_send_data(0x08); 
	  TFT_send_data(0x08); 
	  TFT_send_data(0x08);
	  
	  TFT_send_cmd(0xBD); 	//XXX
	  TFT_send_data(0x06);
	  
	  TFT_send_cmd(0xBC); 	//XXX
	  TFT_send_data(0x00); 
	  
	  TFT_send_cmd(0xFF); 	//XXX	
	  TFT_send_data(0x60); 
	  TFT_send_data(0x01); 
	  TFT_send_data(0x04);
	  
	  TFT_send_cmd(0xC3); 	//	Vreg1a voltage Control
	  TFT_send_data(0x13);
	  
	  TFT_send_cmd(0xC4); 	// Vreg1b voltage Control
	  TFT_send_data(0x13);
	  
	  TFT_send_cmd(0xC9); 	//Vreg2a voltage Control
	  TFT_send_data(0x22);
	  
	  TFT_send_cmd(0xBE); 	//XXX
	  TFT_send_data(0x11); 
	  
	  TFT_send_cmd(0xE1); 	//XXX
	  TFT_send_data(0x10);
	  TFT_send_data(0x0E); 
	  
	  TFT_send_cmd(0xDF); 	//XXX
	  TFT_send_data(0x21); 
	  TFT_send_data(0x0c); 
	  TFT_send_data(0x02); 
	  
	  TFT_send_cmd(0xF0); 	//GAMMA1
	  TFT_send_data(0x45); 
	  TFT_send_data(0x09); 
	  TFT_send_data(0x08); 
	  TFT_send_data(0x08); 
	  TFT_send_data(0x26); 
	  TFT_send_data(0x2A); 
	  
	  TFT_send_cmd(0xF1); 	//GAMMA2
	  TFT_send_data(0x43); 
	  TFT_send_data(0x70); 
	  TFT_send_data(0x72); 
	  TFT_send_data(0x36); 
	  TFT_send_data(0x37); 
	  TFT_send_data(0x6F); 
	  
	  TFT_send_cmd(0xF2); 	//GAMMA3
	  TFT_send_data(0x45); 
	  TFT_send_data(0x09); 
	  TFT_send_data(0x08); 
	  TFT_send_data(0x08); 
	  TFT_send_data(0x26); 
	  TFT_send_data(0x2A); 
	  
	  TFT_send_cmd(0xF3); 	////GAMMA4
	  TFT_send_data(0x43); 
	  TFT_send_data(0x70); 
	  TFT_send_data(0x72); 
	  TFT_send_data(0x36); 
	  TFT_send_data(0x37); 
	  TFT_send_data(0x6F);
	  
	  TFT_send_cmd(0xED); 	//XXX
	  TFT_send_data(0x1B); 
	  TFT_send_data(0x0B);
	  
	  TFT_send_cmd(0xAE); 	//XXX
	  TFT_send_data(0x77);
	  
	  TFT_send_cmd(0xCD); 	//XXX
	  TFT_send_data(0x63);
	  
	  TFT_send_cmd(0x70); 
	  TFT_send_data(0x07); 
	  TFT_send_data(0x07); 
	  TFT_send_data(0x04); 
	  TFT_send_data(0x0E); 
	  TFT_send_data(0x0F); 
	  TFT_send_data(0x09); 
	  TFT_send_data(0x07); 
	  TFT_send_data(0x08); 
	  TFT_send_data(0x03); 
	  
	  TFT_send_cmd(0xE8); 	//Frame Rate
	  TFT_send_data(0x34);
	  
	  TFT_send_cmd(0x62); 	//XXX
	  TFT_send_data(0x18); 
	  TFT_send_data(0x0D); 
	  TFT_send_data(0x71); 
	  TFT_send_data(0xED); 
	  TFT_send_data(0x70); 
	  TFT_send_data(0x70); 
	  TFT_send_data(0x18); 
	  TFT_send_data(0x0F); 
	  TFT_send_data(0x71); 
	  TFT_send_data(0xEF); 
	  TFT_send_data(0x70); 
	  TFT_send_data(0x70); 
	  
	  TFT_send_cmd(0x63); 	//XXX
	  TFT_send_data(0x18); 
	  TFT_send_data(0x11); 
	  TFT_send_data(0x71); 
	  TFT_send_data(0xF1); 
	  TFT_send_data(0x70); 
	  TFT_send_data(0x70); 
	  TFT_send_data(0x18); 
	  TFT_send_data(0x13); 
	  TFT_send_data(0x71); 
	  TFT_send_data(0xF3); 
	  TFT_send_data(0x70); 
	  TFT_send_data(0x70); 
	  
	  TFT_send_cmd(0x64); 	//XXX
	  TFT_send_data(0x28); 
	  TFT_send_data(0x29); 
	  TFT_send_data(0xF1); 
	  TFT_send_data(0x01); 
	  TFT_send_data(0xF1); 
	  TFT_send_data(0x00); 
	  TFT_send_data(0x07);
	  
	  TFT_send_cmd(0x66); 	//XXX
	  TFT_send_data(0x3C); 
	  TFT_send_data(0x00); 
	  TFT_send_data(0xCD); 
	  TFT_send_data(0x67); 
	  TFT_send_data(0x45); 
	  TFT_send_data(0x45); 
	  TFT_send_data(0x10); 
	  TFT_send_data(0x00); 
	  TFT_send_data(0x00); 
	  TFT_send_data(0x00); 
	  
	  TFT_send_cmd(0x67); 	//XXX
	  TFT_send_data(0x00); 
	  TFT_send_data(0x3C); 
	  TFT_send_data(0x00); 
	  TFT_send_data(0x00); 
	  TFT_send_data(0x00); 
	  TFT_send_data(0x01); 
	  TFT_send_data(0x54); 
	  TFT_send_data(0x10); 
	  TFT_send_data(0x32); 
	  TFT_send_data(0x98);
	  
	  TFT_send_cmd(0x74); 	//XXX
	  TFT_send_data(0x10); 
	  TFT_send_data(0x85); 
	  TFT_send_data(0x80); 
	  TFT_send_data(0x00); 
	  TFT_send_data(0x00); 
	  TFT_send_data(0x4E); 
	  TFT_send_data(0x00);
	  
	  TFT_send_cmd(0x98); 		//XXX
	  TFT_send_data(0x3e); 
	  TFT_send_data(0x07); 
	  
	  TFT_send_cmd(0x34); 	//Tearing Effect Line Off
	  TFT_send_cmd(0x21); 	//Display Inversion ON
	  TFT_send_cmd(0x11); 	//????
	  //TFT_send_cmd(0x38);	//Idle Mode OFF
	  delay_ms(120); 
	  TFT_send_cmd(0x29); 	//?????
	  delay_ms(20);
  }


void Add_set(unsigned char x_start,unsigned char y_start,unsigned char x_end,unsigned char y_end)
{
	TFT_send_cmd(0x2A);    //????????
	TFT_send_data(0);		//??????' 
	TFT_send_data(x_start);
	TFT_send_data(0);		//????????
	TFT_send_data(x_end);
	TFT_send_cmd(0x2B);    //????????
	TFT_send_data(0);		//??????' 
	TFT_send_data(y_start);
	TFT_send_data(0);		//????????
	TFT_send_data(y_end);
	TFT_send_cmd(0x2C);    //?????????????    	 
}

void TFT_clear(void)
{
	unsigned char Line,column;
	Add_set(0,0,TFT_COLUMN_NUMBER-1,TFT_LINE_NUMBER-1);
	for(Line=0;Line<TFT_LINE_NUMBER;Line++)             //Line loop
		{ 	  
				for(column=0;column<TFT_COLUMN_NUMBER;column++)	//column loop
					{
							TFT_send_data(0x00);
			TFT_send_data(0x00);
					}
		}
}

void TFT_full(unsigned int color)
  {
    unsigned char Line,column;
	Add_set(0,0,TFT_COLUMN_NUMBER-1,TFT_LINE_NUMBER-1);
    for(Line=0;Line<TFT_LINE_NUMBER;Line++)             //Line loop
      { 
		  
		for(column=0;column<TFT_COLUMN_NUMBER;column++)	//column loop
          {
              TFT_send_data(color>>8);
							TFT_send_data(color);
          }
      }
  }
	
void TFT_dispoint(unsigned char x,unsigned char y,unsigned int color)
{
	Add_set(x,y,x,y);
	TFT_send_data(color>>8);
	TFT_send_data(color);
	
}


void bg_display(uint16_t x,uint16_t y,uint16_t length,uint16_t width,unsigned int color)
{
		uint16_t i,j;
		Add_set(x,y,x+length-1,y+width-1);
		for(i=0;i<length;i++)
		{
			for(j=0;j<width;j++)
			{
				TFT_send_data(color>>8);
				TFT_send_data(color);
			}
		}
}


void Picture_display(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const unsigned char *ptr_pic)
{
	uint16_t i,j;
	Add_set(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			//TFT_send_data(*ptr_pic++);
			//TFT_send_data(*ptr_pic++);
			SPI_SendByte(*ptr_pic++);	 
			SPI_SendByte(*ptr_pic++);	
		}
	}
}

	
void Logo_display(const unsigned char *ptr_pic,unsigned int Bcolor,unsigned int Ccolor)
{
	uint16_t i,j,k;
	unsigned char x;
	Add_set(55,55,182,182);
	for(i=0;i<16;i++)
	{
		for(j=0;j<128;j++)
		{
			x = *ptr_pic++;
			for(k=0;k<8;k++)
			{
				if(x&0x80)
				{
					TFT_send_data(Bcolor>>8);
					TFT_send_data(Bcolor);
				}
				else 
				{
					TFT_send_data(Ccolor>>8);
					TFT_send_data(Ccolor);
				}
				x <<=1;
			}
		}
	}			
}





void LCD_Writ_Bus(u8 dat) 
{	
	SPI_SendByte(dat);
}

void LCD_WR_DATA8(u8 dat)
{
	SPI_SendByte(dat);
}

void LCD_WR_DATA(u16 dat)
{
	SPI_SendByte(dat>>8);
	SPI_SendByte(dat);
}

void LCD_WR_REG(u8 dat)
{
	LCD_DC_0();
	SPI_SendByte(dat);
	LCD_DC_1();
}

void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	LCD_WR_REG(0x2a);
	LCD_WR_DATA(x1);
	LCD_WR_DATA(x2);
	LCD_WR_REG(0x2b);
	LCD_WR_DATA(y1);
	LCD_WR_DATA(y2);
	LCD_WR_REG(0x2c);
}



void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_Address_Set(x,y,x,y);
	LCD_WR_DATA(color);
} 


void LCD_ShowChinese(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	while(*s!=0)
	{
		if(sizey==12) LCD_ShowChinese12x12(x,y,s,fc,bc,sizey,mode);
		else if(sizey==16) LCD_ShowChinese16x16(x,y,s,fc,bc,sizey,mode);
		else if(sizey==24) LCD_ShowChinese24x24(x,y,s,fc,bc,sizey,mode);
		else if(sizey==32) LCD_ShowChinese32x32(x,y,s,fc,bc,sizey,mode);
		else return;
		s+=2;
		x+=sizey;
	}
}


void LCD_ShowChinese12x12(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//汉字数目
	u16 TypefaceNum;//一个字符所占字节大小
	u16 x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	                         
	HZnum=HZnum_typFNT_GB12();	//统计汉字数目
	
	for(k=0;k<HZnum;k++) 
	{
		if((tfont12[k].Index[0]==*(s))&&(tfont12[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont12[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont12[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 



void LCD_ShowChinese16x16(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//汉字数目
	u16 TypefaceNum;//一个字符所占字节大小
	u16 x0=x;
  TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=HZnum_typFNT_GB16();	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont16[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont16[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 


void LCD_ShowChinese24x24(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//汉字数目
	u16 TypefaceNum;//一个字符所占字节大小
	u16 x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=HZnum_typFNT_GB24();	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont24[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont24[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 


void LCD_ShowChinese32x32(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j,m=0;
	u16 k;
	u16 HZnum;//汉字数目
	u16 TypefaceNum;//一个字符所占字节大小
	u16 x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=HZnum_typFNT_GB32();		//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont32[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont32[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
}

void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 temp,sizex,t,m=0;
	u16 i,TypefaceNum;//一个字符所占字节大小
	u16 x0=x;
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	num=num-' ';    //得到偏移后的值
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //设置光标位置 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==12)temp=ascii_1206[num][i];		       //调用6x12字体
		else if(sizey==16)temp=ascii_1608[num][i];		 //调用8x16字体
		else if(sizey==24)temp=ascii_2412[num][i];		 //调用12x24字体
		else if(sizey==32)temp=ascii_3216[num][i];		 //调用16x32字体
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)//非叠加模式
			{
				if(temp&(0x01<<t))LCD_WR_DATA(fc);
				else LCD_WR_DATA(bc);
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
			}
			else//叠加模式
			{
				if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);//画一个点
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
	}   	 	  
}


void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode)
{         
	while(*p!='\0')
	{       
		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}  
}


u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;
	return result;
}

void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey)
{
	u8 t,temp;
	u8 enshow=0;
	u8 sizex=sizey/2;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
} 



void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[])
{
	u16 i,j;
	u32 k=0;
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			LCD_WR_DATA8(pic[k*2]);
			LCD_WR_DATA8(pic[k*2+1]);
			k++;
		}
	}			
}

void LCD_ShowPicture_DMA(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t *pic)
{
    uint32_t size = length * width * 2;
		LCD_Address_Set(x,y,x+length-1,y+width-1);
    LCD_CS_0();

    // 设置 DMA 参数
    DMA_SetSrcAddr(DMA_UNIT, DMA_TX_CH, (uint32_t)pic);
    DMA_SetTransCount(DMA_UNIT, DMA_TX_CH, size);
    DMA_ChCmd(DMA_UNIT, DMA_TX_CH, ENABLE);

    // 等待 DMA 完成
    while (RESET == DMA_GetTransCompleteStatus(DMA_UNIT, DMA_FLAG_TC_CH0)) {}
    DMA_ClearTransCompleteStatus(DMA_UNIT, DMA_FLAG_TC_CH0);
	
	DMA_ChCmd(DMA_UNIT, DMA_TX_CH, DISABLE);
    LCD_CS_1();
}


static void LCD_GPIO_Init(void)
{
    stc_gpio_init_t stcGpioInit;

    (void)GPIO_StructInit(&stcGpioInit);

		stcGpioInit.u16PinState      = PIN_STAT_SET;       
    stcGpioInit.u16PinDir        = PIN_DIR_OUT;       
    stcGpioInit.u16PinOutputType = PIN_OUT_TYPE_CMOS;  // 推挽输出
    stcGpioInit.u16PinDrv        = PIN_HIGH_DRV;        
    stcGpioInit.u16Latch         = PIN_LATCH_OFF;    
    stcGpioInit.u16PullUp        = PIN_PU_OFF;         
    stcGpioInit.u16Invert        = PIN_INVT_OFF;     
    stcGpioInit.u16ExtInt        = PIN_EXTINT_OFF;      
    stcGpioInit.u16PinAttr       = PIN_ATTR_DIGITAL;   
	
    GPIO_Init(LCD_DC_PORT, LCD_DC_PIN, &stcGpioInit);
    GPIO_Init(LCD_CS_PORT, LCD_CS_PIN, &stcGpioInit);
    //GPIO_Init(LCD_SCK_PORT, LCD_SCK_PIN, &stcGpioInit);
    //GPIO_Init(LCD_SDA_PORT, LCD_SDA_PIN, &stcGpioInit);
		GPIO_Init(LCD_BLK_PORT, LCD_BLK_PIN, &stcGpioInit);
		LCD_BLK_PORT_0();
}

void Lcd_init(void){
	
	LCD_GPIO_Init();
	SPI_Config();
	TFT_init();
	TFT_clear();
	LCD_BLK_PORT_1();
}

void Set_BLK_PORT(uint8_t blk)
{
	if(blk == 0){
		GPIO_ResetPins(LCD_BLK_PORT, LCD_BLK_PIN);
	}else{
		GPIO_SetPins(LCD_BLK_PORT, LCD_BLK_PIN);
	}
}      
