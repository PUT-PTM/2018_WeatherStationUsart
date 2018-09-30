#include "defines.h"

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "tm_stm32f4_pcd8544.h"

char city[100];
char temp[3];
char press[4];
char cloud[4];
char sun[5];
char moon[5];

int cityIndex;
int tempIndex;
int pressIndex;
int cloudIndex;
int sunIndex;
int moonIndex;

void clearBuffor();
void init_NVIC(void);

void init_NVIC(void) {

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);

	//GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =
	GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 9999;
	TIM_TimeBaseStructure.TIM_Prescaler = 8399;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitTypeDef NVIC_InitStructure2;
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure2.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure2);
	NVIC_EnableIRQ(USART3_IRQn);

	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM3, ENABLE);
	USART_Cmd(USART3, ENABLE);
}

int main(void) {
	SystemInit();
	init_NVIC();

	//Initialize LCD with 0x38 software contrast
	PCD8544_Init(0x2f);

	clearBuffor();

	PCD8544_GotoXY(0, 10);
	PCD8544_Puts("Oczekiwanie na aplikacje...", PCD8544_Pixel_Set,
			PCD8544_FontSize_5x7);

	PCD8544_Refresh();

	while (1) {
	}
}

void clearBuffor() {
	int i;
	for (i = 0; i < 100; i++)
		city[i] = ' ';

	for (i = 0; i < 3; i++)
		temp[i] = ' ';

	for (i = 0; i < 4; i++)
		press[i] = ' ';

	for (i = 0; i < 4; i++)
		cloud[i] = ' ';

	for (i = 0; i < 5; i++)
		sun[i] = ' ';

	for (i = 0; i < 5; i++)
		moon[i] = ' ';

	cityIndex = tempIndex = pressIndex = cloudIndex = sunIndex = moonIndex = 0;

}

void TIM3_IRQHandler(void) {
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		//	USART_SendData(USART3, 'A');

		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

void USART3_IRQHandler(void) {
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		static int state = 0;

		// jezeli napotka ten dany znaczek
		if (USART3->DR == '`') {
			// to drukuje wszystko na lcd
			PCD8544_Clear();

			///////////////////

			PCD8544_GotoXY(5, 1);
			PCD8544_Puts(city, PCD8544_Pixel_Set, PCD8544_FontSize_5x7);

			PCD8544_GotoXY(68, 4);
			PCD8544_Puts(temp, PCD8544_Pixel_Set, PCD8544_FontSize_3x5);

			// design
			PCD8544_DrawRectangle(65, 0, 83, 12, PCD8544_Pixel_Set);
			PCD8544_DrawLine(5, 12, 65, 12, PCD8544_Pixel_Set);

			//moon
			PCD8544_DrawFilledCircle(54, 43, 4, PCD8544_Pixel_Set);
			PCD8544_DrawFilledCircle(52, 42, 3, PCD8544_Pixel_Clear);

			PCD8544_GotoXY(5, 20);
			PCD8544_Puts("Cisnienie:", PCD8544_Pixel_Set, PCD8544_FontSize_3x5);

			PCD8544_GotoXY(68, 20);
			PCD8544_Puts(press, PCD8544_Pixel_Set, PCD8544_FontSize_3x5);

			PCD8544_GotoXY(5, 30);
			PCD8544_Puts("Zachmurzenie:", PCD8544_Pixel_Set,
					PCD8544_FontSize_3x5);

			PCD8544_GotoXY(68, 30);
			PCD8544_Puts(cloud, PCD8544_Pixel_Set, PCD8544_FontSize_3x5);

			//slonce
			PCD8544_DrawCircle(6, 43, 4, PCD8544_Pixel_Set);
			//PCD8544_DrawFilledRectangle(0, 40, 4, 47, PCD8544_Pixel_Clear);
			PCD8544_DrawLine(13, 41, 15, 41, PCD8544_Pixel_Set);
			PCD8544_DrawLine(15, 44, 18, 44, PCD8544_Pixel_Set);
			PCD8544_DrawLine(13, 47, 15, 47, PCD8544_Pixel_Set);

			PCD8544_GotoXY(23, 42);
			PCD8544_Puts(sun, PCD8544_Pixel_Set, PCD8544_FontSize_3x5);

			PCD8544_GotoXY(64, 42);
			PCD8544_Puts(moon, PCD8544_Pixel_Set, PCD8544_FontSize_3x5);

			///////////////////

			PCD8544_Refresh();

			clearBuffor();

			state = 0;
		}

		else if (USART3->DR == '@') {
			state++;

		} else {
			char c = (char) USART3->DR;

			switch (state) {
			case 0:
				city[cityIndex++] = c;
				break;
			case 1:
				temp[tempIndex++] = c;
				break;
			case 2:
				press[pressIndex++] = c;
				break;
			case 3:
				cloud[cloudIndex++] = c;
				break;
			case 4:
				sun[sunIndex++] = c;
				break;
			case 5:
				moon[moonIndex++] = c;
				break;
			}

		}

	}
}
