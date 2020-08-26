/* copyright https://github.com/stanislavvv/stm32-control-board */
/* string functions */

#include <stdint.h>
#include "bool.h"

/**
 * @brief strlen realization
 * @param *str
 * @return length of *str
 */
static inline uint16_t strlen_local(const char *str)
{
    uint16_t j = 0;
    while (str[j] != '\0')
    {
        ++j;
    }
    return j;
}

/**
 * @brief strncmp realization
 * @param s1 - first string
 * @param s2 - second string
 * @param n - length of comparation
 * @return TRUE if strings equal at n length, or FALSE
 */
static inline boolean strncmp_local(const char *s1, const char *s2, uint16_t n)
{
    uint16_t i;
    for (i = 0; i < n; ++i)
    {
        if (s1[i] != s2[i])
        {
            return FALSE;
        }
    }
    return TRUE;
}

/**
 * @brief strings comparation function
 * @param *first - first string
 * @param *second - second string
 * @return TRUE if strings are equal, or FALSE
 */
static inline boolean compare_strings(const char *first, const char *second)
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

 /**
  * @brief reverse string s in place
  * @param s[] string to reverse
  * @return none
  */
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

/**
 * @brief convert uint16_t n to characters in s
 * @param n number to convert
 * @param s[] result will be here
 * @return none
 */
static inline void itoa_u16(uint16_t n, char s[])
{
    uint16_t i;

    i = 0;
    do
    {       /* generate digits in reverse order */
        s[i++] = (char)(n % 10 + '0');   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    s[i] = '\0';
    reverse(s);
}

/**
 * @brief convert int16_t n to characters in s with sign
 * @param n number to convert
 * @param s[] result will be here
 * @return none
 */
static inline void itoa_s16(int16_t n, char s[])
{
    int16_t i, sign;

    if ((sign = n) < 0)  /* record sign */
    {
        n = (int16_t)(-n);          /* make n positive */
    }
    i = 0;
    do
    {       /* generate digits in reverse order */
        s[i++] = (char)(n % 10 + '0');   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
    {
        s[i++] = '-';
    }
    s[i] = '\0';
    reverse(s);
}