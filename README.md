To cross-compile glibc for i386:

```console
mkdir -p glibc/src glibc/build
git clone git://sourceware.org/git/glibc.git glibc/src

cd glibc/build
../src/configure --prefix=`pwd`/install                         \
    --host=i686-linux-gnu --build=i686-linux-gnu                \
    CC="gcc -m32" CXX="g++ -m32"                                \
    CFLAGS="-g -O2 -march=i686" CXXFLAGS="-g -O2 -march=i686"   \
    LDFLAGS="-g -O2"
make
make install
```

To build `simple_bmp`:

```console
GLIBCDIR=/path/to/glibc/build/install S2EDIR=/path/to/s2e/env make
```
