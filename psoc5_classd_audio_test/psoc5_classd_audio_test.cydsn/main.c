
#include "project.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    PrISM_1_Start();
    isr_1_Start();
    Counter_1_Start();
    
    while(1)
    {
    }
}
