
#' Plots the development of happiness over time
#'
#' @inheritParams plot_grid
#'
#' @return a ggplot2 object, or a gganimate object if animate = TRUE
#' @export
#'
#' @examples
#' sh <- run_schelling()
#' plot_development(sh)
#'
#' \donttest{
#'  # if the gganimate package is installed:
#'  plot_development(sh, animate = TRUE)
#' }
plot_development <- function(data, animate = FALSE, step = 1, title = TRUE) {

  if (!(is.list(data) && all(names(data) %in% c("detailed", "round"))))
    stop("data has to be a list as returned by run_schelling")

  d <- data$detailed %>%
    dplyr::filter(group != 0) %>%
    dplyr::group_by(round, group) %>%
    dplyr::summarise(perc_happy = mean(happy))

  if (animate && is.numeric(step)) {
    states <- d %>%
      dplyr::distinct(round) %>%
      dplyr::ungroup(round)

    if (length(step) == 1) {
       states <- states %>%
         dplyr::mutate(state = cumsum((round + 1) %% step == 0))
    } else {
      states <- states %>%
        dplyr::mutate(state = cumsum((round - 1) %in% step))
    }
    states <- states %>%
      dplyr::group_by(state) %>%
      dplyr::mutate(ss = max(round)) %>%
      dplyr::ungroup() %>%
      dplyr::transmute(round, state = ss)

    d <- dplyr::left_join(d, states, by = "round")
  }

  if (title) title_text <- "Percent of Agents that are Happy"

  d <- dplyr::filter(d, group != 0)

  plot1 <- ggplot2::ggplot(d, ggplot2::aes(x = round, y = perc_happy,
                                        color = as.factor(group))) +
    ggplot2::geom_line() +
    ggplot2::scale_color_brewer(palette = "Set1") +
    ggplot2::labs(x = "Round", y = "Percent Happy", color = "Group") +
    ggplot2::theme_minimal() +
    ggplot2::scale_y_continuous(labels = scales::percent)


  if (animate) {
    if (!requireNamespace("gganimate", quietly = TRUE))
      stop("gganimate needs to be installed for animating the grid\nPlease install using: install.packages(\"gganimate\")")

    plot1 <- plot1 +
      gganimate::transition_manual(state, cumulative = TRUE)

    title_text <- "Percent of Agents that are Happy - Iteration {current_frame}"
  }

  if (title) plot1 <- plot1 + ggplot2::labs(title = title_text)

  return(plot1)
}