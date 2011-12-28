#include <stdio.h>
#include <dirent.h>
#include "global.h"

int main(int argc, const char *argv[])
{
	printf("Content-Type: text/html\n\n");

    printf("<html>\n");
    printf("<head>\n");
    printf("<title>sync</title>\n");
    printf("</head>\n");
    printf("<body bgcolor=\"#48d1cc\">\n");
    printf("<form name=\"input\" action='./syncadm.cgi' method=POST>\n");
    printf("<p align=center>\n");
    printf("<font size=3 color=\"#ffff00\">\n");
    printf("<br><br><br>\n");
    printf("&nbsp&nbsp&nbsp\n");
    printf("ADM:\n");
    printf("<select name=\"choose\">\n");
    printf("<option value=\"Please choose\" selected=\"selected\">请选择设备</option>\n");
    printf("<option value=\"all\">ALL AD Machine</option>\n");
    printf("<option value=\"ADM1\">AD Machine 1</option>\n");
    printf("<option value=\"ADM2\">AD Machine 2</option>\n");
    printf("</select>\n");
    printf("<input type=\"submit\" value=\"Update\"/>\n");
    printf("</font>\n");
    printf("</p>\n");
    printf("</form>\n");
    printf("<form name=\"input\" action='./sync.cgi' method=POST>\n");
    printf("<font size=3 color=\"#ffff00\">\n");
    printf("<p align=\"center\"> <br><br><br>&nbsp&nbsp\n");
    printf("IP:\n");
    printf("<input type=\"text\" name=\"address\" /><br/><br>\n");
    printf("Port:\n");
    printf("<input type=\"text\" name=\"port\" /><br/><br>\n");
    printf("&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp\n");
    printf("&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp\n");
    printf("&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp\n");
    printf("&nbsp&nbsp\n");
    printf("<input type=\"submit\" value=\"Update\"/>\n");
    printf("</p>\n");
    printf("</font>\n");
    printf("</form>\n");
    printf("</body>\n");
    printf("</html>\n");

    return 0;
}

