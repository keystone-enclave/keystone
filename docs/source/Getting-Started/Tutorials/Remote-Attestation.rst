Tutorial 5: Remote Attestation
==============================

This tutorial explains how to build and run a simple proof of work
application leveraging attestation instead of one way functions.

This application consists of three parts: an EApp running in the
keystone enclave, a host application that initializes and launches the
eapp, and a remote verifier verifying that the client did run the EApp
(i.e. the proof of work).

Before jumping into the tutorial, please complete :doc:`Quick Start
<../Running-Keystone-with-QEMU>`.

Prerequisite
------------------------------

Set ``PATH`` to include RISC-V tools and ``KEYSTONE_SDK_DIR`` to point the
absolute path of the installed SDK.

Let's take a look at the example provided in `Keystone SDK
<https://github.com/keystone-enclave/keystone-sdk>`_.

::

	ls sdk/examples/attestation

You can find two directories and ``CMakeLists.txt``.

Enclave Application: attestor.c
--------------------------------

Open ``attestor.c`` file in ``sdk/exmamples/attestation/eapp/``. This
is the source code of the enclave application.

.. code-block:: c

	#include "app/eapp_utils.h"
	#include "app/syscall.h"
	#include "edge/edge_common.h"

	#define OCALL_PRINT_BUFFER 1
	#define OCALL_PRINT_VALUE 2
	#define OCALL_COPY_REPORT 3
	#define OCALL_GET_STRING 4

	int
	main() {
	  struct edge_data retdata;
	  ocall(OCALL_GET_STRING, NULL, 0, &retdata, sizeof(struct edge_data));

	  for (unsigned long i = 1; i <= 10000; i++) {
	    if (i % 5000 == 0) {
	      ocall(OCALL_PRINT_VALUE, &i, sizeof(unsigned long), 0, 0);
	    }
	  }

	  char nonce[2048];
	  if (retdata.size > 2048) retdata.size = 2048;
	  copy_from_shared(nonce, retdata.offset, retdata.size);

	  char buffer[2048];
	  attest_enclave((void*)buffer, nonce, retdata.size);

	  ocall(OCALL_COPY_REPORT, buffer, 2048, 0, 0);

	  EAPP_RETURN(0);
	}

This is the standard C program that we will run isolated in an
enclave. This program gets a random nonce from the verifier via the
host, iterate 100000 times, then send the nonce as part of the
attestation report back

Host Application: host.cpp
--------------------------

Open ``host.h`` in ``sdk/examples/attestation/host/``. This is the C++
class definition of the host application. To simplify this example, we
are implementing the remote verifier and the host in two different
classes (the ``Verifier`` class and the ``Host`` class) but including
them in the same binary (``attestor_runner.cpp`` in
``sdk/examples/attestation/host/``). We use the ``host.cpp`` file to
encapsulate all the code needed by the host, and the ``verifier.cpp``
to encapsulate all the code needed by the verifier.

.. code-block:: cpp

	// The Host class mimicks a host interacting with the local enclave
	// and the remote verifier.
	class Host {
	public:
	Host(
	    const Keystone::Params& params, const std::string& eapp_file,
	    const std::string& rt_file)
	    : params_(params), eapp_file_(eapp_file), rt_file_(rt_file) {}
	    // Given a random nonce from the remote verifier, this method leaves
	    // it for the enclave to fetch, and returns the attestation report
	    // from the enclave to the verifier.
	    Report run(const std::string& nonce);

	private:
	  // ... See host.h for the full source code.
	};

Let’s first take a look at how the ``Host::run`` method is
implemented:

.. code-block:: cpp

	Report
	Host::run(const std::string& nonce) {
	  Keystone::Enclave enclave;
	  enclave.init(eapp_file_.c_str(), rt_file_.c_str(), params_);

	  RunData run_data{
	      SharedBuffer{enclave.getSharedBuffer(), enclave.getSharedBufferSize()},
	      nonce, nullptr};

	  enclave.registerOcallDispatch([&run_data](void* buffer) {
	    assert(buffer == (void*)run_data.shared_buffer.ptr());
	    dispatch_ocall(run_data);
	  });

	  uintptr_t encl_ret;
	  enclave.run(&encl_ret);

	  return *run_data.report;
	}

The main job of the host is to relay messages from the remote verifier
to the EApp, and vice-versa.

Remote Verifier: verifier.cpp
=============================

The remote verifier is the most interesting part of this tutorial. As
mentioned above, although the remote verifier is actually implemented
in the same binary file as the host, in reality the remote verifier
usually runs on a different machine and communicates with the host
remotely. The goal of the remote verifier is to ensure that the host
runs the EApp til the end, and this is achieved by sending a random
nonce to the EApp via the host, and verifying the attestation report
sent from the EApp. The nonce is to prevent replay attacks.

In order for the verification to be sound, the verifier needs to do
the following things:

1. Perform analysis on the security monitor to ensure the security
monitor does the right thing with regards to security.
2. Perform analysis on the EApp binary to ensure the EApp indeed
iterates 10000 times and only sends the attestation report after the
loop.
3. Verify the security monitor used by the host machine is the one
analyzed in 1.
4. Verify the EApp ran by the host is the one analyzed in 2.
5. Verify the nonce sent from the EApp is the one generated by the
verifier.

While 1 and 2 are typically done beforehand (or delegated to a trusted
party), 3-5 are done at runtime by checking signature and payload of
the attestation report. See
:doc:`Attestation<../../Keystone-Applications/Attestation>` for
additional details on Keystone's attestation support.

