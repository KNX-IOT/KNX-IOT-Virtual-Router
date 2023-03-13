# The KNX IoT Router application

A KNX IoT Router is the router functionality between KNX IoT and KNX/NetIP and KNX/TP.
The configuration of the KNX/NETIP side is done via KNX IoT.

Note: This application only supplies the KNX IoT side, e.g. the configuration part only.

## Applications

The following applications are in this folder:

CLI:

- knx_virtual_iot_router Application (CLI) using the following files:
  - knx_virtual_iot_router.c
  - knx_virtual_iot_router.h

Windows GUI using WxWidgets:

- knx_virtual_iot_router_gui Application (wxWidgets) using the following files:
  - knx_virtual_iot_router.c
  - knx_virtual_iot_router.h
  - knx_virtual_iot_router.cpp

The general structure of these programs are:

```
   __________________
  | Application CODE |   <---- WxWidget, Printf, hardware connection
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

## The knx_virtual_iot_router Application

### The KNX information

- serial number : 00FA10019000
- password : U3GNY3906MSRDNG8MUR1
- QR info : KNX:S:00FA10019000;P:U3GNY3906MSRDNG8MUR1

### Functional block

- f/netip, listing the netip data points)
- fp/gm, Table

### Data points

| url  | channel/usage       | instance |resource type | interface type | data type |
|------| --------------------| -------- | -------------| ---------------|-----------|
|p/netip/ttl| net ip multicast | 1 | dpa.11.67 | if.d |integer|
|p/netip/tol| net ip multicast | 1 | dpa.11.95 | if.d |integer|
|p/netip/fra| net ip multicast | 1 | dpa.11.96 | if.d |integer|
|p/netip/mcast| net ip multicast | 1 | dpa.11.66 | if.d  | binary string|
|p/netip/key| net ip multicast | 1 | dpa.11.91 | if.d (put only) |binary string|

### Parameters

| url  | channel/usage   | instance | data type |
|------| ----------------| ---------| --------- |

### MetaData

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

## Building on Windows

From the top level folder of the project execute:

- mkdir build
- cd build
- cmake ..
  - this command retrieves the dependencies from github
- open solution (sln) created in the build folder with visual studio
- build the applications in visual studio

To use knx gitlab as source of the KNX-IOT-STACK use the following command:

- cmake .. -DUSE_GITLAB=true

scripts available:

- build.sh , building unsecured version in folder build
- build_secured.sh , building secured version in folder build_secured

Note that one has to have access to the knx gitlab repo.

## Building on Linux

Note: the GUI variant is not available on Linux, only CLI will be build

From the top level folder of the project execute:

- mkdir build_Linux
- cd build_Linux
- cmake ..
  - this command retrieves the dependencies from github
- make

To use knx gitlab as source of the KNX-IOT-STACK use the following command:

- cmake .. -DUSE_GITLAB=true

Note that one has to have access to the knx gitlab repo.


***

2023-03-13 10:05:12.714872
