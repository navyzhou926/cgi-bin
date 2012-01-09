#include <stdio.h>
#include <stdlib.h>

extern char** environ;
 
int main()
{
	char buf[4096];
	int ret;
    int i;
    FILE *fd;

	printf("Content-Type: text/plain\n\n");

    //for(var = environ; *var!= NULL; ++var)
    //printf("%s\n", *var);
	
    //ret = fread(buf, 1, 8, stdin);
    fd = fopen("./comment.txt", "w");
    if(!fd)
    {
        printf("Cannot open file\n");
        return 0;
    }
    while((ret = fread(buf, 1, sizeof(buf), stdin)) != 0)
    {
        //printf("read %d len\n", ret);
        for (i = 0; buf[i] != '\0'; i++) 
        {
            if (*(buf + i) == '+') 
            {
                *(buf + i) = ' ';
                printf(" ");
            }
            else
                printf("%c",*(buf + i));
        }
        //printf("%s", buf);
        fwrite(buf, ret, 1, fd);
    }
    fclose(fd);
	
	return 0;
}


