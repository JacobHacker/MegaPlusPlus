Licensing
=========
All code licensed under [GPLv3](https://gnu.org/licenses/gpl.html).

See also: [A Practical Guide to GPL Compliance](https://softwarefreedom.org/resources/2008/compliance-guide.html).

Requirements
============
* cppnetlib 11.0
* crypto++
* scons
* boost headers
* boost-serialization
* jsoncpp

Ubuntu (>=14.04):

    sudo apt-get install libcppnetlib-dev scons libcrypto++-dev libboost-dev libboost-serialization jsoncpp-dev
    sudo apt-get install libgtest-dev
    cd /usr/src/gtest && sudo cmake . && sudo make && sudo mv libg* /usr/lib/ && cd -

Install (TODO)
=======
Linux:

    scons && scons -Q /usr/local/bin (TODO)
