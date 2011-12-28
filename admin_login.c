#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sqlite3.h>
#include "global.h"

struct string
{
    char *username;
    char *password;
}str = {NULL, NULL};

int rscallback_register(void *p, int argc, char **argv, char **argvv)
{
    *(int *)p = 1;
/*
        if (strcmp(argv[1], str.username) == 0) 
        {
            *(int *)p = 1;
            return 0;
        }
        */
    return 0;
}

int rscallback_login(void *p, int argc, char **argv, char **argvv)
{
    *(int *)p = 1;
/*
        if (strcmp(argv[1], str.username) == 0) 
        {
            if (strcmp( argv[2] ,str.password) == 0) 
            {
                *(int *)p = 1;
                return 0;
            }
        }
        */
    return 0;
}

int out_html(char *file)
{
    FILE *fp;
    char str[1024];

    //chdir("/var/httpd/htdocs/");

    fp = fopen(file, "r");
    if (fp == NULL) 
    {
        printf("file open error !\n");
        exit(1);
    }

    while (1)
    {
        fgets(str, sizeof(str), fp);
        if (feof(fp)) 
        {
            break;
        }
        printf("%s",str);    
    }

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
    int len = 0;
    int reg = 0;
    int log = 0;
    int login_flag = 0;
    char *confirm;

	printf("Content-Type: text/html\n\n");
    while((ret = fread(buf, 1, sizeof(buf), stdin)) != 0)
    {
        //printf("read %d len\n", ret);
        //printf("%s", buf);
    }

    if ((p = strstr(buf, "=Register")) != NULL)
    {
        reg = 1;
        p -= 4;
        *p = '\0';
        q = strstr(buf, "&confirm=");
        *q = '\0';
        confirm = q + 9;
    }
    if ((p = strstr(buf, "&log=Login")) != NULL)
    {
        log = 1;
        *p = '\0';
    }
    //username=sd&password=sdd&confirm=dsssd®=Register
    p = strchr(buf, '=');
    p++;
    q = strchr(buf, '&');
    if (p == q) 
    {
        send_html(5, 32, "Blank username!");
        if (reg == 1) 
        {
            printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/register.html\">");
        }
        if (log == 1) 
        {
            printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/admin_login.html\">");
        }
        return 0;
    }
    *q = '\0';
    //username=ab&password=ab&confirm=ab®=Register
    if (strchr(p, '+') != NULL)
    {
        send_html(5, 15, "Username can not have blank space !");
        if (reg == 1) 
        {
            printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/register.html\">");
        }
        if (log == 1) 
        {
            printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/admin_login.html\">");
        }
        return 0;
    }
    if ((len = strlen(p)) > 20)
    {
        send_html(5, 23, "Username is overlength !");
        if (reg == 1) 
        {
            printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/register.html\">");
        }
        if (log == 1) 
        {
            printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/admin_login.html\">");
        }
        return 0;
    }
    str.username = p;
    q += 10;
    if (*q == '\0' || *q == '&') 
    {
        send_html(5, 32, "Blank password !");
        if (reg == 1) 
        {
            printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/register.html\">");
        }
        if (log == 1) 
        {
            printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/admin_login.html\">");
        }
        return 0;
    }
    /*
    if ((p = strstr(q, "&reg=Register")) != NULL)
    {
        reg = 1;
        *p = '\0';
    }
    if ((p = strstr(q, "&log=Login")) != NULL)
    {
        log = 1;
        *p = '\0';
    }
    */
    if ((len = strlen(q)) > 20)
    {
        printf("\n%s\n",q);
        send_html(5, 23, "Password is overlength !");
        if (reg == 1) 
        {
            printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/register.html\">");
        }
        if (log == 1) 
        {
            printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/admin_login.html\">");
        }
        return 0;
    }
    str.password = q;
    #if 1
    if (reg == 1) 
    {
        if (strcmp(str.password, confirm) != 0)
        {
            send_html(5, 21, "Confirm password not match !");
            printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/register.html\">");
            return 0;
        }
    }
    #endif

    ret = sqlite3_open("./admin_login.db", &db);
    if (ret != SQLITE_OK) 
    {
        fputs(sqlite3_errmsg(db), stderr);
        fputs("\n", stderr);
        exit(1);
    }
    if (reg == 1) 
    {
        //sprintf(value, "select username='%s', username, password from register_info;", str.username);
        sprintf(value, "select * from register_info where username='%s';", str.username);
        //ret = sqlite3_exec(db, "select * from register_info", rscallback_register, &login_flag, &err);
        ret = sqlite3_exec(db, value, rscallback_register, &login_flag, &err);
        if (login_flag == 1) 
        {
            sqlite3_close(db);
            send_html(5, 26, "Reduplicated username !");
            send_html(1, 18, "Please input your username again !");
            printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/register.html\">");
            return 0;
        }
        sprintf(value, "insert into register_info(username, password) values('%s', '%s');", str.username, str.password);
        ret = sqlite3_exec(db, value, NULL, NULL, &err);
    }
    if (log == 1) 
    {
        //sprintf(value, "select username='%s', username, password from register_info;", str.username);
        sprintf(value, "select * from register_info where username='%s' and password='%s';", str.username, str.password);
        ret = sqlite3_exec(db, value, rscallback_login, &login_flag, &err);
        //ret = sqlite3_exec(db, "select * from register_info", rscallback_login, &login_flag, &err);
    }
    if (ret != SQLITE_OK) 
    {
        fputs(err, stderr);
        fputs("\n", stderr);
        sqlite3_close(db);
        if (reg == 1) 
        {
            printf("\nRegister failure !\n");
        }
        if (log == 1) 
        {
            printf("\nLogin failure !\n");
        }
        exit(1);
    }
    sqlite3_close(db);
    if (reg == 1) 
    {
        //out_html("/var/httpd/htdocs/admin_login.html");
        send_html(5, 48, "Register succesfully !");
        send_html(1, 32, "Turning into administration system...");
        printf("<meta http-equiv=\"refresh\"content=\"1; url=http://192.168.11.252/administration.html\">");
    }
    if (log == 1) 
    {
        if (login_flag == 1) 
        {
            //out_html("/var/httpd/htdocs/admin_login.html");
            send_html(5, 50, "Login succesfully !");
            send_html(1, 32, "Turning into administration system...");
            printf("<meta http-equiv=\"refresh\"content=\"1; url=http://192.168.11.252/administration.html\">");
        }
        else
        {
            send_html(5, 4, "Username or password wrong, please input again !");
            printf("<meta http-equiv=\"refresh\"content=\"2; url=http://192.168.11.252/admin_login.html\">");
        }
    }

    return 0;
}

