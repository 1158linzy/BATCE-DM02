#!/bin/sh

find -type f | xargs enca -L zh_CN -x UTF-8
