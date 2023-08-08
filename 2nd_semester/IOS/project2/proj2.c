#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <unistd.h>


FILE *soubor; //output file
time_t t;//pro generovani random cisel

float elf_time = 0;
int cislo_ulohy = 0, elf_waiting = 0, raindeer_num = 0, raindeer_arrived = 0;
int *cislo_ulohy_pointer = 0, *elf_waiting_pointer = 0, *raindeer_num_pointer = 0, *raindeer_arrived_pointer = 0; //sdilena pamet
sem_t *s_santa_xplick04, *s_elf_xplick04, *s_santa_help_xplick04, *s_raindeer_home_xplick04; //semafory

int argument_handle(int argc, char *argv[])
{
  if(argc != 5)
  {
    fprintf(stderr, "Spatne zadany pocet argumentu\n");
    return 1;
  }

  for(int i = 1; i < argc; i++)
  {
    if(0 > atoi(argv[i]))
    {
      fprintf(stderr, "Argument %d je zaporne cislo\n",i);
      return 1;
    }
    if(!isdigit(*argv[i]))
    {
      fprintf(stderr, "Argument %d neni cislo\n",i);
      return 1;
    }
    if(atof(argv[i]) - atoi(argv[i]) != 0 )
    {
      fprintf(stderr, "Argument %d neni cele cislo\n",i);
      return 1;
    }
  }
  if(atoi(argv[1]) == 0 || atoi(argv[2]) == 0)
  {
    fprintf(stderr, "Pocet skritku/sobu nesmi byt 0\n");
    return 1;
  }
  if(atoi(argv[3]) > 1000 || atoi(argv[4]) > 1000)
  {
    fprintf(stderr, "Zadany interval je vetsi nez 1000\n");
    return 1;
  }
  if(atoi(argv[1]) > 1000 || atoi(argv[2]) > 20)
  {
    fprintf(stderr, "Spatny pocet sobu/skritku\n");
    return 1;
  }
  return 0;
}

float generate_number_elf(int id, int elf_max)
{
  srand(time(&t) + (id));//+id pro vetsi nahodnost
  return (rand() % (elf_max + 1));
}

float generate_number_raindeer(int id, int raindeer_max)
{
  srand(time(&t) + (id));//+id pro vetsi nahodnost
  return ((rand() % (raindeer_max/2)) + raindeer_max/2);// vybere nahodne cislo <0,TR/2> a pricte TR/2 = <TR/2,TR>
}

void elf(int elf_id,int elf_max)
{
  (*cislo_ulohy_pointer)++;
  fprintf(soubor,"%d: Elf %d: started\n",*cislo_ulohy_pointer, elf_id); //elf start
  fflush(NULL);
  usleep(generate_number_elf(elf_id, elf_max)); //simulovani prace elfa
  (*elf_waiting_pointer)++;
  (*cislo_ulohy_pointer)++;
  fprintf(soubor,"%d: Elf %d: need help\n",*cislo_ulohy_pointer, elf_id);
  fflush(NULL);
  sem_post(s_santa_xplick04);
  sem_wait(s_elf_xplick04); //cekani na pomoc od santy
  (*elf_waiting_pointer)--;
  (*cislo_ulohy_pointer)++;
  fprintf(soubor,"%d: Elf %d: get help\n",*cislo_ulohy_pointer, elf_id);
  fflush(NULL);
  sem_post(s_santa_help_xplick04); //rekne santovi, ze skritek dostal pomoc a muze jit spat, pokud potrebuje
}

