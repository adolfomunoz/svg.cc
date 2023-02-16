#include "../../plot/svgplot.h"

int main() {
    svg::plot::SVGPlot plt;
	plt.plot({1, 2, 3, 4, 5}, {0, 3, 8, 15, 24});
	plt.savefig("example_svg.svg");
	plt.savefig("example_pdf.pdf");
	plt.savefig("example_png.png");
	plt.savefig("example_tex.pdf_tex");
}