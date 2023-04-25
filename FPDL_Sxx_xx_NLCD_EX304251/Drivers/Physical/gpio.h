#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "StdTypes.h"
	 
#define GPIO_Port_type uint8_t
#define Pin_type uint8_t
#define GPIO_Mode_type uint8_t
#define GPIO_Mode_OUT_PP 0   //推挽输出,负载10mA
#define GPIO_Mode_INPUT_PU 1 //上拉输入
#define GPIO_Mode_INPUT_HZ 2 //高阻无上拉
#define PORT0 0 
#define PORT1 1 
#define PORT2 2 
#define PORT3 3 
#define PORT4 4 
#define PORT5 5
#define PIN0 0 
#define PIN1 1 
#define PIN2 2 
#define PIN3 3 
#define PIN4 4 
#define PIN5 5 
#define PIN6 6 
#define PIN7 7 
	 
extern uint8_t INT1flag;
void GPIO_Config(GPIO_Port_type Port,Pin_type Pin,GPIO_Mode_type Mode);
void MX_GPIO_Init(void);
void MX_GPIO_DeInit(void);
void Enable_EX_Interrupt(void);
void EnableAntiPrying_Interrupt(void);
void DisableAntiPrying_Interrupt(void);
void INT1_RST(void);

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

