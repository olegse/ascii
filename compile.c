#!/bin/bash

gcc -Wall -O2 -o notesearch notesearch.c func.c && \
chmod u+s notesearch
