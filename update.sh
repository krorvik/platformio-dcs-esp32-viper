#!/bin/bash
git submodule init
git submodule update
./generate.py  > src/f16c_data.h
