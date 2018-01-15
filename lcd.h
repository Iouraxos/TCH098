#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED

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

	\file lcd.h
	\brief driver pour un affichage LCD piloté par un HD44780
	\author Iouri Savard Colbert
	\date 28 avril 2014

*/

#define ENABLE_JAPANESE_CHAR

/******************************************************************************
Includes
******************************************************************************/

#include "utils.h"


/******************************************************************************
Defines et typedef
******************************************************************************/

#define DATA_PORT   PORTC
#define DATA_DDR    DDRC
#define CTRL_PORT   PORTA
#define CTRL_DDR    DDRA
#define E_PIN       7
#define RW_PIN      6
#define RS_PIN      5

#define LCD_NB_COL 16
#define LCD_NB_ROW 2

typedef enum{

    HD44780_SHIFT_RIGHT,
    HD44780_SHIFT_LEFT,

}hd44780_shift_e;

typedef enum{

    LCD_SHIFT_RIGHT,
    LCD_SHIFT_LEFT,
    LCD_SHIFT_UP,
    LCD_SHIFT_DOWN,
	LCD_SHIFT_END,
	LCD_SHIFT_START,
	LCD_SHIFT_TOP,
	LCD_SHIFT_BOTTOM

}lcd_shift_e;

#define CHAR_RIGHT_ARROW 0x7E
#define CHAR_LEFT_ARROW 0x7F

/******************************************************************************
Prototypes
******************************************************************************/

/**
    Ce module est divisé en deux sous-modules suivants :
    - hd44780
    - lcd


    hd44780 :

    Ce sous- module fournis des fonctions pour accomplir les fonctionnalités
    matérielle fournies par le circuit du LCD.  Autrement dit, elles permettent à un
    utilisateur à ne pas avoir générer manuellement des codes binaires pour faire
    fonctionner ce dernier.  Les fonctions de ce module sont très rapides
    et peu gourmandes en mémoire.  Par contre elles sont limitées en
    fonctionnalité.  Les deux limitations majeures sont l'absence de relation
    avec l'affichage réel et l'impossibilité de connaître la position actuelle
    du curseur.  Ces fonctionnalités sont ajoutée par le module englobant lcd.


    lcd :

    Permet d'accomplir les mêmes tâches que le pilote directement, mais palie
    par logiciel aux limitations majeurs de celui-ci.  Cet ajout 
	de fonctionnalité se fait en échange d'un léger coût en temps de traitement 
	et en utilisation de mémoire.  Ceci dit, ces deux derniers restent minimes.  
	Toute personne voulant contrôller l'affichage de LCD devrait passer par ce 
	sous-module et non pas par le pilote de hd44780.


    Utilisation des sous modules :

    De façon à fonctionner correctement, les fonctions des sous modules ne
    peuvent être intermélangées.  Donc, un code qui n'aurait besoin d'une
    seule fois d'une foctionnalité du msous-module LCD text doit quand même utilsier
	LCD pour tous les autres opérations, aussi simples soient-elles.
*/


/** HD44780 ******************************************************************/

/**
    \brief Fait l'initialisation de base du contrôleur LCD.  C'est-à-dire
    l'initialisation, régler la config pour 16 caractères X 2 rangées, effacer
    l'écran et mettre le curseur au début.
    \param increment Si == TRUE, le texte avance vers la droite
    \param cursor Si == TRUE, un curseur est affiché
    \param blink Si == TRUE, la case du curseur clignote
    \return Rien
*/
void hd44780_init(bool increment, bool cursor, bool blink);

/**
    \brief Efface tous les caractères sur l'écran et remet le curseur au début
    \return Rien
*/
void hd44780_clear_display();

/**
    \brief Défini le sens dans lequel les caractères sont inséré.  À moins
    d'être bien spécial dans sa tête, en Amérique du nord le sens normal est
    incrémental et ne devrait pas changer souvent.  Cette fonction est utilisée
    par la fonction qui insert des caractère.
    \param increment Si == TRUE, le exte avance vers la droite
*/
void hd44780_set_entry_mode(bool increment);

