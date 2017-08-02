#include "socket.h"
#include "os_syscall.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <memory.h>
#include <sys/stat.h>
#include <sys/types.h>

/* getaddrinfo, getnameinfo(���gethostbyname, gethostbyaddr) linux: include<netdb.h> windows: include<ws2tcpip.h> */
/* gethostbyname ���̵߳��ò���ȫ */
/* EAGAIN: linux; EWOULDBLOCK: Vxwork��Windwos */

#define     NET_NOBLOCK_RETRY_CNT      (10)     /* ����ʱ����� */
#define     NET_BLOCK_RETRY_CNT      (10)
#define     LISTENQ                 (1024)

/* timeout ms */
int set_sock_attr(int fd, int reuseaddr_flag, int send_timeout, 
						int recv_timeout, int send_size, int recv_size)
{ 
    struct timeval snd_tv, rcv_tv;

	if (fd < 0)
		return -1;
	
    snd_tv.tv_sec  = send_timeout / 1000;
    snd_tv.tv_usec = (send_timeout % 1000) * 1000;
	
    rcv_tv.tv_sec  = recv_timeout / 1000;
    rcv_tv.tv_usec = (recv_timeout % 1000) * 1000;

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuseaddr_flag, sizeof(int)) < 0)
        goto ERROR_OUT;
#ifdef WIN32
    if (send_timeout != 0)
    {
        if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (const char *)&send_timeout, sizeof(snd_tv)) < 0)
            goto ERROR_OUT;
    }
    if (recv_timeout != 0)
    {
        if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&recv_timeout, sizeof(rcv_tv)) < 0)
            goto ERROR_OUT;	
    }
#else
    if (send_timeout != 0)
    {
        if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (const char *)&snd_tv, sizeof(snd_tv)) < 0)
            goto ERROR_OUT;
    }  
    if (recv_timeout != 0)
    {    
        if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&rcv_tv, sizeof(rcv_tv)) < 0)
            goto ERROR_OUT;	
    }
#endif
    if (send_size != 0)
    {  
        if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (void *)&send_size, sizeof(send_size)) < 0)
            goto ERROR_OUT;
    }
    if (recv_size != 0)
    {      
        if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (void *)&recv_size, sizeof(recv_size)) < 0)
            goto ERROR_OUT;
    }

	return 0;

ERROR_OUT:	
    return -1;
}

int set_sock_recvtimeout(int fd, int timeout_ms)
{
    struct timeval rcv_tv;

    rcv_tv.tv_sec  = timeout_ms / 1000;
    rcv_tv.tv_usec = (timeout_ms % 1000) * 1000;

    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (void *)&rcv_tv, sizeof(rcv_tv)) < 0)
        return -1;

    return 0;
}

int set_sock_reuse(int fd, int reuse)
{
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse, sizeof(int)) < 0)
        return -1;

    return 0;
}

void close_socket(int *socket)
{
    if (*socket != -1)
    {
		closesocket(*socket);
        *socket = -1;
    }
}

int get_sock_ip(int sock)
{
	struct sockaddr_in sockAddr;
	int addrLen = sizeof(struct sockaddr);
	
	if (0 != getsockname(sock, (struct sockaddr *)&sockAddr, (socklen_t *)&addrLen))
		return -1;
	return sockAddr.sin_addr.s_addr;
}

int get_sock_port(int sock)
{
	struct sockaddr_in sockAddr;
	int addrLen = sizeof(struct sockaddr);
	
	if (0 != getsockname(sock, (struct sockaddr *)&sockAddr, (socklen_t *)&addrLen))
		return -1;
	return htons(sockAddr.sin_port);
}

