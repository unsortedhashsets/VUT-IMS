VUT-IMS

1) Download library

2) Inside folder `sudo make install`

3) Check directory `ll /usr/local/lib`

4) If `libsimlib.so` in folder => `export LD_LIBRARY_PATH=/usr/local/lib`. Else `goto 1`

5) Try `make` and `./model` => check results in ball2.dat