/**
    \brief Permet de contrôler certain paramètres de l'affichage
    \param display Si == TRUE, l'affichage est allumé
    \param cursor Si == TRUE, un curseur est affiché
    \param blink Si == TRUE, la case du curseur clignote
    \return Rien
*/
void hd44780_set_display_control(bool display, bool cursor, bool blink);

/**
    \brief Permet de déplacer le curseur dans la mémoire du HD44780.  Cette
    fonction permet de faire abstraction des adresses en mémoire qui sont
    un peu batardes et d'y aller directement une position dans l'affichage.
    Rien n'empêche d'aller mettre un caractère à l'extérieur de la zone
    affichable, en autant que les limites des paramètres soient respectées
    \param col La colone de 0 à 39
    \param row La rangé de 0 à 1
    \return Rien
*/
void hd44780_set_cursor_position(uint8_t col, uint8_t row);

/**
    \brief Permet de déplacer le curseur d'une position à gauche ou à droite.
    \param shift La direction du shift
    \return Rien
*/
void hd44780_shift_cursor(hd44780_shift_e shift);

/**
    \brief Écrit un seul caractère à l'endroit actuel du curseur.  Au retour,
    le curseur aura été déplacé d'une position dans le sens déterminé à l'aide
    de hd44780_set_entry_mode.  Si le caractère n'est pas affichable, une boule
    pas rapport sera affiché à la place.
    \param character Le caractère ASCII à afficher.
    \return Rien
*/
void hd44780_write_char(char character);



/** LCD **********************************************************************/

/**
    \brief Fait l'initialisation du LCD.
	Cette fonction doit préalablement être appelée avant d'utiliser les autres
	fonctions du module.
*/
void lcd_init(void);

/**
    \brief Efface l'écran du LCD et retourne le curseur à la position 0,0
	Il n'est pas réelement possible "d'effacer" l'écran du LCD. Bien que
	la fonctionnalité soit offerte par le hd44780, en réalité la fiche technique
	nous apprend que le LCD ne fait que remplacer tous les caractères de l'écran
	par des espaces. C'est pour cette raison que cette fonction est relativement
	lente. Il serait une mauvaise id.e de faire un appel à celle-ci dans une boucle
	avec de délais critiques.
*/
void lcd_clear_display(void);

/**
    \brief Déplace le curseur à un endroit précis dans l'écran
	
	Par exemple, lcd_set_cursor_position(3, 1) : amène le curseur
	à la position du X
	
	 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
	 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	| | | | | | | | | | | | | | | | | 0
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	| | | |X| | | | | | | | | | | | | 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	
	Il y a 16 colonnes et 2 rangées. Si les paramètres dépassent les dimensions
	de l'écran, la demande est ignorée.
*/
void lcd_set_cursor_position(uint8_t col, uint8_t row);

