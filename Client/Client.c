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

//fonction to show a message in case of error
void check(int test, char *message_err)
{
  if (test == -1)
  {
    perror(message_err);
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  int dsc, inputstd = 0, maxDS;
  char *message = calloc(1024, sizeof(char));
  char *message2 = calloc(1024, sizeof(char));
  fd_set readfds, readAllfd;

  struct sockaddr_in adrsrv;
  adrsrv.sin_family = AF_INET;

  //configuration of the connection to the server
  adrsrv.sin_port = htons(8084);
  inet_aton("127.0.0.1", &(adrsrv.sin_addr));
  memset(&(adrsrv.sin_zero), '0', 8);

  dsc = socket(AF_INET, SOCK_STREAM, 0);

  check(dsc, "erreur lors du chargement de socket");
  check(connect(dsc, (struct sockaddr *)&adrsrv, sizeof(struct sockaddr)), "erreur de connection");

  FD_ZERO(&readAllfd);
  FD_SET(inputstd, &readAllfd);
  FD_SET(dsc, &readAllfd);
  maxDS = dsc;
  //start listening on the provided address and port
  while (1)
  {
    FD_ZERO(&readAllfd);
    FD_SET(inputstd, &readAllfd);
    FD_SET(dsc, &readAllfd);
    maxDS = dsc;

    select(maxDS + 1, &readAllfd, NULL, NULL, NULL);

	//send event
    if (FD_ISSET(inputstd, &readAllfd))
    {
      memset(message, '\0', 1024);
      fgets(message, 1024, stdin);
      send(dsc, message, strlen(message), 0);
      memset(message, '\0', 1024);
    }
	//receive event
    else if (FD_ISSET(dsc, &readAllfd))
    {
      recv(dsc, message2, 1024, 0);
      printf("%s\n", message2);
      memset(message2, '\0', 1024);
    }
  }
}