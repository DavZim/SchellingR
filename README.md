
<!-- README.md is generated from README.Rmd. Please edit that file -->

# SchellingR

The goal of SchellingR is to run the Schelling (1971) agent-based model
(ABM) of urban migration and segregation. The ABM is written in C++ to
allow for fast and efficient calculations. Summary statistics of each
round are reported back to R.

## Installation

You can install SchellingR from github with:

``` r
# install.packages("devtools")
devtools::install_github("DavZim/SchellingR")
```

## Example

The base function of the package is `run_schelling()` which returns a
list of two data.frames `round` and `detailed`. The former contains
aggregated statistics per round, i.e., the number of happy/unhappy
agents and the number of moves, the latter contains agent-level
information about the grid, the agents, and their neighbors.

The function allows you to tweak basic inputs to the model, such as grid
size `size`, the percent of empty grid-cells `percent_empty`, the
threshold above which agents are considered to be happy `threshold`, the
number of groups to simulated `number_of_groups`, the maximum number of
simulation rounds `max_rounds`, and a seed for the random number
generator `seed`.

``` r
library(SchellingR)
set.seed(1234567)

sh <- run_schelling(size = 10, percent_empty = 0.2, threshold = 0.5, number_of_groups = 2, max_rounds = 100)

str(sh)
#> List of 2
#>  $ round   :Classes 'tbl_df', 'tbl' and 'data.frame':    24 obs. of  4 variables:
#>   ..$ round         : int [1:24] 0 1 2 3 4 5 6 7 8 9 ...
#>   ..$ happy_agents  : int [1:24] 48 63 64 75 78 77 77 73 76 78 ...
#>   ..$ unhappy_agents: int [1:24] 35 20 19 8 5 6 6 10 7 5 ...
#>   ..$ number_moves  : int [1:24] 0 44 33 32 25 20 23 24 26 20 ...
#>  $ detailed:Classes 'tbl_df', 'tbl' and 'data.frame':    2400 obs. of  8 variables:
#>   ..$ round      : int [1:2400] 0 0 0 0 0 0 0 0 0 0 ...
#>   ..$ x          : int [1:2400] 11 11 11 11 11 11 11 11 11 11 ...
#>   ..$ y          : int [1:2400] 1 2 3 4 5 6 7 8 9 10 ...
#>   ..$ id         : int [1:2400] 0 0 1 0 2 3 4 5 6 7 ...
#>   ..$ group      : int [1:2400] 0 2 1 0 2 1 1 2 2 1 ...
#>   ..$ n_same     : int [1:2400] 0 1 1 0 2 1 2 2 3 0 ...
#>   ..$ n_different: int [1:2400] 0 3 3 0 1 2 2 2 2 3 ...
#>   ..$ happy      : int [1:2400] 0 0 0 0 1 0 1 1 1 0 ...
```

## Visualisation

After running the simulation, we can plot the average happiness per
round using `dplyr` and `ggplot2`.

``` r
library(dplyr)
library(ggplot2)
theme_set(theme_light())

sh$round %>% 
  mutate(perc_happy = happy_agents / (happy_agents + unhappy_agents)) %>% 
  ggplot(aes(x = round, y = perc_happy)) + 
  geom_line() +
  scale_y_continuous(labels = scales::percent) +
  labs(x = "Round", y = "Percent of Agents that are Happy", 
       title = "Happiness Ratio for a Run of the Schelling Model")
```

![](README-unnamed-chunk-2-1.png)<!-- -->

We can also use the detailed information to plot the grid at certain
steps.

``` r
sh$detailed %>% 
  # group == 0 means empty cells
  filter(group != 0, round == 17) %>% 
  ggplot(aes(x = x, y = y, fill = as.factor(group))) +
  geom_tile() +
  coord_equal() +
  theme_void() +
  scale_fill_brewer(palette = "Set1", guide = FALSE) +
  labs(title = "Schelling Model after 17 Simulation Rounds")
```

![](README-unnamed-chunk-3-1.png)<!-- -->

## Animation

Using the wonderful [gganimate](https://github.com/thomasp85/gganimate)
package, we can also create an animation of the model

``` r
library(gganimate)

anim <- sh$detailed %>% 
  # group == 0 means empty cells
  filter(group != 0) %>% 
  ggplot(aes(x = x, y = y, fill = as.factor(group), group = id)) +
  geom_tile() +
  coord_equal() +
  theme_void() +
  scale_fill_brewer(palette = "Set1", guide = FALSE) +
  labs(title = "Schelling Model after {previous_state} Simulation Rounds") +
  transition_states(
    round,
    transition_length = 1,
    state_length = 2
  )

animate(anim, nframes = 200)
```

![](README-unnamed-chunk-4-1.gif)<!-- -->

If you want to get even fancier, you can also use the
[emojifont](https://CRAN.R-project.org/package=emojifont) package to
make the agent’s state more intuitive.

``` r
library(emojifont)

sh$detailed %>% 
  # group == 0 means empty cells
  filter(group != 0, round == 5) %>% 
   mutate(emoji_text = ifelse(happy == 1, emoji("smile"), emoji("angry"))) %>% 
  ggplot(aes(x = x, y = y, fill = as.factor(group), label = emoji_text)) +
  geom_tile() +
  coord_equal() +
  theme_void() +
  scale_fill_brewer(palette = "Set1", guide = FALSE) +
  geom_text(size = 7) +
  labs(title = "Schelling Model after 5 Simulation Rounds")
```

![](README-unnamed-chunk-5-1.png)<!-- -->

## More Complex Example

To give a more complex example on a larger grid with 7
groups:

``` r
sh2 <- run_schelling(size = 100, number_of_groups = 7, max_rounds = 10000, seed = 1234567)

sh2$detailed %>% 
  filter(group != 0) %>% 
  group_by(round, group) %>% 
  summarise(perc_happy = mean(happy)) %>% 
  ggplot(aes(x = round, y = perc_happy, color = as.factor(group))) +
  geom_line() +
  scale_color_brewer(palette = "Set1") +
  labs(x = "Round", y = "Percent Happy", color = "Group",
       title = "Average Happiness per Group")
```

![](README-unnamed-chunk-6-1.png)<!-- -->

``` r


sh2$detailed %>% 
  filter(group != 0, round == 750) %>%
  ggplot(aes(x = x, y = y, fill = as.factor(group))) + 
  geom_tile() +
  scale_fill_brewer(palette = "Set1") +
  theme_void() +
  labs(title = "More Complex Schelling Model after Round 750", fill = "Group")
```

![](README-unnamed-chunk-6-2.png)<!-- -->
