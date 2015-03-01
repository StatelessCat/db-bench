# db-bench
A small app for benchmarking KDE baloo files databases

## compilation:
```shell
clang main.c -o db-bench -lsqlite3
```

## execution:
```
./db-bench <number of requests>
```

### for requesting 1000000 times a pseudo-random file from baloo file database:
```
time ./db-bench 1000000
```
Output:
```
DB /home/raphael/.local/share/baloo/file/fileMap.sqlite3 [OK]
DB contains 190851 tuples
./db-bench 1000000  5.27s user 0.76s system 99% cpu 6.036 total
```

### interpration
1000000 requests are done in 5.27s

So we have approximatly 189.700 req/s, that's nice
