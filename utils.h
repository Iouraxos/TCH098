#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

/**
     __   __                 __     __
    |__) /  \ \_/  /\  |  | |  \ | /  \
    |  \ \__/ / \ /~~\ \__/ |__/ | \__/

    Copyright (c) Roxaudio 2012. All rights reserved.
    This Source Code is the Property of Roxaudio inc. and can only be
    used in accordance with Roxaudio's Source Code License Agreement.

	\file utils.h
	\brief Header de fonctions utiles à tout projet
	\author Iouri Savard Colbert
	\date 13 Mars 2012
	\attention Le fait d'inclure ce header dans un projet inclu automatiquement
	<stdint.h> ce qui rend sont inclusion redondante
*/


/******************************************************************************
Switches
******************************************************************************/

/**
    \brief Empêche de remlacer les fonctions one-liner par une macro.

    \details Pour l'instant, toute les fonctions qui font des opérations sur des
    bits sont des one-liner et sont suceptible d'être utiliser dans des boucles
    es dans les interruptions.  Il est donc intéressant de vouloir les optimiser
    en les rendant inline avec une macro. Bref c'est toujours l'option qui devrait
    toujours être utilisée sauf à la compilation de utils.c ou dans d'autres cas
    louches que j'arrive pas à figurer pour l'instant.

    DISABLE_UTILS_H_MACRO
*/

/******************************************************************************
Includes
******************************************************************************/

#include <stdint.h>


/******************************************************************************
Typedefs
******************************************************************************/

/**
    \brief Si vous avez besoin d'une explication, fermez le document, quittez le
    monde de la programmtion et n'y revennez plus jamais.  Il y a des très bonnes
    émissions de télé-réalité qui ne demandes qu'à être écoutées par vous
*/
typedef enum {
    FALSE = 0,
    TRUE = !(0)
}bool;


/**
    \brief Fucking idem
*/
#ifndef NULL
    #define NULL 0
#endif


/******************************************************************************
Prototypes
******************************************************************************/


/** bit operation ************************************************************/

/**
    \brief Pour mettre à 1 un seul bit à l'intérieur d'un byte
	\param byte le byte à modifier
    \param bit le numréo du bit à modifier à partir de LSB = 0
    \return le byte modifié
*/
uint8_t set_bit(uint8_t byte, uint8_t bit);


/**
    \brief Pour mettre à 0 un seul bit à l'intérieur d'un byte
	\param byte le byte à modifier
    \param bit le numréo du bit à modifier à partir de LSB = 0
    \return le byte modifié
*/
uint8_t clear_bit(uint8_t byte, uint8_t bit);


/**
    \brief Pour togller un seul bit à l'intérieur d'un byte
	\param byte le byte à modifier
    \param bit le numréo du bit à modifier à partir de LSB = 0
    \return le byte modifié
*/
uint8_t toggle_bit(uint8_t byte, uint8_t bit);


/**
    \brief Pour mettre à une certaine valeur un seul bit à l'intérieur d'un byte
	\param byte le byte à modifier
    \param bit le numréo du bit à modifier à partir de LSB = 0
    \param value la valeur à appliquer (ne peut être que 0 ou 1 )
    \return le byte modifié
*/

uint8_t write_bit(uint8_t byte, uint8_t bit, uint8_t value);


/**
    \brief Pour lire un seul bit à l'intérieur d'un byte
	\param byte le byte à lire
    \param bit le numréo du bit à lire à partir de LSB = 0
    \return le bit lu (ne peut être que 0 ou 1 )
*/
uint8_t read_bit(uint8_t byte, uint8_t bit);

/**
    \brief Pour mettre à 1 plusieurs bit à l'intérieur d'un byte
	\param byte le byte à modifier
    \param mask le masque des bits qui seront mis à 1
    \return le byte modifié
*/
uint8_t set_bits(uint8_t byte, uint8_t mask);


/**
    \brief Pour mettre à 0 plusieurs bit à l'intérieur d'un byte
	\param byte le byte à modifier
    \param mask le masque des bits qui seront mis à 0
    \return le byte modifié
*/
uint8_t clear_bits(uint8_t byte, uint8_t mask);


/**
    \brief Pour toggler plusieurs bit à l'intérieur d'un byte
    \param byte le byte à modifier
    \param mask le masque des bits qui seront mis à 1
    \return le byte modifié
*/
uint8_t toggle_bits(uint8_t byte, uint8_t mask);


/**
    \brief Pour mettre à une certaine valeur plusieurs bit à l'intérieur d'un byte
    \param byte le byte à modifier
    \param mask le masque des bits qui seront modifiées
    \param value la valeur à appliquer
    \return le byte modifié
*/
uint8_t write_bits(uint8_t byte, uint8_t mask, uint8_t value);


