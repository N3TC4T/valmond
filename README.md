# Valmond
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
Will release in the debian package repository
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
