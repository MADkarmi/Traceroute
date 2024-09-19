#include "tools.h"
bool not_TOF(chrono::high_resolution_clock::time_point current_time, chrono::high_resolution_clock::time_point time_start);
bool get_packet(int sockfd, uint16_t p_id, uint8_t ttl, timeval timeout, chrono::high_resolution_clock::time_point time_start);