sudo mount /dev/sdb1 mnt
sudo cp bin/pi3/hello-world.hex mnt/kernel7.img
# sudo cp kernel7.img mnt
md5sum mnt/kernel7.img
md5sum bin/pi3/hello-world.hex
sudo umount mnt

