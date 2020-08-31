Attestation
===========

Keystone support a simple attestation scheme using ed25519 signatures
on hashes of the security monitor and enclave content.

See Keystone-Demo for the most up-to-date example of how the
attestation flow works for applications.

Using Attestation
-----------------

To use remote attestation in Keystone a remote client needs 3 things:

 - A public key for the device root private key
 - An expected hash for the enclave they wish to launch
 - An expected hash for the security monitor
 - A copy of the enclave report after launch

Once you have these items, the ``sdk/src/verifier`` library provides
the tools to validate the report.

Device Root Keys
----------------

For security guarantees to hold, the platform must support storage of
a device root key accessible only to the bootloader/sm. Keystone
cannot provide this support on its own.

For testing purposes we provide the ability to hardcode a device
public/private key into the bootloader. This keypair is in
``bootrom/test_dev_key.h``.


Generating Enclave Attestation Report
-------------------------------------

Once an enclave has started, it may request the SM to provide a signed
enclave report and signed SM report:

SM Report contains:
 - A hash of the SM
 - The attestation public key

All signed by the device root key.

Enclave report contains:
 - A hash of the enclave at initialization
 - A data block from the enclave of up-to 1KB in size

All signed by the attestation public key.

The verifier, when provided with the device public key, expected SM
hash, and expected enclave hash, will verify the signatures these
reports.


Enclave Hashes
--------------

Currently generating the expected enclave hash value is a manual
process. Keystone-Demo has an example of a way to do this

 #. Modify enclave host application to emit the hash from given report.
 #. Run modified host and application in qemu
 #. Save emitted hash value on the remote client

Keystone will be adding support for a more streamlined hash generation
process in the future.


Security Monitor Hashes
-----------------------

The Keystone repository will provide in the riscv-pk subproject an
``sm_expected_hash.h`` file that should contain the current expected
hash value of the security monitor as a C array.

If modifications are made to the SM for testing, the hash can be
obtained in the same way as the expected enclave hash.
