Keystone Demo
=============

The Keystone Demo is an example of the current capabilities of the
Keystone enclave framework. The demo consists of:

 - A server application (server-eapp)
 - An untrusted host (enclave-host)
 - A "dummy client" for local testing (dummy-client)
 - A remote client for demonstration of full remote attestation (trusted-client)

This demo shows how a remote client can request computation to be
performed on an untrusted server using an enclave.

Note: The demo uses test keys and is not safe for production use.

Server eapp
-----------

The demo server enclave application include ``libsodium`` for
establishing a secure channel, basic enclave features (attestation
report generation, etc), and a simple word-counting feature.

The enclave first sends a copy of its attestation report, along with
its eccdh public key to the client. Upon receving the client public
key, it establishes a secure channel and asks the enclave-host to wait
for messages.

Once it has received a message, it authenticates and decrypts the
message.

If successful, it supports two message types:

 #. word-count of arbitrarily sized block of text
 #. exit

For word count, it performs a simple word counting calculation, and
returns the result over the secure channel.

Enclave host
------------

The host serves two functions: starting the enclave, and proxying
network messages.

It can also use the 'dummy client' in which case it sends messages to
the dummy client object, and not over the network. This is useful for
testing in a single process in qemu

Dummy Client
------------

A simple, single file copy of the client that runs locally in the
host. Useful for test only.

Trusted Client
--------------

TODO
