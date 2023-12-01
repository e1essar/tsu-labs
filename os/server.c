#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
 
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
 
#define PORT 9999
#define MAX_CLIENTS 10
 
volatile sig_atomic_t was_handled = 0;
 
void signalHandler(int signal) {
    was_handled = 1;
}
 
void registerHandlers(sigset_t* origMask) {
    struct sigaction sa;
    sigaction(SIGINT, NULL, &sa);
    sa.sa_handler = signalHandler;
    sa.sa_flags |= SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
 
    sigset_t blockedMask;
    sigemptyset(&blockedMask);
    sigaddset(&blockedMask, SIGINT);
    sigprocmask(SIG_BLOCK, &blockedMask, origMask);
}
 
int main() {
    // create the server socket
    int server_socket, rc;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
 
    if (server_socket == -1) {
        perror("HH_ERROR: error in calling socket()");
        exit(1);
    }
 
    // define the server address
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;
 
    // bind the socket to our specified IP and port
    rc = bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
 
    if (rc < 0) {
        perror("HH_ERROR: bind() call failed");
        exit(1);
    }
 
    rc = listen(server_socket, 5);
 
    if (rc < 0) {
        perror("HH_ERROR: listen() call failed");
        exit(1);
    }
 
    printf("Listening...\n");
 
    fd_set fds;
    fd_set tmp_fds;
    FD_ZERO(&fds);
    FD_SET(server_socket, &fds);
    int maxfd = server_socket;
 
    int client_sockets[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        client_sockets[i] = -1;
    }
 
    sigset_t origMask;
    registerHandlers(&origMask);
    int keep_socket = 1;
 
    while (keep_socket) {
        if (was_handled) {
            puts("Stopping server...");
            close(server_socket);
            exit(0);
        }
 
        tmp_fds = fds;
 
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            int client_socket = client_sockets[i];
            if (client_socket != -1) {
                FD_SET(client_socket, &fds);
                if (client_socket > maxfd) {
                    maxfd = client_socket;
                }
            }
        }
 
        if (pselect(maxfd + 1, &tmp_fds, NULL, NULL, NULL, &origMask) == -1) {
            if (errno == EINTR) {
                if (was_handled) {
                    puts("Stopping server...");
                    close(server_socket);
                    exit(0);
                }
                continue;
            }
            perror("pselect error");
            exit(1);
        }

     if (FD_ISSET(server_socket, &tmp_fds)) {
            // Accept the new connection
            socklen_t client_len = sizeof(client_address);
            int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_len);
            if (client_socket == -1) {
                perror("HH_ERROR: accept() call failed");
                exit(1);
            }
 
            for (int i = 0; i < MAX_CLIENTS; ++i) {
                if (client_sockets[i] == -1) {
                    client_sockets[i] = client_socket;
                    break;
                }
            }
 
            printf("New connection established.\n");
        }
 
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            int client_socket = client_sockets[i];
            if (client_socket != -1 && FD_ISSET(client_socket, &tmp_fds)) {

                char msg[256];
                ssize_t bytes_received = recv(client_socket, msg, sizeof(msg), 0);

                if (bytes_received <= 0) {
                    close(client_socket);
                    FD_CLR(client_socket, &fds);
                    client_sockets[i] = -1;
                    printf("Connection closed for descriptor %d\n", client_socket);
                } else {
                    printf("%s\n", msg);
                }
            }
        }
    }
 
    return 0;
}
