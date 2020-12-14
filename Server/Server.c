#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{

	int dss, dsc;
	char *pseudonames[20];
	for (int counter = 0; counter < 20; counter++)
	{
		pseudonames[counter] = calloc(1024, sizeof(char));
	}

	int clientlen;

	int unique = 0;
	int counter = 19;

	struct sockaddr_in srvAddr, cltAddr;

	int dsclt[20];
	int n, cptclt, maxDS, i, j;
	fd_set readfds, readAllfd;
	char *msg = calloc(1024, sizeof(char));
	char *tmp = calloc(2048, sizeof(char));
	dss = socket(AF_INET, SOCK_STREAM, 0);
	memset((char *)&srvAddr, '\0', sizeof(srvAddr));

	srvAddr.sin_family = AF_INET;
	srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvAddr.sin_port = htons(8084);

	bind(dss, (struct sockaddr *)&srvAddr, sizeof(srvAddr));
	listen(dss, 5);
	clientlen = sizeof(cltAddr);

	cptclt = 0;
	FD_ZERO(&readAllfd);
	FD_SET(dss, &readAllfd);
	maxDS = dss;

	while (1)
	{

		FD_ZERO(&readfds);
		readfds = readAllfd;
		select(maxDS + 1, &readfds, NULL, NULL, NULL);

		if (FD_ISSET(dss, &readfds))
		{

			dsc = accept(dss, (struct sockaddr *)&cltAddr, &clientlen);

			cptclt++;
			FD_SET(dsc, &readAllfd);
			dsclt[cptclt - 1] = dsc;

			char *welcome = calloc(1024, sizeof(char));
			strcpy(welcome, "saisir un pseudo nom\n");
			send(dsc, welcome, strlen(welcome), 0);
			memset(welcome, '\0', 1024);
			recv(dsc, welcome, 1024, 0);

			while (0 <= counter && strcmp(pseudonames[counter], welcome) != 0)
			{
				counter = counter - 1;
			}

			if (counter == -1)
			{
				unique = 1;
			}

			while (unique == 0)
			{
				strcpy(welcome, "Le nom choisi est déjà pris; veuillez le changer:\n");
				send(dsc, welcome, strlen(welcome), 0);
				memset(welcome, '\0', 1024);
				recv(dsc, welcome, 1024, 0);
				counter = 19;
				while (0 <= counter && strcmp(pseudonames[counter], welcome) != 0)
				{
					counter--;
				}
				if (counter == -1)
				{
					unique = 1;
				}
			}

			if (unique == 1)
			{
				printf("Le nom choisi :%s\n", welcome);
				strcpy(pseudonames[cptclt - 1], welcome);
			}

			unique = 0;

			if (dsc > maxDS)
				maxDS = dsc;
			printf("nouveau client communiquant via DS : %d in\n", dsc);
		}

		else
		{

			for (i = 0; i < cptclt; i++)
			{

				dsc = dsclt[i];

				if (FD_ISSET(dsc, &readfds))
				{

					memset(msg, '\0', 1024);
					n = recv(dsc, msg, 1024, 0);

					if (n == 0)
					{

						FD_CLR(dsc, &readAllfd);

						for (j = i; j < cptclt - 1; j++)
							dsclt[j] = dsclt[j + 1];
						cptclt--;
						close(dsc);

						if ((dsc == maxDS) && cptclt != 0)

							for (j = 1; j < cptclt; i++)

								if (dsclt[i] > maxDS)
									maxDS = dsclt[i];

						memset(msg, '\0', 1024);
						printf("Le client communiquant via DS : %d s'est deconnecte\n", dsc);
						break;
					}

					else if (n > 0)
					{
						for (int k = 0; k < cptclt; k++)
						{

							if (dsclt[k] != dsc)
							{
								printf("found\n");

								strcat(tmp, pseudonames[i]);
								strcat(tmp, " : ");
								strcat(tmp, msg);
								send(dsclt[k], tmp, strlen(tmp), 0);
								memset(tmp, '\0', 2048);
							}
						}
						printf("\n");
					}
				}
			}
		}
	}
	close(dss);
	exit(0);
}