int tcp_create_and_listen(int port)
{
    int      listenfd;
    int     reuseaddr_flag = 1;
	struct sockaddr_in native_addr;

	memset(&native_addr, 0, sizeof(native_addr));
	native_addr.sin_family = AF_INET;
	native_addr.sin_port = htons(port);
#if 1
	native_addr.sin_addr.s_addr = htonl(INADDR_ANY);
#else       /* ����д�� */
    native_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
#endif

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0)
	{
        perror("socket");        
		return -1;
    }
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuseaddr_flag, sizeof(int)) < 0)
    {
		close(listenfd);
        perror("setsockopt");
		return -1;
    }
	if (bind(listenfd, (struct sockaddr *)&native_addr, sizeof(struct sockaddr)) < 0)
	{
		close_socket(&listenfd);
		return -1;
	}
	if (listen(listenfd, 1024) < 0)
	{
        perror("listen");                
		close(listenfd);
		return -1;
    }

	return listenfd;
}
    
int create_sock(int type)
{
	int fd = socket(AF_INET , type, 0);
    
	return fd;
}

int bind_sock(int sockfd, int ip, int nPort)
{
	struct sockaddr_in addr_local;
	memset(&addr_local, 0, sizeof(struct sockaddr_in));

	addr_local.sin_family      = AF_INET;
	addr_local.sin_addr.s_addr = htonl(ip);
	addr_local.sin_port        = htons(nPort);	
	
	set_sock_attr(sockfd, 1, 0, 0, 0, 0);  //set the sock reuser_addr attribute
	if (bind(sockfd,(struct sockaddr*)&addr_local, sizeof(struct sockaddr_in)) < 0)
		return -1;
    
	return 0;
}

int sock_set_linger(int sockfd)
{
	struct linger nLinger;

	if (sockfd <= 0)
		return -1;
	
	memset(&nLinger, 0, sizeof(struct linger));
	//���ַ�ʽ�£��ڵ���closesocket��ʱ��ͬ�������̷��أ������ᷢ��δ������ɵ����ݣ�
	//����ͨ��һ��REST��ǿ�ƵĹر�socket��������Ҳ����ǿ�Ƶ��˳���
	nLinger.l_onoff = 1;
	nLinger.l_linger= 0;

	if (setsockopt(sockfd, SOL_SOCKET, SO_LINGER, (const char *)&nLinger, sizeof(struct linger)) < 0)
    		return -1;
    
	return 0;
}

int set_sock_nodelay(int fd)
{
	int opt = 1;	

	if (fd <= 0)
		return -1;
    
	return setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *)&opt, sizeof(opt));
}

int set_sock_keepalive(int fd)
{
	int opt = 1;	

	if (fd <= 0)
		return -1;
	return setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE,(char *)&opt,sizeof(opt));
}

int my_select(int *fd_array, int fd_num, int fd_type, int time_out)
{
	int maxfd = 0;
	int index = 0;
	int ret   = 0;
	
	fd_set read_fd, *p_read_fd;
	fd_set write_fd, *p_write_fd;
	fd_set except_fd, *p_except_fd;
	struct timeval timeO, *p_time_out;

	if (fd_array == NULL || fd_num < 0)
		return -1;
	p_time_out = NULL;
	if (time_out > 0)
	{
    	timeO.tv_sec = time_out / 1000;
    	timeO.tv_usec= (time_out % 1000)*1000;	
		p_time_out = &timeO;
	}

	p_read_fd = p_write_fd = p_except_fd = NULL;
	
	if (fd_type & 0x1)
	{
		p_read_fd = &read_fd;
		FD_ZERO(p_read_fd);
	}
	
	if (fd_type & 0x2)
	{
		p_write_fd = &write_fd;
		FD_ZERO(p_write_fd);
	}
	
	if (fd_type & 0x4)
	{
		p_except_fd = &except_fd;
		FD_ZERO(p_except_fd);
	}
	
	for (index = 0; index < fd_num; ++index)
	{
		if(fd_array[index] <= 0)
			continue;
		maxfd = maxfd > fd_array[index] ? maxfd : fd_array[index];

		if (p_read_fd)
			FD_SET(fd_array[index], p_read_fd);
		if (p_write_fd)
			FD_SET(fd_array[index], p_write_fd);
		if (p_except_fd)
			FD_SET(fd_array[index], p_except_fd);
	}
	if (maxfd <= 0)
		return -1;
	
	maxfd += 1;

	while (1)
	{
		ret = select(maxfd, p_read_fd, p_write_fd, p_except_fd, p_time_out);
		if ((ret < 0) && (errno == EINTR))
		{
		    continue;
		}    
		else if (ret < 0)
			return -1;
		else if (ret == 0)
			return 0;
		else
		{
			for (index = 0; index < fd_num; ++index)
			{
				if (fd_array[index] <= 0)  //socket error
					continue;
				
				if (p_read_fd)
				{
					if (FD_ISSET(fd_array[index], p_read_fd))
						return fd_array[index] | 0x10000;
				}
				else if (p_write_fd)
				{
					if (FD_ISSET(fd_array[index], p_write_fd))
						return fd_array[index] | 0x20000;
				}
				else if (p_except_fd)
				{
					if (FD_ISSET(fd_array[index], p_except_fd))
						return fd_array[index] | 0x40000;
				}			
			}
			return 0;
		}
	}	
	return -1;	
}

