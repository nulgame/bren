# bren
Business rule calculation engine with LUA script-based rules.

## Requirements

Before you start building the solution witch _clang_ or _g++_, you need to install required dependencies.

- C++ compiler -std=c++17 or higher (g++ or clang)
- Make
- Pistache - REST framework for C++
- RapidJson
- Lua 5.3+ - for rule scripting
- Sol2/3 - C++ bindings for Lua (header only library)
- optional: luarocks install lsqlite3complete (only for running rule018.lua)

### On MAC M1

```shell
$ brew install pistache
$ brew install rapidjson
$ brew install lua
$ brew install luarocks
```

### On Linux

Depending on Linux distribution use proper command to install dependencies.

## Building

Assuming you have already C++ compiler and _make_ and all dependencies, build solution as follow:

### On MAC M1

```shell
$ make -f Makefile-macm1
```

### On Linux

```shell
$ make -f Makefile-linux
```

## Installing


### Rules

The rules are located in the _rules/_ directory.


## Running

_bren_ is starting as foreground service on TCP port 2345 but using _--port number_ you can pass other port.

```shell
$ ./business_rule_engine --port 12345
```


## Testing

_test/_ directory contains some simple examples. Service has been tested on Mac M1, Macbook Pro Intel, VoidLinux and ArchLinux.


## Licence

MIT
