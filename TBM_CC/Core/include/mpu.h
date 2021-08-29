/**
 * @file      mpu.h
 * @author    Ario@Permadev
 * @brief
 * @version   0.1
 * @date      2021-08-29
 *
 * @copyright Copyright (c) 2021, MIT-License included in project toplevel dir
 *
 */

#ifndef MPU_H
#define MPU_H
#include "system_memory_map.h"

/**
 * @brief Configure MPU (Memory Protection Unit) based on armv7m docs
 * NOTE:
 * In Resources/./DDI0489B_cortex_m7_trm.pdf:
 * 4.1(p.56), 5.x(p.61) & 6.x(p.99)
 *
 * In Resources/./DDI0403E_d_armv7m_arm.pdf:
 * A3.5(p.78), A3.6(p.87), A3.7(p.89) & B3.5(p.632)
 *
 * @note The values of the MPU_RASR registers from reset are UNKNOWN. All
 * MPU_RASR registers must be programmed as either enabled or disabled, before
 * enabling the MPU using the MPU_CTRL register.
 **/
typedef struct {
  vuint32_t TYPE;    // RO
  vuint32_t CTRL;    // RW
  vuint32_t RNR;     // RW
  vuint32_t RBAR;    // RW
  vuint32_t RASR;    // RW
  vuint32_t RBAR_A1; // RW
  vuint32_t RASR_A1; // RW
  vuint32_t RBAR_A2; // RW
  vuint32_t RASR_A2; // RW
  vuint32_t RBAR_A3; // RW
  vuint32_t RASR_A3; // RW
} SReg_MPU;

#define MPU_BASE_ADDR MAP_32BIT_ANYREG(SReg_MPU, 0x0e00ed90)
#define MPU_TYPE      (MPU_BASE_ADDR.TYPE)
#define MPU_CTRL      (MPU_BASE_ADDR.CTRL)
#define MPU_RNR       (MPU_BASE_ADDR.RNR)
#define MPU_RBAR      (MPU_BASE_ADDR.RBAR)
#define MPU_RASR      (MPU_BASE_ADDR.RASR)
/**
 * ==========================================================================
 * The MPU_TYPE register characteristics are:
 * Purpose:
 *   The MPU Type Register indicates how many regions the MPU support.
 *   Software can use it to determine if the processor implements an MPU.
 * Usage Constraints - There are no usage constraints.
 * Configurations - Always implemented.
 * Attributes - See Table B3-11 above.
 * The MPU_TYPE register bit assignments are:
 *         _______________________________________
 * Fields: |RESERVED|IREGION|DREGION|RESERVED|SEP|
 * Bitpos: |31    24|23   16|15    8|7      1| 0 |
 *         |________|_______|_______|________|___|
 * IREGION:  MPU_TYPE_IREGION_READ
 * DREGION:  MPU_TYPE_DREGION_READ
 * SEPARATE: MPU_TYPE_SEP_READ
 *           MPU_TYPE_SEP_EN
 *           MPU_TYPE_SEP_DIS
 * ==========================================================================
 **/

/**
 * @brief MPU_TYPE_IREGION_READ - Instruction Region.
 * Instruction region. RAZ. ARMv7-M only supports a unified MPU.
 **/
#define MPU_TYPE_IREGION_READ ((MPU_TYPE >> 0x10) & 0x8)

/**
 * @brief MPU_TYPE_DREGION_READ - Amount of data regions.
 * Number of regions supported by the MPU. If this field reads-as-zero
 * the processor does not implement an MPU.
 **/
#define MPU_TYPE_DREGION_READ ((MPU_TYPE >> 0x8) & 0x8)

/**
 * @brief MPU_TYPE_SEP_READ - Seperate instruction and d-addr support.
 * Indicates support for separate instruction and data address maps. RAZ.
 * ARMv7-M only supports a unified MPU.
 **/
#define MPU_TYPE_SEP_READ (MPU_TYPE & 0x1)

/** @brief Enable SEP */
#define MPU_TYPE_SEP_EN (MPU_TYPE |= 0x1)

/** @brief Disable SEP */
#define MPU_TYPE_SEP_DIS MPU_TYPE &= ~(0x1);

