traceroute: traceroute.cpp
	g++ -std=c++11 -Wall -Wextra -c traceroute.cpp tools.cpp send_icmp.cpp get_packets.cpp
	g++ -std=c++11 -Wall -Wextra -o traceroute traceroute.o tools.o send_icmp.o get_packets.o

clean:
	rm -rf *.o

distclean:
	rm -rf *.o traceroute
