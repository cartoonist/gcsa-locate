gcsa-locate
===========
This tool uses GCSA2 index to locate k-mers in the underlying graph.

Installation
------------
### Installing dependencies
- `g++` (>= 4.9)
- `zlib` (>=1.2.8) and `bz2` libraries:

      sudo apt-get install zlib1g-dev libbz2-dev

- Seqan2 (>=2.3.2): you can download seqan package from [here](http://packages.seqan.de).
  Then, unpack the library package to `PREFIX` (e.g. `/usr/local`). Make sure that the
  `seqan-2.pc` located in the `$PREFIX/lib/pkgconfig` directory contains correct value for
  `prefix`; e.g. in case that the library is installed in `/usr/local`, it should be:

      prefix=/usr/local

- SDSL library (>=2.1.0): to download and install SDSL library see its
  [GitHub page](https://github.com/simongog/sdsl-lite).
- [GCSA2 (>=1.0.0)](https://github.com/jltsiren/gcsa2): this library does not provide a
  proper installation script. After compiling the library, you can use
  [this script](https://gist.github.com/cartoonist/32fc8595d8e1a6255c44067d1eb0dfcc#gistcomment-2172472)
  to install the library to `PREFIX` (e.g. `/usr/local`):

      chmod u+x install.sh
      ./install.sh /usr/local

### Building `gcsa-locate`
In order to build the tool, simply run:

    ./configure
    make
    sudo make install

Usage
-----
Consult with the man page:

    man gcsa_locate
