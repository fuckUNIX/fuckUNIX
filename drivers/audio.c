// This driver was pretty much copy pasted from the osdev wiki go check it out!!

#include "../kernel/util.h"
#include "ports.h"
#include <stdint.h>

//Play sound using built-in speaker
 static void play_sound(uint32_t nFrequence) {
 	uint32_t Div;
 	uint8_t tmp;
 
        //Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	port_byte_out(0x43, 0xb6);
 	port_byte_out(0x42, (uint8_t) (Div) );
 	port_byte_out(0x42, (uint8_t) (Div >> 8));
 
        //And play the sound using the PC speaker
 	tmp = port_byte_in(0x61);
  	if (tmp != (tmp | 3)) {
 		port_byte_out(0x61, tmp | 3);
 	}
 }
 
 //make it shut up
 static void nosound() {
 	uint8_t tmp = port_byte_in(0x61) & 0xFC;
     
 	port_byte_out(0x61, tmp);
 }
 
 //Make a beep
 void beep(uint32_t freq) {
 	 play_sound(freq);
 	 delay(100);
 	 nosound();
          //set_PIT_2(old_frequency);
 }