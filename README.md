# Eurobot - PAMI BW16 Firmware

Ce projet contient le firmware du module BW16 des PAMI.

## Clone

Le projet utilise le système de build `arduino-cli`. Le projet doit être cloné
dans un répertoire nommé `pami-bw16`, nom identique à celui du fichier
`pami-bw16.ino` siuté à la racine. Ce fichier est nécessaire pour le système de
build `arduino-cli`.

## Setup

Les prérequis suivants sont nécessaires pour compiler et utiliser le projet.

### GNU Make

Installation sous Debian :

```shell
sudo apt install make
```

Installation sous Windows :

- [Make for Windows](https://gnuwin32.sourceforge.net/packages/make.htm)

Vérification :

```shell
make --version

GNU Make 4.4.1
Built for x86_64-pc-linux-gnu
Copyright (C) 1988-2023 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
```

### arduino-cli

Téléchargement :

- [arduino-cli v1.4.1 Linux](https://github.com/arduino/arduino-cli/releases/download/v1.4.1/arduino-cli_1.4.1-1_amd64.deb)
- [arduino-cli v1.4.1 Windows](https://github.com/arduino/arduino-cli/releases/download/v1.4.1/arduino-cli_1.4.1_Windows_64bit.msi)

Installation sous Debian :

```shell
sudo apt install ./arduino-cli_1.4.1-1_amd64.deb
```

Installation sous Windows : exécuter le fichier `.msi` et suivre les étapes
d'installation

Vérification :

```shell
arduino-cli version

arduino-cli  Version: 1.3.1 Commit: 08ff7e2b Date: 2025-08-28T13:51:45Z
```

### Realtek Ameba Arduino core

Le firmware est développé sur la version 3.1.9 du SDK Ameba Arduino.

Installation :

```shell
arduino-cli core install realtek:AmebaD@3.1.9
```

Vérification :

```shell
arduino-cli core list

ID             Installed Latest Name
realtek:AmebaD 3.1.9     3.1.9  Realtek Ameba Boards (32-bits ARM Cortex-M33 @200MHz)
```

### Python3

Installation sous Debian :

```shell
sudo apt install python3
```

Installation sous Windows :

- [Python for Windows](https://www.python.org/downloads/windows/)

Vérification :

```shell
python3 --version

Python 3.13.5
```

## Build

La commande suivante permet de compiler, programmer le firmware et lancer une
console série :

```shell
make build && make flash && make monitor
```

Attention, sous Windows, il faut éditer le fichier `Makefile` et configurer le
bon port `COM` :

```makefile
ifeq ($(OS),Windows_NT)
    PORT := COM3
else
    PORT := /dev/ttyUSB0
endif
```

Pour information, l'exécutable se trouve dans :

```
build/release/km0_km4_image2.bin
```

Les targets `make` suivants sont disponibles :

| Commande          | Action
|-------------------|-------
| `make clean`      | Efface les fichiers compilés
| `make build`      | Compile le firmware en mode release
| `make flash`      | Programme le firmware dans le module BW16
| `make monitor`    | Ouvre une console série de debug
| `make compile-db` | Initialise la base de données de compilation pour `clangd`

## Firmware

### Configuration

Le firmware est configuré dynamiquement en plaçant un fichier `config.ini` à la
racine de la carte SD.

Exemple de fichier :

```ini
# configuration PAMI1
ssid = TDS-Team
password = password
hostname = pami1
local_ip = 192.168.254.100
subnet_mask = 255.255.255.0
gateway_ip = 192.168.254.1
time_port = 5000
team_color = yellow
goal_zone = 0
```

Toutes les lignes sont optionnelles. Les lignes vides et les lignes commençant
par un `#` sont ignorées.

### Serveur HTTP

Le firmware embarque un serveur HTTP minimal. Ce serveur permet d'accéder à la
configuration du PAMI et de télécharger les fichiers de capture des données de
jeu.

Les URL ci-dessous font l'hypothèse que l'adresse IP du PAMI est
`192.168.254.100`.

- [http://192.168.254.100](http://192.168.254.100) : affiche la liste des
  fichiers présents à la racine de la carte SD

- [http://192.168.254.100/config.ini](http://192.168.254.100/config.ini) :
  affiche le contenu du fichier `config.ini`, configuration du PAMI

- [http://192.168.254.100/state](http://192.168.254.100/state) : affiche une
  représentation JSON de la trame de vie (heartbeat) envoyée périodiquement au
  module Teensy

Attention : seul le protocole `http` est supporté. Le firmware ne répond pas au
protocole `https`.

## Scripts

### `enable_ap.sh`

Configure la première interface réseau WiFi active en mode WiFi Access Point. Ce
mode est utilisé pour le debug de la communication WiFi.

```shell
./scripts/enable_ap.sh
```

### `disable_ap.sh`

Désactive le mode WiFi Access Point.

```shell
./scripts/enable_ap.sh
```

### `time_server.py`

Démarre un serveur qui envoie une trame de temps via UDP en mode broadcast
toutes les secondes. Ce serveur simule le fonctionnement du robot principal.

```shell
./scripts/time_server.py
```

### `parse_capture.py`

Lit un fichier de capture des données de match au format binaire et écrit une
représentation du fichier au format JSON sur la sortie standard.

```shell
./scripts/parse_capture.py FILE
```
