# Petalinux Projects

This repo contains my personal petalinux project folders:

## Loopback OS
A simple linux project for the Xilinx PYNQ-Z2 that integrates a loopback module implimented on the FPGA fabric. Additionally, the OS is configured to include a C compiler for native development and a baked in C program to test the memory mapped hardware for functionality.

Credit to [kbralten](https://github.com/kbralten) and his repo [pynq_to_zynq](https://github.com/kbralten/pynq_to_zynq) which helped guide me through the learning process of using Vivado and Petalinux as well as provide the C program used to interface with the memory mapped loopback adder.
