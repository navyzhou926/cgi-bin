#include <stdio.h>
#include <dirent.h>
#include "global.h"

int main(int argc, const char *argv[])
{
    DIR *dir;
    struct dirent *ptr;

	printf("Content-Type: text/html\n\n");

    printf("<html>\n");
    printf("<head>\n");
    printf("<title>delete</title>\n");
    printf("</head>\n");
    printf("<body bgcolor=\"#48d1cc\">\n");
    printf("<form name=\"input\" action='./delete.cgi' method=POST>\n");
    printf("<p align=center>\n");
    printf("<font size=3 color=\"#ffff00\">\n");
    printf("<br><br><br><br><br> <br><br><br>\n");  
    printf("File:\n");
    printf("<select name=\"DeleteFile\">\n");
    //printf("<option value=\"choose\" selected=\"selected\">Please choose</option>\n");
    printf("<option value=\"请选择文件\" selected=\"selected\">请选择文件</option>\n");
    //printf("<option value=\"choose\" selected=\"selected\"></option>\n");

    if ((dir = opendir(DIR_UPLOAD)) != NULL) 
    {
        while ((ptr = readdir(dir)) != NULL)
        {
            if (ptr->d_type == DT_REG) 
            {
                //sprintf(buffer, "%s/%s",DIR_UPLOAD, ptr->d_name);
                printf("<option value=%s>%s</option>\n",ptr->d_name, ptr->d_name);
            }
        }
    }

    printf("</select>\n");
    printf("&nbsp<input type=\"submit\" value=\"Delete\" name=\"delete\"/>&nbsp\n");
    printf("<input type=\"submit\" value=\"Delete All\" name=\"deleteall\"/>\n");
    printf("</font>\n");
    printf("</p>\n");
    printf("</form>\n");
    printf("</body>\n");
    printf("</html>\n");

    return 0;
}

