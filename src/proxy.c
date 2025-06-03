#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

#include "proxy.h"
#include "logger.h"
#include "config.h"
#include "cache.h"

// void handle_client(int client_socket);

void* thread_handler(void* arg) {
    int client_socket = *((int*)arg);
    free(arg); // libère la mémoire allouée dynamiquement
    handle_client(client_socket);
    return NULL;
}



void start_proxy(int port) {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(1);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    listen(server_fd, 10);
    printf("Proxy en écoute...\n");

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        printf("Connexion acceptée de %s\n", inet_ntoa(client_addr.sin_addr));
    int* pclient = malloc(sizeof(int)); // mémoire dynamique pour éviter les conflits entre threads
*pclient = client_fd;

pthread_t tid;
pthread_create(&tid, NULL, thread_handler, pclient);
pthread_detach(tid); // libère automatiquement les ressources du thread à la fin

    }
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_read;

    // Lecture de la requête client
    bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) {
        close(client_socket);
        return;
    }

    buffer[bytes_read] = '\0';

    char method[16], path[1024];
    if (sscanf(buffer, "%15s %1023s", method, path) != 2) {
    fprintf(stderr, "Requête invalide : %s\n", buffer);
    close(client_socket);
    return;
}


if (strcmp(method, "GET") == 0) {
    char response_buffer[MAX_RESPONSE_SIZE];
    int response_size;

    if (find_in_cache(path, response_buffer, &response_size)) {
        printf("[CACHE] Réponse servie depuis le cache pour %s\n", path);
        send(client_socket, response_buffer, response_size, 0);
        close(client_socket);
        return;
    }
}


    // Chercher le header Host:
    char hostname[256] = "";
    char *host_header = strstr(buffer, "Host: ");
    if (host_header) {
        sscanf(host_header, "Host: %255s", hostname);
    } else {
        fprintf(stderr, "[ERREUR] Aucun Host trouvé\n");
        close(client_socket);
        return;
    }

    // Résoudre l'IP
   struct addrinfo hints, *res, *p;
int server_socket;
int connected = 0;

memset(&hints, 0, sizeof(hints));
hints.ai_family = AF_UNSPEC; // AF_INET pour IPv4 uniquement, sinon AF_UNSPEC pour IPv4 + IPv6
hints.ai_socktype = SOCK_STREAM;

char port_str[6];
snprintf(port_str, sizeof(port_str), "%d", 80); // Port HTTP en string

if (getaddrinfo(hostname, port_str, &hints, &res) != 0) {
    perror("getaddrinfo");
    close(client_socket);
    return;
}

// Parcourt les adresses jusqu'à trouver une valide
for (p = res; p != NULL; p = p->ai_next) {
    server_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (server_socket == -1) continue;

    if (connect(server_socket, p->ai_addr, p->ai_addrlen) == 0) {
        connected = 1;
        break; // Succès
    }

    close(server_socket);
}

freeaddrinfo(res); // Libère la mémoire

if (!connected) {
    perror("connect");
    close(client_socket);
    return;
}

    // Envoyer la requête au serveur
    send(server_socket, buffer, bytes_read, 0);

    // Variables pour code HTTP et contrôle
    int code_http = 0;
    int header_parsed = 0;

    // Réponse du serveur → client
    while ((bytes_read = recv(server_socket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_read] = '\0';

        if (!header_parsed) {
            // Parse la ligne de status HTTP : ex "HTTP/1.1 200 OK"
            char http_version[16];
            sscanf(buffer, "%15s %d", http_version, &code_http);
            header_parsed = 1;
        }

        send(client_socket, buffer, bytes_read, 0);

        if (strcmp(method, "GET") == 0 && code_http == 301) {
    add_to_cache(path, buffer, bytes_read); // tu peux améliorer pour stocker toute la réponse
}

    }

    log_request(method, hostname, code_http);
    printf("Logging: %s %s %d\n", method, hostname, code_http);
log_request(method, hostname, code_http);


    close(server_socket);
    close(client_socket);
}



