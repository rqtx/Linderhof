/*Code inspired on t50 */
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "common/common.h"

#define DELIM "."


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

int createTCPsocket( bool p_blk )
{
    int fd;
    if ( (fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        ELOG(ERROR_NET, "Cannot open TCP socket\n"); 
    }
    BlockSocket( fd, p_blk); 
    return fd;
}

int createRAWsocket(  bool p_blk )
{
    int fd;
    uint32_t n = 1;

    if ((fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) == -1)
    {
        ELOG(ERROR_NET, "Cannot open raw socket\n");
    }
    BlockSocket( fd, p_blk ); 

    /* Setting IP_HDRINCL. */
    /* NOTE: We will provide the IP header, but enabling this option, on linux,
                still makes the kernel calculates the checksum and total_length. */
    if ( setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &n, sizeof(n)) == -1 )
    {
        ELOG(ERROR_NET, "Cannot set socket options\n");
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
  switch(p_type)
  {
    case TCP:
      return createTCPsocket(p_blk);
      break;
    case UDP:
    default:
      return createRAWsocket(p_blk);
      break;
      
  }
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
    pac->type = Empty;
    return pac;
}

void ReleasePacket( Packet *p_pkt )
{
  memfree( &p_pkt->packet_ptr );
  memfree( &p_pkt );
}

int SendPacket(int p_socket, Packet *p_pkt)
{
    if( -1 == sendto(p_socket, p_pkt->packet_ptr, p_pkt->pkt_size, MSG_DONTWAIT ,  (struct sockaddr *) &p_pkt->saddr, sizeof(struct sockaddr_in))  )
    {
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


