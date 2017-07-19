# semaphore

Classic counting semaphores for R

R allows for parallel processing, for example with the [parallel](https://www.rdocumentation.org/packages/parallel/versions/3.4.1) package.  In such situations any of the standard inter-process synchronization mechanisms can be useful.  This package provides one of these; the classic [counting semaphore](https://en.wikipedia.org/wiki/Semaphore_(programming)).  These are useful any time a number of processes must share some resource of which a limited number are available.  Practical examples of shared limited resources might include database connections, ftp connections, temporary storage...

Consider the following example in which the shared resource is a file system where (for whatever reason) no more than two files should be written to at once.  Yet imagine that it makes sense to run many more than two processes at once, for example because there are more than two cores, and that each process only needs the shared resource for a fraction of its lifetime.  We can use a semaphore to guarantee that there are no more than two simultaneous accessors of the shared resource amoung arbitrarily many processes:

```R
library(semaphore)                                                                                                                                                                                              
library(parallel)                                                                                                                                                                                                 
n_simultaneous_writers <- 2                                                                                                                                                                                       
n_tasks <- 40                                                                                                                                                                                                     
sem <- semaphore(n_simultaneous_writers)                                                                                                                                                                          
work <- function(i=0) {
  n_seconds = sample(1:10,1)
  Sys.sleep(n_seconds)

  # Without the semaphore guard, many threads will fail.
  acquire(sem)
  mock_output()
  release(sem)

  print(paste0("Process complete: ", i))
}

# Create a temp file; sleep 2 seconds; delete the file.
mock_output <- function() {
  t1 <- tempfile("tmp-demo.xxxxxx", tmpdir=getwd())
  system(paste0("touch ", t1))
  n_active_writers = length(grep("tmp-demo", list.files(".")))
  if (n_active_writers > n_simultaneous_writers) {
    Sys.sleep(2)
    system(paste0("rm ", t1))
    stop("Too many active writers!")
  }
  Sys.sleep(2)
  system(paste0("rm ", t1))
}

mclapply(1:n_tasks, work, mc.cores=n_tasks, mc.preschedule=FALSE)

```

In the above example each thread lasts for an average of 7 seconds (average of 5 seconds for the work phase where there is no shared resource use, and 2 seconds when the shared resource is in use.)  If we were to guarantee no more than two writers by limiting the number of threads to 2, the runtime should be about 40 * 7 / 2 = 140 seconds.  Observations bear this out.  But for much of this time there are fewer than the maximum of two writers, because a thread is not in the writing phase.  If we allow full parallelism with up to 40 threads, the observed runtime goes down to about 40 seconds, which is the lowest achievable for 40 writers running 2 at a time that each require 2 seconds of writing.  Watching this process run, there will never be more than 2 open temp files at a time.

Another interesting use case is [dynamic parallelism](https://www.udacity.com/wiki/cs344/lesson-7.2---dynamic-parallelism) where it may not be possible to predict or to constrain in advance the number of processes created but where total resource usage must still be managed.




