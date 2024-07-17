
# Global configuration
export RELAY_SERIAL="AH02O23H"

export RELAY_ID_global=1
export RELAY_ID_mpfs=8
export RELAY_ID_cva6=4
export RELAY_ID_hifive_unmatched=5

# MPFS configuration

export SC_INSTALL_DIR="/opt/microchip/SoftConsole-v2022.2-RISC-V-747/"
export FPGENPROG=$(which fpgenprog)

export HOST_IP_mpfs="10.42.0.1"
export BOARD_IP_mpfs="10.42.0.205"

# CVA6 configuration

export TFTP_DIR="/srv/tftp"

export HOST_IP_cva6="10.42.1.1"
export BOARD_IP_cva6="10.42.1.171"

# Unmatched configuration

export HOST_IP_hifive_unmatched="10.42.1.1"
export BOARD_IP_hifive_unmatched="10.42.1.27"
