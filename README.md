# rippled-agent
> Rippled Monitoring Agent

## What
 The Rippled Monitoring Agent is a lightweight process that monitors system
 metrics and rippled validator and sends information back to monitoring backend.


## Install

**Install the XRPL Labs RPM repository:**

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

**Install the new agent package:**

```
sudo yum install rippledagent
```

**Enable and start the service:**

```
sudo systemctl enable rippledagent.service
sudo systemctl start rippledagent.service
```




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
git clone https://github.com/N3TC4T/rippled-agent.git
git submodule update --init --recursive
mkdir rippled-agent/build
cd $_
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .

```

## License
Released under the terms of the MIT license. See `LICENSE` for more
information.
