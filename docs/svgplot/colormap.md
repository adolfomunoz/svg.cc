# SVGPlot color maps

Two types of plots ([scatter](scatter.html) and [imshow](imshow.html)) have a named parameter `cmap`, that takes a parameter which is either:

- **Type named color maps.** There are a set of color maps defined as type variables: `svg::plot::viridis`, `svg::plot::magma`, `svg::plot::inferno`, `svg::plot::plasma`, `svg::plot::bwr`, `svg::plot::coolwarm`, `svg::plot::seismic`, `svg::plot::spectral`, `svg::plot::piyg` and `svg::plot::grayscale`.
- **String named color maps.** The same  as above, but from a string without the `svg::plot::` prefix (e.g. `"plasma"`).
