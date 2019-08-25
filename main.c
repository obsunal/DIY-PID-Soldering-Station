/*
 * atmel_PID.c
 *
 * Created: 2.07.2019 00:33:23
 * Author : Onur Berk
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#define menu_key PD5
#define down_key PD6
#define up_key  PD7
#define pwm_pin1 PB1
#define pwm_pin2 PB2

//values for PID
uint16_t pwm_value;
int current_pos;

int goal = 270;
int idle = 50;
int target = 270;
	
double p_error;
double i_error;
double d_error;
double last_p;
	
double kp;
double ki;
double kd;

int read_pos();
void set_pwm(uint16_t pwm_value);

bool is_idle = false;

char state = "Booting...";

int main(void) {
	DDRB = 0b00000110;		//PB1 & PB2 Output
	DDRD = 0b00000000;		//Interrupts and Buttons Input
	PORTD = 0b11100100;		//Pull-Ups
	
	EICRA = 0b00000001;		//make interrupt 0's mode logic change
	EIMSK = 0b00000001;		//Assign Interrupt 0
	sei();					//Starts Interrupts
	
	ICR1 = 0xFFFF;			//Set TOP value for fast PWM
	OCR1A = 0x0000;			//Clear PB1 PWM
	OCR1B = 0x0000;			//Clear PB2 PWM
	
	TCCR1B = 0b00011001;	//Enable timer1 with 0 pre-scaler and set 2 bits of waveform generation mode
	TCCR1A = 0b10100010;	//Set Compare mode and wave generation mode
	ICR1 = 0xFFFF;			//Set Top value.
	
	
	state = "Ready!";
	
	while ((PIND & (1 << PIND5)) > 0) {
		
	}
	
	while (1) {
		current_pos = read_pos();
		p_error = goal - current_pos;
		i_error += p_error;
		d_error = p_error - last_p;
		
		pwm_value = (kp * p_error) + (ki * i_error) + (kd * d_error);
		
		OCR1A = pwm_value;		//sets the PWM value;
		
		last_p = p_error;
    }
}

ISR (INT0_vect) {
	is_idle = (PIND >> PIND2);
}

int read_pos() {
	int adc_value;
	ADMUX = 0b11000011;					//Choose ADC and Configure Internal Reference
	ADCSRA = 0b00000111;				//Choose pre-scaler to be 16
	ADMUX |= (1 << ADEN);				//Start ADC
	ADCSRA |= (1 << ADSC);				//Start Conversation
	while(ADCSRA & (1 << ADSC));		//Wait for Conversation to end
	adc_value = ADCL;					//Can be combined with next line but
	adc_value = (ADCH << 8) + adc_value	//IDK how compiler works, so don't want to loose reading
	return adc_value;					//Because of compiler optimizations.
}