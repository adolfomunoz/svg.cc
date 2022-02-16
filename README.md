# svg.cc
A C++17 library for constructing and manipulating SVG files

##Installation

You can install this library by cloning the repository with all the submodules:

```
git clone --recurse-submodules -j8 https://github.com/adolfomunoz/svg.cc.git
```

If you have already cloned this library without the submodules you will have to add the submodules manually:

```
git clone https://github.com/adolfomunoz/svg.cc.git
cd svg.cc
git submodule update --init --recursive
```

If you want to use this as a submodule from your own libary or code, you will have create a folder within your repository (`ext` in the example below) to hold the submodules. 

```
mkdir ext
cd ext
git submodule add https://github.com/adolfomunoz/patterns.git
git submodule add https://github.com/discord/rapidxml.git 
git submodule add https://github.com/adolfomunoz/svg.cc.git
```

The folder will need to be introduced into the include path for the C++ compilation.


