#include "Project.h"
#include "IO.h"
#include "global_variable.h"

 uint8_t LEDsCtrlSwitch = 0;
 uint16_t KeyLEDsFlashTimeCnt;
 uint16_t LEDsWaterLightTimeCnt;
 uint16_t LEDsIdentifyFailTimeCnt;

#define SET_LED_KEYPOUNDSIGN_ON		LEDsMgr.KeyPoundSignShouldBeOn = bTRUE;
#define SET_LED_KEYPOUNDSIGN_OFF	LEDsMgr.KeyPoundSignShouldBeOn = bFALSE;
#define SET_LED_KEYASTERISK_ON		LEDsMgr.KeyAsteriskShouldBeOn  = bTRUE;
#define SET_LED_KEYASTERISK_OFF		LEDsMgr.KeyAsteriskShouldBeOn  = bFALSE;
#define SET_LED_KEY9_ON				LEDsMgr.Key9ShouldBeOn  = bTRUE;
#define SET_LED_KEY9_OFF			LEDsMgr.Key9ShouldBeOn  = bFALSE;
#define SET_LED_KEY8_ON				LEDsMgr.Key8ShouldBeOn  = bTRUE;
#define SET_LED_KEY8_OFF			LEDsMgr.Key8ShouldBeOn  = bFALSE;
#define SET_LED_KEY7_ON				LEDsMgr.Key7ShouldBeOn  = bTRUE;
#define SET_LED_KEY7_OFF			LEDsMgr.Key7ShouldBeOn  = bFALSE;
#define SET_LED_KEY6_ON				LEDsMgr.Key6ShouldBeOn  = bTRUE;
#define SET_LED_KEY6_OFF			LEDsMgr.Key6ShouldBeOn  = bFALSE;
#define SET_LED_KEY5_ON				LEDsMgr.Key5ShouldBeOn  = bTRUE;
#define SET_LED_KEY5_OFF			LEDsMgr.Key5ShouldBeOn  = bFALSE;
#define SET_LED_KEY4_ON				LEDsMgr.Key4ShouldBeOn  = bTRUE;
#define SET_LED_KEY4_OFF			LEDsMgr.Key4ShouldBeOn  = bFALSE;
#define SET_LED_KEY3_ON				LEDsMgr.Key3ShouldBeOn  = bTRUE;
#define SET_LED_KEY3_OFF			LEDsMgr.Key3ShouldBeOn  = bFALSE;
#define SET_LED_KEY2_ON				LEDsMgr.Key2ShouldBeOn  = bTRUE;
#define SET_LED_KEY2_OFF			LEDsMgr.Key2ShouldBeOn  = bFALSE;
#define SET_LED_KEY1_ON				LEDsMgr.Key1ShouldBeOn  = bTRUE;
#define SET_LED_KEY1_OFF			LEDsMgr.Key1ShouldBeOn  = bFALSE;
#define SET_LED_KEY0_ON				LEDsMgr.Key0ShouldBeOn  = bTRUE;
#define SET_LED_KEY0_OFF			LEDsMgr.Key0ShouldBeOn  = bFALSE;


void SET_KEYLED_OFF(keycode_t key)
{
	switch (key)
	{
	case KEY_ZERO:
			SET_LED_KEY0_OFF;
		break;
	case KEY_ONE:
			SET_LED_KEY1_OFF;
		break;
	case KEY_TWO:
			SET_LED_KEY2_OFF;
		break;
	case KEY_THREE:
			SET_LED_KEY3_OFF;	
		break;
	case KEY_FOUR:
			SET_LED_KEY4_OFF;	
		break;
	case KEY_FIVE:
			SET_LED_KEY5_OFF;
		break;
	case KEY_SIX:
			SET_LED_KEY6_OFF;
		break;
	case KEY_SEVEN:
			SET_LED_KEY7_OFF;
		break;
	case KEY_EIGHT:
			SET_LED_KEY8_OFF;
		break;
	case KEY_NINE:
			SET_LED_KEY9_OFF;
		break;
	case KEY_ASTERISK:
			SET_LED_KEYASTERISK_OFF;
		break;
	case KEY_POUNDSIGN:
			SET_LED_KEYPOUNDSIGN_OFF;
		break;
	default:
			break;
  }
}
void SET_KEYLED_ON(keycode_t key)
{
	switch (key)
	{
		case KEY_ZERO:
				SET_LED_KEY0_ON;
			break;
		case KEY_ONE:
				SET_LED_KEY1_ON;
			break;
		case KEY_TWO:
				SET_LED_KEY2_ON;
			break;
		case KEY_THREE:
				SET_LED_KEY3_ON;	
			break;
		case KEY_FOUR:
				SET_LED_KEY4_ON;	
			break;
		case KEY_FIVE:
				SET_LED_KEY5_ON;
			break;
		case KEY_SIX:
				SET_LED_KEY6_ON;
			break;
		case KEY_SEVEN:
				SET_LED_KEY7_ON;
			break;
		case KEY_EIGHT:
				SET_LED_KEY8_ON;
			break;
		case KEY_NINE:
				SET_LED_KEY9_ON;
			break;
		case KEY_ASTERISK:
				SET_LED_KEYASTERISK_ON;
			break;
		case KEY_POUNDSIGN:
				SET_LED_KEYPOUNDSIGN_ON;
			break;
		default:
				break;
	}
}


