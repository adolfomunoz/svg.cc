#pragma once

namespace svg {
namespace plot {

class arange {
	float start, stop, step;
	
public:
	arange(float start, float stop, float step) :
		start(start), stop(stop), step(step) { }
	arange(int n) :
		start(0), stop(n), step(1) {}

	std::size_t size() const {
		return std::size_t((stop-start)/step)+1;
	}
	
	using value_type=float;
    
    arange operator+(float v) const {
        return arange(start+v,stop+v,step);
    }
    arange operator-(float v) const {
        return arange(start-v,stop-v,step);
    }
    arange operator*(float v) const {
        return arange(start*v,stop*v,step*v);
    }
    arange operator/(float v) const {
        return arange(start/v,stop/v,step/v);
    }

	
	class const_iterator {
		friend class arange;
		float x, step;
		const_iterator(float x , float step) :
			x(x), step(step) {}

	public:
		const_iterator() : const_iterator(0,0) {}

		float operator*() const { return x; }
		const_iterator& operator++() {
			x+=step; return (*this);
		}
		const_iterator operator++(int) {
			const_iterator i = (*this); ++(*this); return i;
		}
		bool operator==(const const_iterator& that) const {
			return (that.x > (x-0.5f*step)) && (that.x <= (x+0.5f*step));
		}
		bool operator!=(const const_iterator& that) const {
			return !((*this)==that);
		}
	};
	
	const_iterator begin() const { return const_iterator(start,step); }
	const_iterator end() const { return const_iterator(stop,step); }

	operator std::vector<float>() const {
		std::vector<float> s;
		for (float x : (*this)) s.push_back(x);
		return s;
	}

	operator std::list<float>() const {
		std::list<float> s;
		for (float x : (*this)) s.push_back(x);
		return s;
	}
    
    float front() const { return start; }
    float back() const { unsigned long n = (stop-start)/step; return start+n*step; }
};
}
}

namespace std {
template<>
struct iterator_traits<svg::plot::arange::const_iterator> {
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::size_t;
	using value_type = float;
	using reference = float&;
};
}