#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

int rscallback(void *p, int argc, char **argv, char **argvv)
{
    int i;
    *(int *)p = 0;

    for (i = 0; i < argc; i++) 
    {
        printf("%-2s is %-10s  ",argvv[i], argv[i]?argv[i]:"NULL");
    }
    putchar('\n');

    return 0;
}

int main(int argc, const char *argv[])
{
    sqlite3 *db;
    char *err = 0;
    int ret = 0;
	char buf[4096];
	char value[4096];
    char *p = NULL;
    char *q = NULL;
    char *username = NULL;
    char *password= NULL;
    int len = 0;

	printf("Content-Type: text/plain\n\n");
    while((ret = fread(buf, 1, sizeof(buf), stdin)) != 0)
    {
        //printf("read %d len\n", ret);
        printf("%s", buf);
    }
    p = strchr(buf, '=');
    p++;
    q = strchr(buf, '&');
    if (p == q) 
    {
        printf("\nBlank username !\n");
        return 0;
    }
    *q = '\0';
    if (strchr(p, '+') != NULL)
    {
        printf("\nUsername can not have blank space !\n");
        return 0;
    }
    if ((len = strlen(p)) > 20)
    {
        printf("\nUsername is overlongth !\n");
        return 0;
    }
    username = p;
    q += 10;
    if (*q == '\0' || *q == '&') 
    {
        printf("\nBlank password !\n");
        return 0;
    }
    p = strstr(q, "&reg=Register");
    *p = '\0';
    if ((len = strlen(q)) > 20)
    {
        printf("\nPassword is overlongth !\n");
        return 0;
    }
    password = q;

    ret = sqlite3_open("./register.db", &db);
    if (ret != SQLITE_OK) 
    {
        fputs(sqlite3_errmsg(db), stderr);
        fputs("\n", stderr);
        exit(1);
    }
    sprintf(value, "insert into register_info(username, password) values('%s', '%s');", username, password);
    ret = sqlite3_exec(db, value, NULL, NULL, &err);
    if (ret != SQLITE_OK) 
    {
        fputs(err, stderr);
        fputs("\n", stderr);
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_close(db);
    printf("\nRegister successfully !\n");

    return 0;
}

