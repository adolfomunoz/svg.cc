#include "../../plot/svgplot-headeronly.h"

int main() {
    svg::plot::SVGPlot plt;
	plt.plot({1, 2, 3, 4, 5}, {0, 3, 8, 15, 24});
	plt.savefig("example1.svg");
}