int tcp_listen(const char *host, const char *serv, int *addrlenp)
{
    int      listenfd, n;
    struct addrinfo hints, *res, *resSave;
	
    memset(&hints, 0, sizeof(struct addrinfo)) ;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
	
    if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
        return -1;
	
    resSave = res;
    do {
		listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        set_sock_reuse(listenfd, 1);        
		if(listenfd > 0)
		{    
            set_sock_attr(listenfd, 1, 0, 0, 0, 0);
            if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
                break;               /* success */
            close_socket(&listenfd);        /* bind error, close and try next one */
		}
    } while ( (res = res->ai_next) != NULL);
	
    if (res == NULL)            /* errno from final socket () or bind () */
	{
		if (resSave)
		{
			freeaddrinfo (resSave);
			resSave = NULL;
		}
		return -1;
	}
    listen(listenfd, LISTENQ);

    if (addrlenp)
		*addrlenp = res->ai_addrlen;     /* return size of protocol address */
	if (resSave)
		freeaddrinfo (resSave);
	
    return (listenfd);
}

/* noblock socket */
int create_noblock_socket(int type)
{
	int fd;
	unsigned long block_flag = 1;

	fd = socket(AF_INET, type, 0);
	if (fd < 0)
		return -1;		
	ioctlsocket(fd, FIONBIO, &block_flag);
	
	return fd;
}

int create_noblock_tcp_socket(void)
{
    return create_noblock_socket(SOCK_STREAM);
}

int create_noblock_udp_socket(void)
{
    return create_noblock_socket(SOCK_DGRAM);
}

int set_sock_block(int sockfd)
{	
    unsigned long block_flag = 0;
	
	if (sockfd <= 0)
		return -1;

    if (ioctlsocket(sockfd, FIONBIO, &block_flag) < 0 )	
        return -1;
    return 0;
}

int set_sock_noblock(int sockfd)
{	
    unsigned long block_flag = 1;
	
	if (sockfd <= 0)
		return -1;
	
    if (ioctlsocket(sockfd, FIONBIO, &block_flag) < 0 )
        return -1;
    return 0;
}

int tcp_noblock_connect(const char *server, int port, int timeout_ms)
{
    int sockfd;
    int ret;
    int error;
    int error_len = sizeof(int);
    fd_set wr_set;
    struct timeval timeout;
    struct addrinfo hints, *server_addr = NULL;  
    char szPort[16] = {0};

    if ((server == NULL) || (port > 65535) || (port <= 0))
        return -1;
    
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    sprintf(szPort, "%d", port);
    ret = getaddrinfo(server, szPort, &hints, &server_addr);
    if (ret < 0)
        return -1;
    
    do {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            break;
        set_sock_noblock(sockfd);
        ret = connect(sockfd, server_addr->ai_addr, server_addr->ai_addrlen);
        if (ret == 0)       /* (�������Ϳͻ�����ͬһ̨������,�������سɹ�)���ӳɹ� */
            break;
        else if ((ret < 0) && (errno == EINPROGRESS))
		{
			FD_ZERO(&wr_set);
			FD_SET(sockfd, &wr_set);
			
			if ((ret = select(sockfd + 1, NULL, &wr_set, NULL, &timeout)) > 0)
			{
				if (FD_ISSET(sockfd, &wr_set) > 0)
				{
					if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&error_len) != 0)
						error = 1;
                    if (error == 0)     /* ���ӳɹ� */
                        break;
				}
			}
            close_socket(&sockfd); /* select��ʱ����getsockopt,����ʧ�� */     
		}
        else
        {
            close_socket(&sockfd);
            continue;
        }
    }while ((server_addr = server_addr->ai_next) != NULL);

	if (server_addr)
	{
		freeaddrinfo(server_addr);
		server_addr = NULL;
	}  
    return sockfd;
}

