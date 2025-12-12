<h1 align="center">Welcome to NIKA 👋</h1>
<p>
  <a href="https://www.gnu.org/licenses/gpl-3.0.html" target="_blank">
    <img alt="GPLv3" src="https://img.shields.io/badge/License-GPLv3-yellow.svg" />
  </a>
</p>

> **N**IKA is an **I**ntelligent **K**nowledge-driven **A**ssistant

## About
NIKA is an ostis-system designed with [OSTIS Technology principles](https://github.com/ostis-ai) in mind. 

You can learn more about it by asking the assistant: "What's NIKA?"

## Run documentation

```sh
#Terminal
cd nika
pip3 install mkdocs markdown-include mkdocs-material mkdocs-i18n
mkdocs serve
```

Then open http://127.0.0.1:9001/ in your browser

## ✨ Demo
![demo.png](docs/images/demo.png)


# Requirements
* A computer running Windows 10/11, macOS (10.15+) or 
Linux (Ubuntu/Debian/Fedora-like).
* Internet access.
* Administrator / sudo rights (for installing programs).
* You will need Docker (with Compose plugin) installed and running on your machine.
* A little patience — it takes time to install Git, WSL, Docker and download images (10-60 minutes, depending on the Internet speed).

# 0. General training

* PowerShell / Terminal is the place where you enter commands.
* Windows — PowerShell (Running as an administrator is recommended).
* macOS / Linux — Terminal.
* Git — is a system for downloading source code (repositories).
* Docker — is a program that runs "containers" (as small virtual machines).
* WSL (Windows Subsystem for Linux, only on Windows) — is a special feature in Windows that allows you to run Linux inside Windows.

# 1. Installing Git

### Windows
1. Open a browser, go to the Git for Windows [website](https://git-scm.com/download/win) and download the installer.
2. Run the downloaded file → Next button to the end → Install.
3. After installation, open PowerShell and run:
```
git --version
```
You should see the version number (for example, git version 2.x.x).

### macOS
Open the Terminal (Launchpad → Terminal) and enter:
```
git --version
```
If Git is not installed, macOS will offer to install "Command Line Tools" — you agree and wait for the installation.

### Linux (Ubuntu/Debian)
In the terminal:
```
sudo apt update
sudo apt install -y git
git --version
```
# 2. Installing Docker
Important: On Windows, Docker Desktop uses WSL2 (see the separate section below). On macOS and Linux, Docker Desktop / Docker Engine is installed according to the instructions below.

### Windows (Docker Desktop + WSL2)

1. Download Docker Desktop for Windows from the [official website](https://docs.docker.com/desktop/install/windows-install/) and install it.
2. During installation, make sure that the Use WSL 2 based engine option is enabled.
3. Open PowerShell as an administrator and run:
```
wsl --install
```
4. Restart your computer.
5. Launch Docker Desktop (the icon in the lower right tray). Wait for the Docker is running status.
6. Checking in PowerShell:
```
docker version
```
### macOS

1. Download Docker Desktop for Mac from the [official website](https://docs.docker.com/desktop/install/mac-install/).
2. Open the .dmg installer and drag Docker to Applications.
3. Launch Docker from the Applications folder. Wait for Docker to rise.
4. Checking in the Terminal:
```
docker version
```
### Linux (Ubuntu/Debian, example)

1. Download Docker Desktop [DEB Package](https://desktop.docker.com/linux/main/amd64/docker-desktop-amd64.deb?utm_source=docker&utm_medium=webreferral&utm_campaign=docs-driven-download-linux-amd64)
2. Install the package using apt:
```
sudo apt-get update
sudo apt-get install ./docker-desktop-amd64.deb
```
3. Start Docker Desktop:
```
systemctl --user start docker-desktop
```
4. Signing in with Docker Desktop. Docker Desktop for Linux relies on pass to store credentials in GPG-encrypted files. Before signing in to Docker Desktop with your [Docker ID](https://docs.docker.com/accounts/create-account/), you must initialize pass. Docker Desktop displays a warning if pass is not configured.
* Generate a GPG key. You can initialize pass by using a gpg key. To generate a gpg key, run:
```
gpg --generate-key
```
* Enter your name and email once prompted.
Once confirmed, GPG creates a key pair. Look for the pub line that contains your GPG ID, for example:
```
pubrsa3072 2022-03-31 [SC] [expires: 2024-03-30]
 3ABCD1234EF56G78
uid          Molly <molly@example.com>
```
* Copy the GPG ID and use it to initialize pass. For example:
```
pass init 3ABCD1234EF56G78
```
* You should see output similar to:
```
mkdir: created directory '/home/molly/.password-store/'
Password store initialized for <generated_gpg-id_public_key>
```
5. After you’ve successfully installed Docker Desktop, you can check the versions of these binaries by running the following commands:
```
docker compose version
Docker Compose version v2.39.4
```
```
docker --version
Docker version 28.4.0, build d8eb465
```
```
docker version
Client:
 Version:           28.4.0
 API version:       1.51
 Go version:        go1.24.7
<...>
```
To enable Docker Desktop to start on sign in, from the Docker menu, select Settings > General > Start Docker Desktop when you sign in to your computer:
```
systemctl --user enable docker-desktop
```
To stop Docker Desktop, select the Docker menu icon to open the Docker menu and select Quit Docker Desktop:
```
systemctl --user stop docker-desktop
```

# 3. (Windows) Enabling long paths and configuring Git

Windows may have issues with long paths in large repositories. Use the following command in PowerShell (run as administrator):
```
reg add "HKLM\SYSTEM\CurrentControlSet\Control\FileSystem" /v LongPathsEnabled /t REG_DWORD /d 1 /f
```
```
git config --system core.longpaths true
```

# 4. NIKA repository cloning (general)

At this moment, we'd like to mention that the last stable version of NIKA is 0.1.0. Based on this information, we will set up the following part of this manual for version 0.1.0 of NIKA. Open PowerShell / Terminal in a convenient folder (for example, C:\Users\YourName\Projects or ~/projects) and execute:
```
# download (clone) the NIKA repository with safe Windows settings
git clone -c core.longpaths=true -c core.autocrlf=true https://github.com/ostis-apps/nika
```
```
# go to the NIKA project directory
cd nika
```
```
# go to the verified revision
git checkout 69858165c3b5fb26ca783950c74bfe7bddd64a00
git submodule update --init --recursive
```
# 5. Launch (general)
In the terminal, in the nika folder:
```
# pull the images
docker compose pull
# start the containers in the background
docker compose up --no-build -d
```

# 6. Checking services

* [UI (dialogue)](http://localhost:3033)
* [SC-web (visualization)](http://localhost:8000)
* Problem-solver (TCP/WS port): 8090 is not a page, but the service must listen.

If the pages have opened, everything is fine.

# Useful commands
In the nika directory:
```
# show the status of containers
docker compose ps
```
```
# stop and delete containers and network
docker compose down
```
```
# view logs (last 200 lines) of required services
docker compose logs --tail 200 (server name)
```
```
# getting inside the container
docker exec -it (container name) bash
```

# Troubleshooting

Problem: Docker takes a long time to load images or hangs.  
Solution: Wait. Check your internet speed. Perform a ```docker compose pull``` separately; see error output.

Problem: There is a long paths error with git clone.  
Solution: Follow the "enable long paths" step (see Step 3). Clone it again.

Problem: Port 3033 won't open.   
Solution: Make sure that Docker containers are running (```docker compose ps```). Look at the ```docker compose logs nika-ui``` logs.

Problem: You don't have the rights to run Docker (especially on Linux).
Solution: Add the user to the docker group (```sudo usermod -aG docker $USER```) and log out/log in.

## 🤝 Contributing

Contributions, issues and feature requests are welcome!<br />Feel free to check [issues page](https://github.com/ostis-apps/nika/issues). 

## 📝 License

This project is [GPLv3](https://www.gnu.org/licenses/gpl-3.0.html) licensed.
