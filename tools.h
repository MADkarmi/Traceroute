#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include <errno.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <cassert>
#include <iostream>
#include <chrono>
#include <string>

using namespace std;

#define errorhandler(msg) do {perror(msg "error"); exit(EXIT_FAILURE);} while (0)

extern const uint8_t number_of_sent_packets;
extern uint8_t maximum_ttl;

uint16_t compute_icmp_checksum (const void *buff, int length);
struct icmphdr createicmphdr(uint16_t p_id, int sequencenbr);
void display(uint8_t ttl, string recieved_adresses[],  chrono::duration<double, std::milli> reception_times[], uint8_t packets_recived);

