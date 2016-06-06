#include "encoder.h"

void EncStructInit(Encoder* me) {
	me->isEnable = 0;
	me->prevEncoderCount = 0;
	me->currEncoderCount = 0;
	me->deltaEncoderCount = 0;
	me->accumEncoderCounts = 0;
	me->encoderAddr = 0;
	me->encoderTIM = 0;
}

//use tim1,cc1
void Enc3Config(Encoder* me) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //channel1
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //channel2
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);

	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,
			TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_SetAutoreload(TIM3, 0xffff);
	TIM_SetCounter(TIM3, 0);
	me->encoderTIM = TIM3;
	me->encoderAddr = (unsigned int) &TIM3->CNT;
	EnableEncoder(me); //	TIM_Cmd(TIM3, ENABLE);
}
void Enc0Config(Encoder* me) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //channel1
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //channel2
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);

	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising,
			TIM_ICPolarity_Rising);
	TIM_SetAutoreload(TIM3, 0xffff);
	TIM_Cmd(TIM3, ENABLE);
	TIM_SetCounter(TIM3, 0);
	//	TIM_Cmd(TIM3, ENABLE);
	me->encoderTIM = TIM3;
	me->encoderAddr = (unsigned int) &TIM3->CNT;
	EnableEncoder(me); //	TIM_Cmd(TIM3, ENABLE);
}
//use tim8,cc1
void Enc1Config(Encoder* me) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //channel1
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //channel2
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);

	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,
			TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_SetAutoreload(TIM4, 0xffff);
	TIM_SetCounter(TIM4, 0);
	//	TIM_Cmd(TIM4, ENABLE);
	me->encoderTIM = TIM4;
	me->encoderAddr = (unsigned int) &TIM4->CNT;
	EnableEncoder(me); //	TIM_Cmd(TIM4, ENABLE);
}

void DisableEncoder(Encoder * me) {
	TIM_Cmd(me->encoderTIM, DISABLE);
	me->isEnable = 0;
}
void EnableEncoder(Encoder * me) {
	TIM_Cmd(me->encoderTIM, ENABLE);
	me->isEnable = 1;
}

int IsEncoderEnable(Encoder * me) {
	return me->isEnable;
}

//to avoid overflow
void SetEncoder(Encoder * me, int encoderCounter) {
	DisableEncoder(me);
	me->accumEncoderCounts = encoderCounter;
	me->currEncoderCount = 0;
	me->prevEncoderCount = 0;
	me->deltaEncoderCount = 0;
	TIM_SetCounter(me->encoderTIM, 0);
	EnableEncoder(me);
}
void ResetEncoder(Encoder * me) {
	DisableEncoder(me);
	me->prevEncoderCount = 0;
	me->currEncoderCount = 0;
	me->deltaEncoderCount = 0;
	me->accumEncoderCounts = 0;
	TIM_SetCounter(me->encoderTIM, 0);
	GetEncoderCounts(me);
	EnableEncoder(me);
}

//Port, need to be called frequently
//ret:encoderCounter
int GetEncoderCounts(Encoder * me) {
	me->currEncoderCount = (s16) (*((u16*) me->encoderAddr));
	me->deltaEncoderCount = me->currEncoderCount - me->prevEncoderCount;
	me->prevEncoderCount = me->currEncoderCount;
	me->accumEncoderCounts += (s32) me->deltaEncoderCount;
	return me->accumEncoderCounts;
}

