//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------

#ifndef _ATTESTATION_HOST_H_
#define _ATTESTATION_HOST_H_

#include <optional>
#include <utility>

#include "edge/edge_common.h"
#include "host/keystone.h"
#include "verifier/report.h"

class SharedBuffer {
 public:
  SharedBuffer(void* buffer, size_t buffer_len)
      /* For now we assume the call struct is at the front of the shared
       * buffer. This will have to change to allow nested calls. */
      : edge_call_((struct edge_call*)buffer),
        buffer_((uintptr_t)buffer),
        buffer_len_(buffer_len) {}

  uintptr_t ptr() { return buffer_; }
  size_t size() { return buffer_len_; }

  std::optional<char*> get_c_string_or_set_bad_offset();
  std::optional<unsigned long> get_unsigned_long_or_set_bad_offset();
  std::optional<Report> get_report_or_set_bad_offset();

  void set_ok();
  void setup_ret_or_bad_ptr(unsigned long ret_val);
  void setup_wrapped_ret_or_bad_ptr(const std::string& ret_val);

 private:
  uintptr_t data_ptr();
  int args_ptr(uintptr_t* ptr, size_t* size);
  int validate_ptr(uintptr_t ptr);
  int get_offset_from_ptr(uintptr_t ptr, edge_data_offset* offset);
  int get_ptr_from_offset(edge_data_offset offset, uintptr_t* ptr);
  std::optional<std::pair<uintptr_t, size_t>>
  get_call_args_ptr_or_set_bad_offset();

  void set_bad_offset();
  void set_bad_ptr();
  int setup_ret(void* ptr, size_t size);
  int setup_wrapped_ret(void* ptr, size_t size);

  struct edge_call* const edge_call_;
  uintptr_t const buffer_;
  size_t const buffer_len_;
};

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
 struct RunData {
   SharedBuffer shared_buffer;
   const std::string& nonce;
   std::unique_ptr<Report> report;
 };
 static void dispatch_ocall(RunData& run_data);
 static void print_buffer_wrapper(RunData& run_data);
 static void print_value_wrapper(RunData& run_data);
 static void copy_report_wrapper(RunData& run_data);
 static void get_host_string_wrapper(RunData& run_data);
 const Keystone::Params params_;
 const std::string eapp_file_;
 const std::string rt_file_;
};

#endif /* _ATTESTATION_HOST_H_ */