The ``Verifier::verify_report`` method accomplishes 3-5:

.. code-block:: cpp

	void
	Verifier::verify_report(Report& report, const std::string& nonce) {
	  debug_verify(report, _sanctum_dev_public_key);

	  byte expected_enclave_hash[MDSIZE];
	  compute_expected_enclave_hash(expected_enclave_hash);

	  byte expected_sm_hash[MDSIZE];
	  compute_expected_sm_hash(expected_sm_hash);

	  verify_hashes(
	      report, expected_enclave_hash, expected_sm_hash, _sanctum_dev_public_key);

	  verify_data(report, nonce);
	}

 First, let's dive into how the Enclave hash is computed. It is done
 by leveraging a simulation mode offered by the ``Keystone:Enclave``
 class:

 .. code-block:: cpp

	void
	Verifier::compute_expected_enclave_hash(byte* expected_enclave_hash) {
	  Keystone::Enclave enclave;
	  Keystone::Params simulated_params = params_;
	  simulated_params.setSimulated(true);
	  // This will cause validate_and_hash_enclave to be called when
	  // isSimulated() == true.
	  enclave.init(eapp_file_.c_str(), rt_file_.c_str(), simulated_params);
	  memcpy(expected_enclave_hash, enclave.getHash(), MDSIZE);
	}

Secondly, the Security Monitor's hash is computed using
``compute_expected_sm_hash``:

.. code-block:: cpp

	void
	Verifier::compute_expected_sm_hash(byte* expected_sm_hash) {
	  // It is important to make sure the size of the SM buffer we are
	  // measuring is the same as the size of the SM buffer allocated by
	  // the bootloader. See keystone/bootrom/bootloader.c for how it is
	  // computed in the bootloader.
	  const size_t sanctum_sm_size = 0x1ff000;
	  std::vector<byte> sm_content(sanctum_sm_size, 0);

	  {
	    // Reading SM content from file.
	    FILE* sm_bin = fopen(sm_bin_file_.c_str(), "rb");
	    if (!sm_bin)
	      throw std::runtime_error(
	          "Error opening sm_bin_file_: " + sm_bin_file_ + ", " +
	          std::strerror(errno));
	    if (fread(sm_content.data(), 1, sm_content.size(), sm_bin) <= 0)
	      throw std::runtime_error(
	          "Error reading sm_bin_file_: " + sm_bin_file_ + ", " +
	          std::strerror(errno));
	    fclose(sm_bin);
	  }

	  {
	    // The actual SM hash computation.
	    hash_ctx_t hash_ctx;
	    hash_init(&hash_ctx);
	    hash_extend(&hash_ctx, sm_content.data(), sm_content.size());
	    hash_finalize(expected_sm_hash, &hash_ctx);
	  }
	}

Then, the hashes are compared against the ones in the report:

.. code-block:: cpp

	void
	Verifier::verify_hashes(
	    Report& report, const byte* expected_enclave_hash,
	    const byte* expected_sm_hash, const byte* dev_public_key) {
	  if (report.verify(expected_enclave_hash, expected_sm_hash, dev_public_key)) {
	    printf("Enclave and SM hashes match with expected.\n");
	  } else {
	    printf(
	        "Either the enclave hash or the SM hash (or both) does not "
	        "match with expeced.\n");
	    report.printPretty();
	  }
	}

Finally, let’s do 5: Verify the nonce sent from the EApp is the one
generated by the verifier:

.. code-block:: cpp

	void
	Verifier::verify_data(Report& report, const std::string& nonce) {
	  if (report.getDataSize() != nonce.length() + 1) {
	    const char error[] =
	        "The size of the data in the report is not equal to the size of the "
	        "nonce initially sent.";
	    printf(error);
	    report.printPretty();
	    throw std::runtime_error(error);
	  }

	  if (0 == strcmp(nonce.c_str(), (char*)report.getDataSection())) {
	    printf("Returned data in the report match with the nonce sent.\n");
	  } else {
	    printf("Returned data in the report do NOT match with the nonce sent.\n");
	  }
	}

See ``verifier.h`` and ``verifier.cpp`` for the full implementation of
the ``Verifier`` class.

Enclave Package
------------------------------

``CMakeLists.txt`` contains packaging commands using ``makeself``.
``makeself`` generates a self-extracting archive with a start-up command.

In order to build the example, try the following in the build directory:

::

  make attestor-package

This will generate an enclave package named ``attestor.ke`` under ``<build directory>/examples/attestation``.
``attestor.ke`` is an self-extracting archive file for the enclave.

Next, copy the package and the Security Monitor binary into the buildroot overlay directory.

::

  # in the build directory
  cp examples/attestation/attestor.ke ./overlay/root
  cp sm.build/platform/generic/firmware/fw_payload.bin overlay/root/

Running ``make image`` in your build directory will generate the buildroot disk
image containing the copied package.

::

	# in your <build directory>
	make image

Deploying Enclave
------------------------------

Boot the machine with QEMU.

::

	./scripts/run-qemu.sh

Insert the Keystone driver

::

	# [inside QEMU]
	insmod keystone-driver.ko

Deploy the enclave

::

	# [inside QEMU]
	./attestor.ke

You'll see the enclave running!

::

	Verifying archive integrity... All good.
	Uncompressing Keystone Enclave Package
	Enclave said value: 5000
	Enclave said value: 10000
	Attestation report SIGNATURE is valid
	Enclave and SM hashes match with expected.
	Returned data in the report match with the nonce sent.
