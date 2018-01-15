/**
     __   __                 __     __
    |__) /  \ \_/  /\  |  | |  \ | /  \
    |  \ \__/ / \ /~~\ \__/ |__/ | \__/

    Copyright (c) Roxaudio 2012. All rights reserved.
    This Source Code is the Property of Roxaudio inc. and can only be
    used in accordance with Roxaudio's Source Code License Agreement.

	@file fifo.c
	@brief Module qui implémente un fifo à longueur statiquement variable
	@author Iouri Savard Colbert
	@date 16 septembre 2012 - Création du module
*/

/******************************************************************************
Includes
******************************************************************************/

#include "fifo.h"


/******************************************************************************
Global functions
******************************************************************************/

void fifo_init(fifo_t* fifo, uint8_t* ptr_buffer, uint8_t buffer_size){

    fifo->ptr = ptr_buffer;
    fifo->size = buffer_size;
    fifo->in_offset = 0;
    fifo->out_offset = 0;
    fifo->is_empty = TRUE;
    fifo->is_full = FALSE;
}


void fifo_push(fifo_t* fifo, uint8_t value){

    /* Si le buffer est plein il n'est pas question de rien "pusher" */
    if(fifo->is_full == FALSE){

        fifo->ptr[fifo->in_offset] = value;

        fifo->is_empty = FALSE;

        if(fifo->in_offset == fifo->size - 1){

            fifo->in_offset = 0;
        }

        else{

            fifo->in_offset++;
        }

        /* si l'index de d'entrée ratrappe celui de sortie c'est que le buffer est plein */
        if(fifo->in_offset == fifo->out_offset){

            fifo->is_full = TRUE;
        }
    }
}


uint8_t fifo_pop(fifo_t* fifo){

    uint8_t value;

    /* Si le buffer n'est pas vide il n'est pas question de rien "poper" */
    if(fifo->is_empty == FALSE){

        value = fifo->ptr[fifo->out_offset];

        fifo->is_full = FALSE;

        if(fifo->out_offset == fifo->size - 1){

            fifo->out_offset = 0;
        }

        else{

            fifo->out_offset++;
        }

        /* si l'index de sortie ratrappe celui d'entrée c'est que le buffer est vide */
        if(fifo->out_offset == fifo->in_offset){

            fifo->is_empty = TRUE;
        }
    }

    else{

        /* En orienté objet je ferais une exception, mais en c le mieux que je peux faire
        c'est ça */
        value = 0;
    }

    return value;
}


void fifo_clean(fifo_t* fifo){
	
	fifo->in_offset = fifo->out_offset;
	fifo->is_full = FALSE;
	fifo->is_empty = TRUE;
}


bool fifo_is_empty(fifo_t* fifo) {

    return fifo->is_empty;
}


bool fifo_is_full(fifo_t* fifo){

    return fifo->is_full;
}
