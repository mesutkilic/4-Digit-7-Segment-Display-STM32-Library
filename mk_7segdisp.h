/**
 *  @file mk_7segdisp.h
 *	@brief 4 Digit 7 Segment Display Library
 *  @date Created on: Oct 11, 2019
 *  @author Author: mesut.kilic
 *	@version 1.0.0
 */

#ifndef MK_7SEGDISP_H_
#define MK_7SEGDISP_H_

#include "stm32l0xx.h"

//https://lastminuteengineers.com/wp-content/uploads/2018/06/7-Segment-Display-Number-Formation-Segment-Contol.png

static uint8_t segment_numbers[16][7] = {
//	 	 a,b,c,d,e,f,g
		{1,1,1,1,1,1,0}, //0
		{0,1,1,0,0,0,0}, //1
		{1,1,0,1,1,0,1}, //2
		{1,1,1,1,0,0,1}, //3
		{0,1,1,0,0,1,1}, //4
		{1,0,1,1,0,1,1}, //5
		{1,0,1,1,1,1,1}, //6
		{1,1,1,0,0,0,0}, //7
		{1,1,1,1,1,1,1}, //8
		{1,1,1,1,0,1,1}, //9
		{1,1,1,0,1,1,1}, //10-a
		{0,0,1,1,1,1,1}, //11-b
		{1,0,1,0,0,0,0}, //12-c
		{1,0,0,1,1,1,0}, //13-d
		{1,0,0,1,1,1,1}, //14-e
		{1,0,0,0,1,1,1}, //15-f
};


struct _seven_segment_pin_t{
	uint16_t pins[8]; //a,b,c,d,e,f,g,dp
	GPIO_TypeDef* ports[8];
};
typedef struct _seven_segment_pin_t seven_segment_pin;

struct _seven_segment_digit_t{
	uint16_t pins[4]; //d1,d2,d3,d4
	GPIO_TypeDef* ports[4];
};
typedef struct _seven_segment_digit_t seven_segment_digit_t;

struct _seven_segmet_t{
	seven_segment_pin *pins;
	seven_segment_digit_t *digits;
	uint16_t numbers[4]; //d1,d2,d3,d4
	uint8_t dp_state[4]; //d1,d2,d3,d4
	int wait_time;
	uint8_t inverse;
	TIM_HandleTypeDef *htim;
	uint32_t counter;

};
typedef struct _seven_segmet_t seven_segment_t;

void seven_segment_isr_routine(seven_segment_t *display);

void init_seven_segment(seven_segment_t *seg, seven_segment_pin *pin, seven_segment_digit_t *digit, TIM_HandleTypeDef *htim);
void init_seven_segment_digit(seven_segment_digit_t *dig,
		GPIO_TypeDef* portd1, uint16_t pind1,
		GPIO_TypeDef* portd2, uint16_t pind2,
		GPIO_TypeDef* portd3, uint16_t pind3,
		GPIO_TypeDef* portd4, uint16_t pind4);
void init_seven_segment_pin(seven_segment_pin *pin,
		GPIO_TypeDef* porta, uint16_t pina,
		GPIO_TypeDef* portb, uint16_t pinb,
		GPIO_TypeDef* portc, uint16_t pinc,
		GPIO_TypeDef* portd, uint16_t pind,
		GPIO_TypeDef* porte, uint16_t pine,
		GPIO_TypeDef* portf, uint16_t pinf,
		GPIO_TypeDef* portg, uint16_t ping,
		GPIO_TypeDef* portdp, uint16_t pindp);

void write_number_to_digit(seven_segment_t *seg, uint8_t digit_number, uint8_t number);
void activate_one_digit_only(seven_segment_t *seg, uint8_t digit_number);
void write_segment_all(seven_segment_t *seg);
void write_segment_to_one_digit(seven_segment_t *seg, uint8_t digit_number);
void set_display_numbers_from_number(seven_segment_t *seg, uint32_t number);

#endif /* MK_7SEGDISP_H_ */
