#!/bin/bash
sudo mount /dev/sdb1 /media
cd /media
sudo cp -r /home/totsugekitai/workspace/mywork/minOS/fs/* .
cd ../
sudo umount /media

