#!/bin/bash

source $KEYSTONE_SDK_DIR/scripts/common


runtime="eyrie"
runtime_version=""
force=false

init_runtime() {
  if [[ $# -eq 2 ]]; then
    KEYSTONE_RT_DIR=$KEYSTONE_SDK_DIR/rts/$1
    clean_before_init=$2

    if [[ $runtime_version = "" ]]; then
      runtime_version=`cat $KEYSTONE_SDK_DIR/rts/$runtime.version`
    fi

    case $1 in
      eyrie)
        repo_url=https://github.com/keystone-enclave/keystone-runtime
        ;;
      sel4)
        repo_url=https://github.com/keystone-enclave/keystone-sel4
        ;;
      *)
        script_exit "Unknown runtime '$1'" 2
        ;;
    esac

    if [ $clean_before_init = true ]; then
      rm -rf $KEYSTONE_RT_DIR
    fi

    if [ -d $KEYSTONE_RT_DIR ]; then
      script_exit "Runtime '$1' is already initialized. Try run 'vault init --force --runtime $runtime' to re-initialize."
    fi

    git clone $repo_url $KEYSTONE_RT_DIR
    cd $KEYSTONE_RT_DIR; git checkout $runtime_version
  fi
}

function script_usage() {
  cat << EOF
Usage: vault init [options]

Options
  -h|--help                           Displays this help
  -v|--verbose                        Displays verbose output
  -rt|--runtime [eyrie|sel4]          Set runtime (default: eyrie)
  -f|--force                          Force initializtin
  --runtime-version                   Set commit or branch of the runtime (default: read value from ./rts/<runtime>.version)
EOF
}

function parse_params() {
  local param
  while [[ $# -gt 0 ]]; do
    param="$1"
    shift
    case $param in
      -h|--help)
        script_usage
        exit 0
        ;;
      -v|--verbose)
        verbose=true
        ;;
      -rt|--runtime)
        require_args $param $@
        runtime=$1 && shift
        ;;
      --runtime-version)
        require_args $param $@
        runtime_version=$1 && shift
        ;;
      -f|--force)
        force=true
        ;;
      *)
        script_exit "Invalid parameter was provided: $param" 2
        ;;
    esac
  done
}

command_init() {
  init_runtime $runtime $force
}

function main() {
  trap script_trap_err ERR
  trap script_trap_exit EXIT

  script_init "$@"
  parse_params "$@"

  command_init
}

main "$@"
