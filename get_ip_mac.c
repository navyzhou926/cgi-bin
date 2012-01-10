#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#if 1
int get_local_ip(char *out_ip, char *out_mac)
{
    int i = 0;
    int sockfd;
    struct ifconf ifconf;
    char buf[512];
    struct ifreq *ifreq;
    char *ip;

    ifconf.ifc_len = 512;
    ifconf.ifc_buf = buf;


    if (out_ip == NULL && out_mac == NULL) 
    {
        return -1;
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
        return -1;
    if (ioctl(sockfd, SIOCGIFCONF, &ifconf) < 0)
        return -1;

    ifreq = (struct ifreq *)buf;

    if (out_ip != NULL) 
    {
        for (i = (ifconf.ifc_len/sizeof(struct ifreq)); i > 0; i--) 
        {
            ip = inet_ntoa(((struct sockaddr_in *)&(ifreq->ifr_addr))->sin_addr);
            #if 1
            if (strcmp(ip, "127.0.0.1") == 0 || strcmp(ip, "127.0.1.1") == 0) 
            {
                ifreq++;
                continue;
            }
            #endif
            strcpy(out_ip, ip);
            break;
        }
    }

    if (out_mac != NULL) 
    {
        for (i = 0; i < 10; i++)  //get mac 
        {   
            memset(ifreq, 0, sizeof(ifreq));
            sprintf(ifreq->ifr_name,"eth%c",0x30+i);
            //strncpy(ifreq->ifr_name, "eth0", sizeof(ifreq->ifr_name)-1);
            if (ioctl(sockfd, SIOCGIFHWADDR, ifreq) < 0)
                continue;
            else
                break;
        }   
        if (i >= 10) 
        {
            close(sockfd);
            return -1;
        }
        sprintf(out_mac, "%02x:%02x:%02x:%02x:%02x:%02x",
        (unsigned char)ifreq->ifr_hwaddr.sa_data[0],
        (unsigned char)ifreq->ifr_hwaddr.sa_data[1],
        (unsigned char)ifreq->ifr_hwaddr.sa_data[2],
        (unsigned char)ifreq->ifr_hwaddr.sa_data[3],
        (unsigned char)ifreq->ifr_hwaddr.sa_data[4],
        (unsigned char)ifreq->ifr_hwaddr.sa_data[5]);
    }

    close(sockfd);

    return 0;
}
#else
int get_local_ip(char *out_ip, char *out_mac)
{
    int i = 0;
    int sockfd;
    struct ifconf ifconf;
    char buf[512];
    struct ifreq *ifreq;
    char *ip;

    //struct ifreq ifr_mac;

    ifconf.ifc_len = 512;
    ifconf.ifc_buf = buf;


    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
        return -1;
    if (ioctl(sockfd, SIOCGIFCONF, &ifconf) < 0)
        return -1;


    ifreq = (struct ifreq *)buf;

    for (i = (ifconf.ifc_len/sizeof(struct ifreq)); i > 0; i--) 
    {
        ip = inet_ntoa(((struct sockaddr_in *)&(ifreq->ifr_addr))->sin_addr);
        #if 1
        if (strcmp(ip, "127.0.0.1") == 0 || strcmp(ip, "127.0.1.1") == 0) 
        {
            ifreq++;
            continue;
        }
        #endif
        strcpy(out_ip, ip);
        break;
    }

    for (i = 0; i < 10; i++)  //get mac 
    {   
        memset(ifreq, 0, sizeof(ifreq));
        sprintf(ifreq->ifr_name,"eth%c",0x30+i);
        //strncpy(ifreq->ifr_name, "eth0", sizeof(ifreq->ifr_name)-1);
        if (ioctl(sockfd, SIOCGIFHWADDR, ifreq) < 0)
            continue;
        else
            break;
    }   
    if (i >= 10) 
    {
        close(sockfd);
        return -1;
    }
    sprintf(out_mac, "%02x:%02x:%02x:%02x:%02x:%02x",
    (unsigned char)ifreq->ifr_hwaddr.sa_data[0],
    (unsigned char)ifreq->ifr_hwaddr.sa_data[1],
    (unsigned char)ifreq->ifr_hwaddr.sa_data[2],
    (unsigned char)ifreq->ifr_hwaddr.sa_data[3],
    (unsigned char)ifreq->ifr_hwaddr.sa_data[4],
    (unsigned char)ifreq->ifr_hwaddr.sa_data[5]);

    close(sockfd);

    return 0;
}
#endif

int main(int argc, const char *argv[])
{
    char ip[36] = {0};
    char mac[36] = {0};

    #if 1
    char hostname[256];
    struct hostent *get_host_by_name;

    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        printf("Can not get hostname\n");
        exit(1);
    }
    get_host_by_name = gethostbyname(hostname);
    if ((strcpy(ip, (char *)inet_ntoa(*(struct in_addr*)(get_host_by_name->h_addr_list[0])))) == NULL)
    {
        printf("Can not get loacl ip\n");
        exit(1);
    }
    else
    {
        printf("hostname: %s\n",hostname);
        printf("local ip: %s\n",ip);
    }
    #endif

    if (get_local_ip(ip, mac) == 0) 
    //if (get_local_ip(ip, NULL) == 0) 
    //if (get_local_ip(NULL, mac) == 0) 
    {
        printf("local ip: %s\n",ip);
        printf("local mac: %s\n",mac);
    }
    else
    {
        printf("Can not get loacl ip\n");
    }

    return 0;
}
