#!/bin/bash

cppcheck -I thirdparty/libcutils/include --std=c11 --enable=all code/
