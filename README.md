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


## Requirements
You will need [Docker](https://docs.docker.com/) (with Compose plugin) installed and running on your machine. 

We recommend using Docker Desktop on [macOS](https://docs.docker.com/desktop/install/mac-install/) / [Windows](https://docs.docker.com/desktop/install/windows-install/) and using [Docker Server](https://docs.docker.com/engine/install/#server) distribution for your Linux distribution of choice. Use installation instructions provided in the links above.
## Installation

```sh
git clone -c core.longpaths=true -c core.autocrlf=true https://github.com/ostis-apps/nika
cd nika
git submodule update --init --recursive
docker compose pull
```

## 🚀 Usage
- Launch
  ```sh
  docker compose up --no-build
  ```
    This command will launch 2 Web UIs on your machine: 
  - sc-web - `localhost:8000`
  - dialogue web UI - `localhost:3033`

We've set our system to rebuild KB on each restart. If you're debugging some specific subset of your knowledge base you may want to change repo.path to exclude the folders you don't need. 

If you do not want to rebuild KB on relaunch, you can comment out the `REBUILD_KB` environment variable in `docker-compose.yml`.
You can use `docker compose run machine build` to rebuild KB manually.

## Author

* Website: [sem.systems](https://sem.systems/)
* GitHub: [@ostis-apps](https://github.com/ostis-apps), [@ostis-ai](https://github.com/ostis-ai)

## Show your support

Give us a ⭐️ if you've liked this project!

## Troubleshooting
Windows-specific problems:
- Docker images built on your computer are not launching correctly and logging something along these lines: `bash\r: No such file or directory`
  
  **Solution**: please make sure your Git repo is configured to be compatible UNIX line endings
  ```sh
  cd nika
  git config --local core.autocrlf true
  ```
- Git cannot clone repos or submodules, error looks like `error: unable to create file ... (file too long)`

  **Solution**: please make sure your Git repo has `longpaths` config option enabled:
  ```sh
  cd nika
  git config --local core.longpaths true
  ```
Common issues:
- Docker images cannot be built locally. Error: `status: the --mount option requires BuildKit` 
  
  **Solution**: Please note that you'd only need it for custom images, you can launch our system without building images yourself. Use the [Docker Docs BuildKit reference](https://docs.docker.com/go/buildkit) to enable Docker BuildKit on your computer. **In case you're using Windows**, you could use `$env:DOCKER_BUILDKIT = 1` while building in PowerShell.

- Help! My problem-solver container is `unhealthy`
  
  Looks like your container didn't start properly. There are two main reasons for this: violated `start_period` (in case it naturally takes a lot of time to launch our system on your hardware) or faulty server instance. Since there are 2 reasons to this problem, we'll provide 2 solutions. 
  
  **Solution 1**: Increasing `start_period` in `docker-compose.yml` might help you.

  **Solution 2**: Check [known issues](https://github.com/ostis-apps/nika/issues), and in case your problem is not reported yet, create a new one! 


## 🤝 Contributing

Contributions, issues and feature requests are welcome!<br />Feel free to check [issues page](https://github.com/ostis-apps/nika/issues). 

## 📝 License

This project is [GPLv3](https://www.gnu.org/licenses/gpl-3.0.html) licensed.
