// #include "tm4c123gh6pm.h"

// GPIO Port F                     - Could also be uint32_t
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC)) 
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400)) 
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420)) 
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510)) 
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C)) 
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520)) 
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524)) 
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528)) 
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C)) 

// GPIO Port E
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PUR_R        (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_LOCK_R       (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))

#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

// LEDS
// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E
// pink     R-B    0x06 

// GPIO PortF Init
void ExamplePortF_Init(void)
{ 
        // Examples
        // Board Switches
        // They have negative logic, 0 means pressed!
        // SW1 = GPIO_PORTF_DATA_R&0x10;     // read PF4 into SW1
        // SW2 = GPIO_PORTF_DATA_R&0x01;     // read PF0 into SW2
        //
        // Board Multicolor LED
        // GPIO_PORTF_DATA_R = 0x02;    // LED is red
        // GPIO_PORTF_DATA_R = 0x04;    // LED is blue
        // GPIO_PORTF_DATA_R = 0x08;    // LED is green
        // GPIO_PORTF_DATA_R = 0x00;    // LED is off
        //
        // LOCK
        // Only PC3-0, PD7, and PF0 on the TM4C need to be unlocked.
        volatile unsigned long delay;
        SYSCTL_RCGC2_R |= 0x00000020;   // 1) Enable/Set F clock
        delay = SYSCTL_RCGC2_R;         // delay - wait 3-5 bus cycles 
        GPIO_PORTF_LOCK_R = 0x4C4F434B; // 2) unlock PortF PF0  
        GPIO_PORTF_CR_R = 0x1F;         // allow changes to PF4-PF0       
        GPIO_PORTF_AMSEL_R = 0x00;      // 3) disable analog function
        GPIO_PORTF_PCTL_R = 0x00000000; // 4) GPIO clear bit PCTL  
        GPIO_PORTF_DIR_R = 0x0E;        // 5) PF4,PF0 input, PF3,PF2,PF1 output   
        GPIO_PORTF_AFSEL_R = 0x00;      // 6) no alternate function
        GPIO_PORTF_PUR_R = 0x11;        // enable pullup resistors on PF4,PF0 (Switches)
        GPIO_PORTF_DEN_R = 0x1F;        // 7) enable digital pins PF4-PF0        
}

// GPIO PortE Init
void ExamplePortE_Init(void) 
{
        volatile unsigned long delay;
        SYSCTL_RCGC2_R |= 0x10;         // Port E clock
        delay = SYSCTL_RCGC2_R;         // delay - wait 3-5 bus cycles
        GPIO_PORTE_DIR_R |= 0x02;       // PE1 output
        GPIO_PORTE_DIR_R &= 0xFE;       // PE0 input
        GPIO_PORTE_AFSEL_R = 0x00;      // not alternative
        GPIO_PORTE_AMSEL_R &= 0x00;     // no analog
        GPIO_PORTE_PCTL_R &= 0x00;      // bits for PE4,PE2,PE1,PE0
        GPIO_PORTE_DEN_R |= 0x01;       // enable PE0
	GPIO_PORTE_DEN_R |= 0x02;       // enable PE1
}
