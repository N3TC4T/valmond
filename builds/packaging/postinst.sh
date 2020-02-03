#!/bin/bash

LOG_DIR=/var/log/rippledagent
SCRIPT_DIR=/opt/rippledagent/scripts/
HOME_DIR=/home/rippledagent

function install_init {
    cp -f $SCRIPT_DIR/init.sh /etc/init.d/rippledagent
    chmod +x /etc/init.d/rippledagent

    echo "### You can start rippledagent by executing"
    echo ""
    echo " sudo service rippledagent start"
    echo ""
    echo "###"
}

function install_systemd {
    cp -f $SCRIPT_DIR/rippledagent.service /lib/systemd/system/rippledagent.service
    systemctl enable rippledagent || true
    systemctl daemon-reload || true
    echo "### You can start rippledagent by executing"
    echo ""
    echo "sudo systemctl start rippledagent.service"
    echo ""
    echo "###"
}

function install_update_rcd {
    update-rc.d rippledagent defaults
}

function install_chkconfig {
    chkconfig --add rippledagent
}

id rippledagent &>/dev/null
if [[ $? -ne 0 ]]; then
    useradd --system --user-group --key USERGROUPS_ENAB=yes -M rippledagent --shell /bin/false -d /etc/opt/rippledagent
fi


test -d $LOG_DIR || mkdir -p $LOG_DIR
chown -R -L rippledagent:rippledagent $LOG_DIR
chmod 755 $LOG_DIR


# Create a dummy home directory, Sensu plugins need this for some reason
test -d $HOME_DIR || mkdir -p $HOME_DIR
chown -R -L rippledagent:rippledagent $HOME_DIR

# Remove legacy symlink, if it exists
if [[ -L /etc/init.d/rippledagent ]]; then
    rm -f /etc/init.d/rippledagent
fi

# Add defaults file, if it doesn't exist
if [[ ! -f /etc/default/rippledagent ]]; then
    touch /etc/default/rippledagent
fi

# Make sure the config directory exists
if [ ! -d /etc/opt/rippledagent ]; then
    mkdir -p /etc/opt/rippledagent
fi


# Make sure the pid directory exists
if [ ! -d /var/run/rippledagent ]; then
    mkdir -p /var/run/rippledagent
fi

chown -R -L rippledagent:rippledagent  /var/run/rippledagent
chmod 775 /var/run/rippledagent


# Make sure the binary is executable
chmod +x /usr/bin/rippledagent
chmod +x /opt/rippledagent/rippledagent


# Distribution-specific logic
if [[ -f /etc/redhat-release ]]; then
    # RHEL-variant logic
    which systemctl &>/dev/null
    if [[ $? -eq 0 ]]; then
       install_systemd
    else
       # Assuming sysv
       install_init
       install_chkconfig
    fi
elif [[ -f /etc/debian_version ]]; then
    # Debian/Ubuntu logic
    which systemctl &>/dev/null
    if [[ $? -eq 0 ]]; then
        install_systemd
        systemctl restart rippledagent || echo "WARNING: systemd not running."
    else
        # Assuming sysv
        install_init
        install_update_rcd
        invoke-rc.d rippledagent restart
    fi
elif [[ -f /etc/os-release ]]; then
    source /etc/os-release
    if [[ $ID = "amzn" ]]; then
        # Amazon Linux logic
        install_init
        install_chkconfig
    fi
fi