/**
 * ==========================================================================
 * The MPU_CTRL register characteristics are:
 * Purpose:
 *   Enables the MPU, and when the MPU is enabled, controls whether the default
 *   memory map is enabled as a background region for privileged accesses, and
 *   whether the MPU is enabled for HardFaults, NMIs, and exception handlers
 *   when FAULTMASK is set to 1.
 *
 * Usage Constraints - There are no usage constraints.
 * Configurations - Always implemented.
 * Attributes - See Table B3-11 above.
 * The MPU_TYPE register bit assignments are:
 *         _________________________________________________
 * Fields: |      RESERVED      |PRIVDEFENA|HFNMIENA|ENABLE|
 * Bitpos: |31                 3|     2    |    1   |   0  |
 *         |____________________|__________|________|______|
 * PRIVDEFENA: PRVDFENA_MPU, Use when the ENABLE bit is set to 1.
 * HFNMIENA:   HFNMIENA_MPU, When the ENABLE bit is set to 1, controls whether
 *                           handlers executing with priority less than 0 access
 *                           memory with the MPU enabled or with the MPU
 *                           disabled. This applies to HardFaults, NMIs, and
 *                           exception handlers when FAULTMASK is set to 1:
 * ENABLE:     ENABLE_MPU, ENABLE bit.
 * ==========================================================================
 **/
/** @brief MPU_CTRL_PRVDFENA_CLR - Disables the default memory map. Any
 *instructions or data access that does not access a defined region faults */
#define MPU_CTRL_PRVDFENA_CLR MPU_CTRL &= ~(0x1 << 0x2)
/** @brief MPU_CTRL_PRVDFENA_SET - MPU Enabled */
#define MPU_CTRL_PRVDFENA_SET MPU_CTRL |= (0x1 << 0x2)

/** @brief MPU_CTRL_HFNMIENA_CLR - Disables the MPU for these handlers */
#define MPU_CTRL_HFNMIENA_DIS MPU_CTRL &= ~(0x1 << 0x1)
/** @brief MPU_CTRL_HFNMIENA_SET - Use MPU for mem. access by these handlers */
#define MPU_CTRL_HFNMIENA_EN MPU_CTRL |= (0x1 << 0x1)

/** @brief MPU_CTRL_ENABLE_BIT_CLR - Clears the enable-bit for the MPU. */
#define MPU_CTRL_ENABLE_BIT_CLR MPU_CTRL &= ~(0x1 << 0x1)
/** @brief MPU_CTRL_ENABLE_BIT_SET - Sets the enable-bit for the MPU. */
#define MPU_CTRL_ENABLE_BIT_SET MPU_CTRL |= (0x1 << 0x1)

/**
 * ==========================================================================
 * The MPU_RNR register characteristics are:
 * Purpose:
 *   Selects the region currently accessed by MPU_RBAR and MPU_RASR.
 *
 * Usage Constraints - Used with MPU_RBAR and MPU_RASR,
 *                     see MPU Region Base Address Register, MPU_RBAR (p.639),
 *                     and MPU Region Attr. and Size Register, MPU_RASR (p.640).
 *                     If an implementation supports N regions then the regions
 *                     number from 0 to (N-1), and the effect of writing a value
 *                     of N or greater to the REGION field is UNPREDICTABLE.
 *
 * Configurations - Implemented only if the processor implements an MPU.
 * Attributes - See Table B3-11 above.
 * The MPU_RNR register bit assignments are:
 *         _________________________
 * Fields: |   RESERVED   | REGION |
 * Bitpos: |31           8|7      0|
 *         |______________|________|
 * REGION: Indicates the memory region accessed by MPU_RBAR and MPU_RASR.
 * Normally, software must write the required region number to MPU_RNR to select
 * the required memory region, before accessing MPU_RBAR or MPU_RASR. However,
 * the MPU_RBAR.VALID bit gives an alternative way of writing to MPU_RBAR to
 * update a region base address without first writing the region number to
 * MPU_RNR, see MPU Region Base Address Register, MPU_RBAR.
 * ==========================================================================
 **/
