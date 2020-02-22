# Valmond
[![Build Status](https://travis-ci.org/N3TC4T/valmond.svg?branch=master)](https://travis-ci.org/N3TC4T/valmond)
![Contributions welcome](https://img.shields.io/badge/contributions-welcome-orange.svg)

> XRPL Validator Monitoring Agent

## What
 The Valmond is a lightweight process/service that monitors system
 metrics and XRPL validator and sends information back to monitoring backend.


## Install


<details>
<summary>CentOS/Red Hat</summary>
<br>

**Install the XRPL-Labs RPM repository:**

```
cat << REPOFILE | sudo tee /etc/yum.repos.d/xrpl-labs.repo
[xrpl-labs-stable]
name=XRPL Labs Packages
baseurl=https://packages.xrpl-labs.com/rpm/stable/
enabled=1
gpgcheck=0
gpgkey=https://packages.xrpl-labs.com/rpm/stable/repodata/repomd.xml.key
repo_gpgcheck=1
REPOFILE

```

**Fetch the latest repo updates:**

```
sudo yum -y update
```

**Install the agent package:**

```
sudo yum install valmond
```

**Enable and start the service:**

```
sudo systemctl enable valmond.service
sudo systemctl start valmond.service
```

</details>

<details>
<summary>Ubuntu/Debian</summary>
<br>

**Fetch the latest repo updates:**

```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt -y update
```

**Install utilities:**

```
sudo apt -y install libstdc++6 apt-transport-https ca-certificates wget gnupg
```

**Add XRPL-Labs's package-signing GPG key to your list of trusted keys:**

```
wget -O - https://packages.xrpl-labs.com/deb/conf/public.gpg.key | apt-key add -
```

**Check the fingerprint of the newly-added key:**

```
sudo apt-key finger
```

The output should include an entry for XRPL Labs such as the following:

```
pub   2048R/7517CB9A 2020-02-02 [expires: 2022-02-01]
      Key fingerprint = 61E2 39A8 8D7D 1116 7CAA  B7B1 5BC3 9E2D 7517 CB9A
uid                  XRPL Labs <hello@xrpl-labs.com>
sub   2048R/07E9F8B9 2020-02-02 [expires: 2022-02-01]
```

In particular, make sure that the fingerprint matches. (In the above example, the fingerprint is on the second line, starting with 61E2.)


**Add the appropriate XRPL Labs repository for your operating system version:**

```
echo "deb https://packages.xrpl-labs.com/deb/ bionic stable" | sudo tee -a /etc/apt/sources.list.d/xrpl-labs.list
```

The above example is appropriate for **Ubuntu 18.04 Bionic Beaver**. For other operating systems, replace the word bionic with one of the following:

 - xenial for Ubuntu 16.04 Xenial Xerus
 - stretch for Debian 9 Stretch
 
**Fetch the XRPL Labs repository:**

```
sudo apt -y update
```

**Install the agent package:**

```
sudo apt -y install valmond
```

**Enable and start the service:**

```
sudo systemctl enable valmond.service
sudo systemctl start valmond.service
```


</details>


## Build Instructions
```bash
# grab the dependencies if you haven't already
sudo apt install git \
                 build-essential \
                 cmake \
                 e2fslibs-dev \
                 libblkid-dev \
                 libcurl-dev

# download the source and build
git clone https://github.com/N3TC4T/valmond.git
cd valmond
git submodule update --init --recursive
mkdir build
cd $_
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .

```


## CLI

```
USAGE: ./valmond [sign <content>] [-c/--config <config file path>] [-h / --help]
```

Sign a message with your validator token

```
/usr/bin/valmond sign [message]
```


## License
Released under the terms of the MIT license. See `LICENSE` for more
information.
