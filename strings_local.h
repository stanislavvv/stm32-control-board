/* copyright https://github.com/stanislavvv/stm32-control-board */
/* string functions */

#include "bool.h"

/* strlen realization */
static inline uint16_t strlen_local(const char *str)
{
    uint16_t j = 0;
    while (str[j] != '\0')
    {
        ++j;
    }
    return j;
}

/* strncmp realization */
static inline boolean strncmp_local(const char *s1, const char *s2, uint16_t n)
{
    unsigned i;
    for (i = 0; i < n; ++i)
    {
        if (s1[i] != s2[i])
        {
            return FALSE;
        }
    }
    return TRUE;
}

static inline uint16_t compare_strings(const char *first, const char *second)
{
   while (*first == *second)
   {
      if (*first == '\0' || *second == '\0')
         { break; }

      first++;
      second++;
   }

   if (*first == '\0' && *second == '\0')
      { return TRUE; }
   else
      { return FALSE; }
}
