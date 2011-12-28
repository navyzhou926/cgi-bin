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

#define BUFFER_SIZE     1024 
#define SERVER_PORT	    8000	

int none = 1;

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

int send_html(int count_br, int count_nbsp,  char *str)
{
    int i = count_br;
    printf("<h1>");
    while (i--) 
    {
        printf("<br>");
    }
    for (i = 0; i < count_nbsp; i++) 
    {
        printf("&nbsp");
    }
    printf(" %s</h1>",str);

    return 0;
}

int main(int argc, const char *argv[])
{
    int error = 0;
    int ret = 0;
	char data[4096];
    char buffer[4096];
    int i = 0;
    char *ready[8] = {"ready1", "ready2", "ready3", "ready4", "ready5"};

	int len;
    char *adm;
    char *send_buffer;
	int client_sock;
	socklen_t server_len;
	struct sockaddr_in server;
	struct sockaddr_in server_udp;

	int client_sock_tcp;//server_sock_tcp; 
	struct sockaddr_in server_tcp;//client_tcp;

    pthread_t tid;
    //void *tret = NULL;

    #if 1
	printf("Content-Type: text/html\n\n");
    //while ((ret = fread(data, 1, sizeof(data), stdin)) != 0)
    //while((ret = fread(buf, 1, sizeof(buf), stdin)) != 0)
    while ((ret = fread(data, sizeof(data), 1, stdin)) != 0)
    {
        //printf("read %d len<br>", ret);
        //return 0;
    }
    printf("%s<br>", data);

    adm = data + 7;
    if (strcmp(adm, "Please+choose") == 0) 
    {
        send_html(5, 18, "Please choose AD Machine !");
        printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/sync.html\">");
        return 0;
    }
    else if (strcmp(adm, "all") == 0)
        send_buffer = "readyall";
    else
        send_buffer = *(ready + *(data + 10)-'1');
    #endif

    //sprintf(value, "select * from register_info where username='%s';", str.username);
    #if 1
	if ((client_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("UDP create socket ok!\n");
	}

    int optval = 1;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    //server.sin_port = htons(atoi(str.port));
    server.sin_port = htons(SERVER_PORT);
    //server.sin_addr.s_addr = htons(INADDR_ANY);
    server.sin_addr.s_addr = inet_addr("255.255.255.255");
    //server.sin_addr.s_addr = inet_addr("10.1.14.45");
    setsockopt(client_sock, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)); 

/*
    if (bind(client_sock, (struct sockaddr *)&client, sizeof(client)) < 0) 
    {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("UDP bind addr ok !\n\n");
    }
    */

    #if 1
    error = pthread_create(&tid, NULL, pthread, NULL);
    if (error != 0) 
    {
        fprintf(stderr, "can't create thread:%s",strerror(error));
        exit(1);
    }
    #endif

    server_len = sizeof(server);
    //len = sendto(client_sock, "start1", sizeof("start1"), 0, (struct sockaddr *)&server, server_len);
    //sleep(5);
    //len = sendto(client_sock, "stop1", sizeof("stop1"), 0, (struct sockaddr *)&server, server_len);
    //len = sendto(client_sock, "ready1", sizeof("ready1"), 0, (struct sockaddr *)&server, server_len);
    len = sendto(client_sock, send_buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server, server_len);
    #if 1
    len = recvfrom(client_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server, &server_len);
    printf("<br>%s<br>", buffer);
    if (len < 0) 
    {
        close(client_sock);
        fprintf(stderr, "%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (strcmp(buffer, "ready to sync") == 0) 
    {
        none = 0;
        //error = pthread_cancel(tid);
        if (error != 0) 
        {
            fprintf(stderr, "can't pthread_join thread:%s",strerror(error));
            exit(1);
        }
        printf("<br>UDP Connect server successfully !\n");
        memcpy((void *)&server_udp, (void *)&server, server_len);
    }
    #endif

    //TCP
    #if 1
	if ((client_sock_tcp = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("<br>TCP create socket ok!\n");
	}
    /*
	bzero(&server_tcp, sizeof(server_tcp));
	server_tcp.sin_family = AF_INET;
	server_tcp.sin_port = htons(SERVER_PORT);
	server_tcp.sin_addr.s_addr = inet_addr(*(ip + *(data + 10)-'1'));
    */
    server_tcp = server_udp;
	server_len = sizeof(server_tcp);
	if(connect(client_sock_tcp, (struct sockaddr *)&server_tcp, server_len) < 0)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else
	{
        printf("<br>TCP connect server ok!\n");
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
                    printf("<br><br>%s",buffer + 4);
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
                            printf("<br>read byte = %d",len + 4);
                            //sprintf(value, "%d%c%s", i, '0', buffer);//0 means file data
                            len = send(client_sock_tcp, buffer, BUFFER_SIZE, 0);
                            printf("&nbsp send byte = %d", len);
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
    #endif
    return 0;
}

