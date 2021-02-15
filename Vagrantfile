Vagrant.configure("2") do |config|
  config.vm.box = "bento/ubuntu-20.04"
  config.vm.box_check_update = false
  config.vm.provider "virtualbox" do |v|
    v.gui = true
    v.linked_clone = true
  config.vm.provision "shell", inline: <<-SHELL
    apt-get update
    apt-get install -y nano wget build-essential net-tools ifupdown vim python-is-python3 gdb
    git clone https://github.com/tavaresrodrigo/StackOverflow.git
   SHELL
  end
  config.vm.define "stackoverflow" do |stackoverflow|
    stackoverflow.vm.hostname = "stackoverflow"
    stackoverflow.vm.network "forwarded_port", guest: 22, host: 2201
    stackoverflow.vm.network "private_network", ip: "192.168.10.11" 
  end
end