#include "LCD.h"
#include "LCD_HFG12864.h"
#include "Font.h"


extern uint8_t G_DisRAM[1024];

void Clear_Screen(void)
{
	uint16_t i;

	for (i=0;i<1024;i++)
	{
		G_DisRAM[i] = 0x00;
	}
	
}

void Clear_Screen_Page(uint8_t page)
{
	uint16_t i;

	for (i=0;i<128;i++)
	{
		G_DisRAM[page*128+i] = 0x00;
	}
	
}

void Display_FullScreen(void)
{
	uint16_t i;
	for (i=0;i<1024;i++)
	{
    G_DisRAM[i] = 0xFF;
	}
}

void DisHZ16x14Str(uint8_t StartPage,uint8_t StartColumn, uint8_t /*code */ *BUFF,uint8_t Color)
{
	uint8_t i,j,Page,Column;
	uint16_t StartByte;
	uint8_t *Point;
	
	Page=  StartPage;
	Column = StartColumn;
	
	for (i=0;i<36;i++)
	{
		if ( *BUFF==HZ_end )
		{
			break;
		}
		if (((*BUFF<200)&&( Column >113 ))||((*BUFF>199)&&( Column >120 )))//it is not enough to display one HZ	
		{
			Column=0;
			Page+=2;
		}
		
		StartByte = (128*Page) + Column;

		if (*BUFF<200)		//the char is HZ16*14
		{
			Point = &HZ16x14[*BUFF][0];
			for (j=0;j<14;j++)
			{
				if ( Color == NormalDisplay ) { //normal display
					G_DisRAM[StartByte+j]=*Point;
				}else{		//inverse display
					G_DisRAM[StartByte+j]=(*Point)^0xFF;
				}
				Point++;
			}
			for (j=0;j<14;j++)
			{
				if ( Color == NormalDisplay ) { //normal display
					G_DisRAM[StartByte+j+128]=*Point;
				}else{		//inverse display
					G_DisRAM[StartByte+j+128]=(*Point)^0xFF;
				}
				Point++;
			}
			BUFF++;
			Column+=14;
		}
		else if (*BUFF < HZ_end )		//the char is ZF
		{
			Point = &ZF16x8[(*BUFF)-200][0];
			for (j=0;j<8;j++)
			{
				if ( Color == NormalDisplay ) { //normal display
					G_DisRAM[StartByte+j]=*Point;
				}else{		//inverse display
					G_DisRAM[StartByte+j]=(*Point)^0xFF;
				}
				Point++;
			}
			for (j=0;j<8;j++)
			{
				if ( Color == NormalDisplay ) { //normal display
					G_DisRAM[StartByte+j+128]=*Point;
				}else{		//inverse display
					G_DisRAM[StartByte+j+128]=(*Point)^0xFF;
				}
				Point++;
			}
			BUFF++;
			Column+=8;
		}
	}
}

void DisEN16x8Str(uint8_t StartPage,uint8_t StartColumn, /*code */  uint8_t *BUFF,uint8_t Color)
{
	uint8_t i,j,Page,Column;
	uint16_t StartByte;
	uint8_t *Point;
	
	Page=  StartPage;
	Column = StartColumn;
	
	for (i=0;i<16;i++)
	{
		if ( *BUFF=='\0' )
		{
			break;
		}
		if ( Column >120 )		//it is not enough to display one English char
		{
			break;
		}
		
		StartByte = (128*Page) + Column;

		if  ( (*BUFF>0x1F)&&(*BUFF<0x7B) )		//FROM space to 'z'
		{
			Point = &EN16x8[(*BUFF)-0x20][0];
		}
		else
		{
			Point = &EN16x8[0][0];	//SPACE
		}
		
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j]=(*Point)^0xFF;
			}
			Point++;
		}
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j+128]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j+128]=(*Point)^0xFF;
			}
			Point++;
		}
		BUFF++;
		Column+=8;
	}
}

void DisZF16x8(uint8_t StartPage,uint8_t StartColumn,uint8_t  Value,uint8_t Color)
{
	uint8_t j,Page,Column;
	uint16_t StartByte;
	uint8_t *Point;
	//uint8_t DigitalPosition;

	//DigitalPosition =0x00;
	
	Page=  StartPage;
	Column = StartColumn;

	if ( Column >120 )		//it is not enough to display one English char
	{
		return;
	}
	
	StartByte = (128*Page) + Column;

	if ( Value >199 )
	{
		Point = &ZF16x8[Value-200][0];
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j]=(*Point)^0xFF;
			}
			Point++;
		}
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j+128]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j+128]=(*Point)^0xFF;
			}
			Point++;
		}
	}

}

