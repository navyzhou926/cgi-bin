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
#define SERVER_PORT_UDP	   21203 

int none = 1;

void *pthread(void *arg)
{
    sleep(3);
    if (none == 1) 
    {
        printf("<h2><br>Nothing to update!<h2>\n");
        //printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/sync.html\">");
        exit(1);
    }

    return (void *)1;
}

int main(int argc, const char *argv[])
{
    int error = 0;
    int ret = 0;
    char data[4096] = {0};
    char buffer[4096] = {0};
    int i = 0;
    //char *ready[8] = {"ready1", "ready2", "ready3", "ready4", "ready5"};
    int num = 0;
    int if_is_all_file = 0;
    int if_is_all_device = 0;
    char *p = NULL, *q = NULL;
    int len;
    char *send_buffer = NULL;
    char *send_file = NULL;

    int client_sock;
    socklen_t server_len;
    struct sockaddr_in server;
    struct sockaddr_in server_udp[256];

    int client_sock_tcp;//server_sock_tcp; 
    struct sockaddr_in server_tcp;//client_tcp;

    pthread_t tid;

    printf("Content-Type: text/html\n\n");
    while ((ret = fread(data, sizeof(data), 1, stdin)) != 0)
    {
        //printf("read %d len<br>", ret);
    }
    //printf("%s<br>", data);
    //file=choose+file&choose=choose+device

    //file=choose+file&choose=192.168.11.168@00%3A44%3A3b%3Ae6%3A00%3A01
    //file=choose+file&choose=all_device

    //file=lib.tar.gz&choose=choose+device
    //file=all_file&choose=choose+device

    //file=all_file&choose=all_device
    //file=lib.tar.gz&choose=all_device
    //file=all_file&choose=192.168.11.252@00%3A0c%3A29%3A3a%3A3e%3A87
    //file=lib.tar.gz&choose=192.168.11.168@00%3A44%3A3b%3Ae6%3A00%3A01

    if (strncmp(data + 5, "choose+file", 11) == 0) 
    {
        printf("<h2>请选择要更新的文件!<h2>\n");
        //printf("<meta http-equiv=\"refresh\"content=\"1; url=http://%s/sync.html\">", ip);
        return 0;
    }
    if (strstr(data + 5, "choose=choose+device") != NULL) 
    {
        printf("<h2>请选择要更新的设备!<h2>\n");
        //printf("<meta http-equiv=\"refresh\"content=\"1; url=http://%s/sync.html\">", ip);
        return 0;
    }
    if (strncmp(data + 5, "all_file", 8) == 0) 
    {
        if_is_all_file = 1;
    }
    if (strstr(data + 5, "choose=all_device") != NULL) 
    {
        if_is_all_device = 1;
    }

    if (if_is_all_file == 0) //single file
    {
        //file=lib.tar.gz&choose=all_device
        p = data + 5;
        while (*p != '\0')
        {
            if (*p == '&') 
            {
                if (strncmp(p + 1, "choose=", 7) == 0) 
                {
                    *p = '\0';
                    send_file = data + 5;
                    break;
                }
            }
            p++;
        }
    }

    if (if_is_all_device == 1) //all devices
    {
        send_buffer = "readyall";
    }
    else  // single device
    {
#if 1
        //data = choose=192.168.11.252@00%3A0c%3A29%3A3a%3A3e%3A87
        char *receive_ip = NULL;
        char *temp;
        char receive_mac[36] = {0};
        char receive_device_info[72] = {0};


        if (if_is_all_file == 1) //single file
        {
            p = data + 21;  //strlen("file=all_file&choose=") == 21
        }
        else
        {
            p = data + 5 + strlen(send_file) + 8;       
            //file=lib.tar.gz&choose=192.168.11.168@00%3A44%3A3b%3Ae6%3A00%3A01
        }
        temp = p;
        while (*p != '\0')
        {
            if (*p == '@') 
            {
                *p = '\0';
                receive_ip = temp;
                q = p + 1;
                for (i = 0; i < 5; i++) 
                {
                    receive_mac[i*3] = *q;
                    receive_mac[i*3 + 1] = *(q + 1);
                    receive_mac[i*3 + 2] = ':';
                    q += 5;
                }
                receive_mac[i*3] = *q;
                receive_mac[i*3 + 1] = *(q + 1);
                printf_debug("receive: %s@%s\n",receive_ip, receive_mac);
                sprintf(receive_device_info, "%s@%s", receive_ip, receive_mac);
                break;
            }
            p++;
        }
        //send_buffer = 192.168.11.252@00:0c:29:3a:3e:87 
        send_buffer = receive_device_info;
#endif
        //send_buffer = "ready1";
    }
#if 0
    printf_debug("<br>if_is_all_file = %d<br>\n",if_is_all_file);
    printf_debug("if_is_all_device = %d<br>\n",if_is_all_device);
    printf_debug("send_file: %s<br>\n",send_file);
    printf_debug("send_buffer: %s<br>\n",send_buffer);
    return 0;
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
        printf_debug("<br>UDP create socket ok!<br>\n");
    }

    int optval = 1;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    //server.sin_port = htons(atoi(str.port));
    server.sin_port = htons(SERVER_PORT);
    //server.sin_addr.s_addr = htons(INADDR_ANY);
    server.sin_addr.s_addr = inet_addr("255.255.255.255");
    setsockopt(client_sock, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)); 
