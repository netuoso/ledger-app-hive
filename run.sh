#!/usr/bin/env bash
#
# EOS Docker node manager
# Released under MIT by @netuoso
#

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

BOLD="$(tput bold)"
RED="$(tput setaf 1)"
GREEN="$(tput setaf 2)"
YELLOW="$(tput setaf 3)"
BLUE="$(tput setaf 4)"
MAGENTA="$(tput setaf 5)"
CYAN="$(tput setaf 6)"
WHITE="$(tput setaf 7)"
RESET="$(tput sgr0)"

# Docker image tagname
DOCKER_TAG="hiveio/ledger-app-hive"
# Placeholder for BUILD_VER shared between functions
BUILD_VER="latest"

: ${DOCKER_DIR="$DIR/Dockerfiles"}

function msg () {
  # usage: msg [color] message
  if [[ "$#" -eq 0 ]]; then echo ""; return; fi;
  if [[ "$#" -eq 1 ]]; then
    echo -e "$1"
    return
  fi
  if [[ "$#" -gt 2 ]] && [[ "$1" == "bold" ]]; then
    echo -n "${BOLD}"
    shift
  fi
  _msg="[$(date +'%Y-%m-%d %H:%M:%S %Z')] ${@:2}"
  case "$1" in
    bold) echo -e "${BOLD}${_msg}${RESET}";;
    [Bb]*) echo -e "${BLUE}${_msg}${RESET}";;
    [Yy]*) echo -e "${YELLOW}${_msg}${RESET}";;
    [Rr]*) echo -e "${RED}${_msg}${RESET}";;
    [Gg]*) echo -e "${GREEN}${_msg}${RESET}";;
    * ) echo -e "${_msg}";;
  esac
}

export -f msg
export RED GREEN YELLOW BLUE BOLD NORMAL RESET

if [[ -f .env ]]; then
  source .env
fi

help() {
  echo "Usage: $0 COMMAND [DATA]"
  echo
  echo "Commands: 
  start - starts container
  stop - stops container
  restart - restarts container
  status - show status container
  update - pull the latest docker image
  build - build the ledger-app-hive app
  clean - clean the ledger-app-hive app
  load - load the ledger-app-hive app
  install_docker - install docker
  logs - show container log stream
  enter - enter a bash session in the currently running container
  shell - launch the container then open bash for inspection
  "
  echo
  exit
}

# Usage: ./run.sh install_docker
# Downloads and installs the latest version of Docker using the Get Docker site
# If Docker is already installed, it should update it.
install_docker() {
  sudo apt update
  # curl/git used by docker, xz/lz4 used by dlblocks, jq used by tslogs/pclogs
  sudo apt install curl git xz-utils liblz4-tool jq
  curl https://get.docker.com | sh
  if [ "$EUID" -ne 0 ]; then 
    echo "Adding user $(whoami) to docker group"
    sudo usermod -aG docker $(whoami)
    echo "IMPORTANT: Please re-login (or close and re-connect SSH) for docker to function correctly"
  fi
}

# Internal Use Only
# Checks if the container main container exists. Returns 0 if it does, -1 if not.
#
container_exists() {
  ret=$(docker ps -a -f name="ledger-app-hive" | wc -l)
  if [[ $ret -eq 2 ]]; then
    return 0
  else
    return -1
  fi
}

# Internal Use Only
# Checks if the container main container exists and is running. Returns 0 if it does, -1 if not.
#
container_running() {
  ret=$(docker ps -f 'status=running' -f name="ledger-app-hive" | wc -l)
  if [[ $ret -eq 2 ]]; then
    return 0
  else
    return -1
  fi
}

# Usage: ./run.sh start
# Creates and/or starts the docker container
start() {
  msg bold green " -> Starting container..."
  if container_exists; then
    docker start ledger-app-hive
  else
    BOLOS_SDK="/ledgerdev/nanos-secure-sdk"
    BOLOS_ENV="/ledgerdev/bolosenv/"
    CLANGPATH="/usr/bin/"
    GCCPATH="/usr/bin/"
    docker run -d --name ledger-app-hive --env BOLOS_SDK="$BOLOS_SDK" --env BOLOS_ENV="$BOLOS_ENV" --env CLANGPATH="$CLANGPATH" --env GCCPATH="$GCCPATH" hiveio/ledger-app-hive:latest tail -f /dev/null
  fi

}

# Usage: ./run.sh stop
# Stops and removes the container
#
stop() {
  msg red "Stopping and removing container ..."
  docker rm -f ledger-app-hive
}

# Usage: ./run.sh enter
# Enters the running docker container and opens a bash shell for debugging
#
enter() {
  docker exec -it ledger-app-hive bash
}

