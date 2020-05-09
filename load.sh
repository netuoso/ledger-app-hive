#!/bin/bash

#### Uncomment the following line if you have a Ledger Nano S with firmware 1.3 ####
# python -m ledgerblue.loadApp --appFlags 0x240 --path "48'/13'" --curve secp256k1 --tlv --targetId 0x31100002 --delete --fileName bin/app.hex --appName Hive --appVersion 1.0.0 --dataSize 0x00000040 --icon 0100000000ffffff003ff367e667e6c3ccc3cc81998199003f003f81998199c3ccc3cc67e667e63ff3
####################################################################################


#### Uncomment the following line if you have a Ledger Nano S with firmware 1.4 ####
python -m ledgerblue.loadApp --appFlags 0x240 --path "48'/13'" --curve secp256k1 --tlv --targetId 0x31100003 --delete --fileName bin/app.hex --appName Hive --appVersion 1.0.0 --dataSize 0x00000040 --icon 0100000000ffffff003ff367e667e6c3ccc3cc81998199003f003f81998199c3ccc3cc67e667e63ff3
####################################################################################


#### Uncomment the following line if you have a Ledger Nano S with firmware 1.5+ ####
# python -m ledgerblue.loadApp --appFlags 0x240 --path "48'/13'" --curve secp256k1 --tlv --targetId 0x31100004 --delete --fileName bin/app.hex --appName Hive --appVersion 1.0.0 --dataSize 0x00000040 --icon 0100000000ffffff003ff367e667e6c3ccc3cc81998199003f003f81998199c3ccc3cc67e667e63ff3
#####################################################################################


########## NOT WORKING - NO LEDGER NANO X SDK AVAILABLE ##########
#### Uncomment the following line if you have a Ledger Nano X ####
# python -m ledgerblue.loadApp --appFlags 0x240 --path "48'/13'" --curve secp256k1 --tlv --targetId 0x33000004 --delete --fileName bin/app.hex --appName Hive --appVersion 1.0.0 --dataSize 0x00000040 --icon 0100000000ffffff003ff367e667e6c3ccc3cc81998199003f003f81998199c3ccc3cc67e667e63ff3
##################################################################
