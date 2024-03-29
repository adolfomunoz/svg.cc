#include "../../plot/svgplot.h"
#include <random>


int main(int argc, char** argv) {
	{ // Example 1
    std::mt19937 gen{1}; //Fixed seed
    std::normal_distribution<float> d{0,2};
    std::vector<float> x,y;
    for (int n=0;n<50;++n) { x.push_back(d(gen)); y.push_back(d(gen)); }
    svg::plot::SVGPlot plt;
    plt.scatter(x,y);
    plt.figsize({200,200}).savefig("../docs/svgplot/scatter/example1.svg");
	}
    
	{ // Example 2
    std::mt19937 gen{1}; //Fixed seed
    std::normal_distribution<float> d{0,1};
    svg::plot::SVGPlot plt;
    for (int r = 0; r<3; ++r) {
        std::vector<float> x,y;
        for (int s=0;s<25;++s) {
            x.push_back(-12.5+0.5*s+d(gen)); y.push_back(3*r+0.1*s+d(gen));
        }            
        plt.scatter(x,y);
    }            
    plt.figsize({200,200}).savefig("../docs/svgplot/scatter/example2.svg");
	}


    { // Example 3
    std::mt19937 gen{1}; //Fixed seed
    std::fisher_f_distribution<float> d(5.0,5.0);
    std::vector<float> x,y;
    for (int n=0;n<50;++n) { 
        x.push_back(d(gen)); y.push_back(d(gen)); 
    }
    svg::plot::SVGPlot plt;
    plt.scatter(x,y).c("r").alpha(0.5).s(2).edgecolors("black").linewidths(0.5);
    plt.figsize({200,200}).savefig("../docs/svgplot/scatter/example3.svg");
	}

   
    { // Example 4
    svg::plot::SVGPlot plt; int p=0;
    for (std::string marker : {"o",".",",","v",">","^","<","s","+","P","x","X"}) {
        auto& sub = plt.subplot(2,6,p++);
        sub.title("'"+((marker!="<")?marker:"&lt;")+"'"); //.fontsize(2);
        sub.axis({-1,1,-1,1}).xticks({}).yticks({}).figsize({245,265}).linewidth(1);
        sub.scatter({0},{0}).marker(marker).s(100.0f);
        }
    plt.savefig("../docs/svgplot/scatter/example4.svg");
	}

    { // Example 5
    std::mt19937 gen{1}; //Fixed seed
    std::normal_distribution<float> d{0,2};
    std::list<float> x,y;
    svg::plot::SVGPlot plt;

    for (int n=0;n<10;++n) { x.push_back(d(gen)); y.push_back(d(gen)); }
    plt.scatter(x,y).s(3).marker("X");
    x.clear(); y.clear();
    for (int n=0;n<10;++n) { x.push_back(d(gen)); y.push_back(d(gen)); }
    plt.scatter(x,y).s(6).c(svg::hsv(50,1,1)).marker("../svg/burger.svg");
    x.clear(); y.clear();
    for (int n=0;n<10;++n) { x.push_back(d(gen)); y.push_back(d(gen)); }
    plt.scatter(x,y).s(4).marker("../svg/dice.png");

    plt.figsize({200,200});
    plt.savefig("../docs/svgplot/scatter/example5.svg");
	}
    
    //We need to implement lists as well
    { // Example 6
    std::mt19937 gen{1}; //Fixed seed
    std::uniform_real_distribution<float> d(0,1);
    std::list<float> x,y,opacity,size;
    std::list<svg::Color> colors,edges;
    for (int i=0;i<80;++i) {
        x.push_back(d(gen)); y.push_back(d(gen)); 
        opacity.push_back(std::max(std::min(0.5f*d(gen)+0.5f,1.0f),0.0f));
        size.push_back(2.0f + 1.5f*d(gen));
        float hue = 90 + 120*d(gen);
        colors.push_back(svg::hsv(hue,1.0,1.0));
        edges.push_back(svg::hsv(hue,0.5,0.5));
    }

    svg::plot::SVGPlot plt; 
    plt.figsize({200,200}).scatter(x,y).alpha(opacity).s(size).c(colors).edgecolors(edges).linewidths(1);
    plt.savefig("../docs/svgplot/scatter/example6.svg");
    }

    { // Example 7
    std::mt19937 gen{1}; //Fixed seed
    std::uniform_real_distribution<float> d(0,1);
    std::list<float> x,y, value;
    for (int i=0;i<10;++i) for (int j=0;j<10;++j) for (int s=0;s<3;++s) {
        x.push_back(i+d(gen)); y.push_back(j+d(gen)); 
        value.push_back(sqrt((x.back()-5.0f)*(x.back()-5.0f)+(y.back()-5.0f)*(y.back()-5.0f)));
    }
    svg::plot::SVGPlot plt; int p = 0;
    for (auto cmap : {"grayscale","viridis","plasma"}) 
        plt.subplot(1,3,p++).title(cmap).figsize({200,220}).scatter(x,y).c(value).cmap(cmap);
    
    plt.savefig("../docs/svgplot/scatter/example7.svg");
    }
}
