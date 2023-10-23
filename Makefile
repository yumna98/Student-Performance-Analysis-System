# Makefile for all the performing commands needed for the three files to get .o files
# Also executes the three object files to run CRC programs
all:
	clear
	g++ -o servermain servermain.cpp  -g
	g++ -o client client.cpp  -g
	clear
	
servermain:
	clear
	g++ -o servermain servermain.cpp  -g
	
client:
	clear
	g++ -o client client.cpp  -g						
		