/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  无
********************************************************************************************************/
void SET_AP_LED_ON(void)
{
	SET_LED_KEY1_OFF;
	SET_LED_KEY2_OFF;
	SET_LED_KEY3_OFF;	
	SET_LED_KEY4_OFF;	
	SET_LED_KEY5_OFF;
	SET_LED_KEY6_OFF;
	SET_LED_KEY7_OFF;
	SET_LED_KEY8_OFF;
	SET_LED_KEY9_OFF;
	SET_LED_KEYASTERISK_ON;
	SET_LED_KEY0_OFF;
	SET_LED_KEYPOUNDSIGN_ON;
}

/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  无
********************************************************************************************************/
void SET_12A_LED_ON(void)
{
	SET_LED_KEY1_ON;
	SET_LED_KEY2_ON;
	SET_LED_KEY3_OFF;	
	SET_LED_KEY4_OFF;	
	SET_LED_KEY5_OFF;
	SET_LED_KEY6_OFF;
	SET_LED_KEY7_OFF;
	SET_LED_KEY8_OFF;
	SET_LED_KEY9_OFF;
	SET_LED_KEYASTERISK_ON;
	SET_LED_KEY0_OFF;
	SET_LED_KEYPOUNDSIGN_OFF;
}
/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  无
********************************************************************************************************/
void SET_123A_LED_ON(void)
{
	SET_LED_KEY1_ON;
	SET_LED_KEY2_ON;
	SET_LED_KEY3_ON;	
	SET_LED_KEY4_OFF;	
	SET_LED_KEY5_OFF;
	SET_LED_KEY6_OFF;
	SET_LED_KEY7_OFF;
	SET_LED_KEY8_OFF;
	SET_LED_KEY9_OFF;
	SET_LED_KEYASTERISK_ON;
	SET_LED_KEY0_OFF;
	SET_LED_KEYPOUNDSIGN_OFF;

}
/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  无
********************************************************************************************************/
void SET_1234A_LED_ON(void)
{
	SET_LED_KEY1_ON;
	SET_LED_KEY2_ON;
	SET_LED_KEY3_ON;	
	SET_LED_KEY4_ON;	
	SET_LED_KEY5_OFF;
	SET_LED_KEY6_OFF;
	SET_LED_KEY7_OFF;
	SET_LED_KEY8_OFF;
	SET_LED_KEY9_OFF;
	SET_LED_KEYASTERISK_ON;
	SET_LED_KEY0_OFF;
	SET_LED_KEYPOUNDSIGN_OFF;

}

/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  无
********************************************************************************************************/
void SET_123456A_LED_ON(void)
{
	SET_LED_KEY1_ON;
	SET_LED_KEY2_ON;
	SET_LED_KEY3_ON;	
	SET_LED_KEY4_ON;	
	SET_LED_KEY5_ON;
	SET_LED_KEY6_ON;
	SET_LED_KEY7_OFF;
	SET_LED_KEY8_OFF;
	SET_LED_KEY9_OFF;
	SET_LED_KEYASTERISK_ON;
	SET_LED_KEY0_OFF;
	SET_LED_KEYPOUNDSIGN_OFF;
}