/* ���ݷ�����������ok, ����fail */
int tcp_noblock_send(int fd, char *buf, int size)
{
	int  retry = 0;	
	int  len = size;
	int  sended = 0;
	
	if (fd < 0 || buf == NULL || size <= 0)
		return 0;
	
	while (len > 0)
	{
		sended = send(fd, buf, len, MSG_NOSIGNAL);	/* MSG_NOSIGNAL: ��һ���Ѿ��رյ�socket�������ݻ����BrokePipe�źŵ��½����˳� */
		if (0 == sended)
			return -1;
		else if (sended < 0)
		{
			if ((errno == EWOULDBLOCK) || (errno == EINTR) || (errno == EAGAIN))
			{			
				if (retry++ < (NET_NOBLOCK_RETRY_CNT * 2))
				{
					Sleep(20);
					continue;
				}
				else
					break;
			}
			return -1;
		}
		else
		{	
			buf += sended;
			len -= sended;
		}
	}
		  
	if(len > 0)
		return -1;
	return size;    
}

int tcp_noblock_recv(int fd, char *buf, int size)
{
    int ret = 0;
    int retry = 0;
	
	if (fd < 0 || buf == NULL || size <= 0)
		return -1;	           
	
    while (1)
    {
		ret = recv(fd, buf, size, 0);
		if(ret == 0)
			return -1;
		else if(ret < 0)
		{
			if((errno == EWOULDBLOCK) || (errno == EINTR) || (errno == EAGAIN))
			{
				if(retry++ < NET_NOBLOCK_RETRY_CNT)
				{	
					Sleep(10);
					continue;
				}
				else
				{
					break;
				}
			}
            else 
            {
				break;
            }
		}
        else            /* ���ؽ������ݳ��� */
            return ret;
    }
    
    return ret;
}

/* ���ݷ�����������ok, ����fail */
int udp_send(int sock, void *buf, int size, struct sockaddr *distAddr)
{
    int  retry  = 0, len = size, sended = 0;
    char *pBuf = buf;
	if ((sock < 0) || (pBuf == NULL) || (size <= 0) || (distAddr == NULL))
		return -1;
	
    while(len > 0)
    {
		sended = sendto(sock, pBuf, len,0, distAddr, sizeof(struct sockaddr));
		if(0 == sended)
            return -1;
		else if(sended < 0)
		{
			if((errno == EWOULDBLOCK) || (errno == EINTR) || (errno == EAGAIN))
			{			
				if(retry < NET_NOBLOCK_RETRY_CNT)
				{
					Sleep(10);
                    retry++;
                    continue;
				}
				else
					return -1;
			}
			return -1;
		}
		else
		{		
			pBuf += sended;
			len -= sended;
		}
    }
    
    return size;
}

int set_sock_boardcast(int fd)
{
	int so_boardcast = 1;
	return setsockopt(fd, SOL_SOCKET, SO_BROADCAST, (const char *)&so_boardcast, sizeof(so_boardcast));
}

int send_broadcast(int sockfd, int port, char *data, int len)
{
    struct sockaddr_in broadcast_addr;

    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(port);
    broadcast_addr.sin_addr.s_addr = inet_addr("255.255.255.255");
    return udp_send(sockfd, data, len, (struct sockaddr *)&broadcast_addr);
}

