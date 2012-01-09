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

int main(int argc, const char *argv[])
{
    int ret = 0;
	char data[4096];
    char buffer[4096];
    int i = 0;
    char *start[7] = {"start1", "start2", "start3", "start4", "start5"};
    char ip[36] = {0};

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

    if (get_local_ip(ip, NULL) != 0)  
    {   
        printf("Server is unavailable\n");
        exit(1);
    }

    if (strstr(data, "startall") != NULL) 
    {
        strcpy(buffer, "startall");
    }
    else
    {
        if (*data == 's') 
        {
            send_html(0, 0, "Please choose AD Machine !");
            //printf("<meta http-equiv=\"refresh\"content=\"1; url=http://192.168.11.252/adm_manage.html\">");
            printf("<meta http-equiv=\"refresh\"content=\"1; url=http://%s/adm_manage.html\">",ip);
            return 0;
        }
        for (i = 0; i < 5; i++) 
        {
            if (strchr(data, i + '1') != NULL)
            {
                strcpy(buffer, *(start + i));
                break;
            }
        }
        for (i = i + 1; i < 5; i++) 
        {
            if (strchr(data, i + '1') != NULL)
            {
                strcat(buffer, *(start + i));
            }
        }
    }

	if ((client_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
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

    send_html(0, 0, "Start successfully !");
    //printf("<meta http-equiv=\"refresh\"content=\"1; url=http://192.168.11.252/adm_manage.html\">");
            printf("<meta http-equiv=\"refresh\"content=\"1; url=http://%s/adm_manage.html\">",ip);
    return 0;
}