/**
    \brief Déplace le curseur d'une seule case dans la position spécifiée
	
	Les valeurs possibles pour shift sont :
	
	    LCD_SHIFT_RIGHT		Déplace le curseur d'une case à droite
	    LCD_SHIFT_LEFT		Déplace le curseur d'une case à gauche
	    LCD_SHIFT_UP		Déplace le curseur d'une case en haut
	    LCD_SHIFT_DOWN		Déplace le curseur d'une case en bas
	    LCD_SHIFT_END		Déplace le curseur à la dernière colonne de la ligne (mais reste sur la même rangé)
	    LCD_SHIFT_START		Déplace le curseur à la première colonne de la ligne (mais reste sur la même rangé)
	    LCD_SHIFT_TOP		Déplace le curseur sur la première ligne (mais reste dans la même colonne)
	    LCD_SHIFT_BOTTOM	Déplace le curseur sur la dernière ligne (mais reste dans la même colonne)
	
	Les 4 premières options se comportent comme si l'écran du LCD était une boucle dans les deux directions.
	Par exemple :
	
	Si le curseur est à la position du X
	
	 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
	 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	| | | | | | | | | | | | | | | |X| 0
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	| | | | | | | | | | | | | | | | | 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	
	Et que l'appel suivant est fait : lcd_shift_cursor(LCD_SHIFT_RIGHT);

	Le curseur va se retrouver à la position suivante:
	
	 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
	 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|X| | | | | | | | | | | | | | | | 0
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	| | | | | | | | | | | | | | | | | 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void lcd_shift_cursor(lcd_shift_e shift);

/**
    \brief Écrit un seul caractère ASCII à la position du curseur sur le LCD
	
	Le LCD ne gère pas les accents et les caractères un peu spéciaux. L'exception
	à la règle sont deux flèches. C'est deux caractères ne font pas parti du code
	ASCII. Pour les afficher, il est possible d'appeler la fonction avec l'une des
	deux définitions suivantes :
	
		CHAR_RIGHT_ARROW
		CHAR_LEFT_ARROW
		
	Cette fonction ne gère pas non plus les caractère de contrôle tel que :
	
		"\n", "\r", "\0", "\b", ...
*/
void lcd_write_char(char character);

/**
    \brief Écrit une string à la position du curseur sur le LCD.
	
	Pour que cette fonction produise le résultat attendu, il faut absolument
	que la string soit terminée par un caractère nul ('\0'). Le caractère nul
	n'est pas envoyé au LCD.
	
	Par exemple :
	
	lcd_write_string("Hello World"); produira le résultat suivant :
	
	 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
	 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|H|e|l|l|o| |W|o|r|l|d| | | | | | 0
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	| | | | | | | | | | | | | | | | | 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	
	La fonction déplace automatiquement le curseur au début de la ligne suivante
	si la string à afficher est trop longue.
	
	Par exemple :
	
	lcd_write_string("Hello World ABCDEF"); produira le résultat suivant :
		
	 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
	 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|H|e|l|l|o| |W|o|r|l|d| |A|B|C|D| 0
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|E|F| | | | | | | | | | | | | | | 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	
	La fonction sait gérer deux caractère de contrôle; "\n" et "\r"
	
	"\n" Déplace le curseur d'une ligne vers le bas. Son comportement est identique
	à faire l'appel suivant : lcd_shift_cursor(LCD_SHIFT_DOWN);
	
	Par exemple :
	
	lcd_write_string("Hello\nWorld"); produira le résultat suivant :
	
	 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
	 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|H|e|l|l|o| | | | | | | | | | | | 0
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	| | | | | |W|o|r|l|d| | | | | | | 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	
	"\r" Renvois le curseur au début de la ligne en cours. Son comportement est identique
	à faire l'appel suivant : lcd_shift_cursor(LCD_SHIFT_START);
	
	Par exemple :
	
	lcd_write_string("Hello World\rABC"); produira le résultat suivant :
	
	 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
	 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|A|B|C|l|o| |W|o|r|l|d| | | | | | 0
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	| | | | | | | | | | | | | | | | | 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	
	Pour produire l'effet d'un retour de chariot, il faut combiner les deux caractères
	de contrôle ensemble. Ceux qui viennent du monde UNIX ne trouveront rien de dérangeant
	ici. Par contre, ceux qui viennent de Windows sont d'habitués de n'utiliser que "\n"
	pour produire cet effet. Premièrement Windows est un système d'exploitation sans saveur
	et mal fait. Deuxièmement, la bonne manière c'est "\n\r" point final.
	
	Par exemple :
	
	lcd_write_string("Hello\n\rWorld"); produira le résultat suivant :
	
	 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
	 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|H|e|l|l|o| | | | | | | | | | | | 0
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|W|o|r|l|d| | | | | | | | | | | | 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	
*/
void lcd_write_string(const char* string);


#endif // LCD_H_INCLUDED
