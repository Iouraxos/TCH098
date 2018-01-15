#ifndef FIFO_H_INCLUDED
#define FIFO_H_INCLUDED

/**
     __   __                 __     __
    |__) /  \ \_/  /\  |  | |  \ | /  \
    |  \ \__/ / \ /~~\ \__/ |__/ | \__/

    Copyright (c) Roxaudio 2012. All rights reserved.
    This Source Code is the Property of Roxaudio inc. and can only be
    used in accordance with Roxaudio's Source Code License Agreement.

	@file fifo.h
	@brief Module qui implémente un fifo à longueur statiquement variable
	@author Iouri Savard Colbert
	@date 16 septembre 2012 - Création du module

*/

/******************************************************************************
Includes
******************************************************************************/

#include "utils.h"

/******************************************************************************
Defines et typedef
******************************************************************************/

typedef struct{

    uint8_t*    ptr;
    uint8_t     size;
    uint8_t     in_offset;
    uint8_t     out_offset;
    bool        is_empty;
    bool        is_full;

} fifo_t;

/******************************************************************************
Prototypes
******************************************************************************/

void fifo_init(fifo_t* fifo, uint8_t* ptr_buffer, uint8_t buffer_size);
void fifo_push(fifo_t* fifo, uint8_t value);
uint8_t fifo_pop(fifo_t* fifo);
void fifo_clean(fifo_t* fifo);
bool fifo_is_empty(fifo_t* fifo);
bool fifo_is_full(fifo_t* fifo);

#endif // FIFO_H_INCLUDED
