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
//#include "pin_manager.h"
#define RTC_addres 0x08 // RTC slave device address

uint8_t length, dis; 
I2C1_MESSAGE_STATUS w_status;
I2C1_MESSAGE_STATUS p_status;
char userIn;
char command;
char x; 
char level; 

void sendString(char * str){
    while((*str) != '\0') {
        EUSART1_Write((uint8_t)(*str));
        str++;
    }
}

void SRAM(char a, char b, char c, char d){
    WE_SetLow();
    if (a == 1) IO_RB3_SetHigh();
    else IO_RB3_SetLow();
    if (b == 1) IO_RB2_SetHigh();
    else IO_RB2_SetLow();
    if (c == 1) IO_RB1_SetHigh();
    else IO_RB1_SetLow();
    if (d == 1) IO_RB0_SetHigh();
    else IO_RB0_SetLow();
}

void statusReport(){
    char buffer[80];
    sendString("////////////////////////////////////////////////////////////////////////////////\n");
    sendString("////////\n");
    sprintf(buffer, "////////   Set Position: %d  \n", x);
    sendString(buffer);
    sendString("////////\n");
    sprintf(buffer, "////////   Warning Level: %c  \n", level);
    sendString(buffer);
    sendString("////////\n");
    sendString("////////   Please choose following command:   \n");
    sendString("////////\n");
    sendString("////////      Motor ON <o>   \n");
    sendString("////////      Motor OFF <f>   \n");
    sendString("////////      Set Point <s> (scaled from 1 to 9)   \n"); 
    sendString("////////      Speed Increase <i>   \n");
    sendString("////////      Speed Decrease <d>   \n");
    sendString("////////      Keep Speed <k>   \n");
    sendString("////////\n");
    sendString("////////////////////////////////////////////////////////////////////////////////\n");
}
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
    __delay_ms(1000);
    dis = 0; 
    length = 1;
    x = 0; 
    statusReport();
    
    while (1)
    {
        if (dis == 100) {
            statusReport();
            dis = 0; 
        } else {
            dis = dis + 1; 
        }
        
        I2C1_MasterRead(&level, length, RTC_addres, &p_status);  
        while(w_status != I2C1_MESSAGE_COMPLETE);
        if (level == '0'){
          sendString("WARNING generated progress hold\n");
          command = 'f';
          I2C1_MasterWrite(&command, length, RTC_addres, &w_status);
        }
        
        // Add your application code
        if(EUSART1_is_rx_ready())
        {
            WE_SetHigh();
            userIn = EUSART1_Read();
            if (userIn == 'i') {
                sendString("Speed Increased\n");
                command = 'i';
            } else if (userIn == 'd'){
                sendString("Speed Decreased\n");
                command = 'd';       
            } else if (userIn == '0'){
                sendString("Coordinated received\n");
                command = (char)0;
                x = 0; 
                SRAM(0, 0, 0, 0); 
                sendString("Current position: 0 \n");
            } else if (userIn == '1'){
                sendString("Coordinated received\n");
                command = (char)1;
                x = 1;
                SRAM(0, 0, 0, 1); 
                sendString("Current position: 1 \n");             
            } else if (userIn == '2'){
                sendString("Coordinated received\n");
                command = (char)2;
                x = 2;
                SRAM(0, 0, 1, 0);
                sendString("Current position: 2 \n");              
            } else if (userIn == '3'){
                sendString("Coordinated received\n");
                command = (char)3;
                x = 3;
                SRAM(0, 0, 1, 1);
                sendString("Current position: 3 \n");           
            } else if (userIn == '4'){
                sendString("Coordinated received\n");
                command = (char)4;
                x = 4;
                SRAM(0, 1, 0, 0);
                sendString("Current position: 4 \n");               
            } else if (userIn == '5'){
                sendString("Coordinated received\n");
                command = (char)5;
                SRAM(0, 1, 0, 1);
                x = 5;
                sendString("Current position: 5 \n");             
            } else if (userIn == '6'){
                sendString("Coordinated received\n");
                command = (char)6;
                SRAM(0, 1, 1, 0);
                x = 6;
                sendString("Current position: 6 \n");
            } else if (userIn == '7'){
                sendString("Coordinated received\n");
                command = (char)7;
                x = 7;
                SRAM(0, 1, 1, 1);
                sendString("Current position: 7 \n");
            } else if (userIn == '8'){
                sendString("Coordinated received\n");
                command = (char)8;
                x = 8;
                SRAM(1, 0, 0, 0);
                sendString("Current position: 8 \n");
            } else if (userIn == '9'){
                sendString("Coordinated received\n");
                command = (char)9;
                x = 9;
                SRAM(1, 0, 0, 1);
                sendString("Current position: 9 \n");
            } else if (userIn == 'o') {
                sendString("Turn on motor\n");
                command = 'o';               
            } else if (userIn == 'f') {
                sendString("Turn off motor\n");
                command = 'f';
            } else if (userIn == 'k') {
                sendString("Keep current speed\n");
                command = 'k';
            }
            I2C1_MasterWrite(&command, length, RTC_addres, &w_status);
        } 
        
        __delay_ms(10);
    }
}
/**
 End of File
*/