#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include<fcntl.h>
/* portul folosit */
#define PORT 2014

/* codul de eroare returnat de anumite apeluri */
extern int errno;

int lng(int numar)
{
if(numar%10==0)
numar++;
int rezultat=0;
do{
rezultat++;
numar/=10;
}while(numar>0);
return rezultat;
}

void intToChar(char caracter[10],int numar)
{
int lungime=lng(numar);

for(int i=lungime-1;i>=0;i--)
{
caracter[i]=numar%10+48;
numar/=10;
}
caracter[lungime]='\0';
}

int charToInt(char caracter[10])
{
int rezultat=0;
for(int i=0;i<strlen(caracter);i++)
rezultat=rezultat*10+(caracter[i]-48);

return rezultat;
}

void desparteInComenzi(char mesaj[1000],char comenzi[10][20],int nrSpatii)
{
int N=0,j=0;
for(int i=0;i<strlen(mesaj);i++)
{
if(N==nrSpatii)
break;
if(mesaj[i]==' ')
{
comenzi[N][j]='\0';
j=0;
N++;
}
else
{
comenzi[N][j]=mesaj[i];
j++;
}}}

int nrSpatii(char mesaj[1000])
{
int i,space=0;

for(i=0;i<strlen(mesaj);i++)
if(mesaj[i]==' ')
space++;
else
if(mesaj[i]=='"')
break;

return space;
}

void copyFromToEnd(char *to,char *from,int n)
{
int j=0;
for(int i=n;i<strlen(from);i++,j++)
to[j]=from[i];
to[j]='\0';
}

int modificareCursorDirectii(int cursor,char* text)
{
if(strlen(text)==0)
return cursor;
int i=0,poz=0,sp=0,neg=0;
while(i<strlen(text))
{
while(text[i]!=' ')
{
poz=10*poz+text[i]-48;
i++;
}
i++;
while(text[i]!=' ')
{
if(text[i]=='-')
neg=1;
else 
sp=sp*10+text[i]-48;
i++;
}
i++;
if(neg)
sp=-sp;
if(sp>=0)
{
if(poz<=cursor)
cursor=sp+cursor;
}
else
{
if(poz<=cursor)
cursor=cursor+sp;
else
if(poz+sp<=cursor)
cursor=cursor-(cursor-poz+sp);
}
poz=0;
sp=0;
neg=0;

}
return cursor;
}

int verificareComandaCursor(char comenzi[10][20],int n,int cursor)
{

if(strcmp(comenzi[0],"setCursor")==0)
{
if(strcmp(comenzi[1],"start")==0)
return 0;
if(strcmp(comenzi[1],"end")==0)
return -1;
if(comenzi[1][0]>='0' && comenzi[1][0]<='9')
return charToInt(comenzi[1]);
if(comenzi[1][0]=='+')
{
char aux[10];
copyFromToEnd(aux,comenzi[1],1);
return cursor+charToInt(aux);
}
if(comenzi[1][0]=='-')
{
char aux[10];
copyFromToEnd(aux,comenzi[1],1);
return cursor-charToInt(aux);
}
}
return cursor;
}

int verificareComandaFisier(char comenzi[10][20],int lungime)
{
if(strcmp(comenzi[2],"open")==0)
return open(comenzi[3],O_RDONLY);
else
if(strcmp(comenzi[2],"create")==0)
return open(comenzi[3],O_WRONLY|O_TRUNC|O_CREAT,00777);
else
return -1;
}

void sterge(char *text,int pozitie)
{
for(;pozitie<strlen(text);pozitie++)
text[pozitie]=text[pozitie+1];
}

void prelucrareMesaj(char* text)
{
for(int i=0;i<strlen(text);i++)
{
if(text[i]=='\\')
{
if(text[i+1]=='n')
{
text[i]='\n';
sterge(text,i+1);
}
else if(text[i+1]=='t')
{
text[i]='\t';
sterge(text,i+1);
}}
}
}