/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  无
********************************************************************************************************/
void SET_ALLKEYLED_ON(void)
{
	SET_LED_KEY1_ON;
	SET_LED_KEY2_ON;
	SET_LED_KEY3_ON;	
	SET_LED_KEY4_ON;	
	SET_LED_KEY5_ON;
	SET_LED_KEY6_ON;
	SET_LED_KEY7_ON;
	SET_LED_KEY8_ON;
	SET_LED_KEY9_ON;
	SET_LED_KEYASTERISK_ON;
	SET_LED_KEY0_ON;
	SET_LED_KEYPOUNDSIGN_ON;
}

/********************************************************************************************************
*  Function: 		                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  无
********************************************************************************************************/
void SET_ALLKEYLED_OFF(void)
{
	SET_LED_KEY1_OFF;
	SET_LED_KEY2_OFF;
	SET_LED_KEY3_OFF;	
	SET_LED_KEY4_OFF;	
	SET_LED_KEY5_OFF;
	SET_LED_KEY6_OFF;
	SET_LED_KEY7_OFF;
	SET_LED_KEY8_OFF;
	SET_LED_KEY9_OFF;
	SET_LED_KEYASTERISK_OFF;
	SET_LED_KEY0_OFF;
	SET_LED_KEYPOUNDSIGN_OFF;

	SET_LEDKEYs_OFF;
	SET_LEDRGB_R_OFF;
	SET_LEDRGB_G_OFF;
	SET_LEDRGB_B_OFF;
}

void KEYLED_ASTERISK_Flash(void)
{
	if ( ++KeyLEDsFlashTimeCnt >80 )
	{
		KeyLEDsFlashTimeCnt=0;
	}
	if ( KeyLEDsFlashTimeCnt < 40 )
	{
		SET_ALLKEYLED_OFF();
	}
	else
	{
		SET_LED_KEYASTERISK_ON;
	}
}
void KEYLED_KEY5_Flash(void)
{
	if ( ++KeyLEDsFlashTimeCnt >80 )
	{
		KeyLEDsFlashTimeCnt=0;
	}
	if ( KeyLEDsFlashTimeCnt < 40 )
	{
		SET_ALLKEYLED_OFF();
	}
	else
	{
		SET_LED_KEY5_ON;
	}
}

void Enable_KEYLED_IdentifyFail(void)
{
	LEDsIdentifyFailTimeCnt = 81;
}
void KEYLED_IdentifyFail_Task(void)
{
	uint8_t i;
	if ( LEDsIdentifyFailTimeCnt > 80 )
	{
		SET_ALLKEYLED_OFF();
	}
	else if ( LEDsIdentifyFailTimeCnt > 1 )
	{
		SET_LED_KEY5_ON;	
		i=LEDsIdentifyFailTimeCnt/8;
		if ( (i%2)==0)
		{
			SET_LED_KEY1_ON;
			SET_LED_KEY2_OFF;
			SET_LED_KEY3_ON;	
			SET_LED_KEY4_OFF;	
			SET_LED_KEY5_ON;
			SET_LED_KEY6_OFF;
			SET_LED_KEY7_ON;
			SET_LED_KEY8_OFF;
			SET_LED_KEY9_ON;
			SET_LED_KEYASTERISK_OFF;
			SET_LED_KEY0_ON;
			SET_LED_KEYPOUNDSIGN_OFF;
		}
		else if ( (i%2)==1)
		{
			SET_LED_KEY1_OFF;
			SET_LED_KEY2_ON;
			SET_LED_KEY3_OFF;	
			SET_LED_KEY4_ON;	
			SET_LED_KEY5_OFF;
			SET_LED_KEY6_ON;
			SET_LED_KEY7_OFF;
			SET_LED_KEY8_ON;
			SET_LED_KEY9_OFF;
			SET_LED_KEYASTERISK_ON;
			SET_LED_KEY0_OFF;
			SET_LED_KEYPOUNDSIGN_ON;

		}
	}
	else if ( LEDsIdentifyFailTimeCnt == 1 )
	{
		SET_ALLKEYLED_OFF();
	}
	else 
	{
		//SET_LED_KEYASTERISK_ON;
	}

	if ( LEDsIdentifyFailTimeCnt > 0 )
	{
		LEDsIdentifyFailTimeCnt--;

	}
}
void Enable_KEYLED_WATERLIGHT(void)
{
	LEDsWaterLightTimeCnt = 56;
}

