#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <avr/io.h>
#include "lcd_driver.h"
#include "port_macros.h"

#define F_CPU 20000000
#define PWM_TOP_L 255
#define PWM_TOP_R 255
#define INCREMENT 85

#define PD3 (1<<3)
#define PD5 (1<<5)
#define PD6 (1<<6)
#define PB3 (1<<3)
#define BUTTONA (1<<1)
#define BUTTONB (1<<4)
#define BUTTONC (1<<5)

int s_sel();
int dir_sel();
int sel_sel();
int go();
int t_sel();

int main() {
	initialize_LCD_driver();
	LCD_move_cursor_to_col_row(0, 0);
	LCD_execute_command(TURN_ON_DISPLAY);

  //Configure Motors
	DDRD |= PD3 | PD5 | PD6; //configure pin as output
	DDRB |= PB3;
  int t1, t2, t3, t4 = 0;
  int speed1, speed2, speed3, speed4 = 0;
  int dir1, dir2, dir3, dir4 = 0;

	//cmd 1
  LCD_execute_command(CLEAR_DISPLAY);
  LCD_print_String("CMD 1");
  _delay_us(1000000);
  //speed picking
  speed1 = s_sel();
	_delay_us(1000);
	LCD_execute_command(CLEAR_DISPLAY);
	_delay_us(100);
  dir1 = dir_sel();
	//direction picking
	_delay_us(1000);
	LCD_execute_command(CLEAR_DISPLAY);
	_delay_us(100);
  t1 = t_sel();
	//time selection
	_delay_us(1000);

  //cmd 2
  LCD_execute_command(CLEAR_DISPLAY);
	_delay_us(100);
  LCD_print_String("CMD 2");
  _delay_us(1000000);
  int sel = sel_sel();
  _delay_us(1000);
  if (sel){
    speed2 = s_sel();   //speed picking
    LCD_execute_command(CLEAR_DISPLAY);
		_delay_us(1000);
		//direction picking
    dir2 = dir_sel();
    _delay_us(1000);
		//time selection
    LCD_execute_command(CLEAR_DISPLAY);
		_delay_us(100);
    t2 = t_sel();
		_delay_us(1000);
    //cmd 3
    LCD_execute_command(CLEAR_DISPLAY);
    LCD_print_String("CMD 3");
    _delay_us(1000000);
    sel=0;
    sel = sel_sel();
    _delay_us(100000);
      if (sel){
        speed3 = s_sel();   //speed picking
        _delay_us(1000);  //direction picking
        LCD_execute_command(CLEAR_DISPLAY);
        dir3 = dir_sel();
        _delay_us(1000); 	//time selection
        LCD_execute_command(CLEAR_DISPLAY);
				_delay_us(100);
        t3 = t_sel();
				_delay_us(1000);
        //cmd 4
        LCD_execute_command(CLEAR_DISPLAY);
        LCD_print_String("CMD 4");
        _delay_us(1000000);
        sel=0;
        sel = sel_sel();
        _delay_us(1000);
        if (sel){ //four commands
          speed4 = s_sel();   //speed picking
          _delay_us(1000);  //direction picking
          LCD_execute_command(CLEAR_DISPLAY);
					_delay_us(100);
          dir4 = dir_sel();
          _delay_us(1000); 	//time selection
          LCD_execute_command(CLEAR_DISPLAY);
					_delay_us(100);
          t4 = t_sel();
					_delay_us(100000);
					LCD_execute_command(CLEAR_DISPLAY);
          LCD_print_String("Go");
					go(speed1, dir1, t1);
					go(speed2, dir2, t2);
					go(speed3, dir3, t3);
					go(speed4, dir4, t4);
					return 0; //stop program when done
          }
        else{ //three commands
					_delay_us(100000);
          LCD_execute_command(CLEAR_DISPLAY);
          LCD_print_String("Go");
					go(speed1, dir1, t1);
				  go(speed2, dir2, t2);
				  go(speed3, dir3, t3);
					  return 0;
        }
      }
      else{ //2 commands
				_delay_us(100000);
        LCD_execute_command(CLEAR_DISPLAY);
        LCD_print_String("Go");
				go(speed1, dir1, t1);
				go(speed2, dir2, t2);
				return 0;
      }
    }
    else{ // 1 command
			_delay_us(100000);
      LCD_execute_command(CLEAR_DISPLAY);
      LCD_print_String("Go");
			go(speed1, dir1, t1);
			return 0;
    }

  _delay_us(10000);

  return 0;
}

