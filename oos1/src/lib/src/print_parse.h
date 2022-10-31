#ifndef PRINT_PARSE_H
#define PRINT_PARSE_H

/* ��ǰ��Ҫ��ӡ������convertion form ��Ϣ */ 
struct print_info
{
    int prec;			/* Precision.  */
    int width;			/* Width.  */
    char spec;			/* Format letter.  */
    unsigned int is_long_double;/* L flag.  */
    unsigned int is_short;	/* h flag.  */
    unsigned int is_long;	/* l flag.  */
    unsigned int alt;		/* # flag.  */
    unsigned int space;		/* Space flag.  */
    unsigned int left;		/* - flag.  */
    unsigned int showsign;	/* + flag.  */
    unsigned int extra;		/* For special use.  */
    unsigned int is_char;	/* hh flag.  */
    char pad;			/* Padding character.  */
};

/* ��ǰ���ڱ�������fmt�ַ�������Ϣ */
struct print_spec
{
    char* fmt;
    char* start_fmt;    /* ָ��ǰ�����õ��ַ������׸��ַ� */ 
    char* end_fmt;     /* ָ��ǰ�����õ��ַ����ĺ���һ���ַ� */ 
    struct print_info info; /* ��ǰ���ڷ�����%���convertion form����Ϣ */             
};

/*
* Ѱ����һ��spec�Ŀ�ʼ�ַ�
* ������ spec(��ǰ���ڷ�����fmt�ַ���) 
* ����ֵ�� 0 �ɹ��� -1 eof 
*/
int find_spec( struct print_spec* spec); /* Ѱ����һ��spec�Ŀ�ʼ�ַ� */

int parse_spec( struct print_spec* spec );

unsigned int read_int( char** ppstr );

#endif