void DisOneDigital16x8(uint8_t StartPage,uint8_t StartColumn,uint8_t  Value,uint8_t Color)
{
	uint8_t j,Page,Column;
	uint16_t StartByte;
	uint8_t *Point;
	//uint8_t DigitalPosition;
	//DigitalPosition =0x00;
	Page=  StartPage;
	Column = StartColumn;

	
	StartByte = (128*Page) + Column;

	Point = &ZF16x8[Value%10][0];
	for (j=0;j<8;j++)
	{
		if ( Color == NormalDisplay ) 
		{ //normal display
			G_DisRAM[StartByte+j]=*Point;
		}else
		{		//inverse display
			G_DisRAM[StartByte+j]=(*Point)^0xFF;
		}
		Point++;
	}
	for (j=0;j<8;j++)
	{
		if ( Color == NormalDisplay ) 
		{ //normal display
			G_DisRAM[StartByte+j+128]=*Point;
		}else
		{		//inverse display
			G_DisRAM[StartByte+j+128]=(*Point)^0xFF;
		}
		Point++;
	}

}

void DisBcdDigital16x8(uint8_t StartPage,uint8_t StartColumn,uint8_t  Value,uint8_t Color)
{
	uint8_t i,j,Page,Column;
	uint16_t StartByte;
	uint8_t *Point;

	
	Page=  StartPage;
	Column = StartColumn;


	for (i=0;i<2;i++)
	{
		
		StartByte = (128*Page) + Column;
		Point = &ZF16x8[Value>>4][0];
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) 
			{ //normal display
				G_DisRAM[StartByte+j]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j]=(*Point)^0xFF;
			}
			Point++;
		}
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j+128]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j+128]=(*Point)^0xFF;
			}
			Point++;
		}
		Column+=8;
		Value <<=4;
	}
}

void DisDigital16x8Str(uint8_t StartPage,uint8_t StartColumn,uint16_t  Value,uint8_t Color)
{
	uint8_t i,j,Page,Column;
	uint16_t StartByte;
	const uint8_t *Point;
	uint16_t DataValue;
	
	Page=  StartPage;
	Column = StartColumn;
	DataValue = Value;

	for (i=0;i<3;i++)
	{
		if ( Column >120 )		//it is not enough to display one HZ
		{
			Column=0;
			Page++;
		}
		
		StartByte = (128*Page) + Column;

		Point = &ZF16x8[DataValue/100][0];
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j]=(*Point)^0xFF;
			}
			Point++;
		}
		for (j=0;j<8;j++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+j+128]=*Point;
			}else{		//inverse display
				G_DisRAM[StartByte+j+128]=(*Point)^0xFF;
			}
			Point++;
		}
		Column+=8;
		DataValue=(DataValue%100)*10;

	}
}

void DisImage(uint8_t StartPage,uint8_t StartColumn,uint8_t Width,uint8_t Heigth, /*code */  uint8_t *BUFF,uint8_t Color)
{
	uint8_t i,j,Page,Column;
	uint16_t StartByte;
	
	Page=  StartPage;
	Column = StartColumn;

	StartByte = (128*Page) + Column;	
	for (j=0;j<(Heigth/8);j++)
	{
		for (i=0;i<Width;i++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+i]=*(BUFF+i);
			}else{		//inverse display
				G_DisRAM[StartByte+i]=(*(BUFF+i))^0xFF;
			}
		}
		StartByte+=128;
		BUFF+=Width;
	}	
}

void DisImage_RAM(uint8_t StartPage,uint8_t StartColumn,uint8_t Width,uint8_t Heigth,uint8_t *BUFF,uint8_t Color)
{
	uint8_t i,j,Page,Column;
	uint16_t StartByte;
	
	Page=  StartPage;
	Column = StartColumn;

	StartByte = (128*Page) + Column;	
	for (j=0;j<(Heigth/8);j++)
	{
		for (i=0;i<Width;i++)
		{
			if ( Color == NormalDisplay ) { //normal display
				G_DisRAM[StartByte+i]=*(BUFF+i);
			}else{		//inverse display
				G_DisRAM[StartByte+i]=(*(BUFF+i))^0xFF;
			}
		}
		StartByte+=128;
		BUFF+=Width;
	}	
}

void DisBcdDigital32x20(uint8_t StartPage,uint8_t StartColumn,uint8_t  Value,uint8_t Color)
{
	uint8_t i,j,k,Page,Column;
	uint16_t StartByte;
	const uint8_t *Point;
	
	Page=  StartPage;
	Column = StartColumn;

	for (k=0;k<2;k++)
	{
		
		StartByte = (128*Page) + Column;	
		Point = &ZF32x20[Value>>4][0];
		for (j=0;j<4;j++)
		{
			for (i=0;i<20;i++)
			{
				if ( Color == NormalDisplay ) { //normal display
					G_DisRAM[StartByte+i]=*(Point);
				}else{		//inverse display
					G_DisRAM[StartByte+i]=(*Point)^0xFF;
				}
				Point++;
			}
			StartByte+=128;
		}
		Column+=20;
		Value <<=4;
	}
}



