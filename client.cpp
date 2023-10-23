#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#define PORT "33526" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
    string dept;
   


    memset(&hints, 0, sizeof hints);
    hints.ai_family = PF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo("localhost", PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    cout<<"Client is up and running \n";
    
    
    
    


//------------------------------------------daepartment_name to be sent to server--------------------------------------
 
	while(true){

		char department_name[1024] = {0};
		cout<<"Enter Department Name:"<<endl;
		cin>>department_name;

		// Send data to server
		send(sockfd, department_name, strlen(department_name)+1, 0);
		
		cout<<"Client has sent Department "<<department_name<<" to Main Server using TCP."<<endl;

	    
		//-------receive message of department status from server.cpp----------
		char msg_client[1024] = {0}; // create a character array to store the received message
		memset(msg_client, 0, sizeof(msg_client));
		recv(sockfd, msg_client, sizeof(msg_client), 0); // pass the address of the character array
		cout<<msg_client<<endl;
		cout<<endl<<"----Start a new query----"<<endl;
	}
    
	freeaddrinfo(servinfo); // all done with this structure

	while(1){
		if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
			perror("recv");
			exit(1);
		}
	}

	buf[numbytes] = '\0';


	close(sockfd);

	return 0;
}
