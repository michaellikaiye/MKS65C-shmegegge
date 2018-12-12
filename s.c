#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 200  /* make it a 200 shared memory segment */

int main(int argc, char *argv[]) {
  key_t key;
  int shmid;
  char *data;
  int mode;

  if (argc > 2) {
    fprintf(stderr, "usage: shmdemo [data_to_write]\n");
    exit(1);
  }

  /* make the key: */
  if ((key = ftok("s.c", 'R')) == -1) {
    perror("ftok");
    exit(1);
  }
  else
    printf("key: %d\n", key);
    
  /* connect to (and possibly create) the segment: */
  if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
    perror("shmget");
    exit(1);
  }
  else
    printf("id: %d\n", shmid);

  /* attach to the segment to get a pointer to it: */
  data = shmat(shmid, (void *)0, 0);
  if (data == (char *)(-1)) {
    perror("shmat");
    exit(1);
  }
  
  /* read or modify the segment, based on the command line: */

  if (!*data){
    printf("new shared memory segment\n");
  }
  else
    printf("segment contains: \n%s\n", data);

  printf("change shared memory segment? (y/n):\n");
  char line[SHM_SIZE];
  fgets(line, SHM_SIZE, stdin);
  if (line[0] == 'y'){
    printf("Enter the new data:\n");
    fgets(line, SHM_SIZE, stdin);
    strcpy(data,line);
    printf("segment contains: \n%s\n", data);
  }
  else if(*line == 'n'){
    printf("ok\n");
  }

  /* detach from the segment: */
  if (shmdt(data) == -1) {
    perror("shmdt");
    exit(1);
  }
  else {
    printf("delete shared memory segment? (y/n):\n");
    fgets(line, SHM_SIZE, stdin);
    if (line[0] == 'y'){
      shmctl(shmid, IPC_RMID, NULL);
      printf("deleted\n");
    }
    else if(*line == 'n'){
      printf("ok\n");
    }
  }
    
  return 0;
}