/* ����ʵ�ʽ������ݳ��� */
int udp_recv(int sockfd, char *buf, int size, struct sockaddr *from, int *fromlen)
{
    int ret = 0;
    int retry = 0;

    if ((sockfd < 0) || (size <= 0) || (buf == NULL))
        return -1;	           

    while(1)
    {   
		ret = recvfrom(sockfd, buf, size, 0, from, (socklen_t *)fromlen);
		if(0 == ret)
			return -1;
		else if(ret < 0)
		{
			if((errno == EWOULDBLOCK) || (errno == EINTR) || (errno == EAGAIN))		
			{
				if(retry < NET_NOBLOCK_RETRY_CNT)
				{	
					Sleep(20);
                    			retry++;
					continue;
				}
				else
				{
//                    printf("retry fail\n");
					return -1;
                }
			}
            else
    			return -1;
		}
        else              /* ���ؽ������ݳ��� */
            return ret;
    }
    
    return ret;
}

/* block socket */
int create_block_socket(int type)
{
	int fd;
	unsigned long block_flag = 1;

	fd = socket(AF_INET, type, 0);
	if (fd < 0)
		return -1;
	ioctlsocket(fd, FIONBIO, &block_flag);
	
	return fd;
}

int create_block_tcp_socket(void)
{
	return create_block_socket(SOCK_STREAM);
}

int create_block_udp_socket(void)
{
	return create_block_socket(SOCK_DGRAM);
}

/* ����һ�����ʲ�����IPʱ, connect����ʱ����75s��������*/
int tcp_block_connect(const char *dstHost, int port)
{
    int     sockfd;
    struct addrinfo hints, *dstRes = NULL, *resSave = NULL;
	char	szPort[32] = {0};
		
    if ((dstHost == NULL) || (port < 0) || (port > 65535))
		return -1;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

	sprintf(szPort, "%d", port);
    if (getaddrinfo(dstHost, szPort, &hints, &dstRes) < 0)
        return -1;
	resSave = dstRes;
    do {
		sockfd = socket(dstRes->ai_family, dstRes->ai_socktype, dstRes->ai_protocol);
		if (sockfd < 0)
		{
			Sleep(10);
			continue;                   /*ignore this one */
		}   
		if (connect(sockfd, dstRes->ai_addr, dstRes->ai_addrlen) == 0)
			break;                      /* success */
		close_socket(&sockfd);          
    } while ((dstRes = dstRes->ai_next) != NULL);
    
	if (dstRes == NULL)                 /* errno set from final connect() */
		sockfd = -1;
	if (resSave)
	{
		freeaddrinfo(resSave);
		resSave = NULL;
	}

    return sockfd;
}

/* ����ʵ�ʷ������� */
/* fd ����Ϊ0 */
int tcp_block_send(int fd, const void *data, int n)
{
    int nleft;
    int nwritten;
    int nTryTimes = 0;
    const char *ptr;

	if  ((fd < 0) || (data == NULL) || (n <= 0))
	{
		return -1; 
    }
	
    ptr = (const char *)data;
    nleft = n;
    while ((nleft > 0) && (nTryTimes < NET_BLOCK_RETRY_CNT))
    {
        if ((nwritten = send(fd, ptr, nleft, MSG_NOSIGNAL)) <= 0) 
        {
			if ((nwritten < 0) && (errno == EINTR))     /* �������·��� */	
			    break;
			else
			{
				perror("send data");
#if WIN32
			 	printf("error code[%d]\n", WSAGetLastError()); 			
#else
				printf("error code[%d]\n", errno);
#endif
				return -1;   		 		/* ��������ʧ�� */
			}
		}
		
		nleft -= nwritten;
		if (nleft == 0)								/* ���ݷ������ */
			break;
		ptr += nwritten;
        ++nTryTimes;
		Sleep(10);
    }
    return (n - nleft);
}

