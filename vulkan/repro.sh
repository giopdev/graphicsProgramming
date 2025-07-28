
#!/bin/bash

cd ./build

echo -e "\n\n\n\n" | LD_PRELOAD=./libvk_ipc_override.so ./load_game ./tri
