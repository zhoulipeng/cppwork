# mount memory disk to /path/to/out
mount -t tmpfs -o size=1G,nr_inodes=40k,mode=1777 tmpfs /path/to/out