# Usage: ./run.sh shell
# Runs the container similar to `run` with mounted directories, 
# then opens a BASH shell for debugging
# To avoid leftover containers, it uses `--rm` to remove the container once you exit.
#
shell() {
    msg bold green " -> Container doesn't exist..."
    BOLOS_SDK="/ledgerdev/nanos-secure-sdk"
    BOLOS_ENV="/ledgerdev/bolosenv/"
    CLANGPATH="/usr/bin/"
    GCCPATH="/usr/bin/"
    docker run -it --rm --env BOLOS_SDK="$BOLOS_SDK" --env BOLOS_ENV="$BOLOS_ENV" --env CLANGPATH="$CLANGPATH" --env GCCPATH="$GCCPATH" hiveio/ledger-app-hive:latest bash
}

# Usage: ./run.sh logs
# Shows the last 30 log lines of the running steem container, and follows the log until you press ctrl-c
#
logs() {
  msg blue "DOCKER LOGS: (press ctrl-c to exit) "
  docker logs -f ledger-app-hive
}

# Usage: ./run.sh status
# Very simple status display, letting you know if the container exists, and if it's running.
status() {
  msg green "Checking container status ..."
  if container_exists; then
    if container_running; then
      msg bold green " -> Container is running..."
    else
      msg bold red " -> Container is not running..."
    fi
  else
    msg bold red " -> Container doesn't exist..."
  fi
}

# Usage: ./run.sh build
# Build the ledger-app-hive repository inside the container
build() {
  msg green "Building ledger-app-hive inside container ..."
  if container_running; then
    case $1 in
      1.[4-6])
        docker exec ledger-app-hive bash -cl "/ledger-app-hive/build-hive.sh $1" &&
        msg bold "Copying ledger-app-hive application to host..." &&
        docker cp ledger-app-hive:/ledger-app-hive/bin/app.hex $DIR/bin/app-$1.hex &&
        docker cp ledger-app-hive:/ledger-app-hive/debug/app.map $DIR/debug/app-$1.map &&
        msg green "Finished building ledger-app-hive application..." ||
        msg red "Error building ledger-app-hive application..."
        ;;
      *)
        msg red "Unsupported Ledger Nano S firmware. Versions: [1.4, 1.5, 1.6] ..."
        ;;
    esac
  else
    msg bold red " -> Container is not running..."
  fi
}

# Usage: ./run.sh clean
# Clean the build directory inside the container
clean() {
  msg green "Cleaning ledger-app-hive build directory ..."
  if container_running; then
    docker exec ledger-app-hive bash -c "cd /ledger-app-hive && make clean"
  else
    msg bold red " -> Container is not running..."
  fi
}

# Usage: ./run.sh update
# Pull the latest built docker image from hiveio docker hub
update() {
  msg green "Pulling latest docker image ..."
  docker pull hiveio/ledger-app-hive
}

# Usage: ./run.sh load
# Load the ledger-app-hive application on a Ledger Device
load() {
  # TODO:
  # read user config
  # set target id
  # check for required python 3
  # check for ledgerblue module
  msg green "Loading ledger-app-hive application onto Ledger Device ..."
  case $1 in
    1.4)
      TARGET_ID="0x31100003"
      ;;
    1.[5-6])
      TARGET_ID="0x31100004"
      ;;
    *)
      msg red "Unsupported Ledger Nano S firmware. Versions: [1.4, 1.5, 1.6] ..."
      exit 1
      ;;
  esac

  ICONHEX="0100000000ffffff003ff367e667e6c3ccc3cc81998199003f003f81998199c3ccc3cc67e667e63ff3" # hive icon

  python3 -m ledgerblue.loadApp --appFlags 0x240 --path "48'/13'" --curve secp256k1 --tlv --targetId $TARGET_ID --delete --fileName $DIR/bin/app-$1.hex --appName Hive --appVersion 1.0.0 --dataSize `cat $DIR/debug/app-$1.map |grep _nvram_data_size | tr -s ' ' | cut -f2 -d' '` --icon $ICONHEX &&
  msg green "Successfuly loaded application onto Ledger Device ..." ||
  msg red "Error loading application onto Ledger Device. Did you cancel the install or forget to install the 'ledgerblue' python module? ..."
}

if [ "$#" -lt 1 ]; then
    help
fi

case $1 in
  install_docker)
    install_docker
    ;;
  start)
    start
    ;;
  stop)
    stop
    ;;
  restart)
    stop
    sleep 5
    start
    ;;
  status)
    status
    ;;
  update)
    update
    ;;
  load)
    load "${@:2}"
    ;;
  build)
    build "${@:2}"
    ;;
  clean)
    clean
    ;;
  enter)
    enter
    ;;
  shell)
    shell
    ;;
  logs)
    logs
    ;;
  ver|version)
    ver
    ;;
  help)
    help
    ;;
  *)
    msg red 'unknown command'
    help
    ;;
esac
 