int s_sel(){
  DDRB &= ~(BUTTONA| BUTTONB | BUTTONC); //configure push buttons
	unsigned int last_left_button_state = (PINB & BUTTONA);
	unsigned int left_button_pressed = 0;
	unsigned int last_right_button_state = (PINB & BUTTONC);
	unsigned int right_button_pressed = 0;
	unsigned int last_middle_button_state = (PINB & BUTTONB);
	unsigned int middle_button_pressed = 0;
  int s = 1; // selection starts at medium then can scroll for slow fast
  int speed = -1; //speed options are 0 1 2 for slow med fast
  int delta = 1; //change variable
  while (speed < 0){ //until speed is chosen
    //left button
    if( (PINB & BUTTONA) != last_left_button_state ){
      if( (PINB & BUTTONA) == 0 ){
        left_button_pressed=1;
      }
      last_left_button_state = (PINB & BUTTONA);
      }
      else{
        left_button_pressed=0;
      }

      //right button
      if( (PINB & BUTTONC) != last_right_button_state ){
        if( (PINB & BUTTONC) == 0 ){
          right_button_pressed=1;
        }
        last_right_button_state = (PINB & BUTTONC);
      }
      else{
        right_button_pressed=0;
      }

      //middle button
      if( (PINB & BUTTONB) != last_middle_button_state ){
        if( (PINB & BUTTONB) == 0 ){
          middle_button_pressed=1;
        }
        last_middle_button_state = (PINB & BUTTONB);
      }
      else{
        middle_button_pressed=0;
      }
    if (delta==1){ //if user scrolls btwn options
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
      delta = 0; //display changes only when user scrolls
    }
    if((right_button_pressed == 1) && s<2 ){ //right button increases speed option
      s++; //upper boundaries
      delta=1;
    }
    if((left_button_pressed == 1) && s>0 ){ //left button decreases speed option
      s--; //lower boundaries
      delta=1;
    }
    if(middle_button_pressed==1){ //middle button as selection
      return s;
    }
    _delay_us(100);
  }
  return 0;
}

int dir_sel(){
  DDRB &= ~(BUTTONA| BUTTONB | BUTTONC); //configure push buttons	unsigned int last_left_button_state = (PINB & BUTTONA;
	unsigned int left_button_pressed = 0;
	unsigned int last_right_button_state = (PINB & BUTTONC);
	unsigned int right_button_pressed = 0;
	unsigned int last_middle_button_state = (PINB & BUTTONB);
	unsigned int middle_button_pressed = 0;
  int d= 0; // forward rev cw ccw
  int dir= -1;
  int delta=1;
  while (dir <0){ //loops until player chooses direction
    //left button
    if( (PINB & BUTTONA) != last_left_button_state ){
      if( (PINB & BUTTONA) == 0 ){
        left_button_pressed=1;
      }
      last_left_button_state = (PINB & BUTTONA);
    }
    else{
      left_button_pressed=0;
    }
  //right button
    if( (PINB & BUTTONC) != last_right_button_state ){
      if( (PINB & BUTTONC) == 0 ){
        right_button_pressed=1;
      }
      last_right_button_state = (PINB & BUTTONC);
    }
    else{
      right_button_pressed=0;
    }
  //middle button
    if( (PINB & BUTTONB) != last_middle_button_state ){
      if( (PINB & BUTTONB) == 0 ){
        middle_button_pressed=1;
      }
      last_middle_button_state = (PINB & BUTTONB);
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
      return d;
    }
    _delay_us(100);
  }
  return 0;
}

int t_sel(){
  DDRB &= ~(BUTTONA| BUTTONB | BUTTONC); //configure push buttons
	unsigned int last_left_button_state = (PINB & BUTTONA);
	unsigned int left_button_pressed = 0;
	unsigned int last_right_button_state = (PINB & BUTTONC);
	unsigned int right_button_pressed = 0;
	unsigned int last_middle_button_state = (PINB & BUTTONB);
	unsigned int middle_button_pressed = 0;
  int16_t t= 0; //16 bit t integer
  int16_t time= -1; //16 bit time integer for time
  char dec; //decimal of time string
  char whole[1000]; //complete time string
  char tchar[10000]; //time string
  int delta=1;
  while (time <0){
    //left button
    if( (PINB & BUTTONA) != last_left_button_state ){
      if( (PINB & BUTTONA) == 0 ){
        left_button_pressed=1;
      }
      last_left_button_state = (PINB & BUTTONA);
    }
    else{
      left_button_pressed=0;
    }
    //right button
    if( (PINB & BUTTONC) != last_right_button_state ){
      if( (PINB & BUTTONC) == 0 ){
        right_button_pressed=1;
      }
      last_right_button_state = (PINB & BUTTONC);
    }
    else{
      right_button_pressed=0;
    }
    //middle button
    if( (PINB & BUTTONB) != last_middle_button_state ){
      if( (PINB & BUTTONB) == 0 ){
        middle_button_pressed=1;
      }
      last_middle_button_state = (PINB & BUTTONB);
    }
    else{
      middle_button_pressed=0;
    }

    if (delta==1){ //only prints when user changes time
      _delay_us(10000);
      LCD_execute_command(CLEAR_DISPLAY);
      sprintf(tchar, "%d", t); //converts time int to char
      dec = tchar[strlen(tchar)-1]; //take out dec
      tchar[strlen(tchar)-1] = '\0'; //nulls decimal in time string
      sprintf(whole, "T= %s . %c", tchar, dec); //put together
      LCD_print_String(whole);
      delta=0;
    }
    if(right_button_pressed == 1){ //right button to increment time
      t= t+1; //increment
      delta=1;
    }
    if(left_button_pressed == 1 && t>0){ //left button to decrement time
      t= t-1; //lower boundaries
      delta=1;
    }
    if(middle_button_pressed==1){ //middle button as selection
      return t;
    }
    _delay_us(1000);
  }
  return 0;
}

