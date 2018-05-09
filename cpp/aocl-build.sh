#!/usr/bin/env bash
aoc -march=emulator device/matrix_mult.cl -o bin/matrix_mult.aocx
aoc -march=emulator device/vector_add.cl -o bin/vector_add.aocx