# elen-ths40
This is a commandline utility used for reading the temperature and humidity values from
Elen-THS40 PoE based temperature sensor
Elen-THS40 has a default static ip 192.168.0.68 and default tcp-modbus port 502

# How to access webUI of Elen-THS40 with static ip address on dhcp network(router network 192.168.1.1)
 1. ``ip route add 192.168.0.68 via 192.168.1.1````
 2. ``ping 192.168.0.68````
 3. ``socat tcp-l:8888,fork,reuseaddr tcp:192.168.0.68:80````
 3. ``curl http://127.0.0.1:8888````

# How to checkout, build and run elen-ths40 utility
 1. ```git clone https://github.com/hackboxguy/elen-ths40.git```
 2. ```cd elen-ths40```
 3. ```cmake -H. -BOutput -DCMAKE_INSTALL_PREFIX=~/tmp/elen-ths40/```
 4. ```cmake --build Output -- install -j$(nproc)```
 5. ```~/tmp/elen-ths40/sbin/elen-ths40 -i 192.168.0.68 -p 502 -a humidity```
