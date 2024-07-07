Exercises for Compiler
===============================================

This repository contains the specifications for the three exercises for the Compiler.

The three exercises are:

1 - [Lexers and regular expressions](1-regexes).

2 - [Parsers and ASTs](2-parsers).

3 - [Code Generation](3-codegen).

Test environment
================

The target environment for this is Ubuntu 22.04. Those using Mac OS should watch out in particular, as the build environment is often subtly different.

If you want to work on your own machine, you can install a version of Ubuntu
quite easily. For this, we recommend 3 options:
- [Docker](#docker)
- [Vagrant with Virtual Box](#vagrant)
- [Windows Subsystem for Linux (WSL) - Windows Only](#wsl-windows-only)

A guide to each of the installations is found below.

## Docker
**Requirements:**
- [Docker](https://www.docker.com/)

This repository contains a [Dockerfile](Dockerfile), which is a script that sets up a blank Ubuntu 22.04 environment, and then installs all the tools that the lab exercises require. If you configure your testing environment to match the Dockerfile script, you should be fine.

You can enter the virtual machine described by the Dockerfile by going to the directory that contains the Dockerfile and running the following series of commands. First, create the virtual machine "image" by running (in Terminal on Mac, or Powershell on Windows):

    docker build -t "compilers_labs" .
	
(The `.` at the end is part of the command, by the way. It tells Docker to look in the current directory for the Dockerfile script.) Then create a "container" so that you can enter the virtual machine by running (in Terminal on Mac, or Powershell on Windows):

    docker run --rm -it -v "${PWD}:/code" -w "/code" --name "compilers_labs_machine" compilers_labs
	
You are now in an Ubuntu 22.04 shell with all the required tools installed. (By the way, the `-it` flag instructs Docker to create a shell through which you can interact with the virtual machine. The `-v "${PWD}:/code"` and `-w "/code"` flags mean that your host machine's files are accessible to your virtual machine under the `/code` path in the Docker container.) Note that you should only work under the `/code` path as the rest of the container filesystem is temporary, meaning it is removed when you exit the Docker container.

When you're finished, you can leave the shell by typing:

    exit
	
If you want to clean up, you can remove the container you created by typing:

	docker ps -a
	docker rm compilers_labs_machine
	
and then removing the image by typing:

	docker images
	docker rmi compilers_labs
	
If you are also using VS Code, and have the [Dev Containers extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers), you can develop directly inside the container by using the [.devcontainer/devcontainer.json](.devcontainer/devcontainer.json) file. Make sure that this `.devcontainer` folder is in the same directory as your `Dockerfile`, then press `View -> Command Palette` in VS Code and search for `>Dev Containers: Reopen in Container`.
	
## Vagrant
**Requirements:**
- [VirtualBox](https://www.virtualbox.org/)
- [Vagrant](https://www.vagrantup.com/)

This repository contains a [Vagrantfile](Vagrantfile), which is a script that sets up a blank Ubuntu environment, and then installs all the tools that the lab exercises require. To start a virtual machine (VM) using Vagrant, follow these steps.

- Open a shell in this directory.

- Run `vagrant up` to download, install, and configure the VM.

- Run `vagrant ssh` to log into the VM.

- Run `cd /vagrant` : This will put you in the same directory, with
  your host files shared with the VM.

You can then use command line build tools in the VM, while editing
the source files in your host editor.

If you want to stop the machine,

- Run `exit` to exit the session,

- Run `vagrant halt` to shut down the virtual machine.

If you later run `vagrant up` again, it will not need to download the VM from scratch.

	
## WSL (Windows only)
- [Windows
Subsystem for Linux (WSL)](https://docs.microsoft.com/en-us/windows/wsl/about)

Check which version of WSL you're using by running `lsb_release -d` and check that it's at least Ubuntu 22.04. Once you're on the correct version, simply ensure you have all the right packages installed by running:
```
sudo apt-get install -y lsb-release g++ gdb make dos2unix git bison flex qemu
```
From here you should be good to go!