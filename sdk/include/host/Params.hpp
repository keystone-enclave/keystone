//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#pragma once

#include <cstdint>
#include <cstdio>

namespace detail {
namespace defaults {
constexpr std::uint64_t untrusted_size = 8192;  // 8 KB
#if __riscv_xlen == 64
constexpr std::uint64_t freemem_size  = 1024 * 1024;  // 1 MB
constexpr std::uint64_t stack_size    = 1024 * 16;    // 16k
constexpr std::uint64_t stack_start   = 0x0000000040000000;
constexpr std::uint64_t untrusted_ptr = 0xffffffff80000000;
#elif __riscv_xlen == 32
constexpr std::uint64_t freemem_size  = 1024 * 512;  // 512 KiB
constexpr std::uint64_t stack_size    = 1024 * 8;    // 3 KiB
constexpr std::uint64_t stack_start   = 0x40000000;
constexpr std::uint64_t untrusted_ptr = 0x80000000;
#else
constexpr std::uint64_t freemem_size  = 1024 * 1024;  // 1 MB
constexpr std::uint64_t stack_size    = 1024 * 16;    // 16k
constexpr std::uint64_t stack_start   = 0x0000000040000000;
constexpr std::uint64_t untrusted_ptr = 0xffffffff80000000;
#endif
}  // namespace defaults
}  // namespace detail

/* parameters for enclave creation */
namespace Keystone {

class Params {
 public:
  void setSimulated(bool _simulated) { simulated = _simulated; }

  void setEnclaveEntry(std::uint64_t) {
    printf("WARN: setEnclaveEntry() is deprecated.\n");
  }

  void setUntrustedMem(std::uint64_t ptr, std::uint64_t size) {
    untrusted      = ptr;
    untrusted_size = size;
  }

  void setFreeMemSize(std::uint64_t size) { freemem_size = size; }
  bool isSimulated() { return simulated; }
  std::uintptr_t getUntrustedMem() { return untrusted; }
  std::uintptr_t getUntrustedSize() { return untrusted_size; }
  std::uintptr_t getUntrustedEnd() { return untrusted + untrusted_size; }
  std::uintptr_t getFreeMemSize() { return freemem_size; }

 private:
  bool simulated{false};
  std::uint64_t runtime_entry;
  std::uint64_t enclave_entry;
  std::uint64_t untrusted{detail::defaults::untrusted_ptr};
  std::uint64_t untrusted_size{detail::defaults::untrusted_size};
  std::uint64_t freemem_size{detail::defaults::freemem_size};
};

}  // namespace Keystone
