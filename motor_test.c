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
	initialize_LCD_driver();
	LCD_move_cursor_to_col_row(0, 0);
	LCD_execute_command(TURN_ON_DISPLAY);


  //Configure Motors
	DDRD |= (1<<3) | (1<<5) | (1<<6); //configure pin as output
	DDRB |= (1<<3);
  int t1,t2,t3,t4 =0;
  int speed1,speed2,speed3,speed4 =0;
  int dir1,dir2,dir3,dir4 =0;
  LCD_execute_command(CLEAR_DISPLAY);
  LCD_print_String("CMD 1");
  _delay_us(1000000);
  //speed picking
  speed1 = s_sel();
	_delay_us(10000);
	LCD_execute_command(CLEAR_DISPLAY);
  dir1 = dir_sel(); //direction picking
	_delay_us(10000);
	LCD_execute_command(CLEAR_DISPLAY);
    t1 = t_sel(); 	//time selection
  _delay_us(10000);
  //cmd 2
  LCD_execute_command(CLEAR_DISPLAY);
  LCD_print_String("CMD 2");
  _delay_us(1000000);
  int sel = sel_sel();
  _delay_us(1000);
  if (sel){
    speed2 = s_sel();   //speed picking
    _delay_us(1000);  //direction picking
    LCD_execute_command(CLEAR_DISPLAY);
    dir2 = dir_sel();
    _delay_us(1000); 	//time selection
    LCD_execute_command(CLEAR_DISPLAY);
    t2 = t_sel();
    //cmd 3
    LCD_execute_command(CLEAR_DISPLAY);
    LCD_print_String("CMD 3");
    _delay_us(1000000);
    sel=0;
    sel = sel_sel();
    _delay_us(1000000);
      if (sel){
        speed3 = s_sel();   //speed picking
        _delay_us(100);  //direction picking
        LCD_execute_command(CLEAR_DISPLAY);
        dir3 = dir_sel();
        _delay_us(1000); 	//time selection
        LCD_execute_command(CLEAR_DISPLAY);
        t3 = t_sel();
        //cmd 4
        LCD_execute_command(CLEAR_DISPLAY);
        LCD_print_String("CMD 4");
        _delay_us(1000000);
        sel=0;
        sel = sel_sel();
        _delay_us(1000);
        if (sel){
          speed4 = s_sel();   //speed picking
          _delay_us(100);  //direction picking
          LCD_execute_command(CLEAR_DISPLAY);
          dir4 = dir_sel();
          _delay_us(1000); 	//time selection
          LCD_execute_command(CLEAR_DISPLAY);
          t4 = t_sel();
          }
        else if (sel==0 && t4==0){
          LCD_execute_command(CLEAR_DISPLAY);
          LCD_print_String("Go");
          t4=0;
          speed4=0;
          dir4 =0;
        }
      }
      else{
        LCD_execute_command(CLEAR_DISPLAY);
        LCD_print_String("Go");
        t3=0;
        speed3=0;
        dir3 =0;
        t4=0;
        speed4=0;
        dir4 =0;
      }
    }
    else{
      LCD_execute_command(CLEAR_DISPLAY);
      LCD_print_String("Go");
      t2=0;
      speed2=0;
      dir2 =0;
      t3=0;
      speed3=0;
      dir3 =0;
      t4=0;
      speed4=0;
      dir4 =0;
    }

  _delay_us(10000);

  go(speed1, dir1, t1);
  go(speed2, dir2, t2);
  go(speed3, dir3, t3);
  go(speed4, dir4, t4);
  return 0;
}

int s_sel(){
  DDRB &= ~((1<<1)| (1<<4) | (1<<5)); //configure push buttons
	unsigned int last_left_button_state = (PINB & (1<<1));
	unsigned int left_button_pressed = 0;
	unsigned int last_right_button_state = (PINB & (1<<5));
	unsigned int right_button_pressed = 0;
	unsigned int last_middle_button_state = (PINB & (1<<4));
	unsigned int middle_button_pressed = 0;
  int s = 1; // selection starts at medium then can scroll for slow fast
  int speed = -1; //speed options are 0 1 2 for slow med fast
  int delta = 1; //change variable
  while (speed < 0){ //until speed is chosen
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
  DDRB &= ~((1<<1)| (1<<4) | (1<<5)); //configure push buttons
	unsigned int last_left_button_state = (PINB & (1<<1));
	unsigned int left_button_pressed = 0;
	unsigned int last_right_button_state = (PINB & (1<<5));
	unsigned int right_button_pressed = 0;
	unsigned int last_middle_button_state = (PINB & (1<<4));
	unsigned int middle_button_pressed = 0;
  int d= 0; // forward rev cw ccw
  int dir= -1;
  int delta=1;
  while (dir <0){ //loops until player chooses direction
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
      return d;
    }
    _delay_us(100);
  }
  return 0;
}

int t_sel(){
  DDRB &= ~((1<<1)| (1<<4) | (1<<5)); //configure push buttons
	unsigned int last_left_button_state = (PINB & (1<<1));
	unsigned int left_button_pressed = 0;
	unsigned int last_right_button_state = (PINB & (1<<5));
	unsigned int right_button_pressed = 0;
	unsigned int last_middle_button_state = (PINB & (1<<4));
	unsigned int middle_button_pressed = 0;
  int16_t t= 0; //16 bit t integer
  int16_t time= -1; //16 bit time integer for time
  char dec; //decimal of time string
  char whole[1000]; //complete time string
  char tchar[10000]; //time string
  int delta=1;
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
    _delay_us(100);
  }
  return 0;
}

int sel_sel(){
  DDRB &= ~((1<<1)| (1<<4) | (1<<5)); //configure push buttons
  unsigned int last_left_button_state = (PINB & (1<<1));
  unsigned int left_button_pressed = 0;
  unsigned int last_right_button_state = (PINB & (1<<5));
  unsigned int right_button_pressed = 0;
  unsigned int last_middle_button_state = (PINB & (1<<4));
  unsigned int middle_button_pressed = 0;
  int s= 0; // forward rev cw ccw
  int sel= -1;
  int delta=1;
  while (sel <0){ //loops until player chooses direction
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
  unsigned int pwm_counter=0;
  unsigned int duty_cycle=50;
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
		  PORTD |= (1<<3); //m2 back
		  PORTB &= ~(1<<3);
	  }
	  else if (dir == 3){ //ccw
		  PORTD &= ~(1<<6); //m1 back
		  PORTD |= (1<<5);
		  PORTD &= ~(1<<3); //m2 forward
		  PORTB |= (1<<3);
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