int sel_sel(){
  DDRB &= ~(BUTTONA| BUTTONB | BUTTONC); //configure push buttons
	unsigned int last_left_button_state = (PINB & BUTTONA);
	unsigned int left_button_pressed = 0;
	unsigned int last_right_button_state = (PINB & BUTTONC);
	unsigned int right_button_pressed = 0;
	unsigned int last_middle_button_state = (PINB & BUTTONB);
	unsigned int middle_button_pressed = 0;
  int s= 0; // forward rev cw ccw
  int sel= -1;
  int delta=1;
  while (sel <0){ //loops until player chooses direction
    //left button
    if( (PINB & BUTTONA) != last_left_button_state ){
      if( (PINB & BUTTONA) == 0 ){
        left_button_pressed=1;
      }
      last_left_button_state = (PINB & BUTTONA);
    }
    else{
      left_button_pressed=0;
    }
    //right button
    if( (PINB & BUTTONC) != last_right_button_state ){
      if( (PINB & BUTTONC) == 0 ){
        right_button_pressed=1;
      }
      last_right_button_state = (PINB & BUTTONC);
    }
    else{
      right_button_pressed=0;
    }
  //middle button
    if( (PINB & BUTTONB) != last_middle_button_state ){
      if( (PINB & BUTTONB) == 0 ){
        middle_button_pressed=1;
      }
      last_middle_button_state = (PINB & BUTTONB);
    }
    else{
      middle_button_pressed=0;
    }
    if (delta==1){
      _delay_us(10000);
      LCD_execute_command(CLEAR_DISPLAY);
      if(s ==0){
        LCD_print_String("No");
      }
      if(s ==1){
        LCD_print_String("Yes");
      }
      delta=0;
    }
    if((right_button_pressed == 1) && s<1 ){
      s++; //upper boundaries
      delta=1;
    }
    if((left_button_pressed == 1) && s>0 ){
      s--; //lower boundaries
      delta=1;
    }
    if(middle_button_pressed==1){
      return s;
    }
    _delay_us(100);
  }
  return 0;
}

int go(int speed, int dir, int time){
  //PWM INITIAL
  unsigned int pwm_counter_L = 0;
	unsigned int pwm_counter_R = 0;
  unsigned int duty_cycle_L = 50;
	unsigned int duty_cycle_R = 50;
  if( speed == 0 ){
    duty_cycle_L = PWM_TOP_L*0.21;
		duty_cycle_R = PWM_TOP_R*0.2;
  }
  if( speed == 1 ){
    duty_cycle_L = PWM_TOP_L*0.4;
		duty_cycle_R = PWM_TOP_R*0.4;
  }
  if( speed == 2 ){
    duty_cycle_L = PWM_TOP_L*0.65;
		duty_cycle_R = PWM_TOP_R*0.65; //fast higher
  }
//duration
	time = time * 1000; // micro s to  ms conversion 1.1 s
	int z= 60;
	int t=time;

	while (1) {
	  z--;
	  if( z==0){ t--; //loops for time
	    z=60;
	  }
	  if (t==0){
	    return 0; // stops loop
	  }
	  pwm_counter_L++;
	  if( pwm_counter_L >= PWM_TOP_L ){
	    pwm_counter_L = 0;
	  }
	  if( pwm_counter_L < duty_cycle_L ){
	    if (dir==0){ //forward motion
	    PORTD |= PD6;
	    PORTD &= ~PD5;
	  } else if (dir==1) { //rev motion
	    PORTD &= ~PD6;
	    PORTD |= PD5;
	  } else if (dir==2) { //cw
	    PORTD |= PD6; //m1 forward
	    PORTD &= ~PD5;
	  } else if (dir == 3) { //ccw
	    PORTD &= ~PD6; //m1 back
	    PORTD |= PD5;
	  }
	  } else {
	    PORTD &= ~(PD5 | PD6); //coasting
	  }

	  pwm_counter_R++;
	  if( pwm_counter_R >= PWM_TOP_R ){
	    pwm_counter_R = 0;
	  }
	  if( pwm_counter_R < duty_cycle_R ){
	    if (dir==0){ //forward motion
	    PORTD &= ~PD3;
	    PORTB |= PB3;
	  } else if (dir==1) { //rev motion
	    PORTD |= PD3;
	    PORTB &= ~PB3;
	  } else if (dir==2) { //cw
	    PORTD |= PD3; //m2 back
	    PORTB &= ~PB3;
	  } else if (dir == 3) { //ccw
	    PORTD &= ~PD3; //m2 forward
	    PORTB |= PB3;
	  }
	  } else {
	    PORTD &= ~PD3; //coasting
	    PORTB &= ~PB3;
	  }
	}
  return 0;
}
