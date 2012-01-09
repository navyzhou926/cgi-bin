#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "global.h"

#define META_FILE_TYPE " name=\""
#define META_FILE_NAME " filename=\""
#define META_CONTENT_TYPE "multipart/form-data; boundary="

#define FILE_TYPE_PIC "PictureFile"
#define FILE_TYPE_MEDIA "MediaFile"
#define FILE_TYPE_TEXT "TextFile"

#define DEBUG_RETURN return 0
#define DEBUG_PRINTF printf

void output_err(const char* error)
{
    //printf("Content-Type: text/html\n\n");
    printf("<h2>%s<h2><br>\n", error);
}

int main(int argc, char* argv[])
{
    char* env_ret = getenv("REQUEST_METHOD");
    int content_length;
    char* read_buf;
    char* bound;
    char file_type[32];
    char file_name[256];
    int file_len;
    int read_len;
    char* fc_pos;
    char *path = (char*)malloc(256);

    printf("Content-Type: text/html\n\n");

    if(env_ret == NULL || *env_ret == 0)
    {
        output_err("method is null");
        return 0;
    }

    if(strcmp(env_ret, "POST") != 0)
    {
        output_err("method is not post");
        return 0;
    }

    env_ret = getenv("CONTENT_TYPE");
    if(strncmp(env_ret, META_CONTENT_TYPE, strlen(META_CONTENT_TYPE)) != 0)
    {
        output_err("type is error");
        return 0;
    }
    bound = env_ret + strlen(META_CONTENT_TYPE);

    env_ret = getenv("CONTENT_LENGTH");
    if(env_ret == NULL)
    {
        output_err("no content length");
        return 0;
    }

    content_length = atoi(env_ret);
    if(content_length <= 0)
    {
        output_err("content length <=0");
        return 0;
    }

    file_len = content_length;

    read_buf = (char*)malloc(1024);

    file_len -= strlen(bound);
    file_len -= 8; // why 8?

    read_len = fread(read_buf, 1, 1024, stdin);
    fc_pos = read_buf;
    fc_pos = strstr(fc_pos, "\r\n");
    fc_pos = strstr(fc_pos+1, "\r\n");
    fc_pos = strstr(fc_pos+1, "\r\n");
    *fc_pos = 0;
    fc_pos += 4;
    file_len -= (fc_pos - read_buf);


    //while(1)
    {
        char* b; // begin
        char* e; // end;
        //fgets(read_buf, 1024, stdin);
        //read_len = strlen(read_buf);
        //file_len -= read_len;

        b = strstr(read_buf, META_FILE_TYPE);
        if(b)
        {
            b += strlen(META_FILE_TYPE);
            e = strstr(b, "\"");
            *e = 0;
            strcpy(file_type, b);

            b = strstr(e+1, META_FILE_NAME);
            b += strlen(META_FILE_NAME);
            e = strstr(b, "\"");
            *e = 0;
            strcpy(file_name, b);
        }
        //if(read_len == 2)
        //    break;
    }

    {
        // dir is exist?
        int iret;
        strcpy(path, DIR_UPLOAD);
        iret = access(DIR_UPLOAD, W_OK);
        if(iret != 0)
        {
            output_err("upload-folder not exist or not privileged");
            free(path);
            free(read_buf);
            return 0;
        }

#if 0  //Whether create File_Path(PictureFile) or not
        strcat(path, "/");
        strcat(path, file_type);
        iret = access(path, W_OK);
        if(iret != 0)
        {
            iret = mkdir(path, 0777);
            if(iret != 0)
            {
                output_err("can not create dir");
                printf("%s\n", path);
                free(path);
                free(read_buf);
                return 0;
            }
        }
#endif

        {
            FILE* fd;
            int remain;
            int i = 0;
            char *pp = NULL;
            strcat(path, "/");
            for (i = 0; *(file_name + i) != '\0'; i++) 
            {
                if (*(file_name + i) == 92)
                    pp = file_name + i;
            }
            if (pp != NULL) 
                strcat(path, pp + 1);
            else
                strcat(path, file_name);

            fd = fopen(path, "w+");
            if(!fd)
            {
                output_err("can not create file");
                output_err(path);
                free(path);
                free(read_buf);
                return 0;
            }

            remain = 1024 - (fc_pos - read_buf);

            if(remain < file_len)
            {
                fwrite(fc_pos, remain, 1, fd);
                file_len -= remain;
                while(file_len > 0)
                {
                    read_len = fread(read_buf, 1, 1024, stdin);
                    if(file_len < read_len)
                    {
                        read_len = file_len;
                    }
                    fwrite(read_buf, read_len, 1, fd);
                    file_len -= read_len;
                }
            }
            else
            {
                /* small file, just ok */
                fwrite(fc_pos, file_len, 1, fd);
            }
            fclose(fd);
        }
    }
    free(path);
    free(read_buf);
    output_err("Succeed to upload file");

    char ip[36] = {0};
    if (get_local_ip(ip, NULL) == 0) 
    {
        //printf("<h2>local ip: %s<h2><br>\n",ip);
        //printf("<h2>local mac: %s<h2><br>\n",mac);
        printf("<meta http-equiv=\"refresh\"content=\"1; url=http://%s/upload.html\">", ip);
    }
    else
    {
        printf("<h2>Server wrong, can not get loacl ip !<h2><br>\n");
    }


    return 0;
}

