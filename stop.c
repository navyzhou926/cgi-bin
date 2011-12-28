#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "global.h"

#define BUFFER_SIZE     1024 
#define SERVER_PORT	    8000	

int main(int argc, const char *argv[])
{
    int ret = 0;
	char data[4096];
    char buffer[4096];
    int i = 0;
    char *stop[6] = {"stop1", "stop2", "stop3", "stop4", "stop5"};

	int len;
	int client_sock;
	socklen_t server_len;
	struct sockaddr_in server;

    memset(buffer, 0, sizeof(buffer));

	printf("Content-Type: text/html\n\n");
    while ((ret = fread(data, sizeof(data), 1, stdin)) != 0)
    {
        //printf("read %d len<br>", ret);
        //return 0;
    }
    //printf("%s<br>", data);

    if (strstr(data, "stopall") != NULL) 
    {
        strcpy(buffer, "stopall");
    }
    else
    {
        if (*data == 's') 
        {
            send_html(4, 16, "Please choose AD Machine !");
            printf("<meta http-equiv=\"refresh\"content=\"1; url=http://192.168.11.252/adm_manage.html\">");
            return 0;
        }
        for (i = 0; i < 5; i++) 
        {
            if (strchr(data, i + '1') != NULL)
            {
                strcpy(buffer, *(stop + i));
                break;
            }
        }
        for (i = i + 1; i < 5; i++) 
        {
            if (strchr(data, i + '1') != NULL)
            {
                strcat(buffer, *(stop + i));
            }
        }
    }

	if ((client_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else
	{
        //printf("UDP create socket ok!\n");
	}

    int optval = 1;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    //server.sin_addr.s_addr = htons(INADDR_ANY);
    server.sin_addr.s_addr = inet_addr("255.255.255.255");
    setsockopt(client_sock, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)); 

    server_len = sizeof(server);
    len = sendto(client_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server, server_len);
	close(client_sock);
    //exit(EXIT_SUCCESS);

    send_html(4, 22, "Stop successfully !");
    printf("<meta http-equiv=\"refresh\"content=\"1; url=http://192.168.11.252/adm_manage.html\">");
    return 0;
}

