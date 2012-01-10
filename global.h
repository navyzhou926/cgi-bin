#ifndef _GLOABL_

#include <unistd.h>

//#define DEBUG

#ifdef DEBUG
#define printf_debug(fmt, arg...)  printf(fmt, ##arg)
#else
#define printf_debug(fmt,arg...) do{ } while (0)
#endif

extern char dir_upload[256];
extern char * get_username(void);
extern char * get_userdir(void);
extern char * get_upload_folder(void);
extern int send_html(int count_br, int count_nbsp,  char *str);
extern int get_local_ip(char *out_ip, char *out_mac);

//#define DIR_UPLOAD "/home/navyzhou/upload-folder"
#define DIR_UPLOAD get_upload_folder()
//#define DIR_UPLOAD getlogin()

#define SERVER_PORT	    8000	
#define SERVER_PORT_TCP	    21203	


#endif
