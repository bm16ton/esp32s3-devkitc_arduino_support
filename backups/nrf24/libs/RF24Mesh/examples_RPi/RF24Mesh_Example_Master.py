#!/usr/bin/env python3


from struct import unpack
from RF24 import *
from RF24Network import *
from RF24Mesh import *


# radio setup for RPi B Rev2: CS0=Pin 24
radio = RF24(22, 0)
network = RF24Network(radio)
mesh = RF24Mesh(radio, network)

mesh.setNodeID(0)
if not mesh.begin():
    print("Radio hardware not responding or could not connect to network.")
    exit()
radio.setPALevel(RF24_PA_MAX) # Power Amplifier
radio.printDetails()

while 1:
    mesh.update()
    mesh.DHCP()

    while network.available():
        header, payload = network.read(10)
        if chr(header.type) == 'M':
            print("Rcv {} from 0{:o}".format(unpack("L", payload)[0], header.from_node))
        else:
            print("Rcv bad type {} from 0{:o}".format(header.type, header.from_node))