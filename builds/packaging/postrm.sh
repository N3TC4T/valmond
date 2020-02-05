#!/bin/bash

function disable_systemd {
    systemctl disable valmond
    rm -f /lib/systemd/system/valmond.service
}

function disable_update_rcd {
    update-rc.d -f valmond remove
    rm -f /etc/init.d/valmond
}

function disable_chkconfig {
    chkconfig --del valmond
    rm -f /etc/init.d/valmond
}

if [[ -f /etc/redhat-release ]]; then
    # RHEL-variant logic
    if [[ "$1" = "0" ]]; then
	rm -f /etc/default/valmond

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
	rm -f /etc/default/valmond

	which systemctl &>/dev/null
	if [[ $? -eq 0 ]]; then
	    disable_systemd
	else
	    # Assuming sysv
	    disable_update_rcd
	fi
    fi
fi
