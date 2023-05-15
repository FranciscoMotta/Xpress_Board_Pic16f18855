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
#define Lcd_Ctrl_Ansel  ANSELC
#define Lcd_Ctrl_Rs     0
#define Lcd_Ctrl_Rw     1
#define Lcd_Ctrl_En     2
#define Lcd_Ctrl_Mask  (1 << Lcd_Ctrl_En) | (1 << Lcd_Ctrl_Rw) | (1 << Lcd_Ctrl_Rs)

/* Rutinas del enable */

#define En_On()         Lcd_Ctrl_Lat |= (1 << Lcd_Ctrl_En);
#define En_Off()        Lcd_Ctrl_Lat &= ~(1 << Lcd_Ctrl_En);
#define Lcd_En_Tog()    En_On(); __delay_ms(1); En_Off(); __delay_ms(1);

/* Rutina de modos */

#define Lcd_Mode_Com()  Lcd_Ctrl_Lat &= ~(1 << Lcd_Ctrl_Rs);
#define Lcd_Mode_Char() Lcd_Ctrl_Lat |= (1 << Lcd_Ctrl_Rs);

/* Declaracion de funciones */

void Init_HFINTOSC (void);
void Init_System_Gpio (void);

/* LCD */

void FM_Lcd_Init_Gpio (void);
void FM_Lcd_Init_Config (void);
void FM_Lcd_Send_Nibble(char nibble); 
void FM_Lcd_Send_Command (char comm);
void FM_Lcd_Send_Charact (char charac);

/*
 * MAIN
 */

int main(void) 
{
    Init_HFINTOSC(); // Conf. Reloj Interno
    Init_System_Gpio(); // Iniciamos el GPIO
    FM_Lcd_Init_Config(); // Iniciamos el LCD
    FM_Lcd_Send_Command(0x80);
    FM_Lcd_Send_Charact('a');
    while (true)
    {
        Led_Sys_Lat ^= (1 << Led_Sys_Gpio);
        __delay_ms(200);
    }
    return (EXIT_SUCCESS);
}

/* Definicion de funciones */


void Init_System_Gpio (void)
{
    Led_Sys_Ansel &= ~(1 << Led_Sys_Gpio);
    Led_Sys_Tris &= ~(1 << Led_Sys_Gpio);
}

void FM_Lcd_Init_Config (void)
{
    FM_Lcd_Init_Gpio();// Iniciamos los GPIO para la LCD
    /* Acá procedemos a mandar los comandos de control */
    FM_Lcd_Send_Command(0x01); // Clear Display
    FM_Lcd_Send_Command(0x02); // Return Home
    FM_Lcd_Send_Command(0x06); // Cursor to right
    FM_Lcd_Send_Command(0x0C); // Display On - Cursor Off - Blink Off
    FM_Lcd_Send_Command(0x10); // Shift cursor to right AC + 1
    FM_Lcd_Send_Command(0x2C); // 4bits LCD - 2 Lines - 5x11 dots
}


void FM_Lcd_Send_Charact (char charac)
{
    Lcd_Mode_Char();
    FM_Lcd_Send_Nibble(charac);
}

void FM_Lcd_Send_Command (char comm)
{
    Lcd_Mode_Com();
    FM_Lcd_Send_Nibble(comm);
}

void FM_Lcd_Send_Nibble(char nibble)
{
    char upper_bits,lower_bits = 0;
    upper_bits = (nibble & 0xF0);
    lower_bits = (nibble & 0x0F) << 4;
    Lcd_Data_Lat |= upper_bits;
    Lcd_En_Tog();
    Lcd_Data_Lat &= ~upper_bits;
    
    __delay_us(100); // Esperamos 0.01ms
    
    Lcd_Data_Lat |= lower_bits;
    Lcd_En_Tog();
    Lcd_Data_Lat &= ~lower_bits;
    
    __delay_us(100); // Esperamos un 0.01ms 
}

void FM_Lcd_Init_Gpio (void)
{
    /* Pines de datos digitales */
    Lcd_Data_Ansel &= ~Lcd_Data_Mask;
    /* Pines de control digitales */
    Lcd_Ctrl_Ansel &= ~Lcd_Ctrl_Mask;
    /* Pines de datos como salida: RC7 - RC6 - RC5 - RC4 */
    Lcd_Data_Tris &= ~Lcd_Data_Mask;
    /* Pines de control como salida: RC2 - RC1 - RC0 */
    Lcd_Data_Tris &= ~Lcd_Ctrl_Mask;
    /* Todos los pines en 0 */
    Lcd_Data_Lat &= ~Lcd_Data_Mask;
    Lcd_Ctrl_Lat &= ~Lcd_Ctrl_Mask;
    
    /* Ponemos el RW en 0 */
    Lcd_Ctrl_Lat &= ~(1 << Lcd_Ctrl_Rw);
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