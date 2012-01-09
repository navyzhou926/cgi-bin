#include <stdio.h>
#include <dirent.h>
#include "global.h"

int main(int argc, const char *argv[])
{
    printf("Content-Type: text/html\n\n");
    //printf("Content-Type: image/gif\n\n");

    printf("<html>\n");
    //printf("<meta http-equiv=\"Content-Type content=\"text/html;charset=utf-8\"\">");
    printf("<body bgcolor=\"#ffc0cb\">\n");
    printf("<head> <title>upload file</title> </head>\n");
    printf("<font size=3 color=\"#9932cc\">\n");
    printf("<h1><blink><p align=center>Please to upload your files !</blink></h1>\n");
    printf("</font>\n");
    printf("<br> <br>\n");
    printf("<center>\n");
    printf("<table>\n");
    printf("<tr>\n");
    printf("<form action=\"../cgi-bin/upload.cgi\" method=\"post\" enctype=\"multipart/form-data\">\n");
    printf("<td> <p align=\"right\">File Path:</p> </td>\n");
    printf("<td> <input type=\"file\" name=\"File_Path\" > </td>\n");
    printf("<td align=\"right\"> <input type=\"submit\" value=\"Upload\"> </td> </form> </tr>\n");
    printf("</table>\n");

    //printf("<img id=\"imgframe\" src=\"/usr/local/apache2/htdocs/images/4.gif\" alt=\"happy\" align=middle border=\"0\">\n");
    //printf("<img src=\"../htdocs/images/4.gif\" alt=\"happy\" align=middle border=\"0\">\n");
    //printf("<br><iframe src=\"../htdocs/images/5.jpg\"></iframe>\n");
    //printf("<iframe src=\"../htdocs/images/4.jpg\"></iframe>\n");
    //printf("<br><iframe src=\"../htdocs/images/3.jpg\"></iframe>\n");
    //printf("<iframe src=\"../htdocs/images/1.jpg\"></iframe>\n");
    printf("</center>\n");
    printf("</body>\n");
    printf("</html>\n");

    return 0;
}

