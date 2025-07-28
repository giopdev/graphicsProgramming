#!/bin/bash

cd ./build

LD_PRELOAD=./libvk_ipc_override.so ./load_game ./tri
