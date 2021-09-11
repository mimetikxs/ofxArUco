# ofxArUco

This fork uses a modiffied version of aruco that fixes some issues with the original repo (see commits on [ofxAruco2](https://github.com/mimetikxs/ofxAruco2) for more details).

The sole purpose of this fork is to be able to use aruco with [ofxSpinnaker](https://github.com/mimetikxs/ofxSpinnaker) in my projects. 

I've previously tried to use [ofxAruco2](https://github.com/mimetikxs/ofxAruco2) (which doesn't use ofxAddonLib) with ofxSpinnaker, but I've been unable to compile.

The only way to make these two addons work together was to use this addon, where aruco is pre-compiled as a dll. 

This has the disadvantage of having to use absolute paths when building the libs, so one has to build aruco (and opencv) in every new machine.

To use this addon in your machine:

## 1. Install OpenCv
- Download the pre-built libraries for Visual Studio: OpenCV 4.5.1
- Extract the contents to your desired location (I'm using C:/dev/opencv/)
- Done

## 2. Download ArUco and compile it as dll using the installed opencv's version
- Detailed instructions [here](https://github.com/mimetikxs/Build_ArUco_and_MarkerMapper).

## 3. After compiling aruco:
- Update the libs, includes and dlls with the files you just compiled

## 4. Manually copy opencv's dll to your app bin folder 
- As ofxArucoLib relays on your version of openCv, you'll have to manually copy **the dll (eg: opencv_world451.dll)** to your app's bin folder (or add it to the project properties).