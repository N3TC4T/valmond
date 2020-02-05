#!/bin/bash

LOG_DIR=/var/log/valmond
SCRIPT_DIR=/opt/valmond/scripts/
HOME_DIR=/home/valmond

function install_init {
    cp -f $SCRIPT_DIR/init.sh /etc/init.d/valmond
    chmod +x /etc/init.d/valmond

    echo "### You can start valmond by executing"
    echo ""
    echo " sudo service valmond start"
    echo ""
    echo "###"
}

function install_systemd {
    cp -f $SCRIPT_DIR/valmond.service /lib/systemd/system/valmond.service
    systemctl enable valmond || true
    systemctl daemon-reload || true
    echo "### You can start valmond by executing"
    echo ""
    echo "sudo systemctl start valmond.service"
    echo ""
    echo "###"
}

function install_update_rcd {
    update-rc.d valmond defaults
}

function install_chkconfig {
    chkconfig --add valmond
}

id valmond &>/dev/null
if [[ $? -ne 0 ]]; then
    useradd --system --user-group --key USERGROUPS_ENAB=yes -M valmond --shell /bin/false -d /etc/opt/valmond
fi


test -d $LOG_DIR || mkdir -p $LOG_DIR
chown -R -L valmond:valmond $LOG_DIR
chmod 755 $LOG_DIR


# Create a dummy home directory, Sensu plugins need this for some reason
test -d $HOME_DIR || mkdir -p $HOME_DIR
chown -R -L valmond:valmond $HOME_DIR

# Remove legacy symlink, if it exists
if [[ -L /etc/init.d/valmond ]]; then
    rm -f /etc/init.d/valmond
fi

# Add defaults file, if it doesn't exist
if [[ ! -f /etc/default/valmond ]]; then
    touch /etc/default/valmond
fi

# Make sure the config directory exists
if [ ! -d /etc/opt/valmond ]; then
    mkdir -p /etc/opt/valmond
fi


# Make sure the pid directory exists
if [ ! -d /var/run/valmond ]; then
    mkdir -p /var/run/valmond
fi

chown -R -L valmond:valmond  /var/run/valmond
chmod 775 /var/run/valmond


# Make sure the binary is executable
chmod +x /usr/bin/valmond
chmod +x /opt/valmond/valmond


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
        systemctl restart valmond || echo "WARNING: systemd not running."
    else
        # Assuming sysv
        install_init
        install_update_rcd
        invoke-rc.d valmond restart
    fi
elif [[ -f /etc/os-release ]]; then
    source /etc/os-release
    if [[ $ID = "amzn" ]]; then
        # Amazon Linux logic
        install_init
        install_chkconfig
    fi
fi
