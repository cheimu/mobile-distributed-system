/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.76
        Device            :  PIC18F25K22
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#define RTC_addres 0x08 // RTC slave device address




uint8_t second;

uint8_t h;
uint8_t u;
uint8_t s;
uint8_t k;
uint8_t y;
uint8_t length;
I2C1_MESSAGE_STATUS w_status;
/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    uint8_t count = 0;
    SSP1STAT = 0x80;
    // SSPEN enabled; WCOL no_collision; CKP Idle:Low, Active:High; SSPM FOSC/4_SSPxADD_I2C; SSPOV no_overflow; 
    SSP1CON1 = 0x28;
    // SBCDE disabled; BOEN disabled; SCIE disabled; PCIE disabled; DHEN disabled; SDAHT 100ns; AHEN disabled; 
    SSP1CON3 = 0x00;
    // Baud Rate Generator Value: SSP1ADD 39; 
    SSP1ADD = 0x27;
    while (1)
    {
        h = 0x80;
        u = 0x00;
        /*s = 0x43;
        k = 0x44;
        y = 0x45;
        */
        length = 1;
        
        if (count < 150) {
            I2C1_MasterWrite(&h, length, RTC_addres, &w_status);

            /*for (volatile int i = 0; i < 10000; i++);
            I2C1_MasterWrite(&u, length, RTC_addres, &w_status);
            for (volatile int i = 0; i < 10000; i++);
            I2C1_MasterWrite(&s, length, RTC_addres, &w_status);
            for (volatile int i = 0; i < 10000; i++);
            I2C1_MasterWrite(&k, length, RTC_addres, &w_status);
            for (volatile int i = 0; i < 10000; i++);
            I2C1_MasterWrite(&y, length, RTC_addres, &w_status);*/
            for (volatile int i = 0; i < 10000; i++);
            I2C1_MasterWrite(&h, length, RTC_addres, &w_status);
            count++;
        } else if (count == 150) {
            for (volatile int i = 0; i < 10000; i++);
            I2C1_MasterWrite(&u, length, RTC_addres, &w_status);
            count = 150;
        }
        //for (volatile int i = 0; i < 100000000; i++);
        //I2C1_MasterWrite(&u, length, RTC_addres, &w_status);
        // Add your application code
    }
}
/**
 End of File
*/