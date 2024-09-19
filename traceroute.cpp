/*
* Radoslaw Madzia 283515
*/

#include "tools.h"
#include "send_icmp.h"
#include "get_packets.h"


int main(int argc, char *argv[])
{
	
	/*
	*Sprawdzenie poprawnośći ilości argumentów
	*/
	if(argc != 2) 
		errorhandler("Użyj z argumentem <IPv4>\n");

	/*
	*Sprawdzenie poprawnośći ip
	*/
	
	struct sockaddr_in inp_ip;
	int ip = inet_pton(AF_INET, argv[1], &(inp_ip.sin_addr));
	if(!ip)
		errorhandler("Niepoprawny adres <IPv4>\n");
		

	/*
	* tworzenie surowego socketu
	* obsługa ewentualnego błedu gniazda
	*/
	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if(sockfd < 0)
		errorhandler("tworzenie gniazda nie powiodlo sie ");
	

	uint16_t p_id = getpid(); //indentyfikator pakietów jakie wysyłą proces modulo 2^16
	

	/*
	* timeval dla funkcji select
	*/
	struct timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	
	uint16_t sequence = 0;
	for(uint8_t ttl = 1; ttl<=maximum_ttl; ++ttl)
	{
		auto time_start = chrono::high_resolution_clock::now();
		for(uint8_t i = 0;i < number_of_sent_packets ; ++i)
			send_single_request(sockfd, argv, p_id, sequence+i, ttl);
			
		if(get_packet(sockfd, p_id, ttl, timeout, time_start))
			return EXIT_SUCCESS;
		
		sequence += number_of_sent_packets;
	} 
	return EXIT_FAILURE;
}
