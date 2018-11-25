/*Code inspired on t50 */
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "common/common.h"

#define DELIM "."
#define MAX_SOCKEBUFFER 10485760

/* return true if string contain only digits, else return false */
static bool _valid_digit(char * ip_str)
{
  while (*ip_str) {
    if (*ip_str >= '0' && *ip_str <= '9'){
      ++ip_str;
    }
    else{
      return false;
    }
  }
  return true;
}

int createTCPsocket()
{
    int fd;

    if ( (fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        ELOG(ERROR_NET, "Cannot open TCP socket\n"); 
    }
    return fd;
}

int createRAWsocket()
{
    int fd;

    if ((fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) == -1)
    {
        ELOG(ERROR_NET, "Cannot open raw socket\n");
    }
    return fd;
}

int createUDPsocket()
{
    int fd;

    if ((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        ELOG(ERROR_NET, "Cannot open udp socket");
    }
    return fd;
}

/** 
 * Create socket.
 *
 * @param p_type socket type
 * @param p_blk 0:non-blocking mode, 1:blocking mode
 *
 * @return socket file descriptor or error if < 0.
 **/
int CreateSocket( NetType p_type, bool p_blk )
{
    int fd = -1;
    uint32_t n = 1;

    switch(p_type)
    {
        case UDP:
            fd = createUDPsocket();
            break;
        case TCP:
            fd = createTCPsocket();
            break;
        case RAW:
        default:
            fd = createRAWsocket();
    }
    
    BlockSocket( fd, p_blk);
    /* Setting IP_HDRINCL. */
    /* NOTE: We will provide the IP header, but enabling this option, on linux,
                still makes the kernel calculates the checksum and total_length. */
    if(p_type == RAW)
    {
        if ( setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &n, sizeof(n)) == -1 )
        {
            ELOG(ERROR_NET, "Cannot set socket options\n");
        }
    }

    return fd;
}

void CloseSocket( int fd )
{
  /* Close only if the descriptor is valid. */
  if (fd > 0)
  {
    close(fd);    // AS_SAFE!

    /* Added to avoid multiple socket closing. */
    fd = -1;
  }
}

int SetSocketFlag( int p_socket, int p_flags )
{
    int flags;

    if ((flags = fcntl(p_socket, F_GETFL)) == -1)
    {
        ELOG(ERROR_NET, "Cannot get socket flags\n");
    }
    
    flags |= p_flags;

    if (fcntl(p_socket, F_SETFL, flags) == -1)
    {
        ELOG(ERROR_NET, "Cannot set socket flags\n");
    }

    return SUCCESS;
}

/** 
 * Set a file descriptor to blocking or non-blocking mode.
 *
 * @param fd The file descriptor
 * @param blocking 0:non-blocking mode, 1:blocking mode
 *
 * @return 0:success, rtn < 0:failure.
 **/
int BlockSocket(int p_fd, bool blocking) 
{
    int flags = 0;

    if (blocking)
        flags &= ~O_NONBLOCK;
    else
        flags |= O_NONBLOCK;

    return SetSocketFlag( p_fd, flags );
}

Packet * CreateEmptyPacket( )
{
    Packet *pac = NULL;
    memalloc( &pac, sizeof(Packet) );
    pac->type = EMPTY;
    return pac;
}

void ReleasePacket( Packet *p_pkt )
{
    memfree( &p_pkt->packet_ptr );
    memfree( &p_pkt );
}

int SendPacket(int p_socket, Packet *p_pkt)
{
    switch(p_pkt->type)
    {
        case TCP:
            if( -1 == send(p_socket, p_pkt->packet_ptr, p_pkt->pkt_size, 0)  )
            {
                return ERROR_NET;
            }
            break;

        case UDP:
        case RAW:
            if( -1 == sendto(p_socket, p_pkt->packet_ptr, p_pkt->pkt_size, 0,  (struct sockaddr *) &p_pkt->saddr, sizeof(struct sockaddr_in))  )
            {
                return ERROR_NET;
            }
            break;

        default:
            return ERROR_NET;
    }
    
    return SUCCESS;
}

/* return true if IP string is valid, else return false */
bool is_valid_ipv4(char * ip_str)
{
  int dots = 0;
  char ip_copy[16];
  char *ptr;
  
  if (ip_str == NULL){
    return false;
  }
  
  memcpy(ip_copy, ip_str, strlen(ip_str));
  ip_copy[strlen(ip_str)] = '\0';
  ptr = strtok(ip_copy, DELIM);
 
  if (ptr == NULL){
    return false;
  }
 
  while (ptr) {
    int num; 
    /* after parsing string, it must contain only digits */
    if (!_valid_digit(ptr)){
      return false;
    }

    num = atoi(ptr);
 
    /* check for valid IPV4 */
    if (num >= 0 && num <= 255) {
    /* parse remaining string */
    ptr = strtok(NULL, DELIM);
    if (ptr != NULL)
      ++dots;
    } 
    else{
      return false;
    }
    
  }
 
  /* valid IPV4 string must contain 3 dots */
  if (dots != 3){
    return false;
  }

  return true;
}

int BindPort( int p_socket, struct sockaddr_in saddr )
{
    struct sockaddr_in addrANY = {
        .sin_family      = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port        = saddr.sin_port 
    };
    socklen_t lenAddr = sizeof(struct sockaddr_in);

    if( bind( p_socket, (struct sockaddr *)&addrANY,  lenAddr) == -1 )
    {
        ELOG(ERROR_NET, "BIND FAILED\n");
    }
    if( getsockname( p_socket, (struct sockaddr *)&saddr, &lenAddr) == -1)
    {
        ELOG(ERROR_NET, "GETSOCKNAME FAILED\n");
    }
    return SUCCESS;
}

/* Adapted from T50. */
int Setup_sendbuffer ( int p_fd, uint32_t p_n )
{
    uint32_t i;
    socklen_t len;

    SetSocketFlag(p_fd, SO_SNDBUFFORCE);
    
    /* Getting SO_SNDBUF. */
    len = sizeof ( p_n );
        
    errno = 0;
    if ( getsockopt ( p_fd, SOL_SOCKET, SO_SNDBUF, &p_n, &len ) == -1 )
    {
        Efatal ( ERROR_NET, "Cannot get socket buffer" );
    }

  /* Setting the maximum SO_SNDBUF in bytes.
   * 128      =  1 Kib
   * 10485760 = 80 Mib */
    for ( i = p_n + 128; i < MAX_SOCKEBUFFER; i += 128 )
    {
    /* Setting SO_SNDBUF. */
        errno = 0;
        if ( setsockopt ( p_fd, SOL_SOCKET, SO_SNDBUFFORCE, &i, sizeof ( i ) ) == -1 )
        {
            if ( errno == ENOBUFS || errno == ENOMEM )
            {
                break;
            }
            Efatal( ERROR_NET, "Cannot set socket buffer" );
        }
    }
    return p_n;
}

void ConnectTCP(int p_socket, Packet *p_pkt)
{
    // connect the client socket to server socket 
    if (connect(p_socket, (struct sockaddr *) &p_pkt->saddr, sizeof(struct sockaddr_in)) != 0) 
    { 
        Efatal(ERROR_NET, "Connection with the server failed...\n");
    }
}
