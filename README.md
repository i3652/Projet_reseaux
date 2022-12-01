# Projet_reseaux

# Install and configure the project

```bash
git clone https://github.com/i3652/Projet_reseaux.git

```

#### Delete interfaces

```bash
nmcli connection del "Wired connection 1"
nmcli connection del "Wired connection 2" 

```

#### Start ansible palybook
```bash
sudo ansible-playbook -c local -v /vagrant/config.yml
```

#### Start server and client on VM1 and VM3 and create tun0
##### On VM1
```bash
cd /mnt/partage/src
sudo ./tunnel64d tun0 fc00:1234:ffff::/64
```
##### On VM3:
```bash
cd /mnt/partage/src
sudo ./tunnel64d tun0 fc00:1234:ffff::/64 172.16.2.131
```
