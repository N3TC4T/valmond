#!/bin/bash

function disable_systemd {
    systemctl disable rippledagent
    rm -f /lib/systemd/system/rippledagent.service
}

function disable_update_rcd {
    update-rc.d -f rippledagent remove
    rm -f /etc/init.d/rippledagent
}

function disable_chkconfig {
    chkconfig --del rippledagent
    rm -f /etc/init.d/rippledagent
}

if [[ -f /etc/redhat-release ]]; then
    # RHEL-variant logic
    if [[ "$1" = "0" ]]; then
	rm -f /etc/default/rippledagent

	which systemctl &>/dev/null
	if [[ $? -eq 0 ]]; then
	    disable_systemd
	else
	    # Assuming sysv
	    disable_chkconfig
	fi
    fi
elif [[ -f /etc/debian_version ]]; then
    # Debian/Ubuntu logic
    if [[ "$1" != "upgrade" ]]; then
	# Remove/purge
	rm -f /etc/default/rippledagent

	which systemctl &>/dev/null
	if [[ $? -eq 0 ]]; then
	    disable_systemd
	else
	    # Assuming sysv
	    disable_update_rcd
	fi
    fi
fi