int tcp_block_send_ext(int fd, const void *data, int n)
{
    int nleft;
    int nwritten;
    int nTryTimes = 0;
    const char *ptr;

	if ((fd < 0) || (data == NULL) || (n <= 0))
	{
        printf("param error[%d,%p,%d]\n", fd, data, n);
		return -1;
    }
	
    ptr = data;
    nleft = n;
    while ((nleft > 0) && (nTryTimes < NET_BLOCK_RETRY_CNT))
    {
        if ( (nwritten = send(fd, ptr, nleft, 0)) <= 0) 
        {
			if ((nwritten < 0) && (errno == EINTR))     /* �������·��� */	
			    break;
			else
			{
				perror("send data");
                printf("send fail[%d]\n", n);
				return -1;   		 		/* ��������ʧ�� */
			}
		}
		
		nleft -= nwritten;
		if (nleft == 0)								/* ���ݷ������ */
			break;
		ptr += nwritten;
        ++nTryTimes;
    }
    return (n - nleft);
}

/* ���ؽ��ճ��� */
/* ���ն�ξ�����֤���յ�recSize������(��Ҫ����noblock) */
int tcp_block_recv(int sockfd, void *rcvBuf, int rcvSize)  /* some wrong */
{
    int nleft = 0;
    int nread = 0;
	int nTryTimes = 0;
    char *ptr = NULL;

	if  (sockfd < 0 || rcvBuf == NULL || rcvSize <= 0)
	{
		perror("recv");
		return -1; 
	}
    ptr = (char *)rcvBuf;
    nleft = rcvSize;
	
    while((nleft > 0) && (nTryTimes < NET_BLOCK_RETRY_CNT)) 
    {
        if ((nread = recv(sockfd, ptr, nleft, 0)) < 0)
		{
			if (errno == EINTR||errno ==EAGAIN)
                nread = 0;
            else
            {
                perror("recv");
                return -1;    	
            }
		}
        else if (nread == 0)        /* �׽��ֱ��ر� */
        {
			break;
        }
		nleft -= nread;
        ptr   += nread;
        ++nTryTimes;
		//sleep(1);
		usleep(100*1000);
    }
	
    return  (rcvSize - nleft);   	     	
}

int tcp_block_recv_ext(int sockfd, void *rcvBuf, int rcvSize)
{
    int nleft = 0;
    int nread = 0;
    char *ptr = NULL;

	if  (sockfd < 0 || rcvBuf == NULL || rcvSize <= 0)
		return -1; 

    ptr = (char *)rcvBuf;
    nleft = rcvSize;
	
    while((nleft > 0)) 
    {
        if ((nread = recv(sockfd, ptr, nleft, 0)) < 0)
		{
			if (errno == EINTR)
                nread = 0;
            else
                return -1;    	
		}
        else if (nread == 0)        /* �׽��ֱ��ر� */
        {
			break;
        }		
		nleft -= nread;
        ptr   += nread;
		Sleep(20);          /* ����һ��û�н�����,�ӳ�20ms�ڽ��� */
    }
	
    return  (rcvSize - nleft);   	     	
}

int tcp_block_accept(int fd, struct sockaddr *sa, int *salenptr)
{
	int n = -1;

again:
	if ( (n = accept(fd, sa, (socklen_t *)salenptr)) < 0) 
	{
		if (errno == ECONNABORTED)
			goto again;
	}

	return (n);
}

int tcp_block_recv_once(int sockfd, char *rcvBuf, int rcvSize)
{
	return recv(sockfd, rcvBuf, rcvSize, 0);
}

int tcp_force_recv(int sockfd, void *data, int size)	/* ǿ�ƽ���size���򷵻س��� */
{
	int ret;
	
	ret = recv(sockfd, (char *)data, size, MSG_WAITALL);	/* xp��֧��MSG_WAITALL(�ȴ��������) */
	if (ret != size)
	{
#ifdef		WIN32
		printf("error code[%d]\n", WSAGetLastError()); 	
#else
		perror("recv");
#endif
		printf("size: %d ret: %d\n", size, ret);
		return -1;
	}
	else
		return 0;
}