void santa(int raindeer_total, int elves_total)
{
  bool awake = false;
  (*cislo_ulohy_pointer)++;
  fprintf(soubor,"%d: Santa: going to sleep\n",*cislo_ulohy_pointer);
  fflush(NULL);
  while(true)
  {
    if(*raindeer_arrived_pointer == raindeer_total) //ukonceni
    {
      (*cislo_ulohy_pointer)++;
      fprintf(soubor,"%d: Santa: closing workshop\n",*cislo_ulohy_pointer);
      fflush(NULL);
      for(int i = 1; i <= (*raindeer_num_pointer); i++) //zaprazeni sobu
      {
        (*cislo_ulohy_pointer)++;
        fprintf(soubor,"%d: RD %d: get hitched\n",*cislo_ulohy_pointer, i);
        fflush(NULL);
      }
      for(int j = 1; j <= elves_total; j++) //odchazeni elfu na prazdniny
      {
        (*cislo_ulohy_pointer)++;
        fprintf(soubor,"%d: Elf %d: taking holiday\n",*cislo_ulohy_pointer, j);
        fflush(NULL);
      }
      (*cislo_ulohy_pointer)++;
      fprintf(soubor,"%d: Santa: Christmas started\n",*cislo_ulohy_pointer);
      fflush(NULL);
      break;
    }

      sem_wait(s_santa_xplick04); //ceka na needhelp nebo az prijedou vsichni sobove
      if(*elf_waiting_pointer >= 3)
      {
        if(awake == false) //probudi santu
        {
          (*cislo_ulohy_pointer)++;
          fprintf(soubor,"%d: Santa: helping elves\n",*cislo_ulohy_pointer);
          fflush(NULL);
          awake = true;
        }
        sem_post(s_elf_xplick04);
      }
      if(*elf_waiting_pointer < 3 && awake == true) //koncova podminka pro uspani santy
      {
        sem_wait(s_santa_help_xplick04);//ceka na signal od skritka, ze dostal pomoc
        (*cislo_ulohy_pointer)++;
        fprintf(soubor,"%d: Santa: going to sleep\n",*cislo_ulohy_pointer);
        fflush(NULL);
        awake = false;
      }
  }
}

void raindeers(int raindeer_id, int raindeer_max, int raindeer_count)
{
  (*raindeer_num_pointer)++;
  (*cislo_ulohy_pointer)++;
  fprintf(soubor,"%d: RD %d: rstarted\n",*cislo_ulohy_pointer, raindeer_id);
  fflush(NULL);
  usleep(generate_number_raindeer(raindeer_id, raindeer_max));
  (*cislo_ulohy_pointer)++;
  fprintf(soubor,"%d: RD %d: return home\n",*cislo_ulohy_pointer, raindeer_id);
  fflush(NULL);
  (*raindeer_arrived_pointer)++;
  if(*raindeer_arrived_pointer == raindeer_count)//pokud pojede posledni cyklus a elfove uz by nepousteli santu
  {
    sem_post(s_santa_xplick04);
  }
}

void shared_memory_destroy()
{
  shmdt(cislo_ulohy_pointer); //smazani pameti
  if((shmctl(cislo_ulohy, IPC_RMID, NULL)) == -1)
  {
    fprintf(stderr, "chyba pri mazani pameti\n");
    exit(1);
  }

  shmdt(elf_waiting_pointer); //smazani pameti
  if((shmctl(elf_waiting, IPC_RMID, NULL)) == -1)
  {
    fprintf(stderr, "chyba pri mazani pameti\n");
    exit(1);
  }

  shmdt(raindeer_num_pointer); //smazani pameti
  if((shmctl(raindeer_num, IPC_RMID, NULL)) == -1)
  {
    fprintf(stderr, "chyba pri mazani pameti\n");
    exit(1);
  }

  shmdt(raindeer_arrived_pointer); //smazani pameti
  if((shmctl(raindeer_arrived, IPC_RMID, NULL)) == -1)
  {
    fprintf(stderr, "chyba pri mazani pameti\n");
    exit(1);
  }
}

void delete_sem()
{
  if((sem_destroy(s_santa_xplick04)) == -1)
  {
    fprintf(stderr,"Chyba pri mazani semaforu\n");
    exit(1);
  }

  if((sem_destroy(s_elf_xplick04)) == -1)
  {
    fprintf(stderr,"Chyba pri mazani semaforu\n");
    exit(1);
  }

  if((sem_destroy(s_santa_help_xplick04)) == -1)
  {
    fprintf(stderr,"Chyba pri mazani semaforu\n");
    exit(1);
  }
  if((sem_destroy(s_raindeer_home_xplick04)) == -1)
  {
    fprintf(stderr,"Chyba pri mazani semaforu\n");
    exit(1);
  }
}

