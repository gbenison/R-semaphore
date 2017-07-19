#' semaphore: A package to provide simple counting semaphores for R
#' @name semaphore-package
#' @docType package
#' @author Greg Benison \email{gbenison@gmail.com}
#' @references
#' https://en.wikipedia.org/wiki/Semaphore_(programming)

#' @param initial_value The initial value for the semaphore; default 1
semaphore <- function(initial_value = 1) {
  result <- .C("semaphore_create", semaphore_id=as.integer(0), as.integer(initial_value))
  semaphore_id <- result$semaphore_id
  class(semaphore_id) <- "semaphore"
  return(semaphore_id)
}

acquire <- function(x) UseMethod("acquire")
release <- function(x) UseMethod("release")

# Classic 'V'
release.semaphore <- function(s) {
  .C("semaphore_change", as.integer(s), as.integer(1))
}

# Classic 'P'
# blocks if semaphore value drops below zero
acquire.semaphore <- function(s) {
  .C("semaphore_change", as.integer(s), as.integer(-1))
}


