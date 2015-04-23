#!/bin/bash

sudo losetup /dev/loop0 floppy.img
sudo mount /dev/loop0 /mnt/myfloppy
sudo cp src/kernel /mnt/myfloppy/grub/kernel
sudo cp initrd.img /mnt/myfloppy/initrd.img
sudo umount /dev/loop0
sudo losetup -d /dev/loop0
