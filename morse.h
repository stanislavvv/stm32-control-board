/* morse functions */
/* not for production use */

#define DIT_LENGTH 200 /* length of dot in ms */
#define DAH_LENGTH DIT_LENGTH * 3 /* length of dash in ms */

/* long pause */
void lspc(void);
/* short pause */
void sspc(void);
/* send morse character */
void send_letter(char c);
/* send string */
void send_string(char s[]);

