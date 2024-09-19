/*
* Radoslaw Madzia 283515
*/
#include "get_packets.h"


/*
* funkcja sprawdzajaca czy nie przekroczylismy czasu 1 sekundy (dla wszystkich 3 pakietów)
*/
bool not_TOF(chrono::high_resolution_clock::time_point current_time, chrono::high_resolution_clock::time_point time_start)
{
	auto time_out = time_start + chrono::seconds(1);
	if(current_time<time_out)
		return true;
	return false;
}

/*
*funkcja odbierająca pakiet, sprawdzajace ich naglowki i czy są odpowiedzia na wyslane 3 zapytania,
*zwraca true (sygnał przerwania pętli zwiększajacej ttl) jeżeli otrzymaliśmy odpowiedz od targetowanego ip
*wpp false
*czasy i ip od ktorych wrocily poprawne pakiety przechowywane w 2 tablicach reception_time i recieved_adresses
*(wiekszosciowo wyklad)
*/
bool get_packet(int sockfd, uint16_t p_id, uint8_t ttl, timeval timeout, chrono::high_resolution_clock::time_point time_start)
{
	bool reached_targeted_ip = false;
	uint8_t packets_recived = 0;
	string recieved_adresses[number_of_sent_packets];
	chrono::high_resolution_clock::time_point current_time;
	chrono::duration<double, std::milli> reception_time[number_of_sent_packets];

	while(packets_recived < number_of_sent_packets && not_TOF(chrono::high_resolution_clock::now(), time_start))
	{

		fd_set descriptors;
		FD_ZERO(&descriptors);
		FD_SET(sockfd, &descriptors);

		int ready = select(sockfd + 1, &descriptors, NULL, NULL, &timeout);
		if(ready<0)
			errorhandler("select error: ");
		if(ready == 0)
			break;

		
		struct sockaddr_in sender;
		socklen_t sender_len = sizeof(sender);
		uint8_t buffer[IP_MAXPACKET];
		uint8_t *offset;
					

		ssize_t packet_len = recvfrom(sockfd, buffer, IP_MAXPACKET, MSG_DONTWAIT, (struct sockaddr *)&sender, &sender_len);
	    if(packet_len < 0)
	    	errorhandler("recvfrom ");
		
				
	    char sender_ip_str[20];

	    inet_ntop (AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str));

	    struct iphdr* ip_header = (struct iphdr*) buffer;
	    uint8_t* icmp_packet = buffer + (ip_header->ihl<<2);
	    struct icmphdr* icmp_header = (struct icmphdr*) icmp_packet;

	    if(icmp_header->type == ICMP_ECHOREPLY && icmp_header -> un.echo.id == p_id && (icmp_header-> un.echo.sequence)/number_of_sent_packets == ttl-1 )
	    {	
	    	
	    	recieved_adresses[packets_recived] = sender_ip_str;
	    	reception_time[packets_recived] = chrono::high_resolution_clock::now() - time_start;
	    	packets_recived++;
	    	reached_targeted_ip = true; 
	    }
	    if(icmp_header->type == ICMP_TIME_EXCEEDED) 
	    {
			    	
	    	offset = icmp_packet + 8;
	    	struct iphdr* inner_ipheader = (struct iphdr*)offset;
	    	offset += (inner_ipheader->ihl << 2);
	    	struct icmphdr* inner_icmpheader = (struct icmphdr*)offset;
	    	
	    	if(inner_icmpheader->un.echo.id == p_id && (inner_icmpheader-> un.echo.sequence)/number_of_sent_packets == ttl-1 )
	    	{
	    		recieved_adresses[packets_recived] = sender_ip_str;
	    		reception_time[packets_recived] = chrono::high_resolution_clock::now() - time_start;
	    		reached_targeted_ip = false; 
	    		packets_recived++;
	    	}
	    	
	    }   
	}	    

	display(ttl, recieved_adresses, reception_time, packets_recived);
    return reached_targeted_ip;
}