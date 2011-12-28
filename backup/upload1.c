#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>

#define ACHEAD "ooooooooooooo"
#define AC483PATH "/var/httpd/htdocs/upload-folder/file"

#define CONTENT_TYPE getenv("CONTENT_TYPE")

void fit(char *string, unsigned size)
{
    if (strlen(string) > size) 
    {
        *(string + size) = '\0';
    }
}

int main(int argc, const char *argv[])
{
    printf("Content-Type:text/html\n\n");
    char *pMethod = getenv("REQUEST_METHOD");
    if (pMethod == NULL || *pMethod == 0) 
    {
        printf("No any method");
        return 0;
    }

    if (strcmp(pMethod, "GET") == 0) 
    {
        return 1;
    }
    if (strcmp(pMethod, "POST") == 0) 
    {
        char *pCntLen = getenv("CONTENT_LENGTH");
        if (!pCntLen) 
        {
            printf("Can't get Content_Length\n");
            return 0;
        }
        if (*pCntLen == 0) 
        {
            printf("Can't get Content_Length\n");
            return 0;
        }
        int StrLen = atoi(pCntLen);
        if (StrLen <= 0) 
        {
            printf("String Length <= 0\n");
            return 0;
        }
        char *readstr = (char *)malloc(StrLen + 1);
        fread(readstr, StrLen, 1, stdin);
        int n = 0;
        int firstLineMark = 0;
        int firstLineCount = 0;
        int headCount = 0;
        while (n < 4) 
        {
            if (*(readstr++) == '\n') 
            {
                firstLineMark++;
                n++;
            }
            if (firstLineMark == 0) 
            {
                firstLineCount++;
            }
            headCount++;
        }
        StrLen = StrLen-headCount;
        fit(readstr, 13);
        #if 0
        if (strcmp(readstr, ACHEAD) != 0) 
        {
            printf("< SCRIPT language=JavaScript>alert('file error !');javajcript:history.go(-1)</SCRIPT>\n");
            return 0;
        }
        #endif
        FILE *fp;
        //if ((fp = fopen(AC483PATH, "w+")) == NULL) 
        if ((fp = fopen("./upload-folder/file", "w+")) == NULL) 
        {
            printf("open file error !\n");
            return 0;
        }
        fwrite(readstr + 13, StrLen-13-firstLineCount-5, 1, fp);
        fclose(fp);
        printf("< SCRIPT language=JavaScript>alert('upload successfully !');javajcript:history.go(-1)</SCRIPT>\n");
    }

    return 0;
}
