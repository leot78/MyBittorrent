#ifndef MY_STRING_H
#define MY_STRING_H

char *my_itoa(long value);
char *concat(char *c1, char *c2);
char *concatn(char *c1, char *c2, size_t l1, size_t l2);
char *strcpy_delim(char *dest, char *src, char delim);

#endif /*MY_STRING_H*/
