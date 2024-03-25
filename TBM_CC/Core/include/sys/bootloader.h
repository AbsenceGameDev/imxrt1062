/**
 * @authors   Ario Amin @ Permadev, 
 * @copyright Copyright (c) 2021-2024, MIT-License included in project toplevel dir
 */

#if DEGUB
  #define BL_DEFAULT_PERIPHIAL_DETECT_TIMEOUT 10000
#else
  #define BL_DEFAULT_PERIPHIAL_DETECT_TIMEOUT 5000
#endif // DEBUG

/**
 * @brief: vector tables, 12.5.1, p.261
 **/
// The bootloader will check for this address in the application vector
#ifndef BL_APP_VECTOR_TABLE_ADDRESS
  #define BL_APP_VECTOR_TABLE_ADDRESS (0x10000 + 0x60000000)
#endif

#define BL_PARAMETERS_ADDRESS (0xF000 + 0x60000000)

#define FLASH_VALID_START BL_APP_VECTOR_TABLE_ADDRESS
#define FLASH_VALID_END   (BL_FLEXSPI_AMBA_BASE + 8 * 1024 * 1024)