/**
__ ___  __
|_   |  (_
|__  |  __)

MIT License

Copyright (c) 2018	École de technologie supérieure

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
	
	// Variable qui contiendra l'état du bouton
	bool button_state;
	
	// Mettre la patte PD3 en entrée pour le bouton du joystick
	DDRD = clear_bit(DDRD, PD3);
	
	// On doit activer la "pull-up" interne de la patte pour forcer un état
	// haut quand le bouton n'est pas enfoncé
	PORTD = set_bit(PORTD, PD3);
	
	// Mettre les cinq premier bits du port B en sortie pour les DELs
	DDRB = set_bits(DDRB, 0b00011111);
	
	// Faire l'initialisation du lcd
	lcd_init();
	
	// Dans une boucle infinie
	while(1){
		
		// On efface l'écran LCD
		lcd_clear_display();
		
		// On récupère l'état du bouton
		button_state = read_bit(PIND, PD3);
		
		// Si le bouton est enfoncé (il ne faut pas oublier qu'en appuyant
		// sur le bouton, on connecte la patte du microcontrôleur à la masse,
		// donc on génère un 0 (ou FALSE))
		if(button_state == FALSE){
			
			// On met en fonction les DELs
			PORTB = set_bits(PORTB, 0b00011111);
			
			// On écrit sur le LCD
			lcd_write_string("DELs en fonction");
		}
		
		// Si le bouton n'est pas enfoncé
		else{
			
			// On met hors fonction les DELs
			PORTB = clear_bits(PORTB, 0b00011111);
			
			// On écrit sur le LCD
			lcd_write_string("DELs hors       fonction");
		}
		
		// On attend un petit délai sinon cette boucle irait beaucoup trop vite
		_delay_ms(100);
	}
}