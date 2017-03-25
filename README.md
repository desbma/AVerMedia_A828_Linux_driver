AVerMedia A828 ("AVerTV Hybrid+FM Volar") Linux driver
======================================================

[![Tests status](https://img.shields.io/travis/desbma/AVerMedia_A828_Linux_driver/master.svg?label=tests&style=flat)](https://travis-ci.org/desbma/AVerMedia_A828_Linux_driver)

This is a work in progress attempt to port the A828 Linux driver to recent kernel versions.

The original official driver supports x86 and x86-64 architecture and has binary (pre-compiled) parts, so there is no plan to support other architectures.


## Goals

* Ensure the driver compiles on kernel from versions 2.6.x to the most recent (tested with [Travis CI](https://travis-ci.org/desbma/AVerMedia_A828_Linux_driver))
* Test the tuner with the driver on a few of these versions on x86-64


## Non goals

* Fix non trivial bugs
* Add new features
* Have the driver code included in mainline kernel


## Other A828 Linux drivers versions

* Last official version of the Linux driver from AVerMedia: 0.28 beta ([x86 mirror](https://www.dropbox.com/s/8dggc71dnr5qdrb/A828_Installer_x86_0.28-Beta_091125.zip?dl=0),  [x86-64 mirror](https://www.dropbox.com/s/lglgkypmxmftsp6/A828_Installer_x64_0.28-Beta_091125.zip?dl=0)), officially supports kernel 2.6.17 to 2.6.30, but works up to 2.6.36
* http://coldsun.free.fr/a828/a828-0.28.zip (modifications from official driver detailed [here](https://www.linuxtv.org/wiki/index.php/AVerMedia_A828#Modifications_to_bring)), works up to kernel 3.2
* https://github.com/inzaghi123456789/avermedia-a828, works up to kernel 3.2
* https://github.com/blackwarthog/avermedia-a828, builds (partially works?) for kernel 3.16


## Installation

    make
    make install

*TODO DKMS*