/** @brief MPU_RNR_REQ_REGIONS - Writes the amount of requested regions */
#define MPU_RNR_REQ_REGIONS(x) ((MPU_RNR) = ((MPU_RNR)&0xffffff00) | ((x)&0xff))

/**
 * ==========================================================================
 * (p. 639)  The MPU_RBAR register characteristics are:
 * Purpose - Holds the base addr. of the region identified by MPU_RNR.
 *           On a write, can also be used to update the base address of a
 *           specified region, in the range 0-5, updating MPU_RNR with the new
 *           region number.
 *
 * Usage constraints -
 *                    • Normally, used with MPU_RBAR,
 *                      see MPU Region Number Register, MPU_RNRon page B3-638.
 *                    • The minimum region alignment required by an MPU_RBAR is
 *                      IMPLEMENTATION DEFINED.
 *                      See the register description for more info about
 *                      permitted region sizes.
 *                    • If an implementation supports N regions then the regions
 *                      number from 0 to (N-1). If N is less than 16 the effect
 *                      of writing a value of N or greater to the REGION
 *                      field is UNPREDICTABLE.
 *
 * Configurations - Implemented only if the processor implements an MPU.
 * Attributes - See Table B3-11 above.
 * The MPU_RBAR register bit assignments are:
 *         ________________________________________
 * Fields: |          ADDR.          |VALID|REGION|
 * Bitpos: |31                      5|  4  |3    0|
 *         |_________________________|_____|______|
 * ADDR:   MPU_RBAR_REGION_ADDR
 * VALID:  On writes: Indicates whether the region to update is specified by
 *                    MPU_RNR.REGION, or by the REGION value specified in this
 *                    write. When using the REGION value specified by this
 *                    write, MPU_RNR.REGION is updated to this value.
 *                    MPU_RBAR_VALID_W_0
 *                    MPU_RBAR_VALID_W_1
 *         On reads:  MPU_RBAR_VALID_READ
 *
 * REGION: MPU_RBAR_REGION_ADDR_W
 *         MPU_RBAR_REGION_ADDR_R
 *
 * @note
 * Software can find the minimum size of region supported by an MPU region by
 * writing all ones to MPU_RBAR[31:5] for that region, and then reading the
 * register to find the value saved to bits[31:5].
 *
 * The number of trailing zeros in this bit field indicates the minimum
 * supported alignment and therefore the supported region size.
 *
 * An implementation must support all region size values from the minimum
 * supported to 4GB, see the description of the MPU_RASR.SIZE field in MPU
 * Region Attribute and Size Register, MPU_RASR on page B3-640.
 *
 * Software must ensure that the value written to the ADDR field aligns with the
 * size of the selected region.
 * ==========================================================================
 **/

/** @brief MPU_RBAR_REGION_ADDR - Writes the amount of requested regions */
#define MPU_RBAR_REGION_ADDR MPU_RBAR = (MPU_RBAR & 0xffffffe0) | (x & 0x1f)

/** @brief MPU_RBAR_VALID_W_0 - Apply the base address update to the region
 *         specified by MPU_RNR.REGION. The REGION field value is ignored.*/
#define MPU_RBAR_VALID_W_0 (MPU_RBAR &= 0xffffffef)

/** @brief MPU_RBAR_VALID_W_1 - Update MPU_RNR.REGION to the value obtained by
 *         zero extending the REGION value specified in this write, and apply
 *         the base address update to this region. */
#define MPU_RBAR_VALID_W_1 (MPU_RBAR |= ~(0xffffffef))

/** @brief MPU_RBAR_VALID_READ - This bit reads as zero. */
#define MPU_RBAR_VALID_READ ((MPU_RBAR & 0xef) >> 0x4)

/** @brief MPU_RBAR_REGION_W - Can specify the number of the region to update,
 *                             see VALID field description. */
#define MPU_RBAR_REGION_W(x) (MPU_RBAR = (MPU_RBAR & 0xffffffe0) | ((x)&0x1f))
/** @brief MPU_RBAR_REGION_R - Returns bits[3:0] of MPU_RNR. */
#define MPU_RBAR_REGION_R (MPU_RBAR & 0xf)

