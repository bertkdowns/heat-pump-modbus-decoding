Install with

```
sudo cp hp-modbus.service /etc/systemd/system/hp-modbus.service 
sudo systemctl enable hp-modbus.service

sudo cp hp-electrical.service /etc/systemd/system/hp-electrical.service 
sudo systemctl enable hp-electrical.service


sudo cp hp-ni-cdaq.service /etc/systemd/system/hp-ni-cdaq.service 
sudo systemctl enable hp-ni-cdaq.service


sudo cp hp-control.service /etc/systemd/system/hp-control.service 
sudo systemctl enable hp-control.service
```