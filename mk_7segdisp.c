/**
 *  @file mk_7segdisp.c
 *	@brief 4 Digit 7 Segment Display Library
 *  @date Created on: Oct 11, 2019
 *  @author Author: mesut.kilic
 *	@version 1.0.0
 */

#include "mk_7segdisp.h"

__weak void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

}

void seven_segment_isr_routine(seven_segment_t *display){
	//timer is 2ms
	if(display->counter%250 == 0) display->dp_state[1] = !display->dp_state[1];
	int digit=display->counter%4+1;

	activate_one_digit_only(display, digit);

	HAL_TIM_Base_Start_IT(display->htim);

	write_segment_to_one_digit(display, digit);
	display->counter++;

	set_display_numbers_from_number(display, HAL_GetTick()/1000);
}



void init_seven_segment(seven_segment_t *seg, seven_segment_pin *pin, seven_segment_digit_t *digit, TIM_HandleTypeDef *htim){
	seg->digits = digit;
	seg->pins = pin;
	seg->inverse = 1;
//	for(int i=0; i<4; ++i){	seg->numbers[i] = 0; }
	seg->wait_time=1;
	seg->htim = htim;
}

void init_seven_segment_digit(seven_segment_digit_t *dig,
		GPIO_TypeDef* portd1, uint16_t pind1,
		GPIO_TypeDef* portd2, uint16_t pind2,
		GPIO_TypeDef* portd3, uint16_t pind3,
		GPIO_TypeDef* portd4, uint16_t pind4){
	dig->ports[0] = portd1;
	dig->ports[1] = portd2;
	dig->ports[2] = portd3;
	dig->ports[3] = portd4;

	dig->pins[0] = pind1;
	dig->pins[1] = pind2;
	dig->pins[2] = pind3;
	dig->pins[3] = pind4;
}

void init_seven_segment_pin(seven_segment_pin *pin,
		GPIO_TypeDef* porta, uint16_t pina,
		GPIO_TypeDef* portb, uint16_t pinb,
		GPIO_TypeDef* portc, uint16_t pinc,
		GPIO_TypeDef* portd, uint16_t pind,
		GPIO_TypeDef* porte, uint16_t pine,
		GPIO_TypeDef* portf, uint16_t pinf,
		GPIO_TypeDef* portg, uint16_t ping,
		GPIO_TypeDef* portdp, uint16_t pindp){

	pin->ports[0] = porta;
	pin->ports[1] = portb;
	pin->ports[2] = portc;
	pin->ports[3] = portd;
	pin->ports[4] = porte;
	pin->ports[5] = portf;
	pin->ports[6] = portg;
	pin->ports[7] = portdp;


	pin->pins[0] = pina;
	pin->pins[1] = pinb;
	pin->pins[2] = pinc;
	pin->pins[3] = pind;
	pin->pins[4] = pine;
	pin->pins[5] = pinf;
	pin->pins[6] = ping;
	pin->pins[7] = pindp;
}

void write_number_to_digit(seven_segment_t *seg, uint8_t digit_number, uint8_t number){
	seg->numbers[digit_number-1] = number;
}

void activate_one_digit_only(seven_segment_t *seg, uint8_t digit_number){
	HAL_GPIO_WritePin(seg->digits->ports[0], seg->digits->pins[0], 1);
	HAL_GPIO_WritePin(seg->digits->ports[1], seg->digits->pins[1], 1);
	HAL_GPIO_WritePin(seg->digits->ports[2], seg->digits->pins[2], 1);
	HAL_GPIO_WritePin(seg->digits->ports[3], seg->digits->pins[3], 1);

	HAL_GPIO_WritePin(seg->digits->ports[digit_number-1], seg->digits->pins[digit_number-1], 0);
}

void write_segment_all(seven_segment_t *seg){
	write_segment_to_one_digit(seg, 1);
	HAL_Delay(seg->wait_time);
	write_segment_to_one_digit(seg, 2);
	HAL_Delay(seg->wait_time);
	write_segment_to_one_digit(seg, 3);
	HAL_Delay(seg->wait_time);
	write_segment_to_one_digit(seg, 4);
	HAL_Delay(seg->wait_time);
}

void write_segment_to_one_digit(seven_segment_t *seg, uint8_t digit_number){
	activate_one_digit_only(seg, digit_number);
	for(int i=0; i<7; ++i){
		// add inverse option
		if(seg->inverse == 1){
			HAL_GPIO_WritePin(seg->pins->ports[i], seg->pins->pins[i], !segment_numbers[seg->numbers[digit_number-1]][i]);
		}else {
			HAL_GPIO_WritePin(seg->pins->ports[i], seg->pins->pins[i], segment_numbers[seg->numbers[digit_number-1]][i]);
		}
	}
	//@todo add inverse option
	if(seg->inverse == 1){
		HAL_GPIO_WritePin(seg->pins->ports[7], seg->pins->pins[7], !seg->dp_state[digit_number-1]); //dp
	}else {
		HAL_GPIO_WritePin(seg->pins->ports[7], seg->pins->pins[7], seg->dp_state[digit_number-1]); //dp
	}

}

void set_display_numbers_from_number(seven_segment_t *seg, uint32_t number){
	seg->numbers[3]=number%10;
	number/=10;
	seg->numbers[2]=number%10;
	number/=10;
	seg->numbers[1]=number%10;
	number/=10;
	seg->numbers[0]=number%10;
}
