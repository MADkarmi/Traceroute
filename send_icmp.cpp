/*
* Radoslaw Madzia 283515
*/

#include "send_icmp.h"


/*
* funkcja wysylajaca pojedynczy pakiet
* wywoluje funkcje tworzacza komunikat icmp
* wpisuje adres targetowanego ip z wejscia do struktury adresowej
* (wyklad)
*/
void send_single_request(int sockfd, char *ip[] ,uint16_t p_id, int sequencenbr, int ttl)
{
		
	struct sockaddr_in targeted_ip;
	bzero(&targeted_ip, sizeof(targeted_ip));
	targeted_ip.sin_family = AF_INET;
	int inet_pton_retip = inet_pton(AF_INET, ip[1], &targeted_ip.sin_addr);
	assert(inet_pton_retip == 1); 

	int setsocket = setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));
	if(setsocket < 0)
		errorhandler("wystapil blad podczas zmiany ttl:");

	struct icmphdr request = createicmphdr(p_id, sequencenbr);
	ssize_t sent_request = sendto(sockfd, &request,sizeof(request),0,(struct sockaddr *)&targeted_ip, sizeof(targeted_ip));

	if(sent_request < 0)
		errorhandler("blad wysylania pakietu: ");

}