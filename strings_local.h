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

 /* reverse:  reverse string s in place */
static inline void reverse(char s[])
{
     int i, j;

     for (i = 0, j = strlen_local(s)-1; i<j; i++, j--)
     {
         char c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
}

/* itoa:  convert unsigned n to characters in s */
static inline void itoa_u16(uint16_t n, char s[])
{
    uint16_t i;

    i = 0;
    do
    {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    s[i] = '\0';
    reverse(s);
}

/* itoa:  convert signed n to characters in s */
static inline void itoa_s16(int16_t n, char s[])
{
    int16_t i, sign;

    if ((sign = n) < 0)  /* record sign */
    {
        n = -n;          /* make n positive */
    }
    i = 0;
    do
    {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
    {
        s[i++] = '-';
    }
    s[i] = '\0';
    reverse(s);
}