/**
 * @brief MPU Region Attribute and Size Register. (MPU_SASR)
 * ==========================================================================
 * B3.5.9 (p. 640) MPU Region Attribute and Size Register.
 *The MPU_RASR register characteristics are:
 *
 * Purpose - Defines the size and access behavior of the region identified by
 *           MPU_RNR, and enables that region.
 *
 * Usage constraints -
 *                    • Used with MPU_RNR, see MPU Region Number Register,
 *                      MPU_RNR on B3(p.638)
 *
 *                    • Writing a SIZE value less than the minimum size
 *                      supported by the corresponding MPU_RBAR
 *                      has an UNPREDICTABLE effect.
 *
 * Configurations - Implemented only if the processor implements an MPU.
 * Attributes - See Table B3-11 above.
 * The MPU_RBAR register bit assignments are:
 * RS = reserved
 *         _____________________________________________________________________
 * Fields: |  RS  |XN|RS|  AP |  RS |  TEX | S| C| B|     SRD    | RS| SIZE |EN|
 * Bitpos: |31  29|28|27|26 24|23 22|21  19|18|17|16|15         8|7 6|5    1| 0|
 *         |______|__|__|_____|_____|______|__|__|__|____________|___|______|__|
 * ATTRS: MPU_RASR_SET_ATTR..
 * SRD:   MPU_RASR_EN_SUBREGION, MPU_RASR_DIS_SUBREGION
 * SIZE:   MPU_RASR_REGION_SIZE, MPU_RASR_SET_REGION_SIZE
 * ENABLE: MPU_RASR_ENABLE, MPU_RASR_DISABLE
 **/

/**
 * @brief ENABLE: Enable field.
 * bit[0] in MPU_RASR
 *           Enables this region:
 *              0 -  When the MPU is enabled, this region is disabled.
 *              1 -  When the MPU is enabled, this region is enabled.
 *           Enabling a region has no effect unless the MPU_CTRL.ENABLE
 *           bit is set to 1, to enable the MPU.
 **/
#define MPU_RASR_ENABLE  (MPU_RASR |= (0x1))  // 0x1
#define MPU_RASR_DISABLE (MPU_RASR &= ~(0x1)) // 0x0

/**
 * @brief SIZE: Size Field.
 * bits[5,1] in MPU_RASR
 *          Indicates the region size. The region size, in bytes, is 2(SIZE+1).
 *          SIZE field values less than 4 are reserved, because the smallest
 *          supported region size is 32 bytes.
 **/
#define MPU_RASR_REGION_SIZE ((MPU_RASR >> 0x1) & 0x1f) // 0x1
#define MPU_RASR_SET_REGION_SIZE(power)                                        \
  (MPU_RASR = (MPU_RASR & ~(0b111110)) | (((power)&0b11111) << 0x1)) // 0x0

/**
 * @brief XN: Execute Never Encoding
 * bit[28] in MPU_RASR
 * Table B3-16
 * XN | Description
 * ===|======================================================================
 * 0  |Execution of an instruction fetched from this region permitted
 * 1  |Execution of an instruction fetched from this region not permitted
 * ==========================================================================
 **/
#define MPU_RASR_NOT_PERMIT_EXEC MPU_RASR |= (0x1 << 0x1c)  // 0x1
#define MPU_RASR_PERMIT_EXEC     MPU_RASR &= ~(0x1 << 0x1c) // 0x0

/**
 * @brief SRD:   [15,8]  Subregion Enable/Disable.
 * bits[15,8] in MPU_RASR
 * Memory region subregions
 *    For any region of 256 bytes or larger, the MPU divides the region into
 *    eight equally-sized subregions. Setting a bit in the SRD field to 1
 *    disables the corresponding subregion:
 *      • The least significant bit of the field, MPU_RASR[8], controls the
 *        subregion with the lowest address range.
 *      • The most significant bit of the field, MPU_RASR[15], controls the
 *        subregion with the highest address range.
 *    For region sizes of 32, 64, and 128 bytes, the effect of setting one or
 *    more bits of the SRD field to 1 is UNPREDICTABLE.
 *    See Sub-region support on page B3-633 for more information.
 **/
