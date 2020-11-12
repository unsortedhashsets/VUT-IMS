VUT-IMS

PREPARE

1) Download library `wget https://www.fit.vutbr.cz/~peringer/SIMLIB/source/simlib-3.07-20181019.tar.gz`

2) Inside folder `sudo make install`

3) Check directory `ll /usr/local/lib`

4) If `libsimlib.so` in folder => `export LD_LIBRARY_PATH=/usr/local/lib`. Else `goto 1`

5) Try `make` and `./model` => check results in ball2.dat

START

1) `make`

2) `./model` or with params `./model -v -s 1000000 -c 80 -h 1000` 