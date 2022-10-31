#ifndef STDLIB_H
#define STDLIB_H

int errno;

char* itoa( long value, char* buffer, int radix );
char* ltoa( int value, char* buffer, int radix );
char* ultoa( int value, char* buffer, int radix );
char* _itoa( unsigned long value, int is_sign, char* buffer, int radix);
/* ftoa�����侫�����ⲻ��ʹ��*/
char* ftoa( float value, char* buffer, int precision );
char* lftoa( double value, char* buffer, int precision );
char* exlftoa( double value, char* buffer, int precision, char e );

#endif
