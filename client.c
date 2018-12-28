#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 80
#define GET 0
#define POST 1

enum argType
{TYPE_ERROR=-1, TYPE_P, TYPE_R, TYPE_URL};

typedef struct httpReq{
    char *url;
    int type; // GET / SET
    char* parameters;
}httpReq;

char* parseTypeR(char* str);
int type (char* arr);
int portNumber(char *url);
void error();
httpReq* init();

int main (int argc , char *argv[])
{
    int firstarg=argc;
    int post = 0 ;
    char * postText = NULL;
    int start = -1;
    int end =-1;
    char *url =NULL;
    int curr;

    httpReq *req = init();

    if(argc==1)
    {
        error();
    }

    for (int i = 1; i <argc ; ++i)
    {
        switch(type(argv[i]))
        {
            case TYPE_P: {
                printf("type p!!!");
                break;
            }
            case TYPE_R: {
                req->parameters = parseTypeR(strchr(argv[i], ' '));
                puts(req->parameters);
                break;
            }
            case TYPE_URL: {
                printf("type url!!!");
                break;
            }
            default:
                printf("type error!!!");
                error();
                break;
        }
    }
/*
    int i;
    for (i = 0; i < argc ; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            post = 1;

            if (strcmp(argv[i + 1], "text") != 0) {
                error();
            }
            if (strcmp(argv[i + 1], url) != 0) {

                if (strcmp(argv[i + 2], "-r") != 0) {
                    error();
                } else {
                    curr++;

                    if (argv[curr] <= (char *) '0' && argv[curr] >= (char *) '9') {
                        error();
                    }
                    while (flag) {
                        //if()
                    }
                }
            }
            if ((i + 1) < argc) {
                postText = argv[i - 1];
                i++;
            }
        } else if (strcmp(argv[i], "-r") == 0) {
            for (int j = i + 1; j < argc; j++) {
                if (strstr(argv[j], "*") != NULL) {
                    if (start == -1) {
                        start = j;
                    }
                    end = j;
                    i = j;
                }
            }
        } else {
            url = argv[i];
        }
    }
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
    return newReq;
}

void error() {
    perror("Usage: client [-p] [-r < pr1=value1 pr2=value2 …>]\\n url");
    exit(-1);
}

// returns the type of the argument
int type (char* arr)
{
    char temp[6]="";
    puts(arr);
    strncat(temp,arr, arr[0] == '-' ? 3 : 5);
    puts("\ntemp is:");
    puts(temp);
    if(strcmp(temp,"-p ")==0)
    {
        return TYPE_P;
    }
    else if (strcmp(temp,"-r ")==0)
    {
        return TYPE_R;

    }else if (strcmp(temp,"http:")==0)
    {
        return TYPE_URL;
    }
    else

       return TYPE_ERROR;
}

char* parseTypeR(char* str)
{
    printf("\nlen is: %d\n", strlen(str));
    char* temp = (char*) malloc(sizeof(char) * (strlen(str)));
    int i;

    temp[0] = '?';
    for(i = 1; str[i]; i++)
    {
        temp[i] = (str[i] == ' ' ? '&' : str[i]);
    }

    return temp;
}