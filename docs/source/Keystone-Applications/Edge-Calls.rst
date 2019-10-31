Edge Calls
==========

In Keystone, as in other enclave systems, function calls that cross in
or out of the enclave are called *edge calls*.

For example, if an enclave wishes to send a network packet, it must
communicate the data to transmit to an untrusted host process via an
edge call.

The current version of Keystone supports calls from enclave->host,
referred to internally as *ocalls* (outbound calls, names under
discussion).

If your application requires behavior similar to calls from
host->enclave we suggest emulating these with a polling
ocall. (Keystone-Demo uses this methodology with a
``wait_for_message`` ocall).

All ocall wrapping code currently passes data through shared memory
regions. When referencing data in these regions only offsets into the
region are used, never virtual address pointers.

Example ocall Lifecycle
-----------------------

Note: This is quite specific to the Keystone runtime and driver
design.

Consider an example "print unsigned long" ocall, ``print_value``. This
call exports a value from the enclave to be printed to stdout by the
host process.

The eapp calls ``ocall_print_value(val);`` which is a edge wrapper
function.

``ocall_print_value(val)`` uses the system-call-like interface to the
runtime to run an ocall like ``ocall(OCALL_PRINT_VALUE, &val,
sizeof(unsigned long), 0, 0);`` It passes a pointer to the value, the
size of the argument, and any needed return buffer information. (None
in this case)

The runtime then allocates an ``edge_call`` structure in the shared
memory region, fills out the call type, copies the value into another
part of the shared memory, and sets up the offset to the argument
value. Note that edge calls do not use pointers, but instead offset
values into the shared memory region.

Finally, the runtime exits the enclave with an ``SBI_CALL``, passing a
value indicating the enclave is not shutting down, but executing an
ocall.

The Keystone kernel driver resumes execution, checks the exit status
of the enclave, notes a pending ocall, and passes execution to the
userspace host process.

The userspace host process consumes the ``edge_call`` and dispatches
the registered ocall handler wrapper for ``OCALL_PRINT_VALUE``. The
wrapper generates a pointer to the argument value from the offset in
the shared memory region, and calls ``print_value`` with the value as
an argument.

``print_value`` prints the given argument value.

On return, the host wrapper checks if any return values need to be
copied into the shared memory region (none in this case.) Sets the
``edge_call`` return status to SUCCESS, and returns into the
driver.

The driver re-enters the enclave runtime via an ``SBI_CALL``.

The runtime checks if any return information needs to be copied from
the shared region into return buffers (none in this case) and then
resumes the enclave ocall wrapper code.

Finally, the ocall wrapper code passes any return values to the
function that first called ``ocall_print_value``.

Automatic Wrapper for Edge Calls
--------------------------------

We are currently developing a tool to help with automatic generation
of wrapper code for edge calls. It can be found here:
`Keyedge <https://github.com/keystone-enclave/keyedge>`_.

For further documentation, please check
`Keyedge Documentation
<https://github.com/keystone-enclave/keyedge/blob/master/documentation.rst>`_.

