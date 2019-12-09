#' Plots the grid of a simulation
#'
#' @param data a list of data-frames as returned by run_schelling
#' @param select_round the round number to display
#' @param grid bool value if the grid should be displayed
#' @param title if the title should be displayed
#' @param emoji if emojifont is installed, if the state of the agents should be displayed using emojis
#' @param emoji_size size of the text of the emojis (only used if emoji = TRUE)
#' @param animate if gganimate is installed, if the plot should be animated
#' @param step if animate = TRUE, which steps should be plotted, either a vector or a single number
#' @param transition if in animations the animations should be transitioned (handy if set to F for combining plots!)
#'
#' @return a ggplot2 object, or a gganimate object if animate = TRUE
#' @export
#'
#' @examples
#' sh <- run_schelling()
#' plot_grid(sh)
#' plot_grid(sh, Inf, title = TRUE)
#'
#' \donttest{
#'  # if the emojifont package is installed:
#'  plot_grid(sh, emoji = TRUE)
#'
#'  # if the gganimate package is installed:
#'  plot_grid(sh, animate = TRUE)
#'  plot_grid(sh, animate = TRUE, step = 2, title = TRUE)
#'
#'  # if both are installed:
#'  plot_grid(sh, emoji = TRUE, animate = TRUE, title = TRUE)
#' }
plot_grid <- function(data, select_round = Inf, grid = TRUE, title = FALSE,
                      emoji = FALSE, emoji_size = 7, transition = TRUE,
                      animate = FALSE, step = 1) {

  if (!(is.list(data) && all(names(data) %in% c("detailed", "round"))))
    stop("data has to be a list as returned by run_schelling")

  d <- data$detailed

  # Allows to filter only certain steps inside an animation
  if (animate && is.numeric(step)) {
    if (length(step) == 1) {
      d <- dplyr::filter(d, round %% step == 0)
    } else {
      d <- dplyr::filter(d, round %in% step)
    }
  }

  select_round <- max(min(select_round, max(d$round)), 0)
  if (title) title_text <- sprintf("Schelling Model after %i Iterations",
                                   select_round)
  xx <- max(d$x) + 0.5
  yy <- max(d$y) + 0.5

  if (!animate) d <- dplyr::filter(d, round == select_round)

  d <- dplyr::filter(d, group != 0)
  if (emoji) {
    if (!requireNamespace("emojifont", quietly = TRUE))
      stop("emojifont needs to be installed for displaying emojis correctly\nPlease install using: install.packages(\"emojifont\")")

    d <- dplyr::mutate(d, state = ifelse(happy == 1,
                                         emojifont::emoji("smile"),
                                         emojifont::emoji("angry")))
  }

  plot1 <- ggplot2::ggplot(d, ggplot2::aes(x = x, y = y, group = id,
                                           fill = as.factor(group),
                                           color = as.factor(group))) +
    ggplot2::geom_tile(width = 1, height = 1) +
    ggplot2::coord_equal(ratio = 1) +
    ggplot2::theme_void() +
    ggplot2::scale_fill_brewer(palette = "Set1", guide = FALSE) +
    ggplot2::scale_color_brewer(palette = "Set1", guide = FALSE)

  if (grid) {
    grid_lines <- dplyr::bind_rows(
      dplyr::tibble(xstart = 0.5:xx, xend = 0.5:xx, ystart = 0.5, yend = yy),
      dplyr::tibble(xstart = 0.5, xend = xx, ystart = 0.5:yy, yend = 0.5:yy)
    )

    plot1 <- plot1 +
      ggplot2::geom_segment(inherit.aes = FALSE, data = grid_lines,
                            ggplot2::aes(x = xstart, xend = xend,
                                         y = ystart, yend = yend),
                            color = "lightgray")
  }

  if (emoji) {
    plot1 <- plot1 +
      ggplot2::aes(label = state) +
      ggplot2::geom_text(size = emoji_size)
  }

  if (animate) {
    if (!requireNamespace("gganimate", quietly = TRUE))
      stop("gganimate needs to be installed for animating the grid\nPlease install using: install.packages(\"gganimate\")")

    if (transition) {
      plot1 <- plot1 +
        gganimate::transition_states(round, transition_length = 1, state_length = 2)
    } else {
      plot1 <- plot1 +
        gganimate::transition_manual(round)
    }

  }

  if (title) {
    if (animate) {
      if (transition) {
        title_text <- "Schelling Model - Iteration {closest_state}"
      } else {
        title_text <- "Schelling Model - Iteration {current_frame}"
      }
    }
    plot1 <- plot1 + ggplot2::labs(title = title_text)
  }

  return(plot1)
}
