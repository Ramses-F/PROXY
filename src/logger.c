#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logger.h"

// Couleurs ANSI pour le terminal
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_RED "\x1b[31m"
#define COLOR_RESET "\x1b[0m"

void log_request(const char *method, const char *host, int status_code) {
    FILE *logfile = fopen("proxy.log", "a");
    if (!logfile) return;

    // Timestamp
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);

    // Couleur console
    const char *color;
    if (status_code >= 200 && status_code < 300)
        color = COLOR_GREEN;
    else if (status_code >= 300 && status_code < 400)
        color = COLOR_YELLOW;
    else
        color = COLOR_RED;

    // Console
    printf("%s[INFO] %s | Méthode: %s | Hôte: %s | Code HTTP: %d%s\n",
           color, time_str, method, host, status_code, COLOR_RESET);

    // Fichier log
    fprintf(logfile, "[%s] Méthode: %s | Hôte: %s | Code HTTP: %d\n",
            time_str, method, host, status_code);
        fflush(logfile);

    fclose(logfile);
}
