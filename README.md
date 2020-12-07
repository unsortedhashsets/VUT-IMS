VUT-IMS

Prepare

1) Download library `wget https://www.fit.vutbr.cz/~peringer/SIMLIB/source/simlib-3.07-20181019.tar.gz`

2) Inside folder `sudo make install`

3) Check directory `ll /usr/local/lib`

4) If `libsimlib.so` in folder => `export LD_LIBRARY_PATH=/usr/local/lib`. Else `goto 1`

5) Try `make` and `./covid19` => check results in covid19.csv

Start experiments

1) `make run0`

2) `make run1`

3) `make run2`

4) `make run3`

5) `make runcz-pes3`

6) `make runcz-pes4`
