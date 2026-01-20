# Setup

You need to install Ni-DAQmx for the driver support:

[https://www.ni.com/en/support/downloads/drivers/download.ni-daq-mx.html](https://www.ni.com/en/support/downloads/drivers/download.ni-daq-mx.html)


You also need to install the system configuration utility to register the device: [Download Link](https://www.ni.com/en/support/downloads/drivers/download.system-configuration.html#575750)

If you want, you can also install labview community edition: [https://www.ni.com/en/support/downloads/software-products/download.labview-community.html](https://www.ni.com/en/support/downloads/software-products/download.labview-community.html)


The download for the driver and system configuration utility just adds the apt repositiory in ubuntu. So then you need to install it with:

```
sudo apt update
```
 
 (to register the packages from the new repository)

```
sudo apt install ni-daqmx # I think - or maybe it's ni-software-stream-jammy or ni-software-2025-jammy 
sudo apt install ni-labview-2025-jammy-community # note this might be slightly different if there is a different version, use apt search or something
```


Once you've done all that, you might need to restart your system too - i recommend doing that.


YOu can then use the hardware confgiuration utility (there should be a desktop shortcut in your applications menu) to connect your defice.

hopefully it auto-discovers, and then it's easy. if not, use `ip addr` to figure out what ethernet interface it's on and it's ip adress. if your device and the cDAQ is both connected to the same network, you could look at the router config to get the ip address of the cdaq, or scan the ip range of the router (e.g `nmap -sn 192.168.1.0/24` to scan 192.168.1.*). Then you can manually enter that IP and connect to it. It's pretty obvious with nmap - it'll show up as a NI CDAQ or something if you ca access it.



# MQTT

To view all messages sent over mqtt, run


```
mosquitto_sub -t "#" -v
```