// #include<DNSforge.h>
//
// //DNS query
// void dns_format(unsigned char * dns,unsigned char * host)
// {
// 	int lock = 0 , i;
// 	strcat((char*)host,".");
// 	for(i = 0 ; i < strlen((char*)host) ; i++)
// 	{
// 		if(host[i]=='.')
// 		{
// 			*dns++ = i-lock;
// 			for(;lock<i;lock++)
// 			{
// 				*dns++=host[lock];
// 			}
// 			lock++;
// 		}
// 	}
// 	*dns++=0x00;
// }
//
// //Cria o payload do DNS
// void dns_header(dns_hdr *dns)
// {
// 	dns->id = (unsigned short) htons(getpid());
// 	dns->flags = htons(0x0100);
// 	dns->qcount = htons(1);
// 	dns->ans = 0;
// 	dns->auth = 0;
// 	dns->add = 0;
// }
