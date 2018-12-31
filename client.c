#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

#define PORT 80
#define GET 0
#define POST 1



enum argType
{TYPE_ERROR=-1, TYPE_P, TYPE_R, TYPE_URL};

typedef struct httpReq{
    char *url;
    int type; // GET / SET
    char* parameters;
    char* postContent;
    char* path;

}httpReq;

char* parseTypeR(char** argv, int currIndex, int argsAmount);
int type (char* arr);
int portNumber(char *url);
void error();
httpReq* init();
int isURL(char* arr);
char* getUrlAfterSlash(char* srt);
void ifR(char**argv,char*str,int amount , httpReq *req,int index);
void ifURL(char**argv,char*str,int index);
int validPram(char* param);
int numOfEqual (char**argv, int index);
void parseTypeUrl(httpReq *req,char*url);

int main (int argc , char *argv[])
{
    int firstarg=argc;
    int post = 0 ;
    char * postText = NULL;
    int start = -1;
    int end =-1;
    int rArgsAmount = 0;
    char *url =NULL;
    int curr;
    httpReq *req = init();

    if(argc==1)
    {
        error();
    }

    for (int i = 1; i <argc ; ++i)
    {

        switch(type(argv[i])) {
            case TYPE_P: {
                if(req -> postContent) error();

                req -> postContent = (char*)malloc(sizeof(char)*strlen(argv[i+1])+1);
                req ->type = POST;

                i++;
                //printf("type p!!! %c", req->postContent[strlen(argv[i])+1]);

                strcpy(req->postContent,argv[i]);

                break;//exit(1);
            }


            case TYPE_R: {
                if(req->parameters) error();
                //req ->type = GET;
                rArgsAmount = atoi( (argv[++i]));
                if (rArgsAmount == 0) {
                    error();
                }

                req->parameters = parseTypeR( argv, i+1, rArgsAmount);
                i+=rArgsAmount;

                break;
            }
            case TYPE_URL: {
                if(req->url) error();
               //tolower(argv[i]);
                //strstr(argv[i], "www")? strchr(argv[i],'.'):


                parseTypeUrl(req, argv[i]);
               // puts(req->url);

                //puts(req->path);

                break;
            }
            default:
                printf("type error!!!");
                error();
                break;
        }
    }

    char* str[2000] = {0};
   /* sprintf(str, "Request:\n%s %s?%s HTTP/1.0\nHost: %s", req->type == GET ? "GET":"POST",req->parameters == NULL ? "": req->path, req->parameters, req->url);
    puts(str);*/
    puts(req->url);//
    if(req->parameters == NULL)
    {
        sprintf(str, "Request:\n%s %s HTTP/1.0\nHost:%s\nContent-length:%d\n%s", req->type == POST? "POST":"GET",req->path,req->url,strlen(req->postContent),req->postContent);
    }else
        sprintf(str, "Request:\n%s %s?%s HTTP/1.0\nHost:%s", req->type == GET ? "GET":"POST",req->parameters == NULL?"": req->path, req->parameters, req->url);


    puts(str);

/*
 int sock =0;
    struct sockaddr_in server_addr;
    int new_socket,valread;
    int addrlen= sizeof(server_addr);
    char buffer[1024]={0};
    char *msg ;
    int n=0;
    if((sock = socket(AF_INET,SOCK_STREAM,0))<0)
    {
        printf("socket creation error\n");
        return -1;
    }
    memset(&server_addr,'0', sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons((uint16_t) portNumber(url));
    // server_addr.sin_port=htons(PORT);


    if(inet_pton(AF_INET,url,&server_addr.sin_addr)==0)
    {
        printf("invalid address\n");
        return -1;
    }
    if(connect(sock,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
    {
        printf("connection failed");
        return -1;
    }
    if(bind(sock,(struct sockaddr *)&server_addr,sizeof(server_addr))<0)
    {
        perror("bind failed");
        exit(1);
    }
    if (listen(sock, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(sock, (struct sockaddr *)&server_addr,
                             (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(1);
    }

    n = (int) write(sock, buffer, strlen(buffer));
    if (n < 0)
        perror("ERROR writing to socket");
    bzero(buffer,256);
    n = (int) read(sock, buffer, 255);
    if (n < 0)
        perror("ERROR reading from socket");
    printf("%s\n",buffer);
    close(sock);
 */

    return 0;
}

