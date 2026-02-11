/**
 * @authors   Ario Amin @ Permadev, 
 * @copyright Copyright (c) 2021-2026, MIT-License included in project toplevel dir
 */

#ifndef UTIMER_MGR_H
#define UTIMER_MGR_H

#include "macros/mtimer_mgr.h"
#include "typedefs/ttimer_mgr.h"

#include <stdint.h>

/**
 * @brief   Clamps values to fit 24MHz timer counter
 * @details Clamps values if they exceed max limit of the count cycle
 *          registers 24mhz cycles. Clamps them using arithmetic logic,
 *          instead of control-blocks
 *
 * @returns Either return a clamped value, or return the inputted value,
 *
 * @note    magic numbers calculated by:
 *          max_cycles_for_unit = max_register_value / cycle_rate_per_unit
 **/
uint32_t
__time_clamp_24MHz__(uint32_t intime, timetype_e timetype);

/**
 * @brief   Clamps values to fit 50MHz timer counter
 * @details Clamps values if they exceed max limit of the count cycle
 *          registers 50mhz cycles. Clamps them using arithmetic logic,
 *          instead of control-blocks
 *
 * @returns Either return a clamped value, or return the inputted value.
 *
 * @note    magic numbers calculated by:
 *          max_cycles_for_unit = max_register_value / cycle_rate_per_unit
 **/
uint32_t
__time_clamp_50MHz__(uint32_t intime, timetype_e timetype);

/**
 * @brief   Clamps values to fit 100MHz timer counter
 * @details Clamps values if they exceed max limit of the count cycle
 *          registers 100mhz cycles. Clamps them using arithmetic logic,
 *          instead of control-blocks
 *
 * @returns Either return a clamped value, or return the inputted value.
 *
 * @note    magic numbers calculated by:
 *          max_cycles_for_unit = max_register_value / cycle_rate_per_unit
 **/
uint32_t
__time_clamp_100MHz__(uint32_t intime, timetype_e timetype);

/**
 * @brief   Clamps values to fit 150MHz timer counter
 * @details Clamps values if they exceed max limit of the count cycle
 *          registers 150mhz cycles. Clamps them using arithmetic logic,
 *          instead of control-blocks
 *
 * @returns Either return a clamped value, or return the inputted value.
 *
 * @note    magic numbers calculated by:
 *          max_cycles_for_unit = max_register_value / cycle_rate_per_unit
 **/
uint32_t
__time_clamp_150MHz__(uint32_t intime, timetype_e timetype);

/**
 * @brief   Clamps values to fit 200MHz timer counter
 * @details Clamps values if they exceed max limit of the count cycle
 *          registers 200mhz cycles. Clamps them using arithmetic logic,
 *          instead of control-blocks.
 *
 * @returns uint32_t - A clamped value or the inputted value.
 *
 * @note    magic numbers calculated by:
 *          max_cycles_for_unit = max_register_value / cycle_rate_per_unit
 **/
uint32_t
__time_clamp_200MHz__(uint32_t intime, timetype_e timetype);

/**
 * @brief   Resolve time for 24MHz clocks
 * @details Resolve time the proper ldvalue based on the
 *          time-type of the timer manager.
 * 
 * @param   targetval     Value to resolve
 * @param   timetype      Selector that informs us 
 * @param   conversiondir To time from tick, or to ticks from time 
 * @return  uint32_t      Resolved time (to:24MHz)
 * 
 */
uint32_t
__resolve_time_24MHz__(uint32_t targetval, timetype_e timetype, ttconversiondir_e conversiondir);

/**
 * @brief   Resolve time for 50MHz clocks
 * @details Resolve time the proper ldvalue based on the
 *          time-type of the timer manager.
 * 
 * @param   targetval     Value to resolve
 * @param   timetype      Selector that informs us 
 * @param   conversiondir To time from tick, or to ticks from time 
 * @return  uint32_t      Resolved time (to:50MHz)
 * 
 */
uint32_t
__resolve_time_50MHz__(uint32_t targetval, timetype_e timetype, ttconversiondir_e conversiondir);

/**
 * @brief   Resolve time for 100MHz clocks
 * @details Resolve time the proper ldvalue based on the
 *          time-type of the timer manager.
 * 
 * @param   targetval     Value to resolve
 * @param   timetype      Selector that informs us 
 * @param   conversiondir To time from tick, or to ticks from time 
 * @return  uint32_t      Resolved time (to:100MHz)
 * 
 */
uint32_t
__resolve_time_100MHz__(uint32_t targetval, timetype_e timetype, ttconversiondir_e conversiondir);

/**
 * @brief   Resolve time for 150MHz clocks
 * @details Resolve time the proper ldvalue based on the
 *          time-type of the timer manager.
 * 
 * @param   targetval     Value to resolve
 * @param   timetype      Selector that informs us 
 * @param   conversiondir To time from tick, or to ticks from time 
 * @return  uint32_t      Resolved time (to:150MHz)
 * 
 */
uint32_t
__resolve_time_150MHz__(uint32_t targetval, timetype_e timetype, ttconversiondir_e conversiondir);

/**
 * @brief   Resolve time for 200MHz clocks
 * @details Resolve time the proper ldvalue based on the
 *          time-type of the timer manager.
 * 
 * @param   targetval     Value to resolve
 * @param   timetype      Selector that informs us 
 * @param   conversiondir To time from tick, or to ticks from time 
 * @return  uint32_t      Resolved time (to:200MHz)
 * 
 */
uint32_t
__resolve_time_200MHz__(uint32_t targetval, timetype_e timetype, ttconversiondir_e conversiondir);

#endif // UTIMER_MGR_H