/**
     __   __                 __     __
    |__) /  \ \_/  /\  |  | |  \ | /  \
    |  \ \__/ / \ /~~\ \__/ |__/ | \__/

    Copyright (c) Roxaudio 2012. All rights reserved.
    This Source Code is the Property of Roxaudio inc. and can only be
    used in accordance with Roxaudio's Source Code License Agreement.

	\file uart.c
	\brief Gère la communication bidirectionelle avec le port série
	\author Iouri Savard Colbert
	\date 13 Mars 2012 - Date de création
	\date 16 Septembre 2012 - Ajout du support de UART 1
*/

/******************************************************************************
Includes and defines
******************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"

#include "fifo.h"


/******************************************************************************
Static variables
******************************************************************************/

static uint16_t baudrate_to_UBRR[] = {

#if F_CPU == 8000000UL     /*Fosc = 8.0000MHz*/
    207,    /* BAUDRATE_2400	Error : 0.2%  */
    103,    /* BAUDRATE_4800 	Error : 0.2%  */
    51,     /* BAUDRATE_9600 	Error : 0.2%  */
    25,     /* BAUDRATE_19200 */
    12,     /* BAUDRATE_38400 */
    6,      /* BAUDRATE_57600 */
    3,      /* BAUDRATE_115200 Error : 8.5% */
    1,      /* BAUDRATE_230400 */
    1,      /* BAUDRATE_250000 */
	
#elif F_CPU == 16000000UL     /*Fosc = 16.0000MHz*/
	416,    /* BAUDRATE_2400 */
	207,    /* BAUDRATE_4800 */
	103,    /* BAUDRATE_9600 */
	51,     /* BAUDRATE_19200 */
	25,     /* BAUDRATE_38400 */
	16,     /* BAUDRATE_57600 */
	8,      /* BAUDRATE_115200 */
	3,      /* BAUDRATE_230400 */
	3,      /* BAUDRATE_250000 */

#elif F_CPU == 20000000UL   /*Fosc = 20.0000MHz*/
    520,    /* BAUDRATE_2400 */
    259,    /* BAUDRATE_4800 */
    129,    /* BAUDRATE_9600 */
    64,     /* BAUDRATE_19200 */
    32,     /* BAUDRATE_38400 */
    21,     /* BAUDRATE_57600 */
    10,     /* BAUDRATE_115200 */
    4,      /* BAUDRATE_230400 */
    4,      /* BAUDRATE_250000 */

#else
    #error The UBRR values are not defined for your CPU frequency
#endif
};

static volatile uint8_t rx_buffer[UART_RX_BUFFER_SIZE];
static volatile uint8_t tx_buffer[UART_TX_BUFFER_SIZE];

static fifo_t rx_fifo;
static fifo_t tx_fifo;


/******************************************************************************
Static prototypes
******************************************************************************/

static void enable_UDRE_interupt(void);
static void disable_UDRE_interupt(void);

static void enable_RX_interupt(void);
static void disable_RX_interupt(void);


/******************************************************************************
Interupts
******************************************************************************/

/**
    \brief interupt quand le data register (UDRE) est prêt à recevoir d'autres
    données pour UART 0
*/
ISR(USART_UDRE_vect){

    UDR = fifo_pop(&tx_fifo);

    if(fifo_is_empty(&tx_fifo) == TRUE){

        disable_UDRE_interupt();
    }
}

/**
    \brief interupt quand le data register (UDR) a reçu une nouvelle donnée
    pour UART 0
*/
ISR(USART_RXC_vect){

    fifo_push(&rx_fifo, UDR);
}



