#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define MAX_SIZE 1024

char *gcvt(double, int, char*); //deklarace funkce, kvuli erroru pri prrekladu

/**
 * funkce pro zpracovani argumentu
 * @param  argc
 * @param  argv
 * @return -1 pokud je spatny format argumentu
 * @return cislo portu, ktery byl zadan uzivatelem
 */
int argumentHandler(int argc, char const *argv[])
{
  if(argc == 2)
  {
    return htons(atoi(argv[1]));  //prevede argument do patricneho formatu
  }
  else
  {
    return -1;
  }
}

/**
 * funkce zapise hostname do protokolu
 * @param clientSocket
 */
void getHostName(int clientSocket)
{
  char hostname[MAX_SIZE];
  FILE *fp;
  fp = fopen("/proc/sys/kernel/hostname","r");
  if(fp == NULL)
  {
    fclose(fp);
    perror("ERROR in opening /proc/sys/kernel/hostname");
    exit(EXIT_FAILURE);
  }
  if(fgets(hostname, MAX_SIZE, fp) != NULL)
  {
    write(clientSocket, hostname, strlen(hostname));
  }
  else
  {
    fclose(fp);
    perror("ERROR in reading line from hostname");
    exit(EXIT_FAILURE);
  }
  fclose(fp);
}

/**
 * funkce zapise nazev CPU do protokolu
 * @param clientSocket
 */
void getCPUName(int clientSocket)
{
  FILE *fp;
  fp = fopen("/proc/cpuinfo","r");
  if(fp == NULL)
  {
    fclose(fp);
    perror("ERROR in opening /proc/cpuinfo");
    exit(EXIT_FAILURE);
  }

  char CPUName[MAX_SIZE];
  char c = fgetc(fp);
  int lineCnt = 1;

  while(c != '\0')  //nacita do konce souboru
  {
    if(c == '\n') //pricita cislo radku
    {
      lineCnt++;
    }
    if(lineCnt == 5)  //nazev procesoru je na 5. radku
    {
      while(c != ':')
      {
        c = fgetc(fp);
      }
      c = fgetc(fp);  // ':'
      c = fgetc(fp);  // mezera
      break;
    }
    c = fgetc(fp);
  }

  for(int i = 0; c != '\n'; i++) //nacte nazev po : a mezere
  {
    CPUName[i] = c;
    c = fgetc(fp);
  }
  write(clientSocket,CPUName, strlen(CPUName));
  fclose(fp);
}
/**
 * Funkce pro vraceni statistik, ktere se vyuzivaji pri vypoctu
 * zateze CPU
 * @param prevIdle
 * @param prevNonIdle
 * @param prevTotal
 */
void getCPULoadStats(unsigned long long int *prevIdle, unsigned long long int *prevNonIdle, unsigned long long int *prevTotal)
{
  FILE *fp;
  fp = fopen("/proc/stat","r");
  if(fp == NULL)
  {
    fclose(fp);
    perror("ERROR in opening /proc/stat");
    exit(EXIT_FAILURE);
  }

  char CPULoadBuffer[MAX_SIZE];
  unsigned long long int user, nice, system, idle, iowait, irq, softirq, steal,
   guest, guest_nice;

  if(fgets(CPULoadBuffer, MAX_SIZE, fp) != NULL)  //ziska radek z adresare
  {
    //ziska hodnoty z bufferu a rozdeli je do promenych
    sscanf(CPULoadBuffer,"cpu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu",
    &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);

    *prevIdle = idle + iowait;
    *prevNonIdle = user + nice + system + irq + softirq + steal;
    *prevTotal = idle + *prevNonIdle;

  }
  else
  {
    fclose(fp);
    perror("ERROR in reading line from /proc/stat");
    exit(EXIT_FAILURE);
  }
  fclose(fp);
}

/**
 * funkce pro vypocet zateze CPU
 * @param clientSocket
 */
