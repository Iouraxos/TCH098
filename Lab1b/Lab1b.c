/**
__ ___  __
|_   |  (_
|__  |  __)

MIT License

Copyright (c) 2018	�cole de technologie sup�rieure

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify and/or merge copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "utils.h"
#include "lcd.h"

int main(void){
	
	// Variable qui contiendra l'�tat du bouton
	bool button_state;
	
	// Mettre la patte PD3 en entr�e pour le bouton du joystick
	DDRD = clear_bit(DDRD, PD3);
	
	// On doit activer la "pull-up" interne de la patte pour forcer un �tat
	// haut quand le bouton n'est pas enfonc�
	PORTD = set_bit(PORTD, PD3);
	
	// Mettre les cinq premier bits du port B en sortie pour les DELs
	DDRB = set_bits(DDRB, 0b00011111);
	
	// Faire l'initialisation du lcd
	lcd_init();
	
	// Dans une boucle infinie
	while(1){
		
		// On efface l'�cran LCD
		lcd_clear_display();
		
		// On r�cup�re l'�tat du bouton
		button_state = read_bit(PIND, PD3);
		
		// Si le bouton est enfonc� (il ne faut pas oublier qu'en appuyant
		// sur le bouton, on connecte la patte du microcontr�leur � la masse,
		// donc on g�n�re un 0 (ou FALSE))
		if(button_state == FALSE){
			
			// On met en fonction les DELs
			PORTB = set_bits(PORTB, 0b00011111);
			
			// On �crit sur le LCD
			lcd_write_string("DELs en fonction");
		}
		
		// Si le bouton n'est pas enfonc�
		else{
			
			// On met hors fonction les DELs
			PORTB = clear_bits(PORTB, 0b00011111);
			
			// On �crit sur le LCD
			lcd_write_string("DELs hors       fonction");
		}
		
		// On attend un petit d�lai sinon cette boucle irait beaucoup trop vite
		_delay_ms(100);
	}
}