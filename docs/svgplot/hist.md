# SVGPlot::hist

The `hist` method provides a way to generate histograms from data, from `svg::plot::SVGPlot` class. It expectes a single parameter, which are the samples from which the histogram is going to be calculated and plotted:

{% highlight cpp %}
{% include includelines filename='hist.cc' start=6 count=7 %}
{% endhighlight %}

This generates the histogram in its classic appearance:

![Example 1](./hist/example1.svg)

## Binning 
	
By default the histogram is distributed uniformly among *10* bins. However, these can be tweaked through several named attributes (modeled as methods in C++).

The `bins` named attribute can be set as a number (integer) that indicates the number of bins of the histogram:

{% highlight cpp %}
{% include includelines filename='hist.cc' start=16 count=9 %}
{% endhighlight %}

Note how the number of bins increases, yielding increased resolution on the histogram (at the cost of potential noise):

![Example 2](./hist/example2.svg)
 
 The `weights` named attribute is a sequence of the same size of the samples that defines the weight of each sample (it is otherwise *1*). This can be used to have a weighted average, or to draw a histogram of data that has already been binned (by treating each bin as a single point with a weight equal to its count, although you could use [`bar`](./bar.html) for this).
 
{% highlight cpp %}
{% include includelines filename='hist.cc' start=28 count=3 %}
{% endhighlight %}

generates

![Example 3](./hist/example3.svg)

By default, the range covered by the histogram is limited by the minimum and maximum values of the samples, but is also possible to control this range with the `range` named attribute:

{% highlight cpp %}
{% include includelines filename='hist.cc' start=34 count=3 %}
{% endhighlight %}


that yields

![Example 4](./hist/example4.svg)
 
A finer control is granted when the `bin` named attribute is setup as a sequence of values (as opposed to an integer value). The sequence gives bin edges, including left edge of first bin and right edge of last bin. All but the last (righthand-most) bin is half-open. In other words, if bins is *{1, 2, 3, 4}* then the first bin is *\[1, 2)* (including 1, but excluding 2) and the second *\[2, 3)*. The last bin, however, is *\[3, 4\]*, which **includes** 4. Unequally spaced bins are supported if bins is a sequence, and the `range` parameter is ignored. An example of this is given by the following code:

{% highlight cpp %}
{% include includelines filename='hist.cc' start=40 count=3 %}
{% endhighlight %}

that outputs the following graph:

![Example 5](./hist/example5.svg)

## Distribution modifications

The `density` named parameter is a boolean (that can be setup either with a `.density(true)` call or a `.density()` call). If true, the histogram is normalized to form a probability density, i.e., the area (or integral) under the histogram will sum to *1*. This is achieved by dividing the count by the number of observations times the bin width.

The `cumulative` named parameter is a boolean (that can be setup either with a `.cumulative(true)` call or a `.cumulative()` call). If true, then a histogram is computed where each bin gives the counts in that bin plus all bins for smaller values. The last bin gives the total number of datapoints. If `density` is also true then the histogram is normalized such that the last bin equals *1*.

The usage of both booleans is illustrated in the following example (for the same sample points):

{% highlight cpp %}
{% include includelines filename='hist.cc' start=46 count=10 %}
{% endhighlight %}

This example yields the following result that shows how the histograms are affected by both parameters:

![Example 6](./hist/example6.svg)
 
## Formatting

There are also several options also for formatting the histogram and tweaking its appearance. 

For instance, the `histtype` named parameter (represented by a method) defines the type of graph that represents the histogram:
- `"bar"` is a traditional bar-type histogram (default). Can be setup also as a typed global variable (not a string, without quotes)
- `"step"` generates a lineplot. Can be setup also as a typed global variable (not a string, without quotes).

Also, it is possible to choose the orientation of the histogram with the `orientation` named attribute, which can be either `"vertical"` or `"horizontal"`, that, as well, can be setup as global typed variables (not strings). Both options are illustrated in the follwoing example:

{% highlight cpp %}
{% include includelines filename='hist.cc' start=59 count=11 %}
{% endhighlight %}

that yields

![Example 7](./hist/example7.svg)

Last, it is also possible to tweak the color of the histogram with the `color` named attribute (represented as a method). Colors can be represented as described [here](./color.html). Also, the `alpha` parameter defines transparency, which is specially useful for plotting multiple histograms in the same graph:

{% highlight cpp %}
{% include includelines filename='hist.cc' start=73 count=8 %}
{% endhighlight %}

This code generates the following graph:

![Example 8](./hist/example8.svg)