void KEYLED_WATERLIGHT_Task(void)
{
	uint8_t i;
	if ( LEDsWaterLightTimeCnt > 55 )
	{
		SET_ALLKEYLED_OFF();
	}
	else if ( LEDsWaterLightTimeCnt > 0 )
	{
		i=LEDsWaterLightTimeCnt/5;
		if ( (i%12)==11)
		{
			SET_LED_KEY8_ON;
			SET_LED_KEY5_ON;
		}
		else if ( (i%12)==10)
		{
			SET_LED_KEY8_OFF;
			SET_LED_KEY6_ON;
		}
		else if ( (i%12)==9)
		{
			SET_LED_KEY9_ON;
			SET_LED_KEY5_OFF;
		}
		else if ( (i%12)==8)
		{
			SET_LED_KEYPOUNDSIGN_ON;
			SET_LED_KEY6_OFF;
		}
		else if ( (i%12)==7)
		{
			SET_LED_KEY0_ON;
			SET_LED_KEY9_OFF;
		}
		else if ( (i%12)==6)
		{
			SET_LED_KEYASTERISK_ON;
			SET_LED_KEYPOUNDSIGN_OFF;
		}
		else if ( (i%12)==5)
		{
			SET_LED_KEY7_ON;
			SET_LED_KEY0_OFF;
		}
		else if ( (i%12)==4)
		{
			SET_LED_KEY4_ON;
			SET_LED_KEYASTERISK_OFF;
		}
		else if ( (i%12)==3)
		{
			SET_LED_KEY1_ON;
			SET_LED_KEY7_OFF;
		}
		else if ( (i%12)==2)
		{
			SET_LED_KEY2_ON;
			SET_LED_KEY4_OFF;
		}
		else if ( (i%12)==1)
		{
			SET_LED_KEY3_ON;
			SET_LED_KEY1_OFF;
		}
		else if ( (i%12)==0)
		{
			SET_LED_KEY2_OFF;
			SET_LED_KEY3_OFF;
		}
	}
	else
	{
		//SET_ALLKEYLED_OFF();
		KEYLED_ASTERISK_Flash();
	}

	if ( LEDsWaterLightTimeCnt > 0 )
	{
		LEDsWaterLightTimeCnt--;

	}
}


