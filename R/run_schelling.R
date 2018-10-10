#' Runs the Schelling ABM for given sets of parameters
#'
#' @param size the size of the grid (i.e., an size x size square)
#' @param percent_empty how many of the spaces should be empty?
#' @param threshold under which threshold should agent become unhappy and move
#' @param number_of_groups how many different groups should be simulated
#' @param max_rounds what is the maximum number of simulational rounds allowed
#' @param seed a seed for the random number generator (Mersenne-Twister)
#'
#' @return a list of two data_frames containing the simulational results
#' @export
#'
#' @examples
#' library(dplyr)
#' library(ggplot2)
#'
#' sh <- run_schelling()
#' sh
#'
#' plot_grid(sh)
#' plot_grid(sh, Inf, title = TRUE)
#' \donttest{
#' library(ggplot2)
#' library(dplyr)
#' # takes a while, but has a nice pattern :D
#' sh <- run_schelling(100, percent_empty = 0.2, threshold = 0.5,
#'                     number_of_groups = 9, max_rounds = 20000)
#'
#' plot_grid(sh, Inf, title = TRUE)
#'
#' sh$round %>%
#'   mutate(perc_happy = happy_agents / (happy_agents + unhappy_agents)) %>%
#'   ggplot(aes(x = round, y = perc_happy)) + geom_line()
#'
#' sh$detailed %>%
#'   filter(group != 0) %>%
#'   group_by(round, group) %>%
#'   summarise(perc_happy = mean(happy)) %>%
#'   ggplot(aes(x = round, y = perc_happy, color = as.factor(group))) +
#'   geom_line()
#' }
run_schelling <- function(size = 10, percent_empty = 0.2, threshold = 0.5,
                         number_of_groups = 2, max_rounds = 1000,
                         seed = runif(1, 0L, .Machine$integer.max)) {

  if (percent_empty >= 1) percent_empty <- percent_empty / 100

  res <- SchellingR:::run_schelling_rcpp(size, percent_empty, threshold,
                                         number_of_groups, max_rounds, seed)

  res$detailed <- dplyr::as_data_frame(res$detailed)
  res$round <- dplyr::as_data_frame(res$round)

  return(res)
}
