#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
//alan yousefzadeh lhlh;lh;lhilh kjbkjglklkh
// fl;jsaoifhqw;ofjf;g;fljsa'fpiqwdh;wfkwlhjfo;qlj
#define PORT 80
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

int main (int argc , char *argv[])
{
    int firstarg=argc;
    int post = 0 ;
    char * postText = NULL;
    int start = -1;
    int end =-1;
    char *url =NULL;
    int curr;
    bool flag=true;

    for (int i = 0; i <argc ; ++i) {
        if(strcmp(argv[i],"-p")==0)
        {
            post=1;

            if(strcmp(argv[i+1],"text")!=0)
            {
                printf("Usage: client [-p] [-r < pr1=value1 pr2=value2 …>]\n url");
                exit(1);
            }
            if(strcmp(argv[i+1],url)!=0)
            {

                if(strcmp(argv[i+2],"-r")!=0)
                {
                    printf("Usage: client [-p] [-r < pr1=value1 pr2=value2 …>]\n url");
                    exit(1);
                }else {
                        curr++;

                        if(argv[curr] <= (char *) '0' && argv[curr] >=(char *)'9')
                        {
                            printf("Usage: client [-p] [-r < pr1=value1 pr2=value2 …>]\n url");
                            exit(1);
                        }
                    while (flag)
                    {
                       if()
                    }
                }
            }
            if((i+1)<argc)
            {
                postText=argv[i-1];
                i++;
            }
            else if(strcmp(argv[i],"-r")==0)
            {
                for(int j = i+1 ; j<argc; j++)
                {
                    if(strstr(argv[j],"*")!=NULL)
                    {
                        if(start==-1)
                        {
                            start=j;
                        }
                        end =j;
                        i=j;
                    }
                }
            }else
                url=argv[i];
        }
        int sock =0;
        struct sockaddr_in server_addr;
        int new_socket,valread;
        int addrlen= sizeof(server_addr);
        char buffer[1024]={0};
        char *msg ;
        int n;
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

        msg = (char*)calloc(addrlen, sizeof(char));
        if(!msg)
        {
            exit(EXIT_FAILURE);
        }
        sprintf(msg,"%s %s HTTP/1.1\r\nHost: %s%s%s%s%s\r\nConnection:close\r\n\r\n");

        n = write(sock,buffer,strlen(buffer));
        if (n < 0)
            perror("ERROR writing to socket");
        bzero(buffer,256);
        n = read(sock,buffer,255);
        if (n < 0)
            perror("ERROR reading from socket");
        printf("%s\n",buffer);
        close(sock);

        return 0;
    }
}
