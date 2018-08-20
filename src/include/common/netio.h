
#ifndef NETBASIC_H
  #define NETBASIC_H

#include <netinet/in.h> //struct sockaddr_in

int create_socket( void );

void close_socket( int fd);

bool send_packet(int socket, const void *buf, size_t len, struct sockaddr *saddr);

bool send_data(const void *buf, size_t len, struct sockaddr *saddr);

bool Is_valid_ipv4(char * ip_str);

#endif      //NETBASIC_H
