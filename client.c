#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <netdb.h>
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
void error(httpReq* req);
httpReq* init();
int isURL(char* arr);
char* getUrlAfterSlash(char* str);
//int numOfEqual (char**argv, int index);
void parseTypeUrl(httpReq *req,char*url);
void isParameterValid(char* param);
int strLength(char* string);
void deleteHttpReq(httpReq* req);
void specialCharacter(char* str);
httpReq *req;

int main (int argc , char *argv[])
{
    int rArgsAmount = 0;

    req = init();

    if(argc==1)
    {
        error(req);
    }

    for (int i = 1; i <argc ; ++i)
    {

        switch(type(argv[i])) {
            case TYPE_P: {
                if(argv[i+1]==NULL)
                {
                    error(req);
                }

                if(req -> postContent) error(req);

                req -> postContent = (char*)malloc(sizeof(char)*strlen(argv[i+1])+1);
                req ->type = POST;

                i++;


                strcpy(req->postContent,argv[i]);
                specialCharacter(req->postContent);

                if(req->postContent==NULL)
                {
                    error(req);
                }
                else if(strcmp(argv[i],"")==0||strcmp(argv[i]," ")==0)
                {
                    error(req);
                }

                break;
            }


            case TYPE_R: {
                if(req->parameters) error(req);
                //req ->type = GET;
                rArgsAmount = atoi((argv[++i]));
                if (rArgsAmount == 0) {
                    error(req);
                }

                req->parameters = parseTypeR(argv, i+1, rArgsAmount);
                i+=rArgsAmount;

                break;
            }
            case TYPE_URL: {
                if(req->url) error(req);

                parseTypeUrl(req, argv[i]);

                break;
            }
            default:
                error(req);
                break;
        }
    }


    int port = portNumber(req->url);
    int len = (int) (strlen(req->url) + strLength(req->parameters) + strLength(req->postContent) + strLength(req->path));
    char *str = (char*) malloc(sizeof(char) * (len + 65));
    //4(POST/GET), " HTTP/1.Host:"
    bzero(str,strlen(str));

   /* sprintf(str, "Request:\n%s %s?%s HTTP/1.0\nHost: %s", req->type == GET ? "GET":"POST",req->parameters == NULL ? "": req->path, req->parameters, req->url);
    puts(str);*/
   // puts(req->url);

    /*printf("Request:\n");
    sprintf(str, "%s %s%s HTTP/1.0\nHost: %s", req->type == GET ? "GET":"POST",req->parameters == NULL ? "/": req->path, strcat("?", req->parameters), req->url);
    puts(str);*/
    if(req->postContent==NULL&&req->parameters == NULL)
    {
        sprintf(str,"%s %s HTTP/1.0\r\nHost: %s\r\n\r\n", req->type == POST? "POST":"GET",req->path ? req->path : "/",req->url); // 27
    }else if(req->parameters == NULL)
    {
        printf("Request:\r\n");
        sprintf(str, "%s %s HTTP/1.0\r\nHost:%s\r\nContent-length:%d\r\n\r\n%s", req->type == POST? "POST":"GET",req->path ? req->path : "/", req->url,
                (int) strlen(req->postContent), req->postContent); // 23 (Contet...)
    }else if(req->postContent==NULL) {
        printf("Request:\r\n");
        sprintf(str, "%s %s?%s HTTP/1.0\r\nHost: %s\r\n\r\n", // 2 "/...?"
                req->type == POST ? "POST" : "GET", req->path ? req->path : "/", req->parameters,req->url
                );
    }
    else {
        printf("Request:\r\n");
        sprintf(str, "%s %s?%s HTTP/1.0\r\nHost: %s\r\nContent-length:%d\r\n\r\n%s",
                req->type == POST ? "POST" : "GET", req->path ? req->path : "/", req->parameters, req->url,
                (int) strlen(req->postContent),
                req->postContent);
    }

    puts(str);

    //Initializing socket

    int sockfd=0;
    struct sockaddr_in serv_addr;
    struct hostent *server ;


    if (argc < 2) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        deleteHttpReq(req);
        free(str);

        exit(1);
    }

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        deleteHttpReq(req);
        free(str);

        exit(1);
    }

    //printf("%d\n",port);
    server = gethostbyname(req->url);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        deleteHttpReq(req);
        close(sockfd);
        free(str);

        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    bcopy(server->h_addr_list[0], (char *)  &serv_addr.sin_addr.s_addr, (size_t) server->h_length);
    serv_addr.sin_port = htons((uint16_t) port);

    if (connect(sockfd,(const struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
    {
        perror("ERROR connecting");
        deleteHttpReq(req);
        close(sockfd);
        free(str);
        exit(1);
    }
    //retrieve data form server
    //int size = 0;

    if(write(sockfd,str,strlen(str))<0)
    {
        perror("write");
        deleteHttpReq(req);
        close(sockfd);
        free(str);
        exit(1);
    }

    char rsp[2] = "";
    int reading = (int) read(sockfd, rsp, 1);
    int sizze = 0;
    while(reading != 0)
    {
        if (reading < 0)
        {
            perror("read_error");
            deleteHttpReq(req);
            free(str);
            close(sockfd);
            exit(1);
        }
        printf("%s", rsp);
        sizze++;
        reading = (int) read(sockfd, rsp, 1);
    }
    printf("\n   Total received response bytes: %d\n", sizze);
    close(sockfd);
    free(str);
    //free(req);
    deleteHttpReq(req);
    //str=NULL;
    return 0;
}

void isParameterValid(char* param)
{
    int len = (int)strlen(param);
    char first = param[0];
    char last = param[len-1];

    if(first=='='||last=='=')
    {
        error(req);
    }
}

void parseTypeUrl(httpReq *req,char *url)
{
    char * afterUrl = NULL;
    char * path = NULL;
    int afterUrlMalloc = 0;

    tolower((int)url);

    if(strstr(url, "www"))
    {
        afterUrl = strchr(url, '.')+1;
    }
    else {
        afterUrl = getUrlAfterSlash(url);
        afterUrlMalloc = 1;
    }

    path = strchr(afterUrl,'/');

    if(path != NULL) {
        req->path = (char *) malloc(sizeof(char) * (strlen(path) + 1));
        bzero(req->path, strlen((path) + 1));
        bcopy(path, req->path, strlen(path));
        afterUrl[(int) (path - afterUrl)] = '\0';
    }
    req->url = (char*) malloc(sizeof(char) * ((strlen(afterUrl)-strLength(path)) + 5)); // 1 for null and 3 for www.

    strcpy(req->url, "www.");

    strcat(req->url, afterUrl);

    if(afterUrlMalloc)
    {
        free(afterUrl);
        //afterUrl = NULL;
    }

}
// Returns the new char*.
char* parseTypeR(char** argv, int currIndex, int argsAmount)
{

    int flag = 0;
    char* temp = (char*) malloc (sizeof(char)*strlen(argv[currIndex]));
    for (;  argsAmount>0 ; argsAmount--, currIndex++)
    {
       // printf("argv[%d] = %s", currIndex, argv[currIndex]);
        if(argv[currIndex] == NULL) error(req);

        if(flag != 0)
        {
            temp = (char*) realloc (temp,sizeof(char)*(strlen(argv[currIndex])+strlen(temp)+2)); // +2 is for '\0' and '&'
            strcat(temp, "&");
        }

        strcat(temp,argv[currIndex]);

        if(strstr(argv[currIndex],"=")==NULL)
        {
            error(req);
        }
        isParameterValid(argv[currIndex]);

        flag=1;

    }


    return temp;
}


char* getUrlAfterSlash(char* str)
{
    char*  afterSlash = (char*)malloc(strlen(str)-6);
    bzero(afterSlash,strlen(str)-6);
    afterSlash = strcpy(afterSlash,(str + 7));

    return afterSlash;
}


int isURL(char * arr)
{

    char temp[8]="";
    strncat(temp,arr,7);
    return strcmp(temp, "http://");
}

int portNumber (char *url)
{

    char *arr = strchr(url, ':');
    if(arr != NULL)
    {
        *arr = 0;
        arr++;


        for (int i = 0; i < strlen(arr); i++) {
            if (isdigit(arr[i]) == NULL) {
                error(req);
            }
        }

    }
    else
    {

    }


        return arr == NULL ? PORT : atoi(arr);

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

void error(httpReq *req) {
    deleteHttpReq(req);
    perror("Usage: client [-p <text>] [-r n < pr1=value1 pr2=value2 ...>]\\n <URL>");
    exit(-1);
}

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

/*int numOfEqual (char** str , int index)
{
    int res = 0;

    while(strchr(str[index],'=')!=NULL)
    {
       res++;
       index++;
    }

    return res;
}*/

int strLength (char* string)
{
    return (int) (string ? strlen(string) : 0);
}

void deleteHttpReq(httpReq* req)
{
    if(req->url)
    {
        free(req->url);
        req->url = NULL;
    }
    if(req->path)
    {
        free(req->path);
        req->path = NULL;
    }
    if(req->postContent)
    {
        free(req->postContent);
        req->postContent = NULL;
    }
    if(req->parameters)
    {
        free(req->parameters);
        req->parameters = NULL;
    }

    free(req);

}
void specialCharacter(char* str) {
    char ch;

    for(int i=0; i<strlen(str);i++) {
        ch=str[i];

        if (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ('0' <= ch && ch <= '9')) {

        }
        else
        {
           error(req);
        }
    }
}