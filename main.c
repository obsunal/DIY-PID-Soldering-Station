/*
 * atmel_PID.c
 *
 * Created: 2.07.2019 00:33:23
 * Author : Onur Berk
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#define F_CPU 8000000
#define menu_key PD5
#define down_key PD6
#define up_key  PD7

//values for PID
uint8_t pwm_value;
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
void set_pwm(int8_t pwm_value);

char state = "Booting...";

int main(void) {
	DDRB |= (1 << DDB3);	//Set PWM pin output
	
	DDRD &= ~(1 << DDD2) & ~(1 << DDD5) & ~(1 << DDD6) & ~(1 << DDD7);	//Interrupt 1 and ok key, up key, down key  input
	PORTD |= (1 << PD2) & ~(1 << menu_key) & ~(1 << down_key) & ~(1 << up_key);		//Interrupt 1 and PD5 pull-up
	
	MCUCR |= (1 << ISC00);	//make interrupt 0's mode logic change
	GICR |= (1 << INT0);	//Assign Interrupt 0
	sei();					//Starts Interrupts
	
	state = "Ready!";
	
	while ((PIND & (1 << PIND5)) > 0) {
		
	}
	
	while (1) {
		current_pos = read_pos();
		p_error = goal - current_pos;
		i_error += p_error;
		d_error = p_error - last_p;
		
		pwm_value = (kp * p_error) + (ki * i_error) + (kd * d_error);
		
		set_pwm(pwm_value);
		
		last_p = p_error;
    }
}

ISR (INT0_vect) {
	if((PIND & (1 << PIND2)) == 0) {
		goal = idle;
	}
	else {
		goal = target;
	}
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

void set_pwm(int8_t pwm_value) {
	
}