void getCPULoad(int clientSocket)
{
  unsigned long long int idle, nonIdle, total;
  unsigned long long int prevIdle, prevNonIdle, prevTotal;
  unsigned long long int totald, idled;
  double CPUPer;
  char CPUBuffer[MAX_SIZE];

  //vypocet ze stranky, ktera byla prilozena k zadani
  getCPULoadStats(&prevIdle, &prevNonIdle, &prevTotal);
  sleep(1); //pozastaveni, kvuli hodnotam
  getCPULoadStats(&idle, &nonIdle, &total);

  totald = total - prevTotal;
  idled = idle - prevIdle;

  CPUPer = ( ((float)totald - (float)idled) / (float)totald) * 100;

  //zapis do vystupniho bufferu
  gcvt(CPUPer, 4, CPUBuffer);
  CPUBuffer[5] = '%';
  CPUBuffer[6] = '\n';
  CPUBuffer[7] = '\0';

  write(clientSocket, CPUBuffer, strlen(CPUBuffer));
}

/**
 * funkce, ktera zpracuje pozadavek od klienta
 * @param clientSocket
 */
void whichCommand(int clientSocket)
{
  char header[MAX_SIZE] = "HTTP/1.1 200 OK\r\nContent-type: text/plain;\r\n\r\n";
  char clientSocketContent[MAX_SIZE];

  if(read(clientSocket, clientSocketContent, sizeof(clientSocketContent)) < 0)
  {
    perror("ERROR in client socket read");
    exit(EXIT_FAILURE);
  }

  if(!strncmp(clientSocketContent,"GET /hostname ", 14))
  {
      write(clientSocket,header, strlen(header));
      getHostName(clientSocket);
  }
  else if(!strncmp(clientSocketContent,"GET /cpu-name ", 14))
  {
      write(clientSocket,header, strlen(header));
      getCPUName(clientSocket);
  }
  else if(!strncmp(clientSocketContent,"GET /load ", 10))
  {
      write(clientSocket,header, strlen(header));
      getCPULoad(clientSocket);
  }
  else  //neznamy pozadavek
  {
      write(clientSocket, "HTTP/1.1 400 Bad Request\r\n\r\n", strlen("HTTP/1.1 400 Bad Request\r\n\r\n"));
  }
}

int main(int argc, char const *argv[]) {

  int port = argumentHandler(argc, argv);
  if(port == -1)  //pokud je spatne zadany argument
  {
    fprintf(stderr,"wrong arguments\n");
    return 1;
  }

  int clientSocket, serverSocket;

  //nastaveni adresy podle prezentaci
  struct sockaddr_in clientAdress, serverAdress;
  serverAdress.sin_family = AF_INET;
  serverAdress.sin_port = port;
  serverAdress.sin_addr.s_addr = INADDR_ANY;

  //server soket
  //vytvoreni server soketu
  if((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)
  {
    perror("ERROR in server socket creation");
    exit(EXIT_FAILURE);
  }
  //bind na localport
  if(bind(serverSocket, (struct sockaddr *) &serverAdress, sizeof(serverAdress)) < 0)
  {
    perror("ERROR in server socket bind");
    exit(EXIT_FAILURE);
  }

  if(listen(serverSocket, 0) < 0)
  {
    perror("ERROR in server socket listen");
    exit(EXIT_FAILURE);
  }

  //klient soket
  //vytvoreni klient soketu
  if((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) <= 0)
  {
    perror("ERROR in client socket creation");
    exit(EXIT_FAILURE);
  }

  socklen_t clientAdressLen = sizeof(clientAdress);

  //hlavni cyklus, ktery se zastavi po stisku CTRL+C
  while(1)
  {
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAdress, &clientAdressLen);
    if(clientSocket)
    {
      whichCommand(clientSocket);
    }
    else
    {
      perror("ERROR in accepting client socket");
      exit(EXIT_FAILURE);
    }
    close(clientSocket);
  }

  return 0;
}
