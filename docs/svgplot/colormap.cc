#include "../../plot/svgplot.h"
#include <fstream>
#include <cmath>


int main(int argc, char** argv) {  
    { // All colormaps
        auto x = svg::plot::arange(0,1,0.01);
        auto y = svg::plot::arange(0,1,1);
        auto f = [] (float x, float y) { return x; };
		svg::plot::SVGPlot plt;
        plt.subplots_adjust().wspace(0.15).left(0.05).right(0.95);
        int i = 0;
        for (auto cmap : {"grayscale","viridis","plasma","magma","inferno","bwr","seismic","coolwarm","spectral","piyg"}) 
            plt.subplot(2,5,i++).figsize({200,100}).yticks({}).xticks({0,1},{"vmin","vmax"}).title(cmap)
                .imshow(x,y,f).cmap(cmap);
      
		plt.savefig("../docs/svgplot/colormaps.svg");
    }

}
