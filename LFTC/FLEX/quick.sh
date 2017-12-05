#!/bin/bash

bison -d miniLang.y
flex miniLang.lex
g++ lex.yy.c miniLang.tab.c