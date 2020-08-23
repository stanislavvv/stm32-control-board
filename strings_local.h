/* copyright https://github.com/stanislavvv/stm32-control-board */
/* string functions */

/* strlen realization */
static inline uint16_t strlen_local(const char *str) {
    uint16_t j = 0;
    while(str[j] != '\0') {
        ++j;
    }
    return j;
}

/* strncmp realization */
static inline uint16_t strncmp_local(const char *s1, const char *s2, uint16_t n) {
    unsigned i;
    for( i = 0; i < n; ++i) {
        if(s1[i] != s2[i]) {
            return (1==0);
        }
    }
    return (1==1);
}

/* return true if strings are equal, false if not */
/*static inline uint16_t compare_strings(char a[], char b[])
{
   int i = 0;

   while (a[i] == b[i]) {
      if (a[i] == '\0' || b[i] == '\0')
         break;
      i++;
   }

   if (a[i] == '\0' && b[i] == '\0')
      return (1==1);
   else
      return (1==0);
}*/

static inline uint16_t compare_strings(const char *first, const char *second) {
   while (*first == *second) {
      if (*first == '\0' || *second == '\0')
         break;

      first++;
      second++;
   }

   if (*first == '\0' && *second == '\0')
      return (1==1);
   else
      return (1==0);
}
