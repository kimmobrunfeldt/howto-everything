Install and use gcc-4.9 homebrew
================================

Update just in case

    brew update

Install gcc

    brew tap homebrew/versions
    brew install gcc49


When building with this version, use:

    ./configure CC=/usr/local/Cellar/gcc49/4.9-20131027/bin/gcc-4.9

Give make flags:

    make CFLAGS="-L/usr/local/opt/gmp4/lib -I/usr/local/opt/gmp4/include -L/usr/local/opt/mpfr2/lib -I/usr/local/opt/mpfr2/include -L/usr/local/opt/libmpc08/lib -I/usr/local/opt/libmpc08/include -L/usr/local/opt/isl011/lib -I/usr/local/opt/isl011/include -L/usr/local/opt/cloog018/lib -I/usr/local/opt/cloog018/include -L/usr/local/Cellar/gcc49/4.9-20131027/lib/gcc/x86_64-apple-darwin12.4.0/4.9.0/"

Configure usually sets also some CFLAGS, remember to add them also!
