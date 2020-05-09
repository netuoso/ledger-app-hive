# ledger-app-hive

Hive wallet application framework for Ledger Nano S

This follows the specification available in the doc/ folder

To use the generic wallet refer to `signTransaction.py` or `getPublicKey.py`.

# How to Install developer version
## Configuring Ledger Environment

- Outside of the scope of this repository

## Compile your ledger app

* install your app under apps/ for instance:
```
cd apps/
git clone https://github.com/netuoso/ledger-app-hive

* build Hive app

```
cd apps/ledger-app-hive
make clean
make
```

* connect your ledger Nano S to your computer
* install the app on your ledger: `make load`
* remove the app from the ledger: `make delete`

Install instruction with slight modifications has been taken from [here](https://github.com/fix/ledger-vagrant)

## See App Documentation for More Information
[ledger-app-hive Technical Documentation](doc/hiveapp.asc)
