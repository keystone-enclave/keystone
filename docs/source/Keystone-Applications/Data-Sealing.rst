Data-Sealing
============

The data-sealing feature allows an enclave to derive a key for data encryption,
to be able to save data in untrusted, non-volatile memory outside the enclave.
This key is bound to the identity of the processor, the security monitor and the
enclave. Therefore only the same enclave running on the same security monitor
and the same processor is able to derive the same key. This key can be used to
encrypt data and store them to unprotected, non-volatile memory. After an
enclave restart, the enclave can derive the same key again, fetch the encrypted
data from the untrusted storage and decrypt them using the derived key.


Keystone Key-Hierarchy
######################

The following figure shows the key hierarchy of Keystone:

.. figure:: /_static/images/keystone_key_hierarchy.png

The root of the key hierarchy is the asymmetric processor key pair (SK_D /
PK_D). The asymmetric security monitor key pair (SK_SM / PK_SM) is derived from
the measurement of the security monitor (H_SM) and the private processor key
SK_D.

The resulting security monitor key pair is therefore bound to the processor and
to the identity of the security monitor itself.


Sealing-Key Derivation
######################

The following figure shows, how the sealing-key is derived in Keystone:

.. figure:: /_static/images/sealing_key_deriv.png

The key is derived using three main inputs:

* The private security monitor key (SK_SM)
* The hash of the enclave (H_SM)
* A key identifier

The private security monitor key (SK_SM) ensures that the resulting sealing-key
is bound to the identity of the processor and the identity of the security
monitor. Whenever one of the two components change, the resulting sealing-key is
different.

The enclave hash ensures that the sealing-key is bound to the enclave's
identity. Therefore, no enclave can derive the key from another enclave.

The key identifier is an additional input to the key derivation function, which
can be chosen by the enclave. By choosing different values for the key
identifier, a single enclave is able to derive multiple keys.


Usage
#####

The enclave application library contains the function:

.. code-block:: c

    /* Returns 0 on success */
    int get_sealing_key(void *sealing_key_struct,
                        size_t sealing_key_struct_size,
                        void *key_ident, size_t key_ident_size)

The ``get_sealing_key`` function takes a pointer to the ``sealing_key_struct`` as
first parameter followed by the length of the struct. The third parameter is a
pointer to the buffer containing the key identifier and the last parameter
specifies the length of the key identifier.

The ``sealing_key_struct`` is defined in ``sdk/lib/app/include/sealing.h`` as
follows:

.. code-block:: c

    struct sealing_key {
        uint8_t key[SEALING_KEY_LENGTH];
        uint8_t signature[SIGNATURE_SIZE];
    };

A generic sealing-key derivation example can be found at
``sdk/examples/data-sealing`` and looks as follows:

.. code-block:: c

    struct sealing_key key_buffer;
    char *key_identifier = "identifier";

    int ret = get_sealing_key((void *)&key_buffer, sizeof(key_buffer),
                              (void *)key_identifier, strlen(key_identifier));
