/**
	\file  driver.c
	\brief Code source de fonctions qui pilotent directement du mat�riel
	
	\author *********************************************************
	\author ***                                                   *** 
	\author ***               VOS NOMS ICI                        ***
	\author ***                                                   *** 
	\author *********************************************************
	\date XXXXX
*/

/******************************************************************************
Includes
******************************************************************************/

#include <avr/io.h>
#include "driver.h"


/******************************************************************************
D�finitions de fonctions
******************************************************************************/

void adc_init(void){
	
	// Configuration des broches utilis�es du port A en entr�e (Entre PA0 et PA7)	
	

	// S�lectionner la r�f�rence de tension: la tension d'alimentation (AVCC)


	// Choisir le format du r�sultat de conversion: shift � gauche pour que
	// les 8 MSB se retrouvent dans le registre ADCH
	

	// Choisir un facteur de division d'horloge (64) afin que l'horloge ait
	// une fr�quence entre 50kHz et 200kHz. Donc 8MHz/64 = 125kHz.


	// Activer le CAN
	
}

uint8_t adc_read(uint8_t pin_name){
	
	// Choisir l'entr�e analogique (broche) � convertir

	// D�marrage d'une conversion 
	
	// Attente de la fin de la conversion

	// Lecture et renvoie du r�sultat
	
}

void servo_init(void){
	
	
	// Configuration des broches de sortie


	// Configuration du compteur et du comparateur
	

	// Configuration de la valeur maximale du compteur (top) � 20000
	
	
	// Initialiser la valeur du compteur � 0


	// D�marrer le compteur et fixer un facteur de division de fr�quence � 8
	
}

void servo_set_a(int8_t angle){
	
	// Modification du rapport cyclique du PWM du servomoteur. Min = 600, Max = 2550
	
}

void servo_set_b(int8_t angle){
	
	// Modification du rapport cyclique du PWM du servomoteur. Min = 600, Max = 2550

}

void pwm_init(bool init_a, bool init_b){
	
	// Configuration des broches de sortie
	

	// Configuration du compteur et du comparateur
	

	// D�marrer le compteur et fixer un facteur de division de fr�quence � 1024	
	
	
	/////?????
	// On veut forcer un �tat bas
	PORTB = clear_bit(PORTB, PB3);
	
	// On veut forcer un �tat bas
		PORTD = clear_bit(PORTD, PD7);
	

}

void pwm_set_a(uint8_t duty){
	
	// Pour avoir un duty de 0, il faut �teindre le PWM et directement piloter la sortie � 0
	if(duty == 0){
		
		//Mettre 0 dans la broche PD7 (OC2) du port
		PORTD = clear_bit(PORTD, PD7);
		
		//D�sactive le comparateur
		TCCR0 = clear_bit(TCCR0, COM01);
	}
	
	else{
		// Modification du rapport cyclique du PWM
		
		
		//Active le comparateur
		TCCR0 = set_bit(TCCR0, COM01);
	}
}


void pwm_set_b(uint8_t duty){

	// Pour avoir un duty de 0, il faut �teindre le PWM et directement piloter la sortie � 0
	if(duty == 0){
		
		//Mettre 0 dans la broche PB3 (OC0) du port
		PORTB = clear_bit(PORTB, PB3);
		
		//D�sactive le comparateur
		TCCR2 = clear_bit(TCCR2, COM21);
	}
	
	else{
		// Modification du rapport cyclique du PWM
		
		
		//Active le comparateur
		TCCR2 = set_bit(TCCR2, COM21);
	}
}

void joystick_button_init(void){
	
}


bool joystick_button_read(void){

}