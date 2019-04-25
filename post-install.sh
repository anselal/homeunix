#!/bin/bash

# Make sure only root can run our script
if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

awk -F, '{ print "[INFO] Installing "$2 system("pkg install -y "$1)}' progs.csv

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

echo "[INFO disabling beep for current session]"
sysctl hw.syscons.bell=0

echo "[INFO disabling beep globally]"
cat <<EOF >> /etc/sysctl.conf

hw.syscons.bell=0
EOF
