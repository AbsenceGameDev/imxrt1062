#include "system_memory_map.h"

/**
 * @note The values of the MPU_RASR registers from reset are UNKNOWN. All
 * MPU_RASR registers must be programmed as either enabled or disabled, before
 * enabling the MPU using the MPU_CTRL register.
 **/
void
configure_mpu()
{
  // This is not the actual body I am looking to create
  // But it will show order of execution needed when doing these things
  uint8_t region = 0x1;
  uint8_t power = 0x4;
  // Preamble
  MPU_RNR_REQ_REGIONS(region); // request region 1
  MPU_RBAR_REGION_W(region); // sets rbar to region 1
  MPU_RASR_SET_REGION_SIZE(power); // sets region size to (2^(x+1)) bytes

  // Lock/Unlock
  MPU_RASR_NOT_PERMIT_EXEC; // Lock region
  MPU_RASR_PERMIT_EXEC; // Unlock region

  // Access privilege
  MPU_RASR_AP_NO_RW; // Sets: No Read/Write access
  MPU_RASR_AP_PRIVL_RW; // Sets: Read/Write Privileged access
  MPU_RASR_AP_FULL_RW; // Sets: Read/Write, Full access
  MPU_RASR_AP_PRIVL_RO; // Sets: Read Only, Privileged access
  MPU_RASR_AP_FULL_RO; // Sets: Read Only, Full access
  MPU_RASR_AP_FULL_RO2; // Sets: Read Only, Full access

  // Decide whether to have shareable bits in the S field

  // Set TEX, C, B values

  // Set MPU_CTRL_ENABLE after all RASR regions settings
}