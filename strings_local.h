/** @addtogroup utils
 *  @{
 */
/**
 * @file strings_local.h
 * @brief local functions for working with strings
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */


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
     uint16_t i, j;
     for (i = 0, j = (uint16_t)(strlen_local(s)-1); i<j; i++, j--)
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
    uint16_t i = 0;
    do
    {
        /* generate digits in reverse order */
        s[i++] = (char)(n % 10 + '0');  /* get next digit */
    } while ((n /= 10) > 0);  /* delete it */
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

    /* record sign and check n negativeness */
    if ((sign = n) < 0)
    {
        /* n must be positive for processing */
        n = (int16_t)(-n);
    }
    i = 0;
    do
    {
        /* generate digits in reverse order */
        s[i++] = (char)(n % 10 + '0');  /* get next digit */
    } while ((n /= 10) > 0);  /* delete it */
    if (sign < 0)
    {
        s[i++] = '-';
    }
    s[i] = '\0';
    reverse(s);
}

/**
 * @brief convert uint16_t n to hex string in s
 * @param n number to convert
 * @param s[] result will be here
 * @return none
 */
static inline void itohex_u32(uint32_t n, char s[])
{
    uint32_t i = 0;
    char ch[] = "0123456789abcdef";
    do
    {
        /* generate digits in reverse order */
        s[i++] = ch[n % 16];  /* get next digit */
    } while ((n /= 16) > 0);  /* delete it */
    s[i] = '\0';
    reverse(s);
}


/** @}*/