#define MPU_RASR_DIS_SUBREGION(region) (MPU_RASR |= (0x1 << (0x8 + (region))))
#define MPU_RASR_EN_SUBREGION(region)  (MPU_RASR &= ~(0x1 << (0x8 + (region))))

/**
 * @brief S bit-field
 * bit[18] in MPU_RASR
 * _a_ values in TEX,C & B fields are Shareable if the S bit is set to 1,
 *  Non-shareable if the S bit is set to 0
 **/
#define MPU_RASR_SET_S  (MPU_RASR |= (0x1 << 0x12))  // 0x1
#define MPU_RASR_CLR_S  (MPU_RASR &= ~(0x1 << 0x12)) // 0x0
#define MPU_RASR_READ_S ((MPU_RASR >> 0x12) & 0x1)

/**
 * @brief AP: Access permissions field Encoding:
 * bits[26,24] in MPU_RASR
 * Table B3-15
 * AP[2:0]|Priv.access|Unpriv.access|Notes |
 * =======|===========|=============|=========================================|
 * 000    |No access  |No access    |Any access generates a permission fault  |
 * 001    |Read/write |No access    |Privileged access only                   |
 * 010    |Read/write |Read-only    |Any unprivileged write - permission fault|
 * 011    |Read/write |Read/write   |Full access                              |
 * 100    |     UNPREDICTABLE       |Reserved                                 |
 * 101    |Read-only  |No access    |Privileged read-only                     |
 * 110    |Read-only  |Read-only    |Privileged and unprivileged read-only    |
 * 111    |Read-only  |Read-only    |Privileged and unprivileged read-only    |
 * ===========================================================================|
 *
 **/
typedef enum
{
  NO_RW = 0x0,
  PRIVL_RW = 0x1,
  PARTIAL_RW = 0x2,
  FULL_RW = 0x3,
  PRIVL_RO = 0x5,
  FULL_RO = 0x6,
  FULL_RO_2 = 0x7
} access_enc_e;
#define MPU_RASR_SET_AP(acc_perm_enc)                                          \
  (MPU_RASR = (MPU_RASR & ~(0x7 << 0x18)) | (((acc_perm_enc)&0x7) << 0x18))
#define MPU_RASR_AP_NO_RW      (MPU_RASR &= ~(0x7 << 0x18))
#define MPU_RASR_AP_PRIVL_RW   MPU_RASR_SET_AP(0x1)
#define MPU_RASR_AP_PARTIAL_RW MPU_RASR_SET_AP(0x2)
#define MPU_RASR_AP_FULL_RW    MPU_RASR_SET_AP(0x3)
#define MPU_RASR_AP_PRIVL_RO   MPU_RASR_SET_AP(0x5)
#define MPU_RASR_AP_FULL_RO    MPU_RASR_SET_AP(0x6)
#define MPU_RASR_AP_FULL_RO2   (MPU_RASR |= (0x7 << 0x18))

