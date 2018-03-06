#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include<signal.h>
extern int errno;

int port;

void  SIGQUIT_handler(int sig)
{
     printf("Mor ba");
     exit(0);
}
int main (int argc, char *argv[])
{
char nume[20];

 int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  char msg[100];		// mesajul trimis

  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  port = atoi (argv[2]);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

 /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

int continua=1,fiu;
if(!(fiu=fork()))
{
while(continua)
{
  /* citirea mesajului */
  char mesaj[1000],mesajIntreg[100];
bzero(mesaj,1000);
scanf ("%[^\n]%*c", mesaj);
  /* trimiterea mesajului la server */
    if (write (sd, mesaj, 1000) <= 0)
    {
      printf("[client]Eroare la write() spre server.\n");
      return errno;
    }
}}
else {
while(continua)
{
char mesaj[10000];
bzero(mesaj,10000);
   if (read (sd, mesaj, 10000) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
  if(strstr(mesaj,"quit")!=NULL)
{
printf("Intrerup comunicarea\n");
kill(fiu,SIGQUIT);
break;
}
  printf ("%s\n", mesaj);
}
}

  /* inchidem conexiunea, am terminat */
  close (sd);
}



