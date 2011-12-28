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
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <pthread.h>
#include <dirent.h>
#include <sqlite3.h>
#include "global.h"

#define BUFFER_SIZE     1024 
#define SERVER_PORT	    8000	

int none = 1;

struct string
{
    char *ip;
    char *port;
}str = {NULL, NULL};

void *pthread(void *arg)
{
    sleep(5);
    if (none == 1) 
    {
        printf("<br>Nothing to sync !\n");
        printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/sync.html\">");
        exit(1);
    }

    return (void *)1;
}

int main(int argc, const char *argv[])
{
    int error = 0;
    int ret = 0;
	char data[4096];
    char buffer[4096];
    char *p = NULL;
    int i = 0;

	int len;
	int client_sock;
	socklen_t server_len;
	struct sockaddr_in server;
	struct sockaddr_in server_udp;
    //fd_set input_fd;

	int client_sock_tcp;//server_sock_tcp; 
	struct sockaddr_in server_tcp;//client_tcp;

    pthread_t tid;
    //void *tret = NULL;

	printf("Content-Type: text/html\n\n");
    //while ((ret = fread(data, 1, sizeof(data), stdin)) != 0)
    //while((ret = fread(buf, 1, sizeof(buf), stdin)) != 0)
    while ((ret = fread(data, sizeof(data), 1, stdin)) != 0)
    {
        //printf("read %d len<br>", ret);
        //printf("%s<br>", data);
        //return 0;
    }

    str.ip = data + 8;
    if ((p = strstr(data, "&port=")) != NULL)
    {
        *p = '\0';
    }
    str.port = p + 6;
    if (strlen(str.ip) > 15)
    {
        send_html(5, 20, "Ip address is overlength !");
        printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/sync.html\">");
        return 0;
    }
    //printf("ip %s<br>",str.ip);
    //printf("port %s<br>",str.port);
    if (strlen(str.port) > 5)
    {
        send_html(5, 18, "Port number is overlength !");
        printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/sync.html\">");
        return 0;
    }
    if (str.ip == p && *(str.port) == '\0') 
    {
        send_html(5, 22, "Blank ip address and port !");
        printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/sync.html\">");
        return 0;
    }
    for (i = 0; *(str.ip + i) != '\0'; i++) 
    {
        if (*(str.ip + i) == '.') 
        {
            continue;
        }
        if (isdigit(*(str.ip + i)) == 0) 
        {
            send_html(5, 24, "Ip address wrong !");
            printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/sync.html\">");
            return 0;
        }
    }
    for (i = 0; *(str.port + i) != '\0'; i++) 
    {
        if (isdigit(*(str.port + i)) == 0) 
        {
            send_html(5, 22, "Port number wrong !");
            printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/sync.html\">");
            return 0;
        }
    }
    //sprintf(value, "select * from register_info where username='%s';", str.username);

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
    //server.sin_port = htons(atoi(SERVER_PORT));
    server.sin_port = htons(SERVER_PORT);
    //server.sin_addr.s_addr = htons(INADDR_ANY);
    server.sin_addr.s_addr = inet_addr("255.255.255.255");
    setsockopt(client_sock, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)); 



    #if 1
    error = pthread_create(&tid, NULL, pthread, NULL);
    if (error != 0) 
    {
        fprintf(stderr, "can't create thread:%s",strerror(error));
        exit(1);
    }
    #endif

    server_len = sizeof(server);
    len = sendto(client_sock, "ready1", BUFFER_SIZE, 0, (struct sockaddr *)&server, server_len);
    len = recvfrom(client_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server, &server_len);
    if (len < 0) 
    {
        close(client_sock);
        fprintf(stderr, "%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (strcmp(buffer, "ready to sync") == 0) 
    {
        none = 0;
        error = pthread_cancel(tid);
        if (error != 0) 
        {
            fprintf(stderr, "can't pthread_join thread:%s",strerror(error));
            exit(1);
        }
        printf("<br>UDP Connect server successfully !\n");
        memcpy((void *)&server_udp, (void *)&server, server_len);
    }

    //TCP
    #if 1
	if ((client_sock_tcp = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else
	{
        //printf("<br>TCP create socket ok!\n");
	}
    /*
	bzero(&server_tcp, sizeof(server_tcp));
	server_tcp.sin_family = AF_INET;
	server_tcp.sin_port = htons(SERVER_PORT);
	server_tcp.sin_addr.s_addr = inet_addr(str.ip);
    */
    server_tcp = server_udp;
    server_tcp.sin_port = htons(atoi(str.port));
	server_len = sizeof(server_tcp);
	if(connect(client_sock_tcp, (struct sockaddr *)&server_tcp, server_len) < 0)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else
	{
        printf("<br>TCP connect server successfully !\n");
	}

    //Opendir
    #if 1
    FILE *fd;
    DIR *dir;
    char *filename[256] = {"PictureFile", "MediaFile", "TextFile", "OtherFile"};
    char path[1024];
    struct dirent *ptr;

    memset(path, 0, sizeof(path));
    for (i = 0; i < 4; i++) 
    {
        sprintf(path, "/home/navyzhou/upload-folder/%s", *(filename + i));
        if ((dir = opendir(path)) != NULL) 
        {
            while ((ptr = readdir(dir)) != NULL)
            {
                if (ptr->d_type == DT_REG) 
                {
                    sprintf(path, "/home/navyzhou/upload-folder/%s/%s",*(filename + i), ptr->d_name);
                    fd = fopen(path, "r");
                    if(fd == NULL)
                    {
                        printf("<br>can not open file");
                        return 0;
                    }
                    memset(buffer, 0, sizeof(buffer));
                    buffer[0] =  i + '0'; buffer[1] = '1';
                    len = strlen(ptr->d_name);
                    buffer[2] = (len + 4)>>8;
                    buffer[3] = (len + 4)&0x00ff;
                    strcpy(buffer + 4, ptr->d_name);
                    //sprintf(value, "%d%c%s", i, '1', ptr->d_name);//1 means filename not file data
                    len = send(client_sock_tcp, buffer, BUFFER_SIZE, 0);
                    printf("<br><br>Sending %s",buffer + 4);
                    #if 1
                    buffer[0] =  i + '0'; buffer[1] = '0';
                    while (1)
                    {
                        len = fread(buffer + 4, 1, BUFFER_SIZE-4, fd);
                        //printf("<br>read byte = %d",len);
                        #if 1
                        if (len > 0) 
                        {
                            buffer[2] = (len + 4)>>8;
                            buffer[3] = (len + 4)&0x00ff;
                            //printf("<br>read byte = %d",len + 4);
                            //sprintf(value, "%d%c%s", i, '0', buffer);//0 means file data
                            len = send(client_sock_tcp, buffer, BUFFER_SIZE, 0);
                            //printf("&nbsp send byte = %d", len);
                        }
                        #endif
                        if (feof(fd)) 
                        {
                            fclose(fd);
                            break;
                        }
                    }
                    #endif
                }
            }
            closedir(dir);
        }
    }
    #endif
    /*
    if((len = recv(client_sock_tcp, buffer, BUFFER_SIZE, 0)) > 0)
    {
        //write(STDOUT_FILENO, buffer, len);  //printf to terminal
        printf("<br>%s\n",buffer);
    }
    */
	close(client_sock_tcp);
    #endif

    #if 0
    FD_ZERO(&input_fd);
    FD_SET(server_sock, &input_fd);
    FD_SET(fd, &input_fd);

    while (1)
    {
        if((select(FD_SETSIZE, &input_fd, NULL, NULL, NULL)) < 0)
        {
            fprintf(stderr, "%s\n", strerror(errno));
            continue;
        }
        if(FD_ISSET(server_sock, &input_fd))
        {
        }
    }
    #endif
	close(client_sock);
    //exit(EXIT_SUCCESS);

    printf("<br><br>Sync data successfully!\n");
    //printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/sync.html\">");
    return 0;
}

