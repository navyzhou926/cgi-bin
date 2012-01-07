#ifndef _GLOABL_

//#define DEBUG

#ifdef DEBUG
#define printf_debug(fmt, arg...)  printf(fmt, ##arg)
#else
#define printf_debug(fmt,arg...) do{ } while (0)
#endif

#define DIR_UPLOAD "/home/navyzhou/upload-folder"

#define SERVER_PORT	    8000	
#define SERVER_PORT_TCP	    21203	

extern int send_html(int count_br, int count_nbsp,  char *str);
extern int get_local_ip(char *out_ip, char *out_mac);

#endif
