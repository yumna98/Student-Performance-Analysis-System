#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>


#define PORT "33526"  // the port users will be connecting to

#define BACKLOG 10   // how many pending connections queue will hold

using namespace std;

//----taken from Beej's guide------
void sigchld_handler(int s)
{
     
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}



int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    pid_t childpid;

    //----taken from Beej's guide------
    memset(&hints, 0, sizeof hints);
    hints.ai_family = PF_INET;
    hints.ai_socktype = SOCK_STREAM;
    

    //----taken from Beej's guide------
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    //----taken from Beej's guide------
    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

	//----taken from Beej's guide------
    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

	//----taken from Beej's guide------
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

	//----taken from Beej's guide------
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("Main server is up and running\n");
    
    
    
    
    //---------------------------------Reading list.txt file----------------------------------------------
    
    ifstream infile("list.txt"); // Open the input file
    map<int, vector<string>> departments; // Create a map to store the departments
    string line;
    int id = -1;
    while (getline(infile, line)) {
        if (isdigit(line[0])) { // If the line starts with a digit, it's a department ID
            id = stoi(line); // Convert the ID string to an int
        }
        else { // Otherwise, it's a list of department names
            stringstream ss(line);
            string name;
            while (getline(ss, name, ',')) {
                departments[id].push_back(name); // Add the name to the corresponding department ID
            }
        }
    }
    
    cout<<endl<<"Main server has read the department list from list.txt"<<endl;

    // Print the total number of department ids
    int num_departments = departments.size();
    cout << "Total number of Backend Servers: " << num_departments << endl;

    // Print the number of distinct departments for each department id
    for (auto& it : departments) {
        int id = it.first;
        vector<string>& names = it.second;
        sort(names.begin(), names.end());
        names.erase(unique(names.begin(), names.end()), names.end()); // Remove duplicates
        int num_names = names.size();
        cout << "Backend Servers " << id << " contains " << num_names << " distinct departments" <<endl;
    }
    
    cout<<endl;
    int count = 0;
    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd < 0) {
            exit(1);
        }
        
        count++;
    	    
        if((childpid = fork()) == 0){
        	close(sockfd);
        	
        	        //second while loop to receive message from the client
        		while(1){
        			int client_id = count;
    				char department_name[256]; // to receive string from client
   
    				recv(new_fd, department_name, sizeof(department_name), 0);
    				
    				cout<<"The Main server has received the request on Department " << department_name <<" from client "<< client_id<<" using TCP over port " <<PORT<<endl;
				
				
				// Flag to check if department_name was found
    				bool found_department = false;
    				bool notfound_updated = false;
    				bool flag = false;
    				
    				int send_id;  // department id to be sent to client
    				string message, notfound = "";
    				string msg_client;    
    				
    //To search our map and send the corresponding department id to the client------------
    
    // Loop through each department in the map
    for (const auto& [department_number, department_names] : departments) {
        // Loop through each name in the department
        for (const auto& name : department_names) {
            
            if (name == department_name) {
                send_id= department_number;
                cout<<"Main server has sent searching result to client "<<client_id<<" using TCP over port "<<PORT<<endl;
                message = string(department_name) + " shows up in backend server "+ to_string(send_id);
                found_department = true;
                break;
            }
        }
        
        
        if (found_department) {
            break;
        }
    }
    
    
    if (!found_department) {
          
        message = string(department_name) + " does not shows up in backend server ";
    for (auto& it : departments) {
        int id = it.first;
        message += to_string(id) + ", ";
        if (!notfound_updated) {
                notfound = "The Main Server has sent \"Department Name: Not Found\" to the client " + to_string(client_id) + " using TCP over port " + PORT;
                notfound_updated = true;
            } else {
                notfound += "";
            }
        
        
    }
    message.pop_back(); // Remove the last comma
    message.pop_back(); // Remove the last space
    
    }
        
    
    cout << message<<endl; 
    
    if(!found_department)
    {

      msg_client= string(department_name)+" not found";
      
    }
    else
    {
      msg_client = "Client has received result from Main Server: "+ string(department_name)+ " is associated with backend server "+ to_string(send_id);
    }
    
    if (notfound_updated) {
            cout << notfound << endl;
     }
    
    //------------------sending data to the client if department is found or not----------------
    send(new_fd, msg_client.c_str(), msg_client.length(), 0);
    //------------------------------------------------------------------------------------------------------------------
         	
        		
        	}
        		
        	close(new_fd);
        }
      	close(new_fd);  
        
    }

    return 0;
}
