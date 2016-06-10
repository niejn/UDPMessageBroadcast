#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
 
 
#define PORT 28009 // Choose a free port
#define MAX_CLIENTS 250 // sync between clients
 
 
int compareAddress ( struct sockaddr_in *a1, struct sockaddr_in *a2) // useful function
{
    int Match = 0;
    if ( sizeof (a1)  == sizeof ( a2 ) && (a1->sin_family == a2->sin_family)
         && (a1->sin_addr.s_addr == a2->sin_addr.s_addr) && (a1->sin_port == a2->sin_port) )
    {
      Match++;
    }
    return Match;
   
}
 

 
 
int main(void)
{
    fd_set master;    // master file descriptor list
 
 
 
 
   int clients,  client_addrSize[MAX_CLIENTS], listener, retval, nbytes, i, c, j, newLoopPos, bytesSent;
 
      int len, total, bytesleft, n, clientRemoved;
 
 
   
     clients = 0; // 0 connected clients
     
socklen_t slen_temp;
 
 
    time_t last_update[MAX_CLIENTS]; // checking disconnection or time out
 
        double diff_t;
       
    struct sockaddr_in myaddr, cli_addr[MAX_CLIENTS], cli_temp; ;      /* our address  and client address*/
    socklen_t addrlen = sizeof(myaddr);            /* length of addresses */
 
   
memset((char *)&myaddr, 0, sizeof(myaddr));
myaddr.sin_family = AF_INET;
myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
myaddr.sin_port = htons(PORT);
 
 
        /* create a UDP socket */
 
        if ((listener = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
                perror("cannot create socket\n");
                return 0;
        }
 
        /* bind the socket to any valid IP address and a specific port */
 
if (bind(listener, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
    perror("bind failed");
    return 0;
}
 
 
 
 
 
    // main loop
    for( ; ; ) {
 
   
 
   
        FD_ZERO(&master);
        FD_SET(listener, &master);
           
        retval = select(listener+1, &master, NULL, NULL, NULL) ;
        if (retval == -1) {
            perror("select");
            exit(4);
        }
        else if ( retval )
        {
    printf (" data available\n ");
 
 
        // run through the existing connections looking for data to read
 
            if (FD_ISSET(listener, &master)) { // we got one!!
             
             
            char buf[1024];    // buffer for client data
           
       
                           
                    // handle data from a client
                    if ((n = recvfrom(listener, buf, sizeof(buf), 0, (struct sockaddr*)&cli_temp, &slen_temp)) <= 0) {
                        // got error or connection closed by client
                        if (nbytes == 0)
                            // connection closed
                             printf("selectserver: socket %d hung up\n", listener);
                         else
                         perror("ERROR: recvfrom");
                       
                         
 
                    }
            else {
               
 
 
                    char niceArray[n];
                   
                        for(i = 0; i < n; i++)
                          niceArray[i] = buf[i];
                     
                               
                   
 
                           
                j = 0; // j wil act as bool for checking if client address is already stored in array or no
               
         int this_length =  sizeof ( (struct sockaddr*)&cli_temp);
         
                    if (clients < MAX_CLIENTS) {
                       
                       for(i = 0; i < clients; i++) { // struct sockaddr*)&cli_addr[i]
       
                           if ( compareAddress(&cli_addr[i], &cli_temp) > 0 )
                            {
                                j++;
                            }
                        }
                    }
                    if ( j == 0 ) { // so yeah client doesn't exist so we store the address and last update time
                   
                        cli_addr[clients] = cli_temp;
                        last_update[clients] = time(NULL);
                   
                        clients++;
                       
                        printf("\n Client Added. \n");
                    }
                           
                           
                           newLoopPos = 0; // start loop from 0
 
                           
                           
                           
                   
                           
                    do {  
                          clientRemoved = 0;
                        for(i = newLoopPos; i < clients; i++) {
                           
                             
                             
           
                                                               
                                     diff_t = difftime( time(NULL), last_update[i] );
                   
                                   if ( diff_t >= 15 ) { // client has time out or disconnect
                                     
                                     
                                 
                                   
                                    if ( (i+1) == clients) { // we don't need to DO the do-while loop AGAIN if we are removing the last element
                                     
                                     // nothing here
                                    }
                                   
                                    else
                                       
                                    clientRemoved++;
                                       
                                        newLoopPos = i;  
                                         clients--;
                           
                               struct sockaddr_in cli_source;
                                    cli_source = cli_addr[i]; // client that got disconnected
                                   
                                       for ( c = i - 1 ; c < MAX_CLIENTS - 1 ; c++ ) {
                                          // then its better to remove him from array
                                        cli_addr[c] = cli_addr[c+1];
                                        last_update[c] = last_update[c+1];
                                       }
                                       
                                        // we surely don't want to display disconnection messages from past 10 hours or so <!-- s;) --><img src=\"{SMILIES_PATH}/icon_wink.gif\" alt=\";)\" title=\"Wink\" /><!-- s;) -->  
                                        if ( diff_t < 60 ) {
                                   
                                   
                                         // one client has disconnected. Up to you how you deal with it :)
                                         // handle the message here if you want.
                                        }
                                       
                                       break; // it's important to break for loop now since we don't need to loop again wee
                                    }
                                   
                           
                /* DEBUG PURPOSE          
                       printf("(%d) LOOP and (%d) clients variable.\n", i, clients);
                */
                       
                         int RemoveMeLaterPls = 0;
                       
                             if ( compareAddress(&cli_addr[i], &cli_temp) > 0 && RemoveMeLaterPls > 0 ) {
                               // nothing
                               last_update[i] = time(NULL); // since this was the client who sent us data so we update his last time
                            }
                            else
                            {
                           
                                len = sizeof(niceArray);
                                total = 0;        // how many bytes we've sent
                                bytesleft = len; // how many we have left to send
                                n;
 
                                while(total < len) {
                                 n =  sendto(listener, niceArray+total, bytesleft, 0, (struct sockaddr*)&cli_addr[i],
                                      sizeof (cli_addr[i]) );
   
                                    if (n == -1) { break; }
           
                                    total += n;
                                    bytesleft -= n;
                                }
 
        len = total; // return number actually sent here
                                 
                            }
                        }
                       
                               
                    } while ( clientRemoved > 0);  
 
 
 
 
 
 
                   
                    }
                }
            }
        }
 
     
    return 0;

    }
