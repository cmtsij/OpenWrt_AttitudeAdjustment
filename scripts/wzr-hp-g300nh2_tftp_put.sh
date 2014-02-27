bin=${1:-tftp.bin}
sudo ifconfig eth1 192.168.11.2
sudo arp -s 192.168.11.1 02:aa:bb:cc:dd:1a
curl -T $bin tftp://192.168.11.1
