// Programme d'initiation :
// Programme qui allume les DELs lorsque le bouton du � joystick � est appuy� et 
// affiche le message � DEL en fonction � sur l��cran LCD et �teint les DELs lorsque 
// le bouton du � joystick � est rel�ch� et affiche le message � DEL hors fonction �.

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "utils.h"
#include "lcd.h"

int main(void){
	
	bool button_state; // Variable qui contiendra l'�tat du bouton
	
	DDRD = clear_bit(DDRD, PD3); // Mettre la broche PD3 en entr�e pour le bouton du joystick
	
	PORTD = set_bit(PORTD, PD3); // On doit activer la "pull-up" interne de la broche pour 
								 // forcer un �tat haut quand le bouton n'est pas enfonc�
	
	DDRB = set_bits(DDRB, 0b00011111); // Mettre les cinq premiers bits du port B en 
									   //sortie pour les DELs
	
	lcd_init(); // Faire l'initialisation du LCD
	
	// Dans une boucle infinie
	while(1){
		
		lcd_clear_display(); // On efface l'�cran LCD
		
		button_state = read_bit(PIND, PD3); // On r�cup�re l'�tat du bouton
		
		// Si le bouton est enfonc� (il ne faut pas oublier qu'en appuyant
		// sur le bouton, on connecte la broche du microcontr�leur � la masse,
		// donc on g�n�re un 0 (ou FALSE))
		if(button_state == FALSE){
			
			PORTB = set_bits(PORTB, 0b00011111); // On met en fonction les DELs
			
			lcd_write_string("DELs en fonction"); // On �crit sur le LCD
		}
		
		// Si le bouton n'est pas enfonc�
		else{
			PORTB = clear_bits(PORTB, 0b00011111); // On met hors fonction les DELs

			lcd_write_string("DELs hors       fonction"); // On �crit sur le LCD
		}
		
		_delay_ms(100); // On attend un petit d�lai sinon la boucle irait trop vite
	}
}