void SET_LedCtrl1_H(void)	
{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	GPIO_InitStruct.Pin = GPIO_PIN_6;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
}
void SET_LedCtrl1_L(void)	
{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	GPIO_InitStruct.Pin = GPIO_PIN_6;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
}
void SET_LedCtrl1_Hi(void)	
{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	GPIO_InitStruct.Pin = GPIO_PIN_6;
//	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
void SET_LedCtrl2_H(void)	
{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	GPIO_InitStruct.Pin = GPIO_PIN_11;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
}
void SET_LedCtrl2_L(void)	
{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	GPIO_InitStruct.Pin = GPIO_PIN_11;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
}
void SET_LedCtrl2_Hi(void)
{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	GPIO_InitStruct.Pin = GPIO_PIN_11;
//	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void SET_LedCtrl3_H(void)
{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	GPIO_InitStruct.Pin = GPIO_PIN_10;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
}
void SET_LedCtrl3_L(void)
{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	GPIO_InitStruct.Pin = GPIO_PIN_10;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
}
void SET_LedCtrl3_Hi(void)
{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	GPIO_InitStruct.Pin = GPIO_PIN_10;
//	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void SET_LedCtrl4_H(void)	
{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	GPIO_InitStruct.Pin = GPIO_PIN_12;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
}
void SET_LedCtrl4_L(void)	
{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	GPIO_InitStruct.Pin = GPIO_PIN_12;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
}
void SET_LedCtrl4_Hi(void)
{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	GPIO_InitStruct.Pin = GPIO_PIN_12;
//	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void RGB_LEDMgr_Task(void)
{
	switch (LEDsCtrlSwitch)
	{
	case FPMbreathingLed_Red:
		SET_LEDRGB_R_ON;
		SET_LEDRGB_B_OFF;
		SET_LEDRGB_G_OFF;	
		break;

	case FPMbreathingLed_Green:
		SET_LEDRGB_R_OFF;
		SET_LEDRGB_B_OFF;
		SET_LEDRGB_G_ON;
		break;
	
	default:
		SET_LEDRGB_R_OFF;
		SET_LEDRGB_G_OFF;
		SET_LEDRGB_B_ON;
		break;
	}
}

void KEYLEDsMgr_Task(void)
{
	SET_LEDKEYs_ON;
}

#ifdef Function_LEDwaterLamp
void LEDsMgr_Task(void)
{
	#ifndef DEBUG_MODE
	SET_LOGLED_ON;
	#endif
	
	if ( LEDsCtrlSwitch < 4 ){
		LEDsCtrlSwitch++;
	}else {
	LEDsCtrlSwitch = 1;
	}

	if (
		(LEDsMgr.Key0ShouldBeOn  == bFALSE )&&(LEDsMgr.Key1ShouldBeOn  == bFALSE )&&
		(LEDsMgr.Key2ShouldBeOn  == bFALSE )&&(LEDsMgr.Key3ShouldBeOn  == bFALSE )&&
		(LEDsMgr.Key4ShouldBeOn  == bFALSE )&&(LEDsMgr.Key5ShouldBeOn  == bFALSE )&&
		(LEDsMgr.Key6ShouldBeOn  == bFALSE )&&(LEDsMgr.Key7ShouldBeOn  == bFALSE )&&
		(LEDsMgr.Key8ShouldBeOn  == bFALSE )&&(LEDsMgr.Key9ShouldBeOn  == bFALSE )&&
		(LEDsMgr.KeyAsteriskShouldBeOn  == bFALSE )&&(LEDsMgr.KeyPoundSignShouldBeOn  == bFALSE )
		)
	{
		SET_LedCtrl1_L();	
		SET_LedCtrl2_L();
		SET_LedCtrl3_L();
		SET_LedCtrl4_L();
		return;
	}
		
	if ( LEDsCtrlSwitch == 1 )
	{
		SET_LedCtrl2_Hi();
		SET_LedCtrl3_Hi();
		SET_LedCtrl4_Hi();
		SET_LedCtrl1_L();
		
		if ((LEDsMgr.Key1ShouldBeOn  == bTRUE )&&(LEDsMgr.Keycode != KEY_ONE)){
			SET_LedCtrl2_H();
		}
		if (( LEDsMgr.Key3ShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_THREE)){
			SET_LedCtrl3_H();
		}	
		if (( LEDsMgr.Key5ShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_FIVE)){
			SET_LedCtrl4_H();
		}			
	}	
	else if ( LEDsCtrlSwitch == 2 )
	{
		SET_LedCtrl1_Hi();
		SET_LedCtrl3_Hi();
		SET_LedCtrl4_Hi();
		SET_LedCtrl2_L();
		
		if (( LEDsMgr.Key6ShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_SIX)){
			SET_LedCtrl1_H();
		}
		if (( LEDsMgr.Key4ShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_FOUR)){
			SET_LedCtrl3_H();
		}	
		if (( LEDsMgr.Key8ShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_EIGHT)){
			SET_LedCtrl4_H();
		}		
	}
	else if ( LEDsCtrlSwitch == 3 )
	{
		SET_LedCtrl1_Hi();
		SET_LedCtrl2_Hi();
		SET_LedCtrl4_Hi();
		SET_LedCtrl3_L();
		
		if (( LEDsMgr.Key2ShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_TWO)){
			SET_LedCtrl1_H();
		}
		if (( LEDsMgr.Key9ShouldBeOn == bTRUE)&&(LEDsMgr.Keycode != KEY_NINE)){
			SET_LedCtrl2_H();
		}	
		if (( LEDsMgr.Key7ShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_SEVEN)){
			SET_LedCtrl4_H();
		}			
	}
	else if ( LEDsCtrlSwitch == 4 )
	{
		SET_LedCtrl1_Hi();
		SET_LedCtrl2_Hi();
		SET_LedCtrl3_Hi();
		SET_LedCtrl4_L();
		
		if (( LEDsMgr.KeyAsteriskShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_ASTERISK)){
			SET_LedCtrl1_H();
		}
		if (( LEDsMgr.KeyPoundSignShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_POUNDSIGN)){
			SET_LedCtrl2_H();
		}	
		if (( LEDsMgr.Key0ShouldBeOn  == bTRUE)&&(LEDsMgr.Keycode != KEY_ZERO)){
			SET_LedCtrl3_H();
		}			
	}	
}
#else
void LEDsMgr_Task(void)
{
	//KEYLEDsMgr_Task();
	RGB_LEDMgr_Task();
}
#endif


