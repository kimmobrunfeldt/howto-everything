Build OpenCV(+Python support) with Homebrew on OS X
===================================================

Download latest sources for OpenCV from http://sourceforge.net/projects/opencvlibrary/

    # pip and Python is needed
    brew install python

    pip install numpy

    # Fortran compiler is needed to build scipy
    brew install gfortran
    pip install scipy

    # Install Cmake to build opencv
    brew install cmake

    # Go to unpacked opencv sources and run
    mkdir release
    cd release
    cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D BUILD_NEW_PYTHON_SUPPORT=ON -D BUILD_EXAMPLES=ON ..

    # Then run
    make -j8

The make failed for some reason. But running

    brew tap homebrew/science
    brew install opencv

Worked.

Then I had to switch to older opencv version..

    # This has to be done because homebrew-science is another git repository
    cd /usr/local/Library/Taps/homebrew-science/

    # Find older version of opencv
    brew versions opencv

    # Change formula to install older opencv 2.4.5
    git checkout ae74fe9 /usr/local/Library/Taps/homebrew-science/opencv.rb

    # Unlink the current version and install the specified version
    brew unlink opencv
    brew install opencv
