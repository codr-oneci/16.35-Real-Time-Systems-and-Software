/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
    /* Enable CM4.  CY_CORTEX_M4_APPL_ADDR must be updated if CM4 memory layout is changed. */
    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR); 

    int pinReadValue=0;
    for(;;)
    {if (pinReadValue==1){
        Cy_GPIO_Write(LED5_GREEN_0_PORT,LED5_GREEN_0_NUM,0);
        CyDelay(250);
        Cy_GPIO_Write(LED5_GREEN_0_PORT,LED5_GREEN_0_NUM,1);
        CyDelay(250);}
    else CyDelay(500);
    pinReadValue = pinReadValue =  Cy_GPIO_Read( SW2_PORT, SW2_NUM );
        
    }
}