/**
 * @brief ATTRS:  The MPU Region Attribute fields.
 *                Defined in Region attribute control on (p.B3-641):
 *                [XN, AP[2:0], TEX[2:0], S, C, B]
 * bits[31,16] in MPU_RASR
 *
 * Table B3-13 TEX, C, B, and S Encoding: (OI = Outer & inner)
 * TEX|C|B|Memory type |Descr., or Normal region Cacheability |Shareable?   |
 * ===|=|=|============|======================================|=============|
 * 000|0|0|Strong-order|Strongly ordered                      |Shareable    |
 * 000|0|1|Device      |Shared device                         |Shareable    |
 * 000|1|0|Normal      |OI Write-Through, no write allocate   |S bit(a)     |
 * 000|1|1|Normal      |OI write-back, no write allocate      |S bit(a)     |
 * 001|0|0|Normal      |OI Non-cacheable                      |S bit(a)     |
 * 001|1|0|            |        IMPLEMENTATION DEFINED                      |
 * 001|1|1|Normal      |OI write-back; w/r allocate           |S bit(a)     |
 * 010|0|0|Device      |Non-shared device                     |Non-shareable|
 * 1BB|A|A|Normal      |Cached memory, with AA and BB         |S bit(a)     |
 *                     |indicating the inner and outer                      |
 *                     |Cacheability rules that must be exported on the bus.|
 *                     |See Table B3-14.                                    |
 *                     |BB = Outer policy,AA == Inner policy.               |
 * =========================================================================|
 * Table B3-14 Cache policy Encoding:
 * AA or BB subfield of {TEX,C,B} enc.  | Cacheability policy               |
 * =====================================|===================================|
 * 00                                   |Non-cacheable                      |
 * 01                                   |Write-Back, write and read allocate|
 * 10                                   |Write-Through, no write allocate   |
 * 11                                   |Write-Back, no write allocate      |
 * =========================================================================|
 * Region attribute control
 *    The MPU_RASR.ATTRS field defines the memory type, and where necessary the
 *    cacheable, shareable, and access and privilege properties of the memory
 *    region. The register diagram shows the subfields of this field, where:
 *      • The TEX[2:0], C, and B bits together indicate the memory type of the
 *        region, and:
 *          — For Normal memory, the cacheable properties of the region.
 *          — For Device memory, whether the region is shareable.See Table B3-13
 *            for the encoding of these bits.
 **/
typedef enum
{
  NON_CACHEABLE = 0x0,
  WRITEBACK_RW_ALOC = 0x1,
  WRITETHROUGH_NO_W_ALOC = 0x2,
  WRITEBACK_NO_W_ALOC = 0x3
} cache_policy_e;

/**
 * @brief TEX bit-field
 * bits[21,19] in MPU_RASR
 * _a_ Shareable if the S bit is set to 1, Non-shareable if the S bit is set to 0
 **/
typedef enum
{
  TEX_STRONG_ORDER = 0x0,
  TEX_SHARED_DEVICE = 0x0,
  TEX_a_OI_WT_NO_W_ALOC = 0x0,
  TEX_a_OI_WB_NO_W_ALOC = 0x0,
  TEX_a_OI_NON_CACHEABLE = 0x1,
  TEX_IMPL_DEF = 0x1,
  TEX_a_OI_WB_RW_ALOC = 0x1,
  TEX_NON_SHARED_DEVICE = 0x2,
  TEX_a_CMEM_NC = 0x4 | NON_CACHEABLE,
  TEX_a_CMEM_NC_WB_RW_ALOC = 0x4 | WRITEBACK_RW_ALOC,
  TEX_a_CMEM_NC_WT_NO_W_ALOC = 0x4 | WRITETHROUGH_NO_W_ALOC,
  TEX_a_CMEM_WB_NO_W_ALOC = 0x4 | WRITEBACK_NO_W_ALOC
} tex_enc_e;

/** @brief Input should be of above type */
#define MPU_RASR_SET_ATTR_TEX(texenc)                                          \
  (MPU_RASR = (MPU_RASR & ~(0x7 << 0x13)) | (((texenc)&0x7) << 0x13))
#define MPU_RASR_READ_ATTR_TEX ((MPU_RASR >> 0x13) & 0x7)

/**
 * @brief C bit-field
 * bit[17] in MPU_RASR
 * _a_ Shareable if the S bit is set to 1, Non-shareable if the S bit is set to 0
 **/
typedef enum
{
  C_STRONG_ORDER = 0x0,
  C_SHARED_DEVICE = 0x0,
  C_a_OI_WT_NO_W_ALOC = 0x1,
  C_a_OI_WB_NO_W_ALOC = 0x1,
  C_a_OI_NON_CACHEABLE = 0x0,
  C_IMPL_DEF = 0x1,
  C_a_OI_WB_RW_ALOC = 0x1,
  C_NON_SHARED_DEVICE = 0x0,
  C_a_CMEM_NC = ((NON_CACHEABLE >> 0x2) & 0x1),
  C_a_CMEM_NC_WB_RW_ALOC = ((WRITEBACK_RW_ALOC >> 0x2) & 0x1),
  C_a_CMEM_NC_WT_NO_W_ALOC = ((WRITETHROUGH_NO_W_ALOC >> 0x2) & 0x1),
  C_a_CMEM_WB_NO_W_ALOC = ((WRITEBACK_NO_W_ALOC >> 0x2) & 0x1)
} cbit_enc_e;

