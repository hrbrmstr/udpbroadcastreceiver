
[![Project Status: Active – The project has reached a stable, usable
state and is being actively
developed.](https://www.repostatus.org/badges/latest/active.svg)](https://www.repostatus.org/#active)
[![Signed
by](https://img.shields.io/badge/Keybase-Verified-brightgreen.svg)](https://keybase.io/hrbrmstr)
![Signed commit
%](https://img.shields.io/badge/Signed_Commits-100%25-lightgrey.svg)
[![R-CMD-check](https://github.com/hrbrmstr/udpbroadcastreceiver/workflows/R-CMD-check/badge.svg)](https://github.com/hrbrmstr/udpbroadcastreceiver/actions?query=workflow%3AR-CMD-check)
[![Linux build
Status](https://travis-ci.org/hrbrmstr/udpbroadcastreceiver.svg?branch=master)](https://travis-ci.org/hrbrmstr/udpbroadcastreceiver)
[![Coverage
Status](https://codecov.io/gh/hrbrmstr/udpbroadcastreceiver/branch/master/graph/badge.svg)](https://codecov.io/gh/hrbrmstr/udpbroadcastreceiver)
![Minimal R
Version](https://img.shields.io/badge/R%3E%3D-4.0.0-blue.svg)
![License](https://img.shields.io/badge/License-MIT-blue.svg)

# udpbroadcastreceiver

Receive and Log or Process UDP Broadcast Payloads

## Description

Many remote sensors, such as weather stations, have the ability to
broadcast their record payloads over UDP. Tools are provided to listen
for these broadcast packets and either log them directly to a file or
process them with callback functions.

## What’s Inside The Tin

The following functions are implemented:

-   `udp_callback_logger`: Log UDP broadcast packets to a file
-   `udp_file_logger`: Log UDP broadcast packets to a file

## Installation

``` r
remotes::install_github("hrbrmstr/udpbroadcastreceiver")
```

NOTE: To use the ‘remotes’ install options you will need to have the
[{remotes} package](https://github.com/r-lib/remotes) installed.

## Usage

``` r
library(udpbroadcastreceiver)

# current version
packageVersion("udpbroadcastreceiver")
## [1] '0.1.0'
```

### File Logger

``` r
tf <- tempfile()
udp_file_logger(50222, tf)
# SIGINT/CTRL-C
readLines(tf)
## [1] "{\"serial_number\":\"HB-00069665\",\"type\":\"hub_status\",\"firmware_revision\":\"177\",\"uptime\":8800796,\"rssi\":-47,\"timestamp\":1651318164,\"reset_flags\":\"BOR,PIN,POR\",\"seq\":879076,\"radio_stats\":[25,1,0,3,16637],\"mqtt_stats\":[498,224]}"
## [2] "{\"serial_number\":\"ST-00055227\",\"type\":\"rapid_wind\",\"hub_sn\":\"HB-00069665\",\"ob\":[1651318165,1.39,241]}"
## [3] "{\"serial_number\":\"ST-00055227\",\"type\":\"rapid_wind\",\"hub_sn\":\"HB-00069665\",\"ob\":[1651318167,1.48,233]}"
## [4] "{\"serial_number\":\"ST-00055227\",\"type\":\"rapid_wind\",\"hub_sn\":\"HB-00069665\",\"ob\":[1651318170,1.38,240]}"
## [5] "{\"serial_number\":\"ST-00055227\",\"type\":\"rapid_wind\",\"hub_sn\":\"HB-00069665\",\"ob\":[1651318173,1.39,243]}"
## [6] "{\"serial_number\":\"HB-00069665\",\"type\":\"hub_status\",\"firmware_revision\":\"177\",\"uptime\":8800806,\"rssi\":-47,\"timestamp\":1651318174,\"reset_flags\":\"BOR,PIN,POR\",\"seq\":879077,\"radio_stats\":[25,1,0,3,16637],\"mqtt_stats\":[498,224]}"
unlink(tf)
```

### Callback Logger

``` r
# We will stop after 5 records are read so we need to keep a counter
n <- 1

# We'll store the records here
records <- c()

callback <- \(x) {
  n <<- n + 1 # increment counter
  records <<- append(records, x) # accumulate records
  message(x) # just for this example
  return(n==6) # stop after 5 records
}

udp_callback_logger(50222, callback)
## {"serial_number":"ST-00055227","type":"rapid_wind","hub_sn":"HB-00069665","ob":[1651318077,1.21,231]}
## {"serial_number":"ST-00055227","type":"rapid_wind","hub_sn":"HB-00069665","ob":[1651318080,1.43,231]}
## {"serial_number":"ST-00055227","type":"rapid_wind","hub_sn":"HB-00069665","ob":[1651318083,1.52,243]}
## {"serial_number":"HB-00069665","type":"hub_status","firmware_revision":"177","uptime":8800716,"rssi":-47,"timestamp":1651318084,"reset_flags":"BOR,PIN,POR","seq":879068,"radio_stats":[25,1,0,3,16637],"mqtt_stats":[498,224]}
## {"serial_number":"ST-00055227","type":"rapid_wind","hub_sn":"HB-00069665","ob":[1651318086,1.04,217]}

records
## [1] "{\"serial_number\":\"ST-00055227\",\"type\":\"rapid_wind\",\"hub_sn\":\"HB-00069665\",\"ob\":[1651318077,1.21,231]}"
## [2] "{\"serial_number\":\"ST-00055227\",\"type\":\"rapid_wind\",\"hub_sn\":\"HB-00069665\",\"ob\":[1651318080,1.43,231]}"
## [3] "{\"serial_number\":\"ST-00055227\",\"type\":\"rapid_wind\",\"hub_sn\":\"HB-00069665\",\"ob\":[1651318083,1.52,243]}"
## [4] "{\"serial_number\":\"HB-00069665\",\"type\":\"hub_status\",\"firmware_revision\":\"177\",\"uptime\":8800716,\"rssi\":-47,\"timestamp\":1651318084,\"reset_flags\":\"BOR,PIN,POR\",\"seq\":879068,\"radio_stats\":[25,1,0,3,16637],\"mqtt_stats\":[498,224]}"
## [5] "{\"serial_number\":\"ST-00055227\",\"type\":\"rapid_wind\",\"hub_sn\":\"HB-00069665\",\"ob\":[1651318086,1.04,217]}"   
```

## udpbroadcastreceiver Metrics

| Lang | \# Files |  (%) | LoC |  (%) | Blank lines |  (%) | \# Lines |  (%) |
|:-----|---------:|-----:|----:|-----:|------------:|-----:|---------:|-----:|
| C++  |        2 | 0.11 | 111 | 0.28 |          49 | 0.26 |        7 | 0.03 |
| YAML |        2 | 0.11 |  35 | 0.09 |          10 | 0.05 |        2 | 0.01 |
| R    |        4 | 0.22 |  32 | 0.08 |          14 | 0.07 |       49 | 0.22 |
| Rmd  |        1 | 0.06 |  22 | 0.06 |          23 | 0.12 |       53 | 0.24 |
| SUM  |        9 | 0.50 | 200 | 0.50 |          96 | 0.50 |      111 | 0.50 |

clock Package Metrics for udpbroadcastreceiver

## Code of Conduct

Please note that this project is released with a Contributor Code of
Conduct. By participating in this project you agree to abide by its
terms.
