int flag;
flag = fcntl(sock,F_GETFL,0);
flag |= O_NONBLOCK;
fcntl(sock,F_SETFL,flag);//设置非阻塞
if (type == SOCK_STREAM) 
{
    int ret = -1;
    connected = connect(sock, (struct sockaddr *) &address, sizeof(address));
    if (connected != 0) 
    {
        if(errno != EINPROGRESS)
            logdown(ERROR,"connect error :%s\n",strerror(errno));
        else //esle
        {
            struct timeval tm;
            tm.tv_sec = 3;
            tm.tv_usec = 0;
            fd_set set,rset;
            FD_ZERO(&set);
            FD_ZERO(&rset);
            FD_SET(sock,&set);
            FD_SET(sock,&rset);
            socklen_t len;
            int error = -1;
            int res;
            res = select(sock+1,&rset,&set,NULL,&tm);
            if(res < 0)
            {
                logdown(WARNNING,"network error in connect\n");
                ret =  -1;
            }
            else if(res == 0)
            {
                logdown(WARNNING,"connect time out\n");
                ret = -2;
            }
            else if (1 == res)
            {
                if(FD_ISSET(sock,&set))
                {
                    flag &= ~O_NONBLOCK;
                    fcntl(sock,F_SETFL,flag);//设置阻塞
                    return sock;
                }
                else
                {
                    ret = -3;
                    logdown(WARNNING,"other error when select:%s\n",strerror(errno));
            }
            return ret;
        }
    }
    return sock;
}
