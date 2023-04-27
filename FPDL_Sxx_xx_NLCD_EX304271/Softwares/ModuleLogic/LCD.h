#ifndef _LCD_LOGID_H_
#define _LCD_LOGID_H_

#include "StdTypes.h"
#include "Project.h"


#define NormalDisplay		0x00
#define InverseDisplay		0x01
#define GREEN_COLOR			0x02
#define RED_COLOR			0x03

#ifdef Function_ScreenDisplay
void Clear_Screen(void);
void Clear_Screen_Page(uint8_t page);
void Display_FullScreen(void);
void DisHZ16x14Str(uint8_t StartPage,uint8_t StartColumn, uint8_t /*code */ *BUFF,uint8_t Color);
void DisEN16x8Str(uint8_t StartPage,uint8_t StartColumn, /*code */  uint8_t *BUFF,uint8_t Color);
void DisZF16x8(uint8_t StartPage,uint8_t StartColumn,uint8_t  Value,uint8_t Color);
void DisOneDigital16x8(uint8_t StartPage,uint8_t StartColumn,uint8_t  Value,uint8_t Color);
void DisBcdDigital16x8(uint8_t StartPage,uint8_t StartColumn,uint8_t  Value,uint8_t Color);
void DisDigital16x8Str(uint8_t StartPage,uint8_t StartColumn,uint16_t  Value,uint8_t Color);
void DisImage(uint8_t StartPage,uint8_t StartColumn,uint8_t Width,uint8_t Heigth, /*code */  uint8_t *BUFF,uint8_t Color);
void DisImage_RAM(uint8_t StartPage,uint8_t StartColumn,uint8_t Width,uint8_t Heigth,uint8_t *BUFF,uint8_t Color);
void DisBcdDigital32x20(uint8_t StartPage,uint8_t StartColumn,uint8_t  Value,uint8_t Color);
#endif

#endif