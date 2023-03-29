#include "../../plot/svgplot.h"
#include <cmath>



int main(int argc, char** argv) {
	{ // Example 1
	svg::plot::SVGPlot plt;
	plt.plot({1,2,3,4});
	plt.ylabel("some numbers");
	plt.savefig("../doc/svgplot/plot/example1.svg");
	}

	{ // Example 2
	svg::plot::SVGPlot plt;
	plt.plot({1, 2, 3, 4}, {1, 4, 9, 16});
	plt.savefig("../doc/svgplot/plot/example2.svg");
	}
	
	{ // Example 3
	svg::plot::SVGPlot plt;
	float f=-1.0;
	std::list<float> l; 
	for (int i=0; i<100;++i) l.push_back(f*=-1.005);
	plt.plot(svg::plot::arange(0,5,0.05),l);
	plt.savefig("../doc/svgplot/plot/example3.svg");
	}

	{ // Example 4
	svg::plot::SVGPlot plt;
    auto x = svg::plot::linspace(0,5);
	plt.plot(x,[] (float x) { return x*std::sin(x);});
	plt.plot(x,sqrtf);
	plt.savefig("../doc/svgplot/plot/example4.svg");
	}
	
	/* { // Example 5
	svg::plot::SVGPlot plt;
	plt.plot(svg::plot::arange(0,20,0.05),
		[] (float x) { return x*std::sin(x);},"g-");
	plt.plot(svg::plot::arange(0,20,0.05),
		[] (float x) { return x*std::sin(x+0.4*M_PI);},"r--");
	plt.plot(svg::plot::arange(0,20,0.05),
		[] (float x) { return x*std::sin(x+0.8*M_PI);},"b-.");
	plt.plot(svg::plot::arange(0,20,0.05),
		[] (float x) { return x*std::sin(x+1.2*M_PI);},"y:");
	plt.plot(svg::plot::arange(0,20,0.05),
		[] (float x) { return x*std::sin(x+1.6*M_PI);},"k");
	plt.plot(svg::plot::arange(0,20,0.25),
		[] (float x) { return 50+x*std::sin(x);},"go");
	plt.plot(svg::plot::arange(0,20,0.25),
		[] (float x) { return 50+x*std::sin(x+0.4*M_PI);},"r^");
	plt.plot(svg::plot::arange(0,20,0.25),
		[] (float x) { return 50+x*std::sin(x+0.8*M_PI);},"bs");
	plt.plot(svg::plot::arange(0,20,0.25),
		[] (float x) { return 50+x*std::sin(x+1.2*M_PI);},"y+");
	plt.plot(svg::plot::arange(0,20,0.25),
		[] (float x) { return 50+x*std::sin(x+1.6*M_PI);},"k.");

	plt.savefig("../doc/svgplot/plot/example5.svg");
	}
    
    { // Example 6
	svg::plot::SVGPlot plt;
	plt.plot(svg::plot::arange(0,7,0.05),[] (float x) { return std::sin(x); })
		.linestyle("-.").color( svg::hsv(160,1,1)).linewidth(1);
	plt.plot(svg::plot::arange(0,7,0.5),[] (float x) { return std::sin(x); })
		.marker("s").color(svg::rgb(0.2,0.8,0.2)).markersize(2);
 	plt.savefig("../doc/svgplot/plot/example6.svg");
	}
*/
}
