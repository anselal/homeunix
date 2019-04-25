#!/bin/bash

awk -F, '{ print "[INFO] Installing "$2 system("sudo pkg install -y "$1)}' progs.csv

echo "[INFO] Enabling i915kms..."
sleep 1
cat <<EOF >> /etc/rc.conf

kld_list="/boot/modules/i915kms.ko"
EOF

echo "[INFO] Setting UTF-8 locale"
sleep 1
cat <<EOF >> /etc/profile

LANG=en_US.UTF-8;   export LANG
CHARSET=UTF-8;  export CHARSET
EOF