/**
    \brief Pour lire la valeur de plusieurs bit à l'intérieur d'un byte avec les bits
    non intéressants masqués à 0
	\param byte le byte à modifier
    \param mask le masque des bits qui seront modifiées
    \return la valeur masquée

    \attention Cette fonction est un peu conne mais elle a été ajoutée au lot juste pour la
    symétrie et l'orthogonnalité de la chose
*/
uint8_t read_bits(uint8_t byte, uint8_t mask);


/** Memory management **********************************************************/

/**
    \brief Copie un bloc de mémoire à un autre endroit
    \param destination Pointeur sur la destination de la copie
    \param source Pointeur sur la source de la copie
    \param num le nombre de byte à copier
    \attention num c'est le nombre de bytes et non pas le nombte d'élément d'un
    tableau.  Pour copier un tableau qui conteint autre chose que des bytes,
    l'argument num devrait avoir la forme suivante : nb_elements * sizeof(element)
*/
void mem_copy(void * destination, const void * source, uint8_t num );


/** String stuff *************************************************************/

/**
    \brief Retourne la longueur d'une string (array terminé par '\0')
    \param string Pointeur sur la string
    \return la longueur de la string sans le caractère nul
*/
uint8_t string_length(const char* string);

//non implémenté
//void string_append(char* out_string, const char* string_to_append);

uint8_t string_copy(char* out_string, const char* in_string);


/** Conversion text to number ************************************************/

/**
    \brief Converti un caractère représentant un chiffre en sa valeur
    \param caracter Le caractère en question
    \return La valeur ou zéro si le caractère n'avait pas rapport
*/
uint8_t char_to_uint(char caracter);


/**
    \brief Converti une string représentant un nombre en sa valeur
    \param string La string en question
    \return La valeur ou de la cochonnerie si la string n'avait pas rapport
*/
uint32_t string_to_uint(const char* string);


/**
    \brief Converti un array de char représentant un nombre en sa valeur
    \param string Le array en question
    \param size La grosseur intéressante du array puisque celui n'est pas
    obligé de se terminer par la caractère nule
    \return La valeur ou de la cochonnerie si la string n'avait pas rapport
*/
uint32_t char_array_to_uint(const char* char_array, uint8_t size);


/**
    \brief Converti un caractère représentant un chiffre hexadécimal en sa valeur
    \param caracter Le caractère en question
    \return La valeur ou zéro si le caractère n'avait pas rapport
*/
uint8_t hex_char_to_uint(char caracter);


/**
    \brief Converti une string représentant un nombre hexadécimal en sa valeur
    \param string La string en question
    \return La valeur ou de la cochonnerie si la string n'avait pas rapport
*/
uint32_t hex_string_to_uint(const char* string);


/**
    \brief Converti un array de char représentant un nombre hexadécimal en sa valeur
    \param string Le array en question
    \param size La grosseur intéressante du array puisque celui n'est pas
    obligé de se terminer par la caractère nule
    \return La valeur ou de la cochonnerie si la string n'avait pas rapport
*/
uint32_t hex_char_array_to_uint(const char* char_array, uint8_t size);


/** Conversion number to text ************************************************/

char uint_to_char(uint8_t digit);
char uint_to_hex_char(uint8_t hex_digit);

uint8_t uint8_to_string(char* out_string, uint8_t number);
uint8_t uint16_to_string(char* out_string, uint16_t number);
uint8_t uint32_to_string(char* out_string, uint32_t number);

uint8_t uint8_to_hex_string(char* out_string, uint8_t number);
uint8_t uint16_to_hex_string(char* out_string, uint16_t number);
uint8_t uint32_to_hex_string(char* out_string, uint32_t number);

uint8_t int8_to_string(char* out_string, int8_t number);
uint8_t int16_to_string(char* out_string, int16_t number);
uint8_t int32_to_string(char* out_string, int32_t number);


/******************************************************************************
Macros
******************************************************************************/

/* Selon mon standard de codage, une macro ne peut exister si sa fonction n'a pas étée définie
donc pour savoir comment utiliser ces macros, on devrait logicquement pouvoir se référer au
prototype du même nom */

#ifndef DISABLE_UTILS_H_MACRO
    #define set_bit(byte, bit)              ((byte) | (1 << (bit)))
    #define clear_bit(byte, bit)            ((byte) & ~(1 << (bit)))
    #define toggle_bit(byte, bit)           ((byte) ^ (1 << (bit)))
    #define write_bit(byte, bit, value)     (((byte) & ~(1 << (bit))) | ((value) << (bit)))
    #define read_bit(byte, bit)             (((byte) >> (bit)) & 1)
    #define set_bits(byte, mask)            ((byte) | (mask))
    #define clear_bits(byte, mask)          ((byte) & ~(mask))
    #define toggle_bits(byte, mask)         ((byte) ^ (mask))
    #define write_bits(byte, mask, value)   (((byte) & ~(mask)) | ((value) & (mask)))
    #define read_bits(byte, mask)           ((byte) & (mask))
#endif // DISABLE_UTILS_H_MACRO

/** Math *********************************************************************/

#define abs(value)                          ((value) < 0? -(value) : (value))


#endif // UTILS_H_INCLUDED
