#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "system_config.h"

#ifndef _XTAL_FREQ 
#define _XTAL_FREQ 16000000UL
#endif

/*
 * Declaración de funciones
 */

void Init_Internal_Clock (void);

/*
 * main 
 */
int main(void)
{
    /* Iniciamos el reloj interno */
    Init_Internal_Clock();
    /* Pin digital */
    ANSELA &= ~(1 << _ANSELA_ANSA0_POSITION); 
    /* Pin como salida */
    TRISA &= ~(1 << _TRISA_TRISA0_POSITION);
    /* Pin apagado */
    LATA  &= ~(1 << _LATA_LATA0_POSITION);
    while(true)
    {
        LATA ^= (1 << _LATA_LATA0_POSITION);
        __delay_ms(500);
    }
    return (EXIT_SUCCESS);
}

/*
 * Definición de funciones
 */

void Init_Internal_Clock (void)
{
    /* Reloj interno en HFINTOSC */
    OSCCON1 |= (0b110 << _OSCCON1_NOSC0_POSITION);
    /* División del reloj de entrada entre 1*/
    OSCCON1 |= (0b0000 << _OSCCON1_NDIV0_POSITION);
    /* Frecuencia del reloj interno a 16MHZ */
    OSCFRQ |= (0b101 << _OSCFRQ_HFFRQ0_POSITION);
}
