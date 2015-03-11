#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#define ASSERT_ON_COMPILE(X)   switch (0) {case 0: case (X):;}


//#pragma pack(4)

struct S0 {
    char c;
    char d;
    char e;
};

struct S1{
    char a;
    long b;
};



struct S2 {
    char c;
    struct S1 d;
    long long e;
};

struct S3{
    char a1;
	 char a2;
    long b;
	 short c;
	 char d[11];
};

struct S4{
    char a1;
	 short c;
	 char a2;
    long b;
	 char d[5];
};

struct S5{
    char a1;
	 short c;
	 char a2;
    long b;
	 struct S0 o;
	 char d[4];
};

//#pragma pack()


struct array{
	 int len; 
    char a1;
	 short c;
	 char a2;
    long b;
	 struct S0 o;
	 char d[0];
};

struct S6{
    char a;
    long b;
    char c;
};

struct S7{
    char a;
    char c;
    long b;
};




int main()
{
	
	struct array *p=NULL;
	int a=0x12345678;
	short b=0;
	char temp[32];
	char *ptr;
	unsigned int c=0;
	char *ptr2;
	int i=0;

	ASSERT_ON_COMPILE(sizeof(long)==8);
	printf("sizeof(short):%d\n",sizeof(short));
	printf("sizeof(int):%d\n",sizeof(int));
	printf("sizeof(char):%d\n",sizeof(char));
	printf("sizeof(long):%d\n",sizeof(long));
	printf("sizeof(void *):%d\n",sizeof(void *));
	printf("sizeof(long long):%d\n",sizeof(long long));
	printf("sizeof(S0):%d\n",sizeof(struct S0));
	printf("sizeof(S1):%d offsetof b:%d\n",sizeof(struct S1),offsetof(struct S1,b));
	printf("sizeof(S2):%d\n",sizeof(struct S2));
	printf("sizeof(S3):%d offsetof b:%d\n",sizeof(struct S3),offsetof(struct S3,b));
	printf("sizeof(S4):%d offsetof c:%d  offsetof a2:%d offsetof b:%d offsetof d:%d\n",sizeof(struct S4),offsetof(struct S4,c),offsetof(struct S4,a2),offsetof(struct S4,b),offsetof(struct S4,d));
    printf("sizeof(S5):%d offsetof c:%d  offsetof a2:%d offsetof b:%d offsetof o:%d offsetof d:%d\n",sizeof(struct S5),offsetof(struct S5,c),offsetof(struct S5,a2),offsetof(struct S5,o),offsetof(struct S5,d));
    printf("sizeof(S6):%d\n",sizeof(struct S6));
    printf("sizeof(S7):%d\n",sizeof(struct S7));
    
    ptr=&temp;
    memset(temp,'a',10);
    printf("tmp:%p &temp:%p\n",temp,ptr);
    printf("temp:%s, ptr:%s\n",temp,ptr);
    
    c=(unsigned int)ptr;
    printf("ptr->unsigned int:0x%08x\n",c);

    ptr2=c;
    printf("unsigned int->ptr:%p\n",ptr2);
    for(i=0;i<10;i++,ptr2++)
    	printf("%c",*ptr2);
		
		printf("\n");
		
	printf("sizeof(array):%d\n",sizeof(struct array));

	p=(struct array *)malloc(sizeof(struct array)+10);

	if(NULL==p)
	{
		printf("malloc error\n");
		perror("malloc");
	}
	
	p->len=10;
	
	memset(p,0x30,100);
	
	printf("len:%d\ta1:%c\n");
	printf("ok \n");

}
