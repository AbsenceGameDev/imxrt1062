#include "system_memory_map.h"

int main()
{
    // Configure GPIO B0_03 (PIN 13) for output
    IOMUXC_MUX_PAD_GPIO_B0_CR03 = 0x5;
    IOMUXC_MUX_PAD_GPIO_B0_CR03 = IOMUXC_PAD_DSE(0x7);
    IOMUXC_GPR_GPR27 = 0xffffffff;
    GPIO7_DIRR |= (0x1 << 0x3);

    for(;;) {
        volatile unsigned int i = 0x0;

        // Set PIN 13 HIGH
        GPIO7_DR_SET = (0x1 << 0x3);

        // Poor man's delay
        while(i < 0x10000) {
            i++;
        }

        i = 0;

        // Set PIN 13 LOW
        GPIO7_DR_CLEAR = (0x1 << 0x3);

        // Poor man's delay
        while(i < 0x010000) {
            i++;
        }
    }
}
