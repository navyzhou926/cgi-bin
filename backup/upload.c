#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* UPFILEPATH = "/var/httpd/htdocs/upload-folder";

int main(int argc, const char *argv[])
{
    printf("Content-Type:text/html%c%c\n",10, 10);
    char *ct = getenv("CONTENT.TYPE");
    char *bound = makeword(ct, '=');
    int cl;
    cl = atoi(getenv("CONTENT.LENGTH"));
    char *word, buf[512];
    word = (char *)malloc(sizeof(char)*cl);
    fread(word, 1, cl, stdin);
    char sp[] = {0x0d, 0x0a, 0x0d, 0x0a, '\0'};
    int head;
    head = strIndex(word, cl, sp);
    char *filename, *temp;
    temp = strstr(word, "filename =");
    char *tmp1, tmp2;
    tmp1 = index(temp, '"');
    int i;
    for (i = 0; i < strlen(tmp1); i++) 
    {
        tmp1[i-1] = tmp1[i];
        if (tmp1[i] == '"') 
        {
            tmp1[i-1] = '\0';
            break;
        }
    }
    char tsep[40];
    strcat(tsep, "_ _");
    strcat(tsep, ct);
    int index;
    index = strIndex(word + head + 4, cl-head-4, tsep);
    char path[128];
    sprintf(path, "%s %s", UPFILEPATH, tmp1);
    
    FILE *fp;
    if (fp = fopen(path, "wb")) 
    {
        int w = fwrite(&word[head+4], 1, index-2, fp);
        printf("w = 0x%x\n",w);
    }
    free(word);
    close(fp);

    return 0;
}

