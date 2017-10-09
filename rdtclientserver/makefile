all:
	g++ -c cksum.cpp -o obj/cksum.o
	g++ -c debugmsg.cpp -o obj/debugmsg.o
	g++ -c packet.cpp -o obj/packet.o
	g++ -c rdt.cpp -o obj/rdt.o
	g++ -c timeout.cpp -o obj/timeout.o
	ar rcs libRdtUdp.a obj/*.o
	g++ server.cpp -o server libRdtUdp.a
	g++ client.cpp -o client libRdtUdp.a