void parseTypeUrl(httpReq *req,char *url)
{
    char * afterUrl;
    char * path;
    //ifURL(argv,afterUrl,i);
    //puts("Request:");
    tolower(url);

    afterUrl = strstr(url, "www") ? strchr(url, '.')+1 : getUrlAfterSlash(url);

    //afterUrl = strchr(afterUrl,'/');
    path = strchr(afterUrl,'/');
    req -> path = (char*) malloc(sizeof(char) * (strlen(path) + 1));
    strcpy(req->path, path);

    afterUrl[(int) (path-afterUrl)] = 0;

    req -> url = (char*) malloc (sizeof(char) * (strlen(path) + 4)); // 1 for null and 3 for www

    //puts(req->url);
    strcpy(req->url, "www.");

    //puts(req->url);
    strcat(req->url, afterUrl);

    //puts(req->url);
}
// Returns the new char*.
char* parseTypeR(char** argv, int currIndex, int argsAmount)
{
    /*printf("\?printfnlen is: %d\n", strlen(str));
    char* temp = (char*) malloc(sizeof(char) * (strlen(str)));
    int i;

    temp[0] = '?';
    for(i = 1; str[i]; i++)
    {
        temp[i] = (str[i] == ' ' ? '&' : str[i]);
    }

    return temp;*/
    int amt=argsAmount;
    int flag = 0;
    int check = 0;
    char* temp = (char*) malloc (sizeof(char)*strlen(argv[currIndex]));
    for (;  argsAmount>0 ; argsAmount--, currIndex++)
    {
       // printf("argv[%d] = %s", currIndex, argv[currIndex]);
        if(argv[currIndex] == NULL) error();

        if(flag != 0)
        {
            temp = (char*) realloc (temp,sizeof(char)*(strlen(argv[currIndex])+strlen(temp)+2)); // +2 is for '\0' and '&'
            strcat(temp, "&");
        }

        strcat(temp,argv[currIndex]);

        if(strstr(argv[currIndex],"=")==NULL)
        {
            error();
        }

        flag=1;

    }
    /*isU=type((char *) argv);
    if(isU==TYPE_URL)
    {

    }*/
    //puts(temp);
    //getUrlAfterSlash();


    return temp;
}


char* getUrlAfterSlash(char* srt)
{
    char*  afterSlash = (char*)malloc(strlen( srt));
    afterSlash = strcpy(afterSlash,(srt + 7));
    // printf("GET %s \n",afterSlash);
    return afterSlash;
}


int isURL(char * arr)
{

    char temp[8]="";
    //printf("Host:"); puts(arr);
    strncat(temp,arr,7);
    return strcmp(temp, "http://");
}

int portNumber (char *url)
{
    char arr[12];
    strcpy(arr,"");
    int len = (int) strlen(url);
    for (int i = 0; i <len ; ++i) {
        if(url[i]==':')
        {
            for(int j = i+1 ; j<len ; ++j)
            {
                if(url[j]>='0'&&url[j]<='9')
                {
                    int len2= (int) strlen(arr);
                    arr[len2]=url[j];
                    arr[len2+1]='\0';
                }
                else
                {
                    break;
                }
            }if(strlen(arr)>0)
            {
                break;
            }
        }

    }if(strlen(arr)>0)
    {
        return atoi(arr);
    }
    return PORT;
}

httpReq* init()
{
    httpReq* newReq = (httpReq*) malloc(sizeof(httpReq));
    newReq -> type = GET;
    newReq -> url = NULL;
    newReq -> parameters = NULL;
    newReq->postContent=NULL;
    newReq->path=NULL;
    return newReq;
}

void error() {

    perror("Usage: client [-p] [-r < pr1=value1 pr2=value2 …>]\\n url");
    exit(-1);
}

// returns the type of the argument
int type (char* arr)
{


    if(strcmp(arr,"-p")==0)
    {
        return TYPE_P;
    }
    else if (strcmp(arr,"-r")==0)
    {
        return TYPE_R;
    }
    else if(isURL(arr)==0)
    {
        return TYPE_URL;
    }
    else {
        return TYPE_ERROR;
    }
}

int numOfEqual (char** str , int index)
{
    int res = 0;

   while(strchr(str[index],'=')!=NULL)
   {
       res++;
       index++;
   }

    return res;
}