void create_sem()
{
    if((s_santa_xplick04 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED)
    {
        fprintf(stderr,"Chyba pri vytvareni semaforu\n");
    }

    if((sem_init(s_santa_xplick04, 1, 0)) == -1)
    {
        shared_memory_destroy();
        fprintf(stderr,"Chyba pri vytvareni semaforu\n");
        exit(1);
    }
    //----------------------------------------------------------------------------------------------------------------
    if((s_elf_xplick04 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED)
    {
        fprintf(stderr,"Chyba pri vytvareni semaforu\n");
    }

    if((sem_init(s_elf_xplick04, 1, 0)) == -1)
    {
        shared_memory_destroy();
        fprintf(stderr,"Chyba pri vytvareni semaforu\n");
        exit(1);
    }
    //----------------------------------------------------------------------------------------------------------------
    if((s_santa_help_xplick04 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED)
    {
        fprintf(stderr,"Chyba pri vytvareni semaforu\n");
    }

    if((sem_init(s_santa_help_xplick04, 1, 0)) == -1)
    {
        shared_memory_destroy();
        fprintf(stderr,"Chyba pri vytvareni semaforu\n");
        exit(1);
    }
    //----------------------------------------------------------------------------------------------------------------
    if((s_raindeer_home_xplick04 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED)
    {
        fprintf(stderr,"Chyba pri vytvareni semaforu\n");
    }

    if((sem_init(s_raindeer_home_xplick04, 1, 0)) == -1)
    {
        shared_memory_destroy();
        fprintf(stderr,"Chyba pri vytvareni semaforu\n");
        exit(1);
    }
}

void shared_memory_create()
{

  cislo_ulohy = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
  if(cislo_ulohy == -1)
  {
    fprintf(stderr, "chyba ve vytvareni pameti\n");
    exit(1);
  }
  cislo_ulohy_pointer = (int *)shmat(cislo_ulohy, NULL, 0);
  if(cislo_ulohy_pointer == (int *)-1)
  {
    fprintf(stderr, "chyba ve vytvareni pameti\n");
    exit(1);
  }
//-------------------------------------------------------------------
  elf_waiting = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
  if(elf_waiting == -1)
  {
    fprintf(stderr, "chyba ve vytvareni pameti\n");
    exit(1);
  }
  elf_waiting_pointer = (int *)shmat(elf_waiting, NULL, 0);
  if(elf_waiting_pointer == (int *)-1)
  {
    fprintf(stderr, "chyba ve vytvareni pameti\n");
    exit(1);
  }
//-------------------------------------------------------------------
  raindeer_num = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
  if(raindeer_num == -1)
  {
    fprintf(stderr, "chyba ve vytvareni pameti\n");
    exit(1);
  }
  raindeer_num_pointer = (int *)shmat(raindeer_num, NULL, 0);
  if(raindeer_num_pointer == (int *)-1)
  {
    fprintf(stderr, "chyba ve vytvareni pameti\n");
    exit(1);
  }
  //-------------------------------------------------------------------
    raindeer_arrived = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    if(raindeer_arrived == -1)
    {
      fprintf(stderr, "chyba ve vytvareni pameti\n");
    }
    raindeer_arrived_pointer = (int *)shmat(raindeer_arrived, NULL, 0);
    if(raindeer_arrived_pointer == (int *)-1)
    {
      fprintf(stderr, "chyba ve vytvareni pameti\n");
    }
}


int main(int argc, char *argv[])
{
  if(argument_handle(argc, argv))
  {
    exit(1);
  }
  //Přiřazení argumentů do proměných
  int NE = atoi(argv[1]); //number of elves
  int NR = atoi(argv[2]); //number of raindeers
  int TE = atoi(argv[3]); //maximalni doba v ms, po kterou skritek pracuje solo
  int TR = atoi(argv[4]); //maximalni doba v ms, než se sob vrati z dovolene domu

  soubor = fopen("proj2.out","w"); //otevreni output souboru

  if(soubor == NULL)
  {
    fprintf(stderr, "Soubor se nepodarilo otevrit\n");
    exit(1);
  }

  shared_memory_create();
  create_sem();
  int pid;
  for(int i = 0; i < NE + 1; i++) //+1 = elfove + santa
  {
    pid = fork();
    if(pid == 0)  //fork child
    {
        if(i == 0)
        {
          santa(NR,NE);
          exit(0);
        }
        else
        {
          elf(i, TE);
          exit(0);
        }
    }
    if(pid == -1)//fork error
    {
      fprintf(stderr,"chyba forku\n");
      shared_memory_destroy();
      delete_sem();
      exit(1);
    }
    else//fork parent
    {}
  }

  for(int i = 1; i < NR + 1; i++) //sobove
  {
    pid = fork();
    if(pid == 0)  //fork child
    {
        raindeers(i, TR, NR);
        exit(0);
    }
    if(pid == -1)//fork error
    {
      fprintf(stderr,"chyba forku\n");
      shared_memory_destroy();
      delete_sem();
      exit(1);
    }
    else//fork parent
    {}
  }

  shared_memory_destroy();
  delete_sem();
  fclose(soubor);
  exit(0);
}
