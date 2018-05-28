#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int PORT = 8002;
const int maxCharBufferSize = 2000;
#define HOST "127.0.0.1"


static void die(const char* msg)
{
    fputs(msg, stderr);
    putc('\n', stderr);
    exit(-1);
}

int isput(char* command);
int isget(char* command);
int endsWithDone(char *str);
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
    
    usleep(100000);
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
    char *done = malloc(maxCharBufferSize * sizeof(char));
    if(done == NULL)
    {
        die("can't mallocate string\n");
    }
    while(1)
    {
        int bytesRead;
        if ((bytesRead = read(cfd, buf, sizeof(buf)-1)) < 0)
        {
            die("Couldn't receive message");
        }
        if(bytesRead == 0)
        {
            return 0;
        }
        buf[bytesRead] = '\0';
        printf("%s/", buf);
        
        if(fgets(buf, maxCharBufferSize, stdin) == NULL)
        {
            fprintf(stderr, "can't read command, please try again\n");
        }
        
        //todo handle get and put separately
        if (write(cfd, buf, strlen(buf)) < 0)
        {
            die("Couldn't send message");
        }
        if (read(cfd, done, sizeof(done)) < 0)
        {
            die("can't read done\n");
        }
        
        if(isput(buf))
        {
            buf[strlen(buf) - 1] = '\0';
            char *name = malloc(maxCharBufferSize * sizeof(char));
            if(name == NULL)
            {
                die("can't malloc string\n");
            }
            name = strcpy(name, buf+4);
#ifdef DEBUG
            printf("%s\n", name);
#endif
            FILE *file = fopen(name, "r");
            if(file == NULL)
            {
                die("can't open file to read");
            }
            while(fgets(buf, maxCharBufferSize, file) != NULL)
            {
#ifdef DEBUG
                printf("writing data to file\n");
                printf("%s\n", buf);
#endif
                if (write(cfd, buf, strlen(buf)) < 0)
                {
                    die("Couldn't send message");
                }
            }
            if (write(cfd, "done!", strlen("done!")) < 0)
            {
                die("Couldn't send message");
            }
            if(read(cfd, buf, sizeof(buf) - 1) < 0)
            {
                die("couldn't get donereply\n");
            }
            //three way handshake since this will sdtart reading as next step
            if (write(cfd, "done!", strlen("done!")) < 0)
            {
                die("Couldn't send message");
            }
        }
        
        
        
        if(isget(buf))
        {
            buf[strlen(buf) - 1] = '\0';
            char *name = malloc(maxCharBufferSize * sizeof(char));
            if(name == NULL)
            {
                die("can't malloc string\n");
            }
            name = strcpy(name, buf+4);
#ifdef DEBUG
            printf("file name \"%s\"\n", name);
#endif
            FILE *file = fopen(name, "w");
            if(file == NULL)
            {
                die("can't open file to read");
            }
            free(name);
            char *txt = malloc(maxCharBufferSize * sizeof(char));
            if(txt == NULL)
            {
                die("can't allocat char array\n");
            }
            while(1)
            {
                if ((bytesRead = read(cfd, txt, maxCharBufferSize-1)) < 0)
                {
                    die("Couldn't read message, please try again");
                }
                txt[bytesRead] = '\0';
                if(endsWithDone(txt))//will be problematic if text was maxcharbuffersize-3 we should try to concatenate the last :/
                {
#ifdef DEBUG
                    fprintf(stderr, "done writing\n");
                    txt[strlen(txt)-5]='\0';
                    fprintf(file, "%s", txt);
#endif
                    if (write(cfd, "done!", strlen("done!")) < 0)
                    {
                        die("couldn't reply done");
                    }
                    break;
                }
            
#ifdef DEBUG
                fprintf(stderr, "writing..\n");
                fprintf(stderr, "%s", txt);
#endif
                fprintf(file, "%s", txt);
            }
            fclose(file);
            free(txt);
        }
        
        
        
        while(1)
        {
#ifdef DEBUG
            printf("reading output\n");
#endif
            if ((bytesRead = read(cfd, buf, sizeof(buf)-1)) < 0)
            {
                die("Couldn't get message");
            }
            if(bytesRead == 0)
            {
                return 0;
            }
            buf[bytesRead] = '\0';
            if(endsWithDone(buf))
            {
#ifdef DEBUG
                printf("done reading output\n");
#endif
                buf[strlen(buf) - 5] = '\0';
                printf("%s", buf);
                if (write(cfd, "done!", strlen("done!")) < 0)
                {
                    die("Couldn't send message");
                }
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

int endsWithDone(char *str)
{
    if(strlen(str) < 5)
    {
        return 0;
    }
    int l = strlen(str);
    return str[l-1] == '!' && str[l-2] == 'e' && str[l-3] == 'n' && str[l-4] == 'o' && str[l-5] == 'd';
}

