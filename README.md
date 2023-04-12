![Linux](https://github.com/KNX-IOT/KNX-IOT-Virtual-Router/actions/workflows/cmake-linux.yml/badge.svg)
![Windows](https://github.com/KNX-IOT/KNX-IOT-Virtual-Router/actions/workflows/cmake-windows.yml/badge.svg)

<!-- TOC -->

- [1. The KNX IoT Router application](#1-the-knx-iot-router-application)
  - [1.1. Applications](#11-applications)
  - [1.2. The knx\_virtual\_iot\_router Application](#12-the-knx_iot_virtual_router-application)
    - [1.2.1. The KNX information](#121-the-knx-information)
    - [1.2.2. Functional block](#122-functional-block)
    - [1.2.3. Data points](#123-data-points)
    - [1.2.4. Parameters](#124-parameters)
    - [1.2.5. MetaData](#125-metadata)
- [2. Other instructions](#2-other-instructions)

<!-- /TOC -->

# 1. The KNX IoT Router application

A KNX IoT Router is the router functionality between KNX IoT and KNX/NetIP and KNX/TP.
The configuration of the KNX/NETIP side is done via KNX IoT.

Note: This application only supplies the KNX IoT side, e.g. the configuration part only. So From KNX IoT side this is a full implementation, but from KNX/NetIP there is nothing there. So group messages are NOT transported between KNX/IoT and KNX/NetIP.

## 1.1. Applications

The following applications are in this folder:

CLI:

- knx_iot_virtual_router Application (CLI) using the following files:
  - knx_iot_virtual_router.c
  - knx_iot_virtual_router.h

Windows GUI using WxWidgets:

- knx_iot_virtual_router_gui Application (wxWidgets) using the following files:
  - knx_iot_virtual_router.c
  - knx_iot_virtual_router.h
  - knx_iot_virtual_router.cpp

The general structure of these programs are:

```
   __________________
  | Application CODE |   <---- WxWidget, Printf
  |__________________|
  |  POINT API CODE  |   <---- Generic code handling all Point API CoAP code
  |__________________|
  |      STACK       |   <---- Generic The KNX IoT Point API Stack (other repo)
  |__________________|

  General structure
```

The Point API Code is shared code between the different applications
it implementes the datapoint resources.

The point code has an API so that one can:

- Set/retrieve data from an URL
- Callback on PUT data changes
- Functions to figure out what type of data the url conveys

These functions then can be used to add the functionallity for
handling the GUI (wxWidgets) and embedded (chili) to connect to the hardware.

## 1.2. The knx_iot_virtual_router Application

### 1.2.1. The KNX information

- serial number : 00FA10019000
- password : U3GNY3906MSRDNG8MUR1
- QR info : KNX:S:00FA10019000;P:U3GNY3906MSRDNG8MUR1

### 1.2.2. Functional block

- f/netip, listing the netip data points.
- fp/gm, Table

### 1.2.3. Data points

| url  | channel/usage       | instance |resource type | interface type | data type |
|------| --------------------| -------- | -------------| ---------------|-----------|
|p/netip/ttl| net ip multicast | 1 | dpa.11.67 | if.d |integer|
|p/netip/tol| net ip multicast | 1 | dpa.11.95 | if.d |integer|
|p/netip/fra| net ip multicast | 1 | dpa.11.96 | if.d |integer|
|p/netip/mcast| net ip multicast | 1 | dpa.11.66 | if.d  | binary string|
|p/netip/key| net ip multicast | 1 | dpa.11.91 | if.d (put only) |binary string|

### 1.2.4. Parameters

| url  | channel/usage   | instance | data type |
|------| ----------------| ---------| --------- |

### 1.2.5. MetaData

The mandatory metadata parameters per data points implemented:

- id (9) - the unique url
- rt - the resource type
- if - the interface type
- dpt - the data point type
- ga - the array of group addresses (if initialized)

Next to the mandatory metadata fields the following datapoint specific metadata fields are implemented:

| url  | name   | metadata tag | metadata value |
|------| ----------------| ---------| --------- |

For querying the metadata items implemented one can use the following commands:

```
```

# 2. Other instructions

Please take a look at https://github.com/KNX-IOT/KNX-IOT-Virtual