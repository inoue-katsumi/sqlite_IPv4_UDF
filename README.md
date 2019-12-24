# sqlite_IPv4_UDF

sqlite_IPv4_UDF is UDF(User Defined Function) for SQLite3. It converts IPv4 string<->Int64 representations. Int64->String is still under development.

## Usage:

```bash
sqlite3 -bail -cmd "SELECT load_extension('/mnt/c/kinoue/misc/ntop.so', 'sqlite3_extension_init');"
sqlite> select inet_ntop_ipv4int("176.32.64.13");
2954903565
```

## ipv4_ntop_int64.c:
Main file which implements sqlite interface and calls glibc inet_ntop, inet_pton


## ipv4_ntop_int64.h:
Just declares functions. May be eliminated in future.

