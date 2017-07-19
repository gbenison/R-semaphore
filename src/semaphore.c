#include "config.h"

#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>

#ifndef HAVE_UNION_SEMUN
union semun {
  int val;
  struct semid_ds *buf;
  unsigned short int *array;
};
#endif

#define __MAX_N_SEMAPHORES 128
static int n_semaphores = 0;
static int semaphore_ids[__MAX_N_SEMAPHORES];

void cleanup()
{
  int i;
  for (i = 0; i < n_semaphores; ++i) {
    semctl(semaphore_ids[i], 0, IPC_RMID);
  }
}

void semaphore_change(int * semaphore_id, int * delta)
{
  struct sembuf tmp;
  tmp.sem_num = 0;
  tmp.sem_flg = 0;
  tmp.sem_op = *delta;
  int retval = semop(*semaphore_id, &tmp, 1);
  assert(retval != -1);
}

void semaphore_create(int * semaphore_id, int * initial_value)
{
  assert(n_semaphores < __MAX_N_SEMAPHORES);
  if (n_semaphores == 0) {
    int retval = atexit(&cleanup);
    assert(retval == 0);
  }
  *semaphore_id = 0;
  *semaphore_id = semget(IPC_PRIVATE, 1, 0660);
  assert(*semaphore_id > 0);
  semaphore_ids[n_semaphores] = *semaphore_id;
  ++n_semaphores;

  union semun tmp;
  tmp.val=*initial_value;
  semctl(*semaphore_id, 0, SETVAL, tmp);
}



