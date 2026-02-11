/**
 * @authors   Ario Amin @ Permadev, 
 * @copyright Copyright (c) 2021-2026, MIT-License included in project toplevel dir
 */

/* Definition@Header */
#include "utils/utimer_mgr.h"

// TIME CLAMPING

uint32_t
__time_clamp_24MHz__(uint32_t intime, timetype_e timetype)
{
  uint32_t clamped_val = TRANSLATE_TIMECLAMP_24MHz(intime, timetype);
  return (intime * (clamped_val == 0)) + clamped_val;
};

uint32_t
__time_clamp_50MHz__(uint32_t intime, timetype_e timetype)
{
  uint32_t clamped_val = TRANSLATE_TIMECLAMP_50MHz(intime, timetype);
  return (intime * (clamped_val == 0)) + clamped_val;
};

uint32_t
__time_clamp_100MHz__(uint32_t intime, timetype_e timetype)
{  
  uint32_t clamped_val = TRANSLATE_TIMECLAMP_100MHz(intime, timetype);
  return (intime * (clamped_val == 0)) + clamped_val;
};

uint32_t
__time_clamp_150MHz__(uint32_t intime, timetype_e timetype)
{
  uint32_t clamped_val = TRANSLATE_TIMECLAMP_150MHz(intime, timetype);
  return (intime * (clamped_val == 0)) + clamped_val;
};

uint32_t
__time_clamp_200MHz__(uint32_t intime, timetype_e timetype)
{
  uint32_t clamped_val = TRANSLATE_TIMECLAMP_200MHz(intime, timetype);
  return (intime * (clamped_val == 0)) + clamped_val;
};

// UNIT CONVERSION

uint32_t
__resolve_time_24MHz__(uint32_t targetval, timetype_e timetype, ttconversiondir_e conversiondir)
{
  uint32_t time_unit = targetval;
  switch (conversiondir)
  {
    default:
    case FROMTIME_E:
      // ensuring safety
      time_unit = __time_clamp_24MHz__(targetval, timetype);

      // 50MHz = 20nanoseconds periods
      return TRANSLATE_TIME_24MHz(time_unit, timetype);
    case FROMTICKS_E:
      return BACKTRANSLATE_TIME_24MHz(time_unit, timetype);
  }
}

uint32_t
__resolve_time_50MHz__(uint32_t targetval, timetype_e timetype, ttconversiondir_e conversiondir)
{
  uint32_t time_unit = targetval;
  switch (conversiondir)
  {
    default:
    case FROMTIME_E:
      // ensuring safety
      time_unit = __time_clamp_50MHz__(targetval, timetype);

      // 50MHz = 20nanoseconds periods
      return TRANSLATE_TIME_50MHz(time_unit, timetype);
    case FROMTICKS_E:
      return BACKTRANSLATE_TIME_50MHz(time_unit, timetype);
  }
}

uint32_t
__resolve_time_100MHz__(uint32_t targetval, timetype_e timetype, ttconversiondir_e conversiondir)
{
  uint32_t time_unit = targetval;
  switch (conversiondir)
  {
    default:
    case FROMTIME_E:
      // ensuring safety
      time_unit = __time_clamp_100MHz__(targetval, timetype);

      // 50MHz = 20nanoseconds periods
      return TRANSLATE_TIME_100MHz(time_unit, timetype);
    case FROMTICKS_E:
      return BACKTRANSLATE_TIME_100MHz(time_unit, timetype);
  }
}

uint32_t
__resolve_time_150MHz__(uint32_t targetval, timetype_e timetype, ttconversiondir_e conversiondir)
{
  uint32_t time_unit = targetval;
  switch (conversiondir)
  {
    default:
    case FROMTIME_E:
      // ensuring safety
      time_unit = __time_clamp_150MHz__(targetval, timetype);

      // 50MHz = 20nanoseconds periods
      return TRANSLATE_TIME_150MHz(time_unit, timetype);
    case FROMTICKS_E:
      return BACKTRANSLATE_TIME_150MHz(time_unit, timetype);
  }
}

uint32_t
__resolve_time_200MHz__(uint32_t targetval, timetype_e timetype, ttconversiondir_e conversiondir)
{
  uint32_t time_unit = targetval;
  switch (conversiondir)
  {
    default:
    case FROMTIME_E:
      // ensuring safety
      time_unit = __time_clamp_200MHz__(targetval, timetype);

      // 50MHz = 20nanoseconds periods
      return TRANSLATE_TIME_200MHz(time_unit, timetype);
    case FROMTICKS_E:
      return BACKTRANSLATE_TIME_200MHz(time_unit, timetype);
  }
}