/******************************************************************************
Global functions
******************************************************************************/
/*** initialize uart ***/
void uart_init(void){

    /* configure asynchronous operation, no parity, 1 stop bit, 8 data bits,  */
    UCSRC = (	(1 << URSEL) |	/*Doit absolument être a 1 pour écrire le registe UCSRC (gros caca d'ATmega32) */
                (0 << UMSEL) |	/*USART Mode Select : Asynchronous USART*/
                (0 << UPM1) |	/*Parity Mode : No parity*/
                (0 << UPM0) |   /*Parity Mode : No parity*/
                (0 << USBS) |	/*Stop Bit Select : 1-bit*/
                (1 << UCSZ1) |  /*Character Size : 8-bit*/
                (1 << UCSZ0) |  /*Character Size : 8-bit*/
                (0 << UCPOL));  /*0 when asynchronous mode is used*/

    /* enable RxD/TxD and ints */
    UCSRB = (	(1 << RXCIE) |  /*RX Complete Interrupt Enable*/
                (0 << TXCIE) |  /*TX Complete Interrupt Enable */
                (0 << UDRIE) |  /*Data Register Empty Interrupt Enable */
                (1 << RXEN) |   /*Receiver Enable*/
                (1 << TXEN) |   /*Transmitter Enable*/
                (0 << UCSZ2));  /*Character Size : 8-bit*/

    UCSRA = (	(0 << U2X) |    /*Double the USART Transmission Speed*/
				(0 << MPCM));   /*Multi-processor Communication Mode*/

    /*initialisation des fifos respectifs */
    fifo_init(&rx_fifo, (uint8_t*)rx_buffer, UART_RX_BUFFER_SIZE);
    fifo_init(&tx_fifo, (uint8_t*)tx_buffer, UART_TX_BUFFER_SIZE);

    uart_set_baudrate(DEFAULT_BAUDRATE);
}


/*** uart_set_baudrate ***/
/// \todo (iouri#1#): implémenter qqch qui empêche la corruption de la transmission.  La mise à jour de UBRR est immédiate.  Voir doc p. 196
void uart_set_baudrate(baudrate_e baudrate){

    UBRRL = (uint8_t)(baudrate_to_UBRR[baudrate] & 0xFF);
	UBRRH = (uint8_t)((baudrate_to_UBRR[baudrate] >> 8) & 0xFF);
}



/*** uart_put_byte ***/
void uart_put_byte(uint8_t byte){

    //on commence par désactiver l'interuption pour éviter que celle-ci
    //se produise pendant qu'on ajoute un caractère au buffer
    disable_UDRE_interupt();

    fifo_push(&tx_fifo, byte);

    // On active l'interrupt après avoir incrémenté le pointeur
    // d'entré pour éviter un dead lock assez casse-tête
    enable_UDRE_interupt();

}


/*** uart_put_string ***/
void uart_put_string(char* string){
	
	uint8_t i = 0;
	
	while(string[i] != '\0'){
		
		while(fifo_is_full(&tx_fifo)  == TRUE);
		
		//on commence par désactiver l'interuption pour éviter que celle-ci
		//se produise pendant qu'on ajoute un caractère au buffer
		disable_UDRE_interupt();
		
		while((string[i] != '\0') && (fifo_is_full(&tx_fifo)  == FALSE)){
			
			fifo_push(&tx_fifo, string[i]);
			
			i++;
		}

		// On active l'interrupt après avoir incrémenté le pointeur
		// d'entré pour éviter un dead lock assez casse-tête
		enable_UDRE_interupt();

	}
}

/*** uart_get_byte ***/
uint8_t uart_get_byte(void){

    uint8_t byte;

    disable_RX_interupt();

    byte = fifo_pop(&rx_fifo);

    enable_RX_interupt();

    return byte;
}


void uart_get_string(char* out_buffer, uint8_t buffer_length){
	
	uint8_t index = 0;
	
	while(uart_is_rx_buffer_empty() == FALSE){
		
		out_buffer[index] = uart_get_byte();
		index++;
		
		// Si il ne reste de la place que pour le \0
		if(index >= buffer_length - 1){
			
			// On sort
			break;
		}
	}
	
	
	// Très temporaire pour déboguer
	if(index == 0){
		
		string_copy(out_buffer, "-vide-");
	}
	
	// On ferme la string
	out_buffer[index] = '\0';
}


/*** uart_clean_rx_buffer ***/
void uart_clean_rx_buffer(void){
	
	fifo_clean(&rx_fifo);
}

/*** uart_flush ***/
void uart_flush(void){
	
	while(uart_is_tx_buffer_empty() == FALSE);
}

/*** is_rx_buffer_empty ***/
bool uart_is_rx_buffer_empty(void){

    return fifo_is_empty(&rx_fifo);
}

/*** is_tx_buffer_empty ***/
bool uart_is_tx_buffer_empty(void){

    return fifo_is_empty(&tx_fifo);
}


/******************************************************************************
Static functions
******************************************************************************/

static void enable_UDRE_interupt(void){

	UCSRB = set_bit(UCSRB, UDRIE);
}

static void disable_UDRE_interupt(void){

    UCSRB = clear_bit(UCSRB, UDRIE);
}

static void enable_RX_interupt(void){

    UCSRB = set_bit(UCSRB, RXCIE);
}

static void disable_RX_interupt(void){

    UCSRB = clear_bit(UCSRB, RXCIE);
}
