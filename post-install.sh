#!/bin/bash

echo -n "[INFO] Enabling i915kms..."
sleep 1
cat <<EOF >> /etc/rc.conf

kld_list="/boot/modules/i915kms.ko"
EOF


