#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
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
#include "global.h"

#define BUFFER_SIZE     1024 

int main(int argc, const char *argv[])
{
    printf("Content-Type: text/html\n\n");

    printf("<html>\n");
    printf("<head>\n");
    printf("<title>sync</title>\n");
    printf("</head>\n");
    printf("<body bgcolor=\"#48d1cc\">\n");
    printf("<form name=\"input\" action='./syncadm.cgi' method=POST>\n");
    printf("<p align=center>\n");
    printf("<font size=3 color=\"#ffff00\">\n");
    printf("<br><br><br>\n");
    printf("&nbsp&nbsp&nbsp\n");

#if 1
    DIR *dir;
    struct dirent *ptr;
    int if_have_file = 0;

    printf("File:\n");
    printf("<select name=\"file\">\n");
    //printf("<option value=\"choose\" selected=\"selected\">Please choose</option>\n");
    printf("<option value=\"choose file\" selected=\"selected\">请选择文件</option>\n");

    if ((dir = opendir(DIR_UPLOAD)) != NULL) 
    {
        while ((ptr = readdir(dir)) != NULL)
        {
            if (ptr->d_type == DT_REG) 
            {
                //sprintf(buffer, "%s/%s",DIR_UPLOAD, ptr->d_name);
                if (if_have_file == 0) 
                {
                    printf("<option value=\"all_file\">All files</option>\n");
                    if_have_file = 1;
                }
                printf("<option value=%s>%s</option>\n",ptr->d_name, ptr->d_name);
            }
        }
    }
    printf("</select>\n");
    #endif

    printf("&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp\n");

    printf("Device:\n");
    printf("<select name=\"choose\">\n");
    printf("<option value=\"choose device\" selected=\"selected\">请选择设备</option>\n");
#if 1
    int len;
    int client_sock;
    socklen_t server_len;
    struct sockaddr_in server;

    int ret = 0;
    char buffer[4096];
    int if_have_live_device = 0;
    char *ip = NULL, *mac = NULL;
    char *p = NULL;

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
    //server.sin_port = htons(atoi(str.port));
    server.sin_port = htons(SERVER_PORT);
    //server.sin_addr.s_addr = htons(INADDR_ANY);
    server.sin_addr.s_addr = inet_addr("255.255.255.255");
    setsockopt(client_sock, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)); 


    strcpy(buffer, "heartbeat");
    server_len = sizeof(server);
    len = sendto(client_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server, server_len);

#if 1  //UDP
    int maxfd = client_sock;
    fd_set input_fd;
    struct timeval tv;
    while (1)
    {
        FD_ZERO(&input_fd);
        FD_SET(client_sock, &input_fd);
        tv.tv_sec = 2;
        tv.tv_usec= 0;
        if((ret = select(maxfd + 1, &input_fd, NULL, NULL, &tv)) < 0)
        {
            fprintf(stderr, "%s\n", strerror(errno));
            continue;
        }
        else if(ret == 0)
        {
            break;
        }
        else
        {
            if(FD_ISSET(client_sock, &input_fd))
            {
                server_len = sizeof(server);
                len = recvfrom(client_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server, &server_len);
                //printf("<br>%s<br>", buffer);
                if (len < 0) 
                {
                    close(client_sock);
                    fprintf(stderr, "%s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
                //buffer = heartbeat@192.168.11.252@00:0c:29:3a:3e:87
                if (strncmp(buffer, "heartbeat@", 10) == 0) 
                {
                    p = buffer+10;
                    while (*p != '\0')
                    {
                        if (*p == '@') 
                        {
                            if (if_have_live_device == 0) 
                            {
                                printf("<option value=\"all_device\">All devices</option>\n");
                                if_have_live_device = 1;
                            }
                            *p = '\0';
                            ip = buffer + 10;
                            mac = p + 1;
                            printf("<option value=%s@%s>%s</option>\n",ip, mac, ip);
                            break;
                        }
                        p++;
                    }
                    //printf("<option value=\"ADM1\">AD Machine 1</option>\n");
                    //printf("<option value=\"ADM1\">%s</option>\n",ip);
                    //FILE *fp = NULL;
                    //fp = fopen("test_test", "w+");
                    //printf("<h2>%s<h2>\n",buffer+10);
                    //fwrite(buffer+10, strlen(buffer+10), 1, fp);
                    //fclose(fp);
                }
            }
        }
    }
    if (if_have_live_device == 0) 
    {
        printf("<option value=\"none\">None live device</option>\n");
    }
    close(client_sock);
#endif
#else
printf("<option value=\"all\">All devices</option>\n");
printf("<option value=\"192.168.11.250@00:44:00:00:00:00\">192.168.11.250</option>\n");
printf("<option value=\"192.168.11.252@00:0c:29:3a:3e:87\">192.168.11.252</option>\n");
#endif

    printf("</select>\n");
    printf("<input type=\"submit\" value=\"Update\"/>\n");
    printf("</font>\n");
    printf("</p>\n");
    printf("</form>\n");
    printf("<form name=\"input\" action='./sync.cgi' method=POST>\n");
    printf("<font size=3 color=\"#ffff00\">\n");
    printf("<p align=\"center\"> <br><br><br>&nbsp&nbsp\n");
    printf("IP:\n");
    printf("<input type=\"text\" name=\"address\" /><br/><br>\n");
    printf("Port:\n");
    printf("<input type=\"text\" name=\"port\" /><br/><br>\n");
    printf("&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp\n");
    printf("&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp\n");
    printf("&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp\n");
    printf("&nbsp&nbsp\n");
    printf("<input type=\"submit\" value=\"Update\"/>\n");
    printf("</p>\n");
    printf("</font>\n");
    printf("</form>\n");
    printf("</body>\n");
    printf("</html>\n");

    return 0;
}

