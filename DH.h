#ifndef DH_H
#define DH_H
#define NUMP 2 //已提供的素数个数
char p_set[NUMP][110]=
{
    {"96872095226205901155646436505653"},
    {"96872095226205901155646436505653"}
};

int get_n(char*a);
int get_n1(unsigned char*a);
int get_mul(char*a,char*b);
int get_mod(char*a,char*b);
void reverse(char*a);
void reverse1(unsigned char*a);
void get_a(char*a);
void get_g(char*a);
void get_p(char*a,int n);
void display(char*a);
void display1(unsigned char*a);
void sent(int a,char* p,char* g,char *ans);
void copy(char *a,unsigned char *b);
void get_key(int b,char *rec,char*p,char*g,char *ans);
#endif
