#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <pwd.h>

char dir_upload[256];
#define USER_NAME get_username()
#define USER_DIR  get_userdir()

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

uid_t get_userid(void)
{
    return getuid();
}

char * get_username(void)
{
    uid_t userid;
    struct passwd *pwd;

    userid = getuid();
    pwd = getpwuid(userid);
    
    return pwd->pw_name;
}

char * get_userdir(void)
{
    uid_t userid;
    struct passwd *pwd;

    userid = getuid();
    pwd = getpwuid(userid);
    
    return pwd->pw_dir;
}

char * get_upload_folder(void)
{
    sprintf(dir_upload,"%s/upload-folder",get_userdir());
    return dir_upload;
}

