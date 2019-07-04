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

//values for PID
int8_t pwm_value;
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

double read_pos();
void set_pwm(int8_t pwm_value);

char state = "Booting";

int main(void) {
	DDRB |= (1 << DDB3);	//Set PWM pin output
	
	
	DDRD &= ~(1 << DDD2);	//Interrupt 1 input
	PORTD |= (1 << PD2);	//Interrupt 1 pull-up
	
	MCUCR |= (1 << ISC00);	//make interrupt 0's mode logic change
	GICR |= (1 << INT0);	//Assign Interrupt 0
	sei();					//Starts Interrupts
	
	
	
	set_pwm(255);
	
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
	if((PIND & (1 << PIND0)) == 0) {
		goal = idle;
	}
	else {
		goal = target;
	}
}