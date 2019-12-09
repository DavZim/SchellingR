#' Combines two gifs side by side
#'
#' @param a gif a, either an output directly from gganimate or the path to a gif (faster)
#' @param b gif b, either an output directly from gganimate or the path to a gif (faster)
#' @param outfile if the file should directly be written to a gif
#'
#' @return an magick-image
#' @export
#'
#' @examples
#' sh <- run_schelling()
#' \donttest{
#'   grid <- plot_grid(sh, animate = TRUE, title = TRUE)
#'   dev  <- plot_development(sh, animate = TRUE)
#'   combined <- combine_gifs(grid, dev)
#'   print(combined)
#' }
combine_gifs <- function(a, b, outfile = NA) {

  if (!requireNamespace("magick", quietly = TRUE))
    stop("magick needs to be installed for combining gifs\nPlease install using: install.packages(\"magick\")")

  if ("gganim" %in% class(a)) {
    tmp <- tempfile(fileext = ".gif")
    gganimate::anim_save(tmp, a)
    a <- tmp
  }
  if ("gganim" %in% class(b)) {
    tmp <- tempfile(fileext = ".gif")
    gganimate::anim_save(tmp, b)
    b <- tmp
  }

  a_mgif <- magick::image_read(a)
  b_mgif <- magick::image_read(b)

  if (length(a_mgif) != length(b_mgif))
    warning("a and b have different frame lengths")

  new_gif <- magick::image_append(c(a_mgif[1], b_mgif[1]))

  for(i in 2:length(a_mgif)){
    combined <- magick::image_append(c(a_mgif[i], b_mgif[i]))
    new_gif <- c(new_gif, combined)
  }

  if (!is.na(outfile)) {
    magick::image_write_gif(new_gif, outfile)
    return(invisible(new_gif))
  } else {
    return(new_gif)
  }
}