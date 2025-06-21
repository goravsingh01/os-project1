#ifndef STRING_H
#define STRING_H

int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, unsigned int n);
unsigned int strlen(const char* s);
char* strcpy(char* dest, const char* src);
char* strcat(char* dest, const char* src);

#endif
