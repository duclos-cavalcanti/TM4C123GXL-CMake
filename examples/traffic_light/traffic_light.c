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

// GPIO Port B
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_PUR_R        (*((volatile unsigned long *)0x40005510))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_LOCK_R       (*((volatile unsigned long *)0x40005520))
#define GPIO_PORTB_CR_R         (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))

// Clock for GPIOs
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

// NVICs for Systick initializtion
#define NVIC_ST_CTRL_R      (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R    (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R   (*((volatile unsigned long *)0xE000E018))


#define TRAFFIC_LIGHTS      (*((volatile unsigned long *)0x400053FC))
#define SENSORS             (*((volatile unsigned long *)0x400243FC))

#define GoW     0
#define waitW   1
#define GoE     2
#define waitE   3

typedef struct state {
        unsigned long out;
        unsigned short wait;
        // unsigned long walk;
        unsigned long next[4];

} state_t;

//   Global Variables
unsigned long traffic, sensors;
unsigned long input;
unsigned char current_state;
state_t FSM[4] = {                                      
//      out   wait   input Combination                 
//                   PE1=0, PE0=0 - no cars                             -       Translates to 0 (HEXA)
//                   PE1=0, PE0=1 - cars on the East road               -       Translates to 1 (HEXA)
//                   PE1=1, PE0=0 - cars on the West road               -       Translates to 2 (HEXA)
//                   PE1=1, PE0=1 - cars on both East && North roads    -       Translates to 3 (HEXA)
//
//                                                                           RYG RYG
        {0x21, 30,  {GoW, waitW, GoW, waitW}},         // Go West      - 00 100 001 - RedE GreenW
        {0x22, 20,  {GoE, GoE, GoE, GoE}},             // Wait West    - 00 100 010 - RedE YellowW
        {0x0C, 30,  {GoE, GoE, waitE, waitE}},         // Go East      - 00 001 100 - GreenE RedW
        {0x14, 20,  {GoW, GoW, GoW, GoW}},             // Wait East    - 00 010 100 - YellowE RedW
};

//   Function Prototypes
void SysTick_Init(void);
void Ports_Init(void);
void SysTick_Wait(unsigned long delay);
void SysTick_Wait10ms(unsigned long delay);
// void EnableInterrupts(void);
// void DisableInterrupts(void);

int main(void)
{    
        SysTick_Init();         // Initialize Systick
        Ports_Init();           // Initialize Ports
        EnableInterrupts();     // Enables interrupts
        current_state = GoW;    // Go West State
        while(1){
                TRAFFIC_LIGHTS = FSM[current_state].out;
                SysTick_Wait10ms(FSM[current_state].wait);
                input = GPIO_PORTE_DATA_R & 0x03; // 0-PE2-PE1-PE0 & 0011
                current_state = FSM[current_state].next[input];
        }
}

void SysTick_Init(void)
{
        NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
        NVIC_ST_CTRL_R = 0x00000005;      // enable SysTick with core clock
}

void SysTick_Wait10ms(unsigned long delay)
{
        unsigned long i;
        for(i=0; i<delay; i++){
                unsigned long val = 800000;             // 800000*12.5ns equals 10ms
                NVIC_ST_RELOAD_R = val-1;               // number of counts to wait
                NVIC_ST_CURRENT_R = 0;                  // any value written to CURRENT clears
                while((NVIC_ST_CTRL_R&0x00010000)==0){} // Wait for count flag
        }
}

void Ports_Init()
{
        unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x32; // activate clock for Port B,E,F
        delay = SYSCTL_RCGC2_R; // allow time for clock to start

	// Port B - PortB5-B3 Traffic Light East / PortB2-B0 Traffic Light West
        GPIO_PORTB_LOCK_R = 0x4C4F434B;   // unlock port
        GPIO_PORTB_CR_R = 0x3F;           // allow changes to PB5-PB0
	GPIO_PORTB_PCTL_R = 0x00000000;   // clear PCTL
        GPIO_PORTB_AMSEL_R &= ~0x3F;      // disable analog on PB5-PB0
        GPIO_PORTB_AFSEL_R &= ~0x3F;      // disable alt funct on PB5-PB0
        GPIO_PORTB_DEN_R |= 0x3F;         // enable digital I/O on PB5-PB0
	GPIO_PORTB_DIR_R |= 0x3F;         // PB5-PB0 outputs

	// Ports E - PortE0 East Sensor / PortE1 West Sensor / PortE2 Walk Sensor
        GPIO_PORTE_LOCK_R = 0x4C4F434B;   // unlock port
        GPIO_PORTE_CR_R = 0x07;           // allow changes to PE2-PE0
        GPIO_PORTE_PCTL_R = 0x00000000;   // clear PCTL
        GPIO_PORTE_AMSEL_R &= ~0x07;      // disable analog on PE2-PE0
        GPIO_PORTE_AFSEL_R &= ~0x07;      // disable alt funct on PE2-PE0
        GPIO_PORTE_PUR_R &= ~0x07;        // disableb pull-up on PE2-PE0
        GPIO_PORTE_DEN_R |= 0x07;         // enable digital I/O on PE2-PE0
	GPIO_PORTE_DIR_R &= ~0x07;        // PE2-PE0 inputs

	// Port F - PortF1 Dont Walk Light(RED) / Port F3 Walk Light(GREEN)
        GPIO_PORTF_LOCK_R = 0x4C4F434B;   // unlock port
        GPIO_PORTF_CR_R = 0x0A;           // allow changes to PF1 & PF3
	GPIO_PORTF_PCTL_R = 0x00000000;   // clear PCTL
        GPIO_PORTF_AMSEL_R &= ~0x0A;      // disable analog on PF1 & PF3
        GPIO_PORTF_AFSEL_R &= ~0x0A;      // disable alt funct on PF1 & PF3
        GPIO_PORTF_DEN_R |= 0x0A;         // enable digital I/O on PF1 & PF3
	GPIO_PORTF_DIR_R |= 0x0A;         // PF1 & PF3 outputs
}
