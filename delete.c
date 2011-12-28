#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include "global.h"


int delete_all_file(void)
{
    DIR *dir;
    struct dirent *ptr;
    char buffer[1024];
    int iret = 0;
    int i = 0;

    if ((dir = opendir(DIR_UPLOAD)) != NULL) 
    {
        while ((ptr = readdir(dir)) != NULL)
        {
            if (ptr->d_type == DT_REG) 
            {
                sprintf(buffer, "%s/%s",DIR_UPLOAD, ptr->d_name);
                if ((iret = remove(buffer)) < 0)
                {
                    printf("<h2>Failed to delete %s <h2><br>",ptr->d_name);
                }
                else
                {
                    printf("<h2>Succeed to delete %s<h2>",ptr->d_name);
                    i++;
                }
            }
        }
        if (i == 0) 
        {
            printf("<h2>Nothing to delete !<h2><br>");
        }
    }

    return 0;
}

int main(int argc, const char *argv[])
{
    int ret = 0;
	char data[4096];
    char *p = NULL;
    char *delete_file_name = NULL;
    int iret = 0;
    char buffer[1024];

	printf("Content-Type: text/html\n\n");
    while ((ret = fread(data, sizeof(data), 1, stdin)) != 0)
    {
        //return 0;
    }
    //printf("read len = %d <br>\n", ret);
    //printf("%s<br>\n", data);

    if ((strstr(data, "deleteall") != NULL) && (strncmp(data + 11, "deleteall", 9) != 0)) 
    {
        delete_all_file();
    }
    else
    {
        delete_file_name= data + 11;
        p = strchr(delete_file_name, '&');
        *p = '\0';
                                     //请选择文件
        if (strcmp(delete_file_name, "%E8%AF%B7%E9%80%89%E6%8B%A9%E6%96%87%E4%BB%B6") == 0) 
        {
            printf("<h2>请选择要删除的文件<h2><br>\n");
            //printf("<meta http-equiv=\"refresh\"content=\"1; url=http://192.168.11.252/delete.html\">");
            exit(1);
        }

        sprintf(buffer, "%s/%s",DIR_UPLOAD, delete_file_name);
        if ((iret = remove(buffer)) < 0)
            printf("<h2>Failed to delete %s <h2><br>",delete_file_name);
        else
            printf("<h2>Succeed to delete %s<h2><br>",delete_file_name);
    }
    //printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/delete.html\">");

    return 0;
}

