all:
	g++ -c cksum.cpp -o obj/cksum.o
	g++ -c debugmsg.cpp -o obj/debugmsg.o
	g++ -c packet.cpp -o obj/packet.o
	g++ -c rdt.cpp -o obj/rdt.o
	g++ -c timeout.cpp -o obj/timeout.o
	g++ -c testcases.cpp -o obj/testcases.o
	ar rcs libRdtUdp.a obj/*.o
	g++ client.cpp -o client libRdtUdp.a
