sqlite-json
===========

A simple C program that runs a sqlite session, the SQL query results are presented in JSON. 
Queries run interactively in a very simple shell.


Dependency library debian/ubuntu
--------------------

libsqlite3-dev

Compile
-------

gcc -o sqlite_json main.c -lsqlite3 -std=c99


Run
---
./sqlite_json database.db

