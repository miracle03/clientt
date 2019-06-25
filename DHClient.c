#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include<errno.h>  //错误
#include"aes_gcm.h"
#include"DH.h"
#define MAX 500 //运算中的最高可能位数
#define NUMP 2 //已提供的素数个数
#define NUMG 32 //随机数g的位数
#define  MAXSIZE 500
#define SER_PORT 8080

int connectToSer(){
	struct sockaddr_in servaddr;
    char buf[MAX];
    int sockfd,n;
    char *str;
    sockfd = socket(AF_INET,SOCK_STREAM,0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
    inet_pton(AF_INET,"192.168.146.161",&servaddr.sin_addr);
    servaddr.sin_port = htons(SER_PORT);

    if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){            	
		printf("connet error:%s\n",strerror(errno));
    }   
    char sa[MAX],sb[MAX],p[MAX],g[MAX],ak[MAX];
    
    int a;
    srand((int)time(0));
	a=rand()%10000+1;
    memset(p,0,MAXSIZE);
    memset(g,0,MAXSIZE);
    memset(sa,0,MAXSIZE);
    sent(a,p,g,sa);

    int i;
    printf("生成密钥中————\nrand:%d\n",a); 
    for(int i=0;i<3;i++)
    {    
        if(i==0)
        {write(sockfd,p,MAX);}   //把我们的输入，写到服务器
        if(i==1)
       	{write(sockfd,g,MAX);}
        if(i==2)
       	{write(sockfd,sa,MAX);}
    }
    memset(sb,0,MAXSIZE);
    n = read(sockfd,sb,MAX);    //从服务器读取数据
    get_key(a,sb,p,g,ak);
    printf("密钥为:"); 
    display(ak);
    memset(gcm_key,0,MAX);
    copy(ak,gcm_key);
    //display1(gcm_key);
	unsigned char temp[MAX]={'0'};
	int flag=0,flag2=0;
    while(1){
		memset(temp,0,MAX);
		memset(gcm_pt,0,MAX);
		memset(gcm_tag,0,MAX);
		memset(gcm_ct,0,MAX);		
		
		printf("input message(input q quit):\n");
    	//getchar();
    	fgets(gcm_pt,MAX,stdin);
		gcm_pt[(int)strlen(gcm_pt)-1]='\0';
		printf("pt:%s\n",gcm_pt);
		if(strcmp(gcm_pt,"q")==0){
			write(sockfd,"quit",(int)strlen("quit"));
			printf("quit!!\n");
			break;			
		}
   		
    	aes_gcm_encrypt();
		if(flag==0){
			write(sockfd,gcm_aad,MAX);//aad
			write(sockfd,gcm_iv,MAX);//iv   		
			flag=1;
		}
    	
		write(sockfd,gcm_tag,16);//tag
    	write(sockfd,gcm_ct,MAX);//ct
    	read(sockfd,temp,MAX);
    	printf("server:%s\n",temp);

    }
    /*printf("input message:\n");
    getchar();
    fgets(gcm_pt,MAX,stdin);
    gcm_pt[(int)strlen(gcm_pt)-1]='\0';
    aes_gcm_encrypt();
    unsigned char temp[MAX]={'0'};
    write(sockfd,gcm_aad,MAX);//aad

    write(sockfd,gcm_tag,MAX);//tag

    write(sockfd,gcm_iv,MAX);//iv

    write(sockfd,gcm_ct,MAX);//ct
    read(sockfd,temp,MAX);
    printf("server:%s\n",temp);
*/
    close(sockfd);
}
int main(int argc,char *argv[]){    
    connectToSer();
    return 0;

}

