#include "../../plot/svgplot.h"
#include <fstream>
#include <cmath>


int main(int argc, char** argv) {
    { // Example 1
    svg::plot::SVGPlot plt;
    plt.figsize({200,200});
    plt.imshow({ {svg::red,svg::green,svg::blue},
                    {svg::green,svg::blue,svg::red},
                    {svg::blue,svg::red,svg::green} });
    plt.savefig("../docs/svgplot/imshow/example1.svg");
	}

    { // Example 1 again, with strings
    svg::plot::SVGPlot plt;
    plt.figsize({200,200});
    plt.imshow({ {"red","green","blue"},
                    {"green","blue","red"},
                    {"blue","red","green"} });
    plt.savefig("../docs/svgplot/imshow/example1.svg");
	}  

    { // Example 2
    auto red = std::array<float,4>{1.0f,0.0f,0.0f,1.0f};
    auto green = std::array<float,4>{0.0f,1.0f,0.0f,1.0f};
    auto black_transparent = std::array<float,4>{0.0f,0.0f,0.0f,0.5f};
    svg::plot::SVGPlot plt;
    plt.figsize({200,200});
    plt.imshow({ {black_transparent,green,black_transparent},
                    {green,black_transparent,red},
                    {black_transparent,red,black_transparent} });
    plt.savefig("../docs/svgplot/imshow/example2.svg");
	}

    { // Example 3
    svg::plot::SVGPlot plt;
    plt.figsize({200,200});
    plt.imshow({ {0.0,0.1,0.2},
                 {0.3,0.4,0.5},
                 {0.6,0.7,0.8} });
    plt.savefig("../docs/svgplot/imshow/example3.svg");
    }
    
    { // Example 4
    svg::plot::SVGPlot plt;
    plt.figsize({200,200});
    std::list<std::list<float>> data;
    for (float f = 0.0f; f<=1.0f; f+=0.1f) {
        data.push_back(std::list<float>());
        for (float g = f; g<=1.0f; g+=0.1f) 
            data.back().push_back(f+g);
    }
    plt.imshow(data);
    plt.savefig("../docs/svgplot/imshow/example4.svg");
	}

   
    { // Example 5
    svg::plot::SVGPlot plt;
    plt.figsize({200,200});
    auto f = [] (float x, float y) {
        float r = 0.5f+0.5f*std::sin(x);
        float g = 0.5f+0.5f*std::sin(y);
        float b = std::max(0.0f,1.0f-(r+g));
        return std::array<float,3>{r,g,b};
    };
    plt.imshow(svg::plot::arange(0,10,0.25),svg::plot::arange(0,10,0.25),f);
    plt.savefig("../docs/svgplot/imshow/example5.svg");
    }
    
    { // Example 6
    svg::plot::SVGPlot plt;
    plt.figsize({200,50}).yticks({});
    std::list<std::list<float>> data; 
    data.push_back(std::list<float>()); 
    for (float f = 0.0f; f<=1.0f; f+=0.01f) data.back().push_back(f);
    plt.imshow(data).extent({0,1,0,1});
    plt.savefig("../docs/svgplot/imshow/example6.svg");   
    }  

    { // Example 7
    svg::plot::SVGPlot plt;
    plt.figsize({200,200}).axis({-5,5,-5,5});
    plt.imshow({ {svg::red,svg::green,svg::blue},
                {svg::green,svg::blue,svg::red},
                {svg::blue,svg::red,svg::green} }).extent({-1,1,1,-1});
    plt.savefig("../docs/svgplot/imshow/example7.svg");
	}    
    
    { // Example 8
    auto x = svg::plot::arange(-5,5,0.2);
    auto y = svg::plot::arange(-5,5,0.2);
    auto himmelblau = [] (float x, float y) {
        return (x*x + y - 11.0f)*(x*x + y - 11.0f) + (x + y*y -7)*(x + y*y -7);
        };
    svg::plot::SVGPlot plt;
    plt.subplot(1,4,0).figsize({200,200}).title("Default").imshow(x,y,himmelblau);
    plt.subplot(1,4,1).figsize({200,200}).title("vmax to 100").imshow(x,y,himmelblau).vmax(100);
    plt.subplot(1,4,2).figsize({200,200}).title("vmin to 100").imshow(x,y,himmelblau).vmin(100);
    plt.subplot(1,4,3).figsize({200,200}).title("Reverse").imshow(x,y,himmelblau).vmin(1000).vmax(0);
    plt.savefig("../docs/svgplot/imshow/example8.svg");
    }   
    
    
    /*    { // Example 6
    	svg_cpp_plot::SVGPlot plt;
        plt.figsize({200,200});
        auto f = [] (float x, float y) {
            float r = 0.5f+0.5f*std::sin(x);
            float g = 0.5f+0.5f*std::sin(y);
            float b = std::max(0.0f,1.0f-(r+g));
            return std::tuple(r,g,b);
        };
        plt.imshow(svg_cpp_plot::arange(0,10,0.25),svg_cpp_plot::arange(0,10,0.25),f).interpolation("bicubic");
		plt.savefig("../doc/svgplot/imshow/example6.svg");
    } */

}
