//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"


//*****************************************************************************
//
// Define pin to LED mapping.
//
//*****************************************************************************

#define USER_LED1  GPIO_PIN_1 //Número do pino, nao especifica a porta
#define USER_LED2  GPIO_PIN_0
#define USER_LED3  GPIO_PIN_4 //Número do pino, nao especifica a porta
#define USER_LED4  GPIO_PIN_0


//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//*****************************************************************************
//
// Main 'C' Language entry point.
//
//*****************************************************************************
int
main(void)
{
    uint32_t ui32SysClock;
    uint32_t estado = 0;


    //
    // Run from the PLL at 120 MHz.
    //
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                       SYSCTL_OSC_MAIN |
                                       SYSCTL_USE_PLL |
                                       SYSCTL_CFG_VCO_480), 120000000);

    //
    // Enable and wait for the port to be ready for access
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
    {
    }

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ); // Habilita GPIO J (push-button SW1 = PJ0, push-button SW2 = PJ1)
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ)); // Aguarda final da habilitação

    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1); // push-buttons SW1 e SW2 como entrada
    GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);
    //
    // Loop Forever
    //
    while(1)
    {
        if(!(GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0))){
                SysCtlDelay(ui32SysClock/20);
                if(estado == 0){
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
                    GPIOPinWrite(GPIO_PORTN_BASE, (USER_LED1|USER_LED2), USER_LED1);
                    SysCtlDelay(ui32SysClock/20);
                    estado++;
                }else if(estado == 1){
                    GPIOPinWrite(GPIO_PORTN_BASE, (USER_LED1|USER_LED2), USER_LED2);
                    SysCtlDelay(ui32SysClock/20);
                    estado++;
                }else if(estado == 2){
                    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
                    GPIOPinWrite(GPIO_PORTF_BASE, (USER_LED3|USER_LED4), USER_LED3);
                    SysCtlDelay(ui32SysClock/20);
                    estado++;
                }else if(estado == 3){
                    GPIOPinWrite(GPIO_PORTF_BASE, (USER_LED3|USER_LED4), USER_LED4);
                    SysCtlDelay(ui32SysClock/20);
                    estado++;
                }else if(estado == 4){
                        estado = 0;

                }
                } if(!(GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_1))){
                    SysCtlDelay(ui32SysClock/20);
                    estado = 0;
                    SysCtlDelay(ui32SysClock/20);
               }
    }
    }
