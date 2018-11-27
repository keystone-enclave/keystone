Edge Calls
==========

In Keystone, as in other enclave systems, function calls that cross in
or out of the enclave are called *edge calls*.

For example, if an enclave wishes to send a network packet, it must
communicate the data to transmit to an untrusted host process via an
edge call.
