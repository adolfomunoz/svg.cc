#include "../../plot/svgplot-headeronly.h"

int main() {
    svg::plot::SVGPlot plt;
	plt.plot({1, 2, 3, 4}, {1, 4, 9, 16});
	plt.savefig("example1.svg");
}