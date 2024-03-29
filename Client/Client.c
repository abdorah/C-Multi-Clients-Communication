#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void check(int test, char *message_err);

int main(int argc, char *argv[])
{
    int dsc, inputstd = 0, maxDS;
    char *message = calloc(1024, sizeof(char));
    char *message2 = calloc(1024, sizeof(char));
    fd_set readfds, readAllfd;

    struct sockaddr_in adrsrv;
    adrsrv.sin_family = AF_INET;
    adrsrv.sin_port = htons(8084);
    inet_aton("127.0.0.1", &(adrsrv.sin_addr));
    memset(&(adrsrv.sin_zero), '0', 8);

    dsc = socket(AF_INET, SOCK_STREAM, 0);

    check(dsc, "socket err");
    check(connect(dsc, (struct sockaddr *)&adrsrv, sizeof(struct sockaddr)), "connection err");

    FD_ZERO(&readAllfd);
    FD_SET(inputstd, &readAllfd);
    FD_SET(dsc, &readAllfd);
    maxDS = dsc;
    while (1)
    {

        FD_ZERO(&readAllfd);
        FD_SET(inputstd, &readAllfd);
        FD_SET(dsc, &readAllfd);
        maxDS = dsc;

        select(maxDS + 1, &readAllfd, NULL, NULL, NULL);

        if (FD_ISSET(inputstd, &readAllfd))
        {
            memset(message, '\0', 1024);
            fgets(message, 1024, stdin);
            send(dsc, message, strlen(message), 0);
            memset(message, '\0', 1024);
        }
        else if (FD_ISSET(dsc, &readAllfd))
        {
            recv(dsc, message2, 1024, 0);
            printf("%s\n", message2);
            memset(message2, '\0', 1024);
        }
    }
}

void check(int test, char *message_err)
{
    if (test == -1)
    {
        perror(message_err);
        exit(1);
    }
}