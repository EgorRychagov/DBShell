# Data Base Shell

## *Description*
Data base is built by using unordered key-value containers. It lets you contain values as soon as program is running, otherwise records will not be saved.\
Since program is running, firstly choose a host mode: `DEF` (default host launch) or `BM` (benchmark). If not specifying mode as `BM`, program will start in default mode in any other case.

## Host launch
This is a default program launch, that provides an access to data base via *TCP client* or *local network connection* while program is running. You are able to interract with it through console and possible options will be shown there as well by typing `help`.

> **Interraction**
> - For recording use next synthax: `<type> <key> <record>`. Possible types are string, bool or double;
> - In order to get a record, command `get <key>` is required (returns a record at the key if exists);
> - Deleting record takes `delete <key>` erases record at the key if exists;
> - The command `keys` returns existing keys or warns about its absence;
> - `size` returns amount of records;
> - Use `disconnect` to finish your work on a programm.

## Benchmark
While running a program in benchmark mode, it measures average recording time in *ms* of 100 records for every type with different initial size of records. Initial size varies from 10 thousands to 10 millions of records, multiplaying by 10 on evey stage.\
> **Latest results**
> |Type|AVG Time (ms)|Initial Size|
> |:-|:-|:-|
> |string|0.00358|10000|
> |bool|0.00395|10000|
> |double|0.00372|10000|
> |string|0.0037 |100000|
> |bool|0.00338|100000|
> |double|0.00377 |100000|
> |string|0.00446|1000000|
> |bool|0.00826 |1000000|
> |double|0.00405|1000000|
> |string|0.08846|10000000|
> |bool|0.07921|10000000|
> |double|0.01131|10000000|
