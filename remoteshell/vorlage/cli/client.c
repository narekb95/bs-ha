#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int maxCharBufferSize = 2000;
const int PORT = 8000;
#define HOST "127.0.0.1"


static void die(const char* msg)
{
    fputs(msg, stderr);
    putc('\n', stderr);
    exit(-1);
}

int isput(char* command);
int isget(char* command);
int main()
{
    struct sockaddr_in staddr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = inet_addr(HOST)
    };
    char buf[maxCharBufferSize];
    int cfd;
    
    if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        die("Couldn't open the socket");
    }
    
    if (connect(cfd, (struct sockaddr*) &staddr, sizeof(staddr)) < 0)
    {
        die("Couldn't connect to socket");
    }
    
    if (read(cfd, buf, sizeof(buf)) < 0)
    {
        die("Couldn't receive message");
    }
    int communicationport = atoi(buf);
#ifdef DEBUG
    printf("using port number: %d\n", communicationport);
#endif
    close(cfd);
    
    usleep(1000000);
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(communicationport),
        .sin_addr.s_addr = inet_addr(HOST)
    };
    
    if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        die("Couldn't open the socket");
    }

    if (connect(cfd, (struct sockaddr*) &addr, sizeof(addr)) < 0)
    {
        die("Couldn't connect to socket");
    }
    
    while(1)
    {
        int bytesRead;
        if ((bytesRead = read(cfd, buf, sizeof(buf)-1)) < 0)
        {
            die("Couldn't receive message");
        }
        buf[bytesRead] = '\0';
        printf("%s/", buf);
        
        if(fgets(buf, maxCharBufferSize, stdin) == NULL)
        {
            fprintf(stderr, "can't read command, please try again\n");
        }
        
        //todo handle get and put separately
        if(isput(buf))
        {
            
        }
        else if(isget(buf))
        {
            
        }
        else
        {
            if (write(cfd, buf, strlen(buf)) < 0)
            {
                die("Couldn't send message");
            }
        }
        while(1)
        {
            if ((bytesRead = read(cfd, buf, sizeof(buf)-1)) < 0)
            {
                die("Couldn't get message");
            }
            buf[bytesRead] = '\0';
            if(strcmp(buf, "done!") == 0)
            {
                break;
            }
            printf("%s", buf);
        }
        
    }
	return 0;
}
int isput(char* command)
{
    if(strlen(command) < 5)return 0;
    return (command[0] == 'p' && command[1] == 'u' && command[2] == 't' && command[3] == ' ');
}
int isget(char* command)
{
    if(strlen(command) < 5)return 0;
    return (command[0] == 'g' && command[1] == 'e' && command[2] == 't' && command[3] == ' ');
}
