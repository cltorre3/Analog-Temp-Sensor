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
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18F66J65
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
#include "mcc_generated_files/eusart1.h"
#include <xc.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

uint16_t convertedValue; //defines variable of value from ADC 
int calculatedValue;


  uint8_t rxData = 0;
  
  //float   boundingCondition;
  //float   rxDataConvertL;
  //float   rxDataConvertU;

  int rx_Convert = 1; //receive
  int tx_Convert = 0; //transmit
  
  void EUSART1ISR(){
         
        IO_RB4_SetHigh(); // Turns red to know it has exceeded temperature
        __delay_ms(500);
        IO_RB4_SetLow();   //rns red to know it has exceeded temperature
        __delay_ms(500);
  
     EUSART1_Receive_ISR();
  
  if(rx_Convert == 1)
     
      while(!EUSART1_DataReady);
      
      rxData = EUSART1_Read();
      
      tx_Convert = 1;
      rx_Convert = 0;
      
  }
  
  
  void txADC()
{
    if(tx_Convert == 1){ // if tx flag is ready
        while(!EUSART1_is_tx_ready()); // wait until tx is ready
        printf("Temperature: %d", calculatedValue);
       __delay_ms(3000);
          while(!EUSART1_is_tx_done());
    }
  }
  


void main(void){
    // Initialize the device
    SYSTEM_Initialize();
    ADC_Initialize(); // initialize for ADC thermistor 
    EUSART1_Initialize(); //initialize EUSART to send to ESP32
    EUSART1_SetRxInterruptHandler(EUSART1ISR); // sets handler to send to esp
    
    int newVariable; // sets variable for idk yet
    int button;
    
    adc_result_t convertedValue; 

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
    
    //RB5 = Red
    //RB4 = Green
    //RF7 = Blue
    //RB3 = White

    while (1)
    {
        ADC_SelectChannel(channel_AN0); //tell the adc to read signal from that port
        ADC_StartConversion(); //start the conversion of data
        convertedValue = ADC_GetConversionResult(); //set the converted value to the value that is converted
        calculatedValue = convertedValue * 0.0757; //Multiply decimal value by bit per temperature range
        
        if (calculatedValue > 75)
        {
         IO_RB4_SetHigh(); // Turns red to know it has exceeded temperature
        }
        else
        {
        IO_RB4_Toggle(); // Turns red to know if it has not exceeded temperature
        }
        
        /*button = IO_RC2_SetDigitalInput();
        if (button = 1)
        {
            IO_RB5_SetHigh();
        }
        else
        {
            IO_RB5_SetLow();
        }
         */   
        }
        
    }  
