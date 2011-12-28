#gcc admin_login.c global.c -Wall -o admin_login.cgi -lsqlite3
#gcc comment.c global.c Wall -o comment.cgi
#gcc delete.c global.c -Wall -o delete.cgi
#gcc delete_file.c global.c -Wall -o delete_file.cgi
#gcc login.c global.c -Wall -o login.cgi -lsqlite3
#gcc register.c global.c -Wall-o register.cgi -lsqlite3
#gcc register_login.c global.c -Wall -o register_login.cgi -lsqlite3
#gcc start.c global.c -Wall -o start.cgi -lsqlite3
#gcc stop.c global.c -Wall -o stop.cgi -lsqlite3
gcc syncadm.c global.c -Wall -o syncadm.cgi -lpthread
gcc sync.c global.c -Wall -o sync.cgi  -lpthread
gcc update_data.c global.c -Wall -o update_data.cgi -lpthread 
#gcc test.c global.c -Wall -o test.cgi
#gcc upload.c global.c -Wall -o upload.cgi
#gcc upload_file.c global.c -Wall -o upload_file.cgi
