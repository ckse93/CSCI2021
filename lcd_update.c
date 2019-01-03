#include "lcd.h"

int set_tod_from_secs(int time_of_day_sec, tod_t *tod)
{
 if(time_of_day_sec < 0 || time_of_day_sec > 86400)
 {
  //printf ("time of sex error\n");
  return 1; //error
 }
 else
 {
  tod->hours = (time_of_day_sec/3600)%12;

  if(tod->hours == 0)
  {
   tod->hours = 12;
  }
  tod->ispm = time_of_day_sec/43200;
  tod->minutes = ((time_of_day_sec % 3600)/60);
  tod->seconds = (time_of_day_sec % 60);
  return 0;
 }
}

int set_display_bits_from_tod(tod_t tod, int *display)
{
 if(tod.hours > 12 || tod.hours < 0 || tod.minutes < 0 || tod.seconds < 0 || tod.minutes > 59 || tod.seconds > 59)
 {
  //printf("tod variable error\n");
  return 1; // error
 }
 else
 {
  int arr[11] = {0b0111111, 0b0000110, 0b1011011, 
		0b1001111, 0b1100110, 0b1101101, 
		0b1111101, 0b0000111, 0b1111111, 
		0b1101111, 0b0000000}; //index = 0 1 2 3 4 5 6 7 8 9 10 corrosponds to the lcd display
  //update, need 000000000?
  int disp_bit = 0b00000000000000000000000000000000; // for the lcd
  int h_10 = tod.hours/10;
  int h_1 = tod.hours%10;
  int m_10 = tod.minutes/10;
  int m_1 = tod.minutes%10;
  int s_10 = tod.seconds/10;
  int s_11 = tod.seconds%10;
  h_10 = arr[h_10];
  h_1 = arr[h_1];
  m_10 = arr[m_10];
  m_1 = arr[m_1];
  s_10 = arr[s_10];
  s_11 = arr[s_11];
  if(tod.ispm >= 1 && tod.ispm < 2)
  {
   tod.ispm = 0b0010;  // set to pm
  }
  else
  {
   tod.ispm = 0b0001;  // set to am
 }
  disp_bit = disp_bit << 4 | tod.ispm; // push 4 bits, add ispm

  if(h_10 == arr[0])
  {
   disp_bit = disp_bit << 7 | arr[10]; // add blank if h_10 == 0
  }
  else
  {
   disp_bit = disp_bit << 7 | h_10;// push 7 bits add 1 in hour tenth place
  }
  disp_bit = disp_bit << 7 | h_1;// push 7 bits add hour
  disp_bit = disp_bit << 7 | m_10; // push 7 bits add min
  disp_bit = disp_bit << 7 | m_1; // push 7 bits add min
  *display = disp_bit;
  return 0; // success
  }
}

int lcd_update()
{
  //printf("running lcd_update...\n");
 tod_t tod;
 if(TIME_OF_DAY_SEC < 0 || TIME_OF_DAY_SEC > 86400)
 {
  //printf("error in update fnc")
  return 1;
 }
 else
 {
  //printf("running lcd_update_else loop");
  set_tod_from_secs(TIME_OF_DAY_SEC, &tod);
  set_display_bits_from_tod(tod, &LCD_DISPLAY_PORT);
  return 0;
 }
}
