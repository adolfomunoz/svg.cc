# SVGPlot::scatter

The `scatter` method of the `svg::plot::SVGPlot` class generates scatter plots: a type of plot or mathematical diagram using Cartesian coordinates to display values for typically two variables for a set of data (passed as the two parameters *x* and *y* of the method). The points can be coded coded (color/shape/size) for displaying additional variables. Generating scatter plots is very quick:

{% highlight cpp %}
{% include includelines filename='scatter.cc' start=7 count=7 %}
{% endhighlight %}
which generates a scatter plot with points distributed randomly according to a normal distribution:

![Example 1](./scatter/example1.svg)
	
As expected, the *x* and *y* parameters can be any C++ collection or bracketed list. Also, if needed (although not very useful for scatter plots), list generators such as `arange(<start>,<stop>,<step>)` and `linspace(<start>,<stop>,<nsamples = 50>)` are provided. Many scatter plots can be included in the same graph through multiple calls of the `scatter` method:

{% highlight cpp %}
{% include includelines filename='scatter.cc' start=17 count=11 %}
{% endhighlight %}

which automatically assigns a specific color (with a palette similar to `matplotlib`'s) to each of the scatter plots in order to differenciate them:

![Example 2](./scatter/example2.svg)
 
## Formatting

The appearance of a scatter plot can be setup in multiple ways through named parameters (which are represented as methods):

- `s(<float>)`: The marker size in points
- `c(<color>)`: defines the [color](color.html) of the marker.
- `edgecolors(<color>)`: defines the [color](color.html) of the edge of the marker. It cannot be seen if the `linewidths` is *0*.
- `linewidths(<float>)`: The width of the edge line of the marker (default is *0*).
- `alpha(<float>)`: Transparency of the markers from *0* (totally transparent, invisible) to *1* (totally opaque, default value).

These are illustrated here:

{% highlight cpp %}
{% include includelines filename='scatter.cc' start=32 count=9 %}
{% endhighlight %}

which generates

![Example 3](./scatter/example3.svg)
 
The shape of the marker can be defined by the `marker(<string>)` named parameter. There are multiple enabled options for marker shapes, as illustrated in the following code:

{% highlight cpp %}
{% include includelines filename='scatter.cc' start=45 count=8 %}
{% endhighlight %}

generates a visualization of all the available marker shapes:

![Example 4](./scatter/example4.svg)

Additionally, markers can be generated from an image file (`.png`, `.jpg` and `.svg`), by puting the file name of the image instead of a character that defines the marker. In the case of `.svg` file format it preserves the color given by the `c` named parameter(or a default color). Images are embedded in the output file directly, so there is no need to carry them along with the output graph. This is illustrated in the following source code:

{% highlight cpp %}
{% include includelines filename='scatter.cc' start=56 count=16 %}
{% endhighlight %}

that gives the following output:

![Example 5](./scatter/example5.svg)

## Sequence parameters and aditional variables

All the named parameters related to format discussed above (`c`, `s`, `alpha`, `edgecolors` and `linewidths`) can also have a sequence (any iterable collection) for the same data type as parameter. Each element of the sequence is assigned to each `x`, `y` data point. This sequence parameters can be used to show more data than two dimensions for the same plot. This is illustrated in the following source code:

{% highlight cpp %}
{% include includelines filename='scatter.cc' start=76 count=12 %}
{% endhighlight %}

that gives the following output:

![Example 6](./scatter/example6.svg)

## Color maps

TODO

Additional variables can be visualized by modifying each marker's color through the `c(<sequence>)` named parameter, when passing a sequence (C++ collection) of floating point values. The mapping to RGB values is done (like in Matplotlib) through a color map, where the maximum and minimum labeled values are calculated automatically from the data so there is no clamping. It is possible, however, to specifically set those clamping values through the `vmin(<float>)` and `vmax(<float>)` named parameters (represented in C++ as methods). Furthermore, the color map can be specified through the `cmap(<string>)` named parameter. The following color maps are available:

<div style="text-align:center"><img 
 src="./imshow/example10.svg" alt="example10" width="100%" /></div>

The following example illustrates three different color maps for the same set of values:

```cpp
std::mt19937 gen{1}; //Fixed seed
std::uniform_real_distribution<float> d(0,1);
std::list<float> x,y, value;
for (int i=0;i<10;++i) for (int j=0;j<10;++j) for (int s=0;s<3;++s) {
    x.push_back(i+d(gen)); y.push_back(j+d(gen)); value.push_back(sqrt((x.back()-5.0f)*(x.back()-5.0f)+(y.back()-5.0f)*(y.back()-5.0f)));
}
svg_cpp_plot::SVGPlot plt; int p = 0;
for (auto cmap : {"grayscale","viridis","plasma"})
    plt.subplot(1,3,p++).figsize({200,200}).scatter(x,y).c(value).cmap(cmap);
plt.savefig("../doc/svgplot/scatter/example5.svg");
```

which generates

<div style="text-align:center"><img 
 src="./scatter/example6.svg" alt="example5" width="100%" /></div>

It is also possible not only to set values through a sequence of floating point numbers in `c(<sequence>)` but also specific colors through a sequence of RGB tuples, as illustrated in the following code:

```cpp
svg_cpp_plot::SVGPlot plt;
plt.scatter({0,0,0},{0,2,4}).c({ {0,1,0.25},{1.0,0.75,0},{0.75,0.25,0.25} });
plt.figsize({10,25}).axis({-2,2,-2,6}).xticks({}).yticks({}).linewidth(0);
plt.savefig("../doc/svgplot/scatter/example6.svg");
```

which generates:

<div style="text-align:center"><img 
 src="./scatter/example6.svg" alt="example6" width="20%" /></div>
 
In this case the parameters `cmap`, `vmin` and `vmax` will just be ignored.

Actually, multiple variables can be defined: in scatter plots, most named parameters that define appearance can be translated into a sequence, enabling new ways of showing information through the markers size (`s`) and border width (`linewidth`). The following illustrates different sizes for the same scatter plot:

```cpp
svg_cpp_plot::SVGPlot plt;
plt.scatter({0,0,0},{0,2,3.5}).s({4,3,2});
plt.scatter({0},{3.4}).s(0.4).marker("v");
plt.scatter({-0.3,0.3,-0.3,-0.1,0.1,0.3},{3.8,3.8,3.2,3.2,3.2,3.2}).s({0.4,0.4,0.3,0.2,0.2,0.3}).marker("+");
plt.figsize({10,25}).axis({-2,2,-2,5}).xticks({}).yticks({}).linewidth(0);
plt.savefig("../doc/svgplot/scatter/example7.svg");
```

yielding

<div style="text-align:center"><img 
 src="./scatter/example7.svg" alt="example7" width="20%" /></div>