/** @brief Input should be of above type */
#define MPU_RASR_SET_ATTR_C(cenc)                                              \
  (MPU_RASR = (MPU_RASR & ~(0x1 << 0x11)) | (((cenc)&0x1) << 0x11))
#define MPU_RASR_READ_ATTR_C ((MPU_RASR >> 0x11) & 0x7)

/**
 * @brief B bit-field
 * bit[16] in MPU_RASR
 * _a_ Shareable if the S bit is set to 1, Non-shareable if the S bit is set to 0
 **/
typedef enum
{
  B_STRONG_ORDER = 0x0,
  B_SHARED_DEVICE = 0x0,
  B_a_OI_WT_NO_W_ALOC = 0x0,
  B_a_OI_WB_NO_W_ALOC = 0x0,
  B_a_OI_NON_CACHEABLE = 0x1,
  B_IMPL_DEF = 0x1,
  B_a_OI_WB_RW_ALOC = 0x1,
  B_NON_SHARED_DEVICE = 0x2,
  B_a_CMEM_NC = NON_CACHEABLE & 0x1,
  B_a_CMEM_NC_WB_RW_ALOC = WRITEBACK_RW_ALOC & 0x1,
  B_a_CMEM_NC_WT_NO_W_ALOC = WRITETHROUGH_NO_W_ALOC & 0x1,
  B_a_CMEM_WB_NO_W_ALOC = WRITEBACK_NO_W_ALOC & 0x1
} bbit_enc_e;

/** @brief Input should be of above type */
#define MPU_RASR_SET_ATTR_B(benc)                                              \
  (MPU_RASR = (MPU_RASR & ~(0x1 << 0x10)) | (((benc)&0x1) << 0x10))
#define MPU_RASR_READ_ATTR_B ((MPU_RASR >> 0x10) & 0x7)

/**
 * ==========================================================================
 * B3.5.10  MPU alias register support
 * The MPU_RBAR and MPU_RASR form a pair of words in the address range
 * 0xE000ED9C-0xE000EDA3.
 *
 * An ARMv7-M processor implements aliases of this address range at offsets of
 * +8 bytes, +16 bytes, and +24 bytes from the MPU_RBAR address of 0xE000ED9C,
 * as Table B3-11 on page B3-635 shows.
 *
 * Using these register aliases with the MPU_RBAR.REGION field, and the
 * MPU_RBAR.VALID field set to 1, software can use a stream of word writes to
 * update efficiently up to four regions, provided all the regions accessed are
 * in the range region 0 to region 15
 * p.642 0xE000EDA4  MPU_RBAR_A1 RW         -       Alias 1 of MPU_RBAR
 * p.642 0xE000EDA8  MPU_RASR_A1 RW         -       Alias 1 of MPU_RASR
 * p.642 0xE000EDAC  MPU_RBAR_A2 RW         -       Alias 2 of MPU_RBAR
 * p.642 0xE000EDB0  MPU_RASR_A2 RW         -       Alias 2 of MPU_RASR
 * p.642 0xE000EDB4  MPU_RBAR_A3 RW         -       Alias 3 of MPU_RBAR
 * p.642 0xE000EDB8  MPU_RASR_A3 RW         -       Alias 3 of MPU_RASR
 * ==========================================================================
 **/
#define MPU_RBAR_A1 MPU_BASE_ADDR.RBAR_A1
#define MPU_RASR_A1 MPU_BASE_ADDR.RASR_A1
#define MPU_RBAR_A2 MPU_BASE_ADDR.RBAR_A2
#define MPU_RASR_A2 MPU_BASE_ADDR.RASR_A2
#define MPU_RBAR_A3 MPU_BASE_ADDR.RBAR_A3
#define MPU_RASR_A3 MPU_BASE_ADDR.RASR_A3

/**
 * @brief Configure the MPU
 * @param
 **/
void
configure_mpu();

#endif // MPU_H