#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

/**
     __   __                 __     __
    |__) /  \ \_/  /\  |  | |  \ | /  \
    |  \ \__/ / \ /~~\ \__/ |__/ | \__/

    Copyright (c) Roxaudio 2012. All rights reserved.
    This Source Code is the Property of Roxaudio inc. and can only be
    used in accordance with Roxaudio's Source Code License Agreement.

	\file uart.h
	\brief Gère la communication bidirectionelle avec le port série
	\author Iouri Savard Colbert
	\date 13 Mars 2012
*/

/******************************************************************************
Includes
******************************************************************************/

#include "utils.h"

/******************************************************************************
Defines
******************************************************************************/

#define UART_RX_BUFFER_SIZE 64	//Certaines réponses du ESP8266 prennent jusqu'à 60 caractères
#define UART_TX_BUFFER_SIZE 64


typedef enum{

    BAUDRATE_2400 = 0,
    BAUDRATE_4800,
    BAUDRATE_9600,
    BAUDRATE_19200,
    BAUDRATE_38400,
    BAUDRATE_57600,
    BAUDRATE_115200,
    BAUDRATE_230400,
    BAUDRATE_250000,

}baudrate_e;

#define DEFAULT_BAUDRATE BAUDRATE_9600

/******************************************************************************
Prototypes
******************************************************************************/

/**
    \brief Fait l'initialisation du UART
*/
void uart_init(void);


/**
    \brief Définit le badrate du port choisit
*/
void uart_set_baudrate(baudrate_e baudrate);


/**
    \brief Ajoute un byte au rolling buffer à envoyer par le UART
    \param byte le byte à ajouter
*/
void uart_put_byte(uint8_t byte);


/**
    \brief Ajoute la string (par copie) au rolling buffer à envoyer par le UART.
    \param un pointeur sur le premier char de la string
	
	La copie s'arrête au premier \0. Ce dernier n'est pas copié. Si la string est plus
	longue que l'espace qui est libre dans le buffer, la fonction va patiement attendre
	que de l'espace se libère. Dans cette situation, cette fonction peut être très longue
	à retourner, ce qui pourrait briser des timmings critiques dans le code.
*/
void uart_put_string(char* string);

/**
    \brief Retire un byte au rolling buffer reçu par le UART.
    \return le byte reçu

    Si le buffer est vide cette fonction retourne '\0'.  Si le caractère à retourner
    est '\0' la fonction retourne aussi cette valeur.  Pour faire la différence entre
    les deux il faut préalablement vérifier si le buffer est vide avec la fonction
    uart_is_rx_buffer_empty()
*/
uint8_t uart_get_byte(void);

/**
    \brief Retourne tout ce que que le buffer de réception contient sous la forme d'une string

    Pour que cette fonction puisse fonctionner le buffer qui lui est passé doit minimalement
	avoir deux bytes de long. Un byte pour retourner au moins un caractère et un autre pour
	le \0. Si le buffer est plus petit que ça, cette fonction va corrompre la mémoire sans
	prévenir.
	
	Dès que le buffer de réception est vide, la fonction termine la string avec un \0 et
	retourne. Comme le UART est relativement très lent par rapport au processeur, il faut
	attendre un peu avant d'appeler cette fonction.
	
	Si le buffer est vide au moment d'appeler cette fonction, celle-ci retourne une string
	d'un seul caractère; \0
	
	Si le uart est utilisé pour communiquer autre chose que des strings, par exemple
	des données binaires, il se peut très bien que la valeur 0 soit présente dans le buffer.
	Dans ce cas il est impossible de faire la distinction entre une valeur 0 et un \0.
	
	Si le buffer contient plus de bytes que buffer_length - 1, la fonction va copier 
	buffer - 1 bytes et utiliser le dernier byte pour le \0. Par conséquent, le buffer de
	réception ne sera totalement vidé. Pour savoir si cette situation s'est produite, un
	appel à la fonction uart_is_rx_buffer_empty() retournera FALSE.
	
	Pour garantir que cette situation ne se produira jamais, il suffit de passer un
	buffer plus gros que UART_RX_BUFFER_SIZE
*/
void uart_get_string(char* out_buffer, uint8_t buffer_length);


/**
    \brief Vide le buffer de réception
	
	Dans cette situation les bytes reçu sont juste effacés et perdus à jamais.
*/
void uart_clean_rx_buffer(void);


/**
    \brief Attend que tous les caractères dans le buffer soient envoyés. Cette
	fonction bloque l'éxcécution du code.
*/
void uart_flush(void);


/**
    \brief Indique si le buffer de réception est vide.
    \param TRUE si il est vide, FALSE s'il contient 1 byte ou plus
*/
bool uart_is_rx_buffer_empty(void);

/**
    \brief Indique si le buffer de transmission est vide.
    \param TRUE si il est vide, FALSE s'il contient 1 byte ou plus
*/
bool uart_is_tx_buffer_empty(void);


#endif // UART_H_INCLUDED
