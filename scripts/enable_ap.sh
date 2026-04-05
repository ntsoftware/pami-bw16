#!/bin/bash

# Enable WiFi access point on Debian Linux.

ifname=wlp2s0
ssid=TDS-Team
password=password

nmcli device wifi hotspot ifname $ifname ssid $ssid password $password
nmcli connection modify Hotspot \
  ipv6.method disabled \
  ipv4.method manual \
  ipv4.addresses 192.168.254.1/24
nmcli connection down Hotspot
nmcli connection up Hotspot

echo "Enabled WiFi access point."
echo "SSID: $ssid"
echo "password: $password"
