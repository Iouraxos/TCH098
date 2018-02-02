#ifndef DRIVER_H_INCLUDED
#define DRIVER_H_INCLUDED

/*
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
/**
	\file
	\brief Header de fonctions qui pilotent directement du matériel
	\author Iouri Savard Colbert
	\date 24 janvier 2018
*/

/* ----------------------------------------------------------------------------
Includes
---------------------------------------------------------------------------- */

#include "utils.h"

/* ----------------------------------------------------------------------------
Prototypes
---------------------------------------------------------------------------- */

/**
    \brief Initialise le module de l'ADC
    \return rien.
	Le module de l'ADC utilise le PORT A. Cependant, les 3 derniers bits du PORT A
	sont utilisés pour faire fonctionner l'écran LCD. Il est donc possible d'utiliser la broche
	PA0 à PA4 inclusivement ce qui correspond respectivement aux broches 40 à 36 du DIP.
	
			    +---- ----+
			  --| 1  U 40 |-- ADC 0
			  --| 2    39 |-- ADC 1
			  --| 3    38 |-- ADC 2
			  --| 4    37 |-- ADC 3
			  --| 5    36 |-- ADC 4
			  --| 6    35 |--
			  --| 7    34 |--
			  --| 8    33 |--
			  --| 9    32 |--
			  --| 10   31 |--
			  --| 11   30 |--
			  --| 12   29 |--
			  --| 13   28 |--
			  --| 14   27 |--
			  --| 15   26 |--
			  --| 16   25 |--
			  --| 17   24 |--
	          --| 18   23 |--
			  --| 19   22 |--
			  --| 20   21 |--
			    +---------+
*/
void adc_init(void);


/**
    \brief Fait une conversion de la valeur analogique présente sur une entrée
    \param[in]	channel	Le channel (entrée) sur lequel la conversion doit être effectuée (entre PA0 et PA4 inclusivement)
    \return La valeur convertie.
	C'est une erreur d'appeler cette fonction avec un channel négatif ou plus grand que 4
	Il est important de noter que cette fonction ne s'exécute pas instantanément. La conversion
	prend un certain temps à s'effectuer et la fonction attend la fin de la conversion avant de
	retourner. C'est une mauvaise idée d'appeler cette fonction dans une boucle avec des temps
	critiques.
*/
uint8_t adc_read(uint8_t channel);


/**
    \brief Initialise le module de contrôle des servomoteurs
    \return rien.
	Le module de contrôle des servomoteurs utilise le timer 1.
	Servo A correspond à la broche PD5 du microcontrôleur ce qui correspond à la broche 19 du DIP.
	Servo B correspond à la broche PD4 du microcontrôleur ce qui correspond à la broche 18 du DIP.
	
			  	  +---- ----+
			    --| 1  U 40 |--
			    --| 2    39 |--
			    --| 3    38 |--
			    --| 4    37 |--
			    --| 5    36 |--
			    --| 6    35 |--
			    --| 7    34 |--
			    --| 8    33 |--
			    --| 9    32 |--
			    --| 10   31 |--
			    --| 11   30 |--
			    --| 12   29 |--
			    --| 13   28 |--
			    --| 14   27 |--
			    --| 15   26 |--
			    --| 16   25 |--
			    --| 17   24 |--
		Servo B --| 18   23 |--
		Servo A --| 19   22 |--
			    --| 20   21 |--
			      +---------+
*/
void servo_init(void);

/**
    \brief Applique la bonne impulsion à la sortie du servomoteur A pour lui donner un angle particulier
	\param[in]	angle Une valeur entre 0 et 255
    \return rien.
	La valeur de l'angle qui est passée en paramètre n'a aucune relation avec une grandeur physique
	définie (degré, radian, h:m:s...). Il s'agit d'une valeur abstraite. La seule règle qui est attendue
	c'est que le plus petit angle soit obtenu avec 0, le plus grand avec 255 et avec 127 le servomoteur
	devrait être droit.
*/
void servo_set_a(uint8_t angle);

/**
    \brief Applique la bonne impulsion à la sortie du servomoteur B pour lui donner un angle particulier
	\param[in]	angle Une valeur entre 0 et 255
    \return rien.
	La valeur de l'angle qui est passé en paramètre n'a aucune relation avec une grandeur physique
	définie (degré, radian, h:m:s...). Il s'agit d'une valeur abstraite. La seule règle qui est attendue
	c'est que le plus petit angle soit obtenu avec 0, le plus grand avec 255 et avec 127 le servomoteur
	devrait être droit.
*/
void servo_set_b(uint8_t angle);

/**
    \brief Initialise les modules de PWM
    \param init_a Si == TRUE, le PWM A est initialisé
    \param init_b Si == TRUE, le PWM B est initialisé
    \return rien.
	Le PWM A correspond au timer 0. Le timer 0 utilise la broche PB3 du microcontrôleur
	ce qui correspond à la broche 4 du DIP. Le PWM B correspond au timer 2. Le timer 2
	utilise la broche PD7 du microcontrôleur ce qui correspond à la broche 21 du DIP.
	
			    +---- ----+
			  --| 1  U 40 |--
			  --| 2    39 |--
			  --| 3    38 |--
		PWM A --| 4    37 |--
			  --| 5    36 |--
			  --| 6    35 |--
			  --| 7    34 |--
			  --| 8    33 |--
			  --| 9    32 |--
			  --| 10   31 |--
			  --| 11   30 |--
			  --| 12   29 |--
			  --| 13   28 |--
			  --| 14   27 |--
			  --| 15   26 |--
			  --| 16   25 |--
			  --| 17   24 |--
			  --| 18   23 |--
			  --| 19   22 |--
			  --| 20   21 |-- PWM B
			    +---------+
*/
void pwm_init(bool init_a, bool init_b);

/**
    \brief Applique un PWM à la sortie PWM A (broche 4 du DIP)
	\param[in]	duty Une valeur entre 0 et 255
    \return rien.
	Un duty (rapport cyclique) de 0 correspond à un PWM de 0% et un duty de 255 correspond à un
	PWM de 100%. Cette relation est linéaire sur toute l'intervalle.
*/
void pwm_set_a(uint8_t duty);

/**
    \brief Applique un PWM à la sortie PWM B (broche 21 du DIP)
	\param[in]	duty Une valeur entre 0 et 255
    \return rien.
	Un duty (rapport cyclique) de 0 correspond à un PWM de 0% et un duty de 255 correspond à un
	PWM de 100%. Cette relation est linéaire sur toute l'intervalle.
*/
void pwm_set_b(uint8_t duty);

/**
    \brief Initialise le bouton du joystick
    \return rien.
*/
void joystick_button_init(void);

/**
    \brief Retourne l'état du joystick
    \return TRUE si enfoncé, FALSE si relâché
*/
bool joystick_button_read(void);


#endif /* DRIVER_H_INCLUDED */
