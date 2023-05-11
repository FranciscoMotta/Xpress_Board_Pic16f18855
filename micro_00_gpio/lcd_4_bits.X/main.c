/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on 10 de mayo de 2023, 04:17 PM
 */

/* Includes */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 16000000UL
#endif

#include "system_config.h"
#include "project_defines.h"

/* Macros (defines) */

/* Nota: Se harán todas las funciones en el main
 * luego se implementará una librería para el uso
 * del LCD a 4 bits
 *  */

#define Lcd_Data_Tris   TRISC
#define Lcd_Data_Lat    LATC
#define Lcd_Data_Ansel  ANSELC
#define Lcd_Data_Mask   (0b1111 << 4)

#define Lcd_Ctrl_Tris   TRISC
#define Lcd_Ctrl_Lat    LATC
#define Lcd_Ctrl_Ansel
#define Lcd_Ctrl_Rs     0
#define Lcd_Ctrl_Rw     1
#define Lcd_Ctrl_En     2
#define Lcd_Ctrl_Mask  (1 << Lcd_Ctrl_En) | (1 << Lcd_Ctrl_Rw) | (1 << Lcd_Ctrl_Rs)


/* Declaracion de funciones */

void Init_HFINTOSC (void);

/* LCD */

void FM_Lcd_Init_Gpio (void);

/*
 * MAIN
 */

int main(void) 
{
    Init_HFINTOSC(); // Conf. Reloj Interno
    
    /* Led como salida */
    Led_Sys_Ansel &= ~(1 << Led_Sys_Gpio);
    Led_Sys_Tris &= ~(1 << Led_Sys_Gpio);
    
    while (true)
    {
        Led_Sys_Lat ^= (1 << Led_Sys_Gpio);
        __delay_ms(100);
    }
    return (EXIT_SUCCESS);
}

/* Definicion de funciones */

void FM_Lcd_Init_Gpio (void)
{
    /* Pines de datos digitales */
    Lcd_Data_Ansel &= ~Lcd_Data_Mask;
    /* Pines de control digitales */
    
    /* Pines de datos como salida: RC7 - RC6 - RC5 - RC4 */
    Lcd_Data_Tris &= ~Lcd_Data_Mask;
    /* Pines de control como salida: RC2 - RC1 - RC0 */
    Lcd_Data_Tris &= ~Lcd_Ctrl_Mask;
}

void Init_HFINTOSC (void)
{
    /* Limpiamos los registros */
    OSCFRQ  = 0x00; // <- Importante!!!!
    
    /* Configuramos el registro OSCCON1 */
    OSCCON1 |= (0b110 << _OSCCON1_NOSC0_POSITION); // HFINTOSC
    OSCCON1 |= (0b0000 << _OSCCON1_NDIV0_POSITION); // CLOCK DIV 1
    
    /* Habilitamos el registro del HFINTOSC */
    OSCEN |= (1 << _OSCEN_HFOEN_POSITION);
    
    /* Configuramos el registro OSCFRQ */
    OSCFRQ |= (0b101 << _OSCFRQ_HFFRQ0_POSITION); // Freq 16mhz  
}