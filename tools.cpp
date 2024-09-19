/*
* Radoslaw Madzia 283515
*/
#include "tools.h"

const uint8_t number_of_sent_packets = 3; //ilość wyslanych pakietów z danym ttl 
uint8_t maximum_ttl = 30; //maxymalne ttl do którego sprawdzamy (max 255)
	
/*
 * kod ~mbi
 * oblicznie sumy kontronlej
 */ 
uint16_t compute_icmp_checksum (const void *buff, int length)
{
	uint32_t sum;
	const uint16_t* ptr = (const uint16_t *)buff;
	assert (length % 2 == 0);
	for (sum = 0; length > 0; length -= 2)
		sum += *ptr++;
	sum = (sum >> 16) + (sum & 0xffff);
	return (uint16_t)(~(sum + (sum >> 16)));
}

/*
 * tworzenie nagłówka icmp_echo_reqest (bo wg wykladu wystarcza)
 * p_id - identyfikatorem pakietu jest numer procesu
 * sequence_nbr - sekwencja sa numery od [0, 3*maximum_ttl+3]
 * (wyklad)
 */
struct icmphdr createicmphdr(uint16_t p_id, int sequence_nbr)
{
	struct icmphdr icmp_header;
    icmp_header.type = ICMP_ECHO;
    icmp_header.code = 0;
    icmp_header.un.echo.id = p_id;  
    icmp_header.un.echo.sequence = sequence_nbr;
    icmp_header.checksum = 0;
    icmp_header.checksum = compute_icmp_checksum ((u_int16_t *)&icmp_header, sizeof(icmp_header)); //obliczenie sumy kontrolnej
    return icmp_header;
}


/*
 * wypisanie wyników na wyjście
 * ttl - numer ttl
 * recieved_adresses - tablica adresów źródłą
 * reception_times - tablica czasów otrzymania pakietów
 * packets_recived = 0(brak jakiejkolwiek odpowiedzi)/ =[1,number_of_sent_packest-1](nieotrzymanie 3 odpoweidzi w czasie)/ =number_of_sent_packest otrzymanie 3 pakietów
 */

void display(uint8_t ttl, string recieved_adresses[number_of_sent_packets],  chrono::duration<double, std::milli> reception_times[number_of_sent_packets], uint8_t packets_recived)
{	
	cout << unsigned(ttl) << ". ";

	float time_average = 0;
	switch(packets_recived)
	{
		case 0:
		{
			cout << "*\n";
			break;
		}
		case number_of_sent_packets:
		{	
			cout << recieved_adresses[0] << " ";
			for(uint8_t i = 1; i<packets_recived; i++)
				for(uint8_t j = 0; j<i; j++)
					if(recieved_adresses[i] != recieved_adresses[j])
						cout << recieved_adresses[i] << " ";
			for(uint8_t i = 0; i < packets_recived; i++)
			{
				time_average+=reception_times[i].count();
			}
			break;
		}
		default:
		{
			cout << recieved_adresses[0] << " ";
			for(uint8_t i = 1; i<packets_recived; i++)
				for(uint8_t j = 0; j<i; j++)
					if(recieved_adresses[i] != recieved_adresses[j])
						cout << recieved_adresses[i] << " ";
			cout<< "??? \n";
			break;
		}
		
	}
	if(packets_recived == number_of_sent_packets)
		cout << time_average/number_of_sent_packets << "ms\n";

}