int primulChar(char *text,char c)
{
for(int i=0;i<strlen(text);i++)
if(text[i]==c)
return i;
return -1;
}

void insert(char* text,int pozitie,char c)
{
for(int i=strlen(text)+1;i>pozitie;i--)
{
text[i]=text[i-1];
}
text[pozitie]=c;
}

void overwrite(char *text,int pozitie,char c)
{
text[pozitie]=c;
}

void  SIGQUIT_handler(int sig)
{
     exit(1);
}

int main ()
{
  struct sockaddr_in server;
  struct sockaddr_in from;
  char msg[100]; 
  char msgrasp[100]=" ";
  int sd;
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }

  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);
  
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

    if (listen (sd, 5) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }

  
  while (1)
    {
int ruleaza=1;
      int client1,client2;
      int length = sizeof (from);

      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);

      client1 = accept (sd, (struct sockaddr *) &from, &length);
      client2 = accept (sd, (struct sockaddr *) &from, &length);
if(!fork())
{

int input[2],dir1[2],dir2[2];
pipe(dir1);
pipe(dir2);
pipe(input);
int flags=fcntl(dir1[0],F_GETFL,0);
flags=flags|O_NONBLOCK;
fcntl(dir1[0],F_SETFL,flags);
flags=fcntl(dir2[0],F_GETFL,0);
flags=flags|O_NONBLOCK;
fcntl(dir2[0],F_SETFL,flags);


int coord,fiu1,fiu2;
if(fiu1=fork())
{
if(fiu2=fork())
{
close(dir1[0]);
close(dir2[0]);
close(input[1]);
coord=0;
}
else
{
close(dir1[1]);
close(dir2[1]);
close(dir1[0]);
close(input[0]);
coord=2;
}
}
else
{
close(dir1[1]);
close(dir2[1]);
close(dir2[0]);
close(input[0]);
coord=1;
}

if(coord==1)
{
int cursor=0;
while(1)
{
char mesaj[1000],caracter[10],mesajFinal[1000],nr[2],comenzi[10][20],directii[100];
bzero(mesaj,1000);
read(client1,mesaj,1000);
mesaj[strlen(mesaj)]=' ';
bzero(directii,100);
read(dir1[0],directii,100);

cursor=modificareCursorDirectii(cursor,directii);

bzero(mesajFinal,1000);
desparteInComenzi(mesaj,comenzi,nrSpatii(mesaj));
cursor=verificareComandaCursor(comenzi,nrSpatii(mesaj),cursor);
strcat(mesajFinal,"1 ");
if(cursor>=0)
{
bzero(caracter,10);
intToChar(caracter,cursor);
strcat(mesajFinal,caracter);
}
else
strcat(mesajFinal,"F");
strcat(mesajFinal," ");
strcat(mesajFinal,mesaj);
strcat(mesajFinal," ");
if(strcmp(comenzi[0],"setCursor")!=0)
write(input[1],mesajFinal,1000);
bzero(mesaj,1000);
}
}
if(coord==2)
{
int cursor=0;
while(1)
{
char mesaj[1000],caracter[10],mesajFinal[1000],nr[2],comenzi[10][20],directii[100];
bzero(mesaj,1000);
read(client2,mesaj,1000);
mesaj[strlen(mesaj)]=' ';
bzero(directii,100);
read(dir2[0],directii,100);


cursor=modificareCursorDirectii(cursor,directii);

bzero(mesajFinal,1000);
desparteInComenzi(mesaj,comenzi,nrSpatii(mesaj));
cursor=verificareComandaCursor(comenzi,nrSpatii(mesaj),cursor);
strcat(mesajFinal,"2 ");
if(cursor>=0)
{
bzero(caracter,10);
intToChar(caracter,cursor);
strcat(mesajFinal,caracter);
}
else
strcat(mesajFinal,"F");
strcat(mesajFinal," ");
strcat(mesajFinal,mesaj);
if(strcmp(comenzi[0],"setCursor")!=0)
write(input[1],mesajFinal,1000);
bzero(mesaj,1000);
}
}
if(!coord)
{
int g=0,fisier=0,descFis=-1;
char continutFisier[10000],directii[10],comenzi[10][20];
while(1)
{
char mesaj[1000];
bzero(mesaj,1000);
bzero(directii,10);
read(input[0],mesaj,1000);

printf("%s",mesaj);
printf(" Parinte\n");
int lungime=nrSpatii(mesaj);
desparteInComenzi(mesaj,comenzi,lungime);
if(!fisier)
{
descFis=verificareComandaFisier(comenzi,lungime);
if(descFis!=-1)
	{
	if(strcmp(comenzi[2],"create")==0)
		{
		fisier=1;
		bzero(continutFisier,10000);
		}
	else
		{
		fisier=2;
		bzero(continutFisier,10000);
		read(descFis,continutFisier,10000);
		close(descFis);
		descFis=open(comenzi[3],O_TRUNC|O_WRONLY);
		}
	}
else
	{
        if(comenzi[0][0]=='1')
		write(client1,"Nu ai deschis un fisier cu succes",35);
	else
		write(client2,"Nu ai deschis un fisier cu succes",35);
	}
}
else
{

int cursor;
if(comenzi[1][0]=='F')
cursor=strlen(continutFisier);
else
cursor=charToInt(comenzi[1]);
if(strcmp(comenzi[2],"download")==0)
	{
        if(comenzi[0][0]=='1')
                write(client1,continutFisier,strlen(continutFisier));
        else
                write(client2,continutFisier,strlen(continutFisier));
	}
if(strcmp(comenzi[2],"delete")==0)
	{
	int deSters=charToInt(comenzi[3]);
	while(cursor>=0 && deSters>0)
		{
		sterge(continutFisier,cursor);
		cursor--;
		deSters--;
		}
	strcat(directii,comenzi[1]);
	strcat(directii," -");
	char aux[10];
	intToChar(aux,charToInt(comenzi[3])-deSters);
	strcat(directii,aux);
	strcat(directii," ");
	}
if(strcmp(comenzi[2],"insert")==0)
	{
	int inceput=primulChar(mesaj,'"');
	if(inceput!=-1)
	{
	char textDeInserat[1000];
	copyFromToEnd(textDeInserat,mesaj,inceput+1);
	textDeInserat[strlen(textDeInserat)-3]='\0';
	prelucrareMesaj(textDeInserat);
	for(int i=0;i<strlen(textDeInserat);i++)
		insert(continutFisier,cursor+i,textDeInserat[i]);
	strcat(directii,comenzi[1]);
        strcat(directii," ");
	char aux[10];
	intToChar(aux,strlen(textDeInserat));
        strcat(directii,aux);
	strcat(directii," ");
	}
	}
if(strcmp(comenzi[2],"overwrite")==0)
	{
        int inceput=primulChar(mesaj,'"'),trecutFinal=0;
	if(inceput!=-1)
	{
	char textDeScris[1000];
	copyFromToEnd(textDeScris,mesaj,inceput+1);
        textDeScris[strlen(textDeScris)-2]='\0';
	prelucrareMesaj(textDeScris);
	if(cursor+strlen(textDeScris)>strlen(continutFisier))
        trecutFinal=1;

        for(int i=0;i<strlen(textDeScris);i++)
                overwrite(continutFisier,cursor+i,textDeScris[i]);
	if(trecutFinal)
		continutFisier[cursor+strlen(continutFisier)]='\0';
	}
	}
if(strlen(directii)>0)
{
	write(dir2[1],directii,strlen(directii));
	write(dir1[1],directii,strlen(directii));
}
}
if(strcmp(comenzi[2],"quit")==0)
	{
	write(client1,"quit",5);
	write(client2,"quit",5);
	kill(fiu1,SIGQUIT);
	kill(fiu2,SIGQUIT);
	write(descFis,continutFisier,strlen(continutFisier));
	close(descFis);
	close(client1);
	close(client2);
	exit(1);
	}


}
}


exit(0);
}
close(client1);
close(client2);

      }
}