#endif

    error = pthread_create(&tid, NULL, pthread, NULL);
    if (error != 0) 
    {
        fprintf(stderr, "can't create thread:%s",strerror(error));
        exit(1);
    }

    server_len = sizeof(server);
    len = sendto(client_sock, send_buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server, server_len);
    //if (strcmp(send_buffer, "readyall") != 0) //just update one device
    if (if_is_all_device == 0) //single device 
    {
        server_len = sizeof(server);
ready_again:
        len = recvfrom(client_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server, &server_len);
        printf_debug("<br>buffer: %s<br>\n",buffer);

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
            printf_debug("<br>UDP Connect server successfully !\n");
            memcpy((void *)&server_udp[num++], (void *)&server, server_len);
        }
        else
            goto ready_again;
    }
    else 
    {
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
                    //printf_debug("<br>%s<br>", buffer);
                    if (len < 0) 
                    {
                        close(client_sock);
                        fprintf(stderr, "%s\n", strerror(errno));
                        exit(EXIT_FAILURE);
                    }
                    if (strcmp(buffer, "ready to sync") == 0) 
                    {
                        none = 0;
                        if (error != 0) 
                        {
                            fprintf(stderr, "can't pthread_join thread:%s",strerror(error));
                            exit(1);
                        }
                        printf_debug("<br>UDP Connect server successfully !\n");
                        memcpy((void *)&server_udp[num++], (void *)&server, server_len);
                    }
                }
            }
        }
    }
    if (num == 0) 
    {
        printf("<h2><br>Nothing to update!<h2>\n");
        error = pthread_cancel(tid);
        //printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/sync.html\">");
        exit(1);
    }

    //TCP
    int success_count = 0;
    int k = 0;
    struct in_addr in;
    for (k = num-1; k >= 0; k--) 
    {
        if ((client_sock_tcp = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            fprintf(stderr, "%s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        else
        {
            printf_debug("<br>TCP create socket ok!\n");
        }
#if 0
        bzero(&server_tcp, sizeof(server_tcp));
        server_tcp.sin_family = AF_INET;
        server_tcp.sin_port = htons(SERVER_PORT_TCP);
        //server_tcp.sin_addr.s_addr = inet_addr(*(ip + *(data + 10)-'1'));
        server_tcp.sin_addr.s_addr = inet_addr("192.168.11.252");
#else
        server_tcp = server_udp[k];
        server_tcp.sin_port = htons(SERVER_PORT_TCP);
        server_len = sizeof(server_tcp);
        in.s_addr = server_tcp.sin_addr.s_addr;
        //sleep(10);
#endif
        int flag;
        int connected = 0;
        flag = fcntl(client_sock_tcp, F_GETFL, 0);
        flag |= O_NONBLOCK;
        fcntl(client_sock_tcp, F_SETFL, flag);//设置非阻塞
        connected = connect(client_sock_tcp, (struct sockaddr *)&server_tcp, server_len);
        //connected = connect(sock, (struct sockaddr *) &address, sizeof(address));
        if (connected != 0) 
        {
            if(errno != EINPROGRESS)
            {
                printf("<h2>%s update failed !<br><h2>\n", inet_ntoa(in));
                goto tcp_end;
                //logdown(ERROR,"connect error :%s\n",strerror(errno));
            }
            else //esle
            {
                struct timeval tm;
                tm.tv_sec = 3;
                tm.tv_usec = 0;
                fd_set set,rset;
                FD_ZERO(&set);
                FD_ZERO(&rset);
                FD_SET(client_sock_tcp, &set);
                FD_SET(client_sock_tcp, &rset);
                int res;
                res = select(client_sock_tcp+1, &rset,&set, NULL, &tm);
                if(res < 0)
                {
                    printf("<h2>Network error in connect, %s update failed !<br><h2>\n", inet_ntoa(in));
                    goto tcp_end;
                    //ret =  -1;
                }
                else if(res == 0)
                {
                    //logdown(WARNNING,"connect time out\n");
                    printf("<h2>Connect time out, %s update failed !<br><h2>\n", inet_ntoa(in));
                    //printf_debug("client port: %d\n", ntohs(client_tcp.sin_port));
                    //close(client_sock_tcp);
                    goto tcp_end;
                    //ret = -2;
                }
                else if (res == 1)
                {
                    if(FD_ISSET(client_sock_tcp, &set))
                    {
                        flag &= ~O_NONBLOCK;
                        fcntl(client_sock_tcp, F_SETFL, flag);//设置阻塞
                        //return client_sock_tcp;
                        //Opendir
                        FILE *fd;
                        char path[1024];

                        if (if_is_all_file == 1) 
                        {
                            DIR *dir;
                            struct dirent *ptr;

                            memset(path, 0, sizeof(path));
                            //sprintf(path, "%s/%s", DIR_UPLOAD, *(filename + i));
                            if ((dir = opendir(DIR_UPLOAD)) != NULL) 
                            {
                                while ((ptr = readdir(dir)) != NULL)
                                {
                                    if (ptr->d_type == DT_REG) 
                                    {
                                        sprintf(path, "%s/%s",DIR_UPLOAD, ptr->d_name);
                                        fd = fopen(path, "r");
                                        if(fd == NULL)
                                        {
                                            printf("<h2><br>can not open file<h2>");
                                            return 0;
                                        }
                                        memset(buffer, 0, sizeof(buffer));
                                        //buffer[0] =  i + '0'; buffer[1] = '1';
                                        buffer[0] =  '0'; buffer[1] = '1';
                                        len = strlen(ptr->d_name);
                                        buffer[2] = (len + 4)>>8;
                                        buffer[3] = (len + 4)&0x00ff;
                                        strcpy(buffer + 4, ptr->d_name);
                                        //sprintf(value, "%d%c%s", i, '1', ptr->d_name);//1 means filename not file data
                                        len = send(client_sock_tcp, buffer, BUFFER_SIZE, 0);
                                        printf("<h5>Sending %s ...<h5>",buffer + 4);
                                        //buffer[0] =  i + '0'; buffer[1] = '0';
                                        buffer[0] =  '0'; buffer[1] = '0';
                                        while (1)
                                        {
                                            len = fread(buffer + 4, 1, BUFFER_SIZE-4, fd);
                                            //printf_debug("<br>read byte = %d",len);
                                            if (len > 0) 
                                            {
                                                buffer[2] = (len + 4)>>8;
                                                buffer[3] = (len + 4)&0x00ff;
                                                //printf_debug("<br>read byte = %d",len + 4);
                                                //sprintf(value, "%d%c%s", i, '0', buffer);//0 means file data
                                                len = send(client_sock_tcp, buffer, BUFFER_SIZE, 0);
                                                //printf_debug("&nbsp send byte = %d", len);
                                            }
                                            if (feof(fd)) 
                                            {
                                                fclose(fd);
                                                break;
                                            }
                                        }
                                    }
                                }
                                closedir(dir);
                            }
                        }
                        else
                        {
                            sprintf(path, "%s/%s",DIR_UPLOAD, send_file);
                            fd = fopen(path, "r");
                            if(fd == NULL)
                            {
                                printf("<h2><br>can not open file<h2>");
                                return 0;
                            }
                            memset(buffer, 0, sizeof(buffer));
                            //buffer[0] =  i + '0'; buffer[1] = '1';
                            buffer[0] =  '0'; buffer[1] = '1';
                            len = strlen(send_file);
                            buffer[2] = (len + 4)>>8;
                            buffer[3] = (len + 4)&0x00ff;
                            strcpy(buffer + 4, send_file);
                            len = send(client_sock_tcp, buffer, BUFFER_SIZE, 0);
                            printf("<h5>Sending %s ...<h5>",buffer + 4);
                            //buffer[0] =  i + '0'; buffer[1] = '0';
                            buffer[0] =  '0'; buffer[1] = '0';
                            while (1)
                            {
                                len = fread(buffer + 4, 1, BUFFER_SIZE-4, fd);
                                //printf("<br>read byte = %d",len);
                                if (len > 0) 
                                {
                                    buffer[2] = (len + 4)>>8;
                                    buffer[3] = (len + 4)&0x00ff;
                                    //printf("<br>read byte = %d",len + 4);
                                    //sprintf(value, "%d%c%s", i, '0', buffer);//0 means file data
                                    len = send(client_sock_tcp, buffer, BUFFER_SIZE, 0);
                                    //printf("&nbsp send byte = %d", len);
                                }
                                if (feof(fd)) 
                                {
                                    fclose(fd);
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        fprintf(stderr, "%s\n", strerror(errno));
                        printf("<h2>%s update failed !<br><h2>\n", inet_ntoa(in));
                        goto tcp_end;
                        //ret = -3;
                        //logdown(WARNNING,"other error when select:%s\n",strerror(errno));
                    }
                }
                //return ret;
            }
        }
        //return client_sock_tcp;

#if 0
        if(connect(client_sock_tcp, (struct sockaddr *)&server_tcp, server_len) < 0)
        {
            fprintf(stderr, "%s\n", strerror(errno));
            printf("<h2>%s update failed !<br><h2>\n", inet_ntoa(in));
            //printf_debug("client port: %d\n", ntohs(client_tcp.sin_port));
            //close(client_sock_tcp);
            goto tcp_end;
            //continue;
            //exit(EXIT_FAILURE);
        }
        else
        {
            printf_debug("<br>TCP connect server successfully !\n");
        }
#endif

        printf("<h2>%s update successfully<br><h2>\n", inet_ntoa(in));
        //printf_debug("client port: %d\n", ntohs(client_tcp.sin_port));
        success_count++;
tcp_end:
        close(client_sock_tcp);
    }

    close(client_sock);
    if (success_count == num) 
    {
        printf("<h2><br>All devices update data successfully!<h2>\n");
    }
    else
    {
        printf("<h2><br>%d/%d devices update data successfully!<h2>\n",success_count, num);
    }
    //printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/sync.html\">");
    return 0;
}

