#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <avr/io.h>
#include "lcd_driver.h"
#include "port_macros.h"

#define F_CPU 20000000
#define PWM_TOP 255
#define INCREMENT 85

int main() {
  DDRB &= ~((1<<1)| (1<<4) | (1<<5)); //configure push buttons
	unsigned int last_left_button_state = (PINB & (1<<1));
	unsigned int left_button_pressed = 0;
	unsigned int last_right_button_state = (PINB & (1<<5));
	unsigned int right_button_pressed = 0;
	unsigned int last_middle_button_state = (PINB & (1<<4));
	unsigned int middle_button_pressed = 0;

	initialize_LCD_driver();
	LCD_move_cursor_to_col_row(0, 0);
	LCD_execute_command(TURN_ON_DISPLAY);

  //PWM INITIAL
  unsigned int pwm_counter=0;
	unsigned int duty_cycle=50;
  //Configure Motors
	DDRD |= (1<<3) | (1<<5) | (1<<6); //configure pin as output
	DDRB |= (1<<3);

  //speed picking
  int s = 1;
  int speed = -1;
  int delta = 1;
  while (speed < 0){
    //left button
    if( (PINB & (1<<1)) != last_left_button_state ){
      if( (PINB & (1<<1)) == 0 ){
        left_button_pressed=1;
      }
      last_left_button_state = (PINB & (1<<1));
      }
      else{
        left_button_pressed=0;
      }

      //right button
      if( (PINB & (1<<5)) != last_right_button_state ){
        if( (PINB & (1<<5)) == 0 ){
          right_button_pressed=1;
        }
        last_right_button_state = (PINB & (1<<5));
      }
      else{
        right_button_pressed=0;
      }

      //middle button
      if( (PINB & (1<<4)) != last_middle_button_state ){
        if( (PINB & (1<<4)) == 0 ){
          middle_button_pressed=1;
        }
        last_middle_button_state = (PINB & (1<<4));
      }
      else{
        middle_button_pressed=0;
      }
    if (delta==1){
      _delay_us(10000);
      LCD_execute_command(CLEAR_DISPLAY);
      if(s == 0){
        LCD_print_String("Slow");
      }
      if(s == 1){
        LCD_print_String("Med");
      }
      if(s == 2){
        LCD_print_String("Fast");
      }
      delta = 0;
    }
    if((right_button_pressed == 1) && s<2 ){
      s++; //upper boundaries
      delta=1;
    }
    if((left_button_pressed == 1) && s>0 ){
      s--; //lower boundaries
      delta=1;
    }
    if(middle_button_pressed==1){
      speed= s;
    }
    _delay_us(100);
  }
//direction picking
	_delay_us(30000);
	LCD_execute_command(CLEAR_DISPLAY);
	int d= 0; // forward rev cw ccw
	int dir= -1;
	delta=1;
	while (dir <0){
		//left button
		if( (PINB & (1<<1)) != last_left_button_state ){
			if( (PINB & (1<<1)) == 0 ){
				left_button_pressed=1;
			}
			last_left_button_state = (PINB & (1<<1));
		}
		else{
			left_button_pressed=0;
		}
	
		//right button
		if( (PINB & (1<<5)) != last_right_button_state ){
			if( (PINB & (1<<5)) == 0 ){
				right_button_pressed=1;
			}
			last_right_button_state = (PINB & (1<<5));
		}
		else{
			right_button_pressed=0;
		}
	
		//middle button
		if( (PINB & (1<<4)) != last_middle_button_state ){
			if( (PINB & (1<<4)) == 0 ){
				middle_button_pressed=1;
			}
			last_middle_button_state = (PINB & (1<<4));
		}
		else{
			middle_button_pressed=0;
		}
		if (delta==1){
			_delay_us(10000);
			LCD_execute_command(CLEAR_DISPLAY);
			if(d ==0){
				LCD_print_String("Forw");
			}
			if(d ==1){
				LCD_print_String("Rev");
			}
			if(d ==2){
				LCD_print_String("CW");
			}
			if(d ==3){
				LCD_print_String("CCW");
			}
			delta=0;
		}
		if((right_button_pressed == 1) && d<3 ){
			d++; //upper boundaries
			delta=1;
		}
		if((left_button_pressed == 1) && d>0 ){
			d--; //lower boundaries
			delta=1;
		}
		if(middle_button_pressed==1){
			dir= d;
		}
	}
		//time selection
		_delay_us(30000);
		LCD_execute_command(CLEAR_DISPLAY);
		int16_t t= 0; //
		int16_t time= -1;
		char dec;
		char whole[1000];
		char tchar[10000];
		delta=1;
		while (time <0){
			//left button
			if( (PINB & (1<<1)) != last_left_button_state ){
				if( (PINB & (1<<1)) == 0 ){
					left_button_pressed=1;
				}
				last_left_button_state = (PINB & (1<<1));
			}
			else{
				left_button_pressed=0;
			}
			//right button
			if( (PINB & (1<<5)) != last_right_button_state ){
				if( (PINB & (1<<5)) == 0 ){
					right_button_pressed=1;
				}
				last_right_button_state = (PINB & (1<<5));
			}
			else{
				right_button_pressed=0;
			}
			//middle button
			if( (PINB & (1<<4)) != last_middle_button_state ){
				if( (PINB & (1<<4)) == 0 ){
					middle_button_pressed=1;
				}
				last_middle_button_state = (PINB & (1<<4));
			}
			else{
				middle_button_pressed=0;
			}
			
			if (delta==1){
				_delay_us(10000);
				LCD_execute_command(CLEAR_DISPLAY);
				sprintf(tchar, "%d", t); //converts time int to char
				dec = tchar[strlen(tchar)-1]; //take out dec
				tchar[strlen(tchar)-1] = '\0'; //nulls decimal in time string
				sprintf(whole, "T= %s . %c", tchar, dec); //put together
				LCD_print_String(whole);
				delta=0;
			}
			if(right_button_pressed == 1){
				t= t+1; //increment
				delta=1;
			}
			if(left_button_pressed == 1 && t>0){
				t= t-1; //lower boundaries
				delta=1;
			}
			if(middle_button_pressed==1){
				time= t;
			}
		}

  if( speed == 0 ){
    duty_cycle = PWM_TOP*0.2;
  }
  if( speed == 1 ){
    duty_cycle = PWM_TOP*0.4;
  }
  if( speed == 2 ){
    duty_cycle = PWM_TOP*0.65; //fast higher
  }
  
//duration
	time = time * 1000; // micro s to  ms conversion 1.1 s 
	int z= 60; //
	t=time;

	while (1) {
	z--;
	if( z==0){ t--;
		z=60; 
	}
	if (t==0){
		return 0; // stops loop
	}
    pwm_counter++;
    if( pwm_counter >= PWM_TOP ){
      pwm_counter = 0;
    }
    if( pwm_counter < duty_cycle ){
      if (dir==0){ //forward motion
		  PORTD |= (1<<6); 
		  PORTD &= ~((1<<5) | (1<<3));
		  PORTB |= (1<<3);
	  }
	  else if (dir==1){ //rev motion
		  PORTD &= ~(1<<6); 
		  PORTD |= ((1<<5) | (1<<3));
		  PORTB &= ~(1<<3);
	  }
	  else if (dir==2){ //cw
		  PORTD |= (1<<6); //m1 forward
		  PORTD &= ~(1<<5);
		  //PORTD |= (1<<3); //m2 back
		  //PORTB &= ~(1<<3);
	  }
	  else if (dir == 3){ //ccw
		  PORTD &= ~(1<<6); //m1 back
		  PORTD |= (1<<5);
		  //PORTD &= ~(1<<3); //m2 forward
		  //PORTB |= (1<<3);
	  }
	}
    else{
      PORTD &= ~((1<<3) | (1<<5) | (1<<6)); //coasting
      PORTB &= ~(1<<3);
    }
    //delay_us(100);
  }


  return 0;
}
