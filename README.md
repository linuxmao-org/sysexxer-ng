# sysexxer-ng
A universal tool to exchange MIDI system-exclusive data.

_It's a rewrite from scratch of [sysexxer](https://sourceforge.net/projects/sysexxer/)._

[![Build Status](https://semaphoreci.com/api/v1/jpcima/sysexxer-ng/branches/master/badge.svg)](https://semaphoreci.com/jpcima/sysexxer-ng)

Documentation page (in French): http://linuxmao.org/Sysexxer-NG

## Building

### Debian Strech

Build dependencies are: cmake, gettext, libasound2-dev, libfltk1.3-dev, and libjack-jackd2-dev or libjack-dev

To launch the build, you can use:

```
git clone --recursive https://github.com/linuxmao-org/sysexxer-ng.git
mkdir sysexxer-ng/build
cd sysexxer-ng/build
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
cmake --build .
```
