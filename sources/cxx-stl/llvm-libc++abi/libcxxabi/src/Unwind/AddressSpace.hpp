//===------------------------- AddressSpace.hpp ---------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//
// Abstracts accessing local vs remote address spaces.
//
//===----------------------------------------------------------------------===//

#ifndef __ADDRESSSPACE_HPP__
#define __ADDRESSSPACE_HPP__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#if __APPLE__
#include <mach-o/dyld_priv.h>
namespace libunwind {
   bool checkKeyMgrRegisteredFDEs(uintptr_t targetAddr, void *&fde);
}
#endif

#include "libunwind.h"
#include "config.h"
#include "dwarf2.h"
#include "Registers.hpp"

namespace libunwind {

/// Used by findUnwindSections() to return info about needed sections.
struct UnwindInfoSections {
  uintptr_t        dso_base;
#if _LIBUNWIND_SUPPORT_DWARF_UNWIND
  uintptr_t       dwarf_section;
  uintptr_t       dwarf_section_length;
#endif
#if _LIBUNWIND_SUPPORT_DWARF_INDEX
  uintptr_t       dwarf_index_section;
  uintptr_t       dwarf_index_section_length;
#endif
#if _LIBUNWIND_SUPPORT_COMPACT_UNWIND
  uintptr_t       compact_unwind_section;
  uintptr_t       compact_unwind_section_length;
#endif
#if _LIBUNWIND_SUPPORT_ARM_UNWIND
  uintptr_t       arm_section;
  uintptr_t       arm_section_length;
#endif
};


/// LocalAddressSpace is used as a template parameter to UnwindCursor when
/// unwinding a thread in the same process.  The wrappers compile away,
/// making local unwinds fast.
class __attribute__((visibility("hidden"))) LocalAddressSpace {
public:
#if __LP64__
  typedef uint64_t pint_t;
  typedef int64_t  sint_t;
#else
  typedef uint32_t pint_t;
  typedef int32_t  sint_t;
#endif
  uint8_t         get8(pint_t addr)      { return *((uint8_t *)addr); }
  uint16_t        get16(pint_t addr)     { return *((uint16_t *)addr); }
  uint32_t        get32(pint_t addr)     { return *((uint32_t *)addr); }
  uint64_t        get64(pint_t addr)     { return *((uint64_t *)addr); }
  double          getDouble(pint_t addr) { return *((double *)addr); }
  v128            getVector(pint_t addr) { return *((v128 *)addr); }
  uintptr_t       getP(pint_t addr);
  static uint64_t getULEB128(pint_t &addr, pint_t end);
  static int64_t  getSLEB128(pint_t &addr, pint_t end);

  pint_t getEncodedP(pint_t &addr, pint_t end, uint8_t encoding);
  bool findFunctionName(pint_t addr, char *buf, size_t bufLen,
                        unw_word_t *offset);
  bool findUnwindSections(pint_t targetAddr, UnwindInfoSections &info);
  bool findOtherFDE(pint_t targetAddr, pint_t &fde);

  static LocalAddressSpace sThisAddressSpace;
};


inline uintptr_t LocalAddressSpace::getP(pint_t addr) {
#if __LP64__
  return get64(addr);
#else
  return get32(addr);
#endif
}

/// Read a ULEB128 into a 64-bit word.
inline uint64_t LocalAddressSpace::getULEB128(pint_t &addr, pint_t end) {
  const uint8_t *p = (uint8_t *)addr;
  const uint8_t *pend = (uint8_t *)end;
  uint64_t result = 0;
  int bit = 0;
  do {
    uint64_t b;

    if (p == pend)
      _LIBUNWIND_ABORT("truncated uleb128 expression");

    b = *p & 0x7f;

    if (bit >= 64 || b << bit >> bit != b) {
      _LIBUNWIND_ABORT("malformed uleb128 expression");
    } else {
      result |= b << bit;
      bit += 7;
    }
  } while (*p++ >= 0x80);
  addr = (pint_t) p;
  return result;
}

/// Read a SLEB128 into a 64-bit word.
inline int64_t LocalAddressSpace::getSLEB128(pint_t &addr, pint_t end) {
  const uint8_t *p = (uint8_t *)addr;
  const uint8_t *pend = (uint8_t *)end;
  int64_t result = 0;
  int bit = 0;
  uint8_t byte;
  do {
    if (p == pend)
      _LIBUNWIND_ABORT("truncated sleb128 expression");
    byte = *p++;
    result |= ((byte & 0x7f) << bit);
    bit += 7;
  } while (byte & 0x80);
  // sign extend negative numbers
  if ((byte & 0x40) != 0)
    result |= (-1LL) << bit;
  addr = (pint_t) p;
  return result;
}

inline LocalAddressSpace::pint_t LocalAddressSpace::getEncodedP(pint_t &addr,
                                                         pint_t end,
                                                         uint8_t encoding) {
  pint_t startAddr = addr;
  const uint8_t *p = (uint8_t *)addr;
  pint_t result;

  // first get value
  switch (encoding & 0x0F) {
  case DW_EH_PE_ptr:
    result = getP(addr);
    p += sizeof(pint_t);
    addr = (pint_t) p;
    break;
  case DW_EH_PE_uleb128:
    result = (pint_t)getULEB128(addr, end);
    break;
  case DW_EH_PE_udata2:
    result = get16(addr);
    p += 2;
    addr = (pint_t) p;
    break;
  case DW_EH_PE_udata4:
    result = get32(addr);
    p += 4;
    addr = (pint_t) p;
    break;
  case DW_EH_PE_udata8:
    result = (pint_t)get64(addr);
    p += 8;
    addr = (pint_t) p;
    break;
  case DW_EH_PE_sleb128:
    result = (pint_t)getSLEB128(addr, end);
    break;
  case DW_EH_PE_sdata2:
    result = (uint16_t)get16(addr);
    p += 2;
    addr = (pint_t) p;
    break;
  case DW_EH_PE_sdata4:
    result = (uint32_t)get32(addr);
    p += 4;
    addr = (pint_t) p;
    break;
  case DW_EH_PE_sdata8:
    result = (pint_t)get64(addr);
    p += 8;
    addr = (pint_t) p;
    break;
  default:
    _LIBUNWIND_ABORT("unknown pointer encoding");
  }

  // then add relative offset
  switch (encoding & 0x70) {
  case DW_EH_PE_absptr:
    // do nothing
    break;
  case DW_EH_PE_pcrel:
    result += startAddr;
    break;
  case DW_EH_PE_textrel:
    _LIBUNWIND_ABORT("DW_EH_PE_textrel pointer encoding not supported");
    break;
  case DW_EH_PE_datarel:
    _LIBUNWIND_ABORT("DW_EH_PE_datarel pointer encoding not supported");
    break;
  case DW_EH_PE_funcrel:
    _LIBUNWIND_ABORT("DW_EH_PE_funcrel pointer encoding not supported");
    break;
  case DW_EH_PE_aligned:
    _LIBUNWIND_ABORT("DW_EH_PE_aligned pointer encoding not supported");
    break;
  default:
    _LIBUNWIND_ABORT("unknown pointer encoding");
    break;
  }

  if (encoding & DW_EH_PE_indirect)
    result = getP(result);

  return result;
}

inline bool LocalAddressSpace::findUnwindSections(pint_t targetAddr,
                                                  UnwindInfoSections &info) {
#if __APPLE__
  dyld_unwind_sections dyldInfo;
  if (_dyld_find_unwind_sections((void *)targetAddr, &dyldInfo)) {
    info.dso_base                      = (uintptr_t)dyldInfo.mh;
 #if _LIBUNWIND_SUPPORT_DWARF_UNWIND
    info.dwarf_section                 = (uintptr_t)dyldInfo.dwarf_section;
    info.dwarf_section_length          = dyldInfo.dwarf_section_length;
 #endif
    info.compact_unwind_section        = (uintptr_t)dyldInfo.compact_unwind_section;
    info.compact_unwind_section_length = dyldInfo.compact_unwind_section_length;
    return true;
  }
#else
  // TO DO

#endif

#if _LIBUNWIND_SUPPORT_ARM_UNWIND
#warning TODO(danakj): Set these to something legit.
  info.arm_section        = (uintptr_t)0x10101010;
  info.arm_section_length = 0;
#endif

  return false;
}


inline bool LocalAddressSpace::findOtherFDE(pint_t targetAddr, pint_t &fde) {
#if __APPLE__
  return checkKeyMgrRegisteredFDEs(targetAddr, *((void**)&fde));
#else
  // TO DO: if OS has way to dynamically register FDEs, check that.
  return false;
#endif
}

inline bool LocalAddressSpace::findFunctionName(pint_t addr, char *buf,
                                                size_t bufLen,
                                                unw_word_t *offset) {
#warning FIXME
  return false;
  /*
  dl_info dyldInfo;
  if (dladdr((void *)addr, &dyldInfo)) {
    if (dyldInfo.dli_sname != NULL) {
      strlcpy(buf, dyldInfo.dli_sname, bufLen);
      *offset = (addr - (pint_t) dyldInfo.dli_saddr);
      return true;
    }
  }
  return false;
  */
}



#if UNW_REMOTE

/// OtherAddressSpace is used as a template parameter to UnwindCursor when
/// unwinding a thread in the another process.  The other process can be a
/// different endianness and a different pointer size which is handled by
/// the P template parameter.
template <typename P>
class OtherAddressSpace {
public:
  OtherAddressSpace(task_t task) : fTask(task) {}

  typedef typename P::uint_t pint_t;

  uint8_t   get8(pint_t addr);
  uint16_t  get16(pint_t addr);
  uint32_t  get32(pint_t addr);
  uint64_t  get64(pint_t addr);
  pint_t    getP(pint_t addr);
  uint64_t  getULEB128(pint_t &addr, pint_t end);
  int64_t   getSLEB128(pint_t &addr, pint_t end);
  pint_t    getEncodedP(pint_t &addr, pint_t end, uint8_t encoding);
  bool      findFunctionName(pint_t addr, char *buf, size_t bufLen,
                        unw_word_t *offset);
  bool      findUnwindSections(pint_t targetAddr, UnwindInfoSections &info);
  bool      findOtherFDE(pint_t targetAddr, pint_t &fde);
private:
  void *localCopy(pint_t addr);

  task_t fTask;
};

template <typename P> uint8_t OtherAddressSpace<P>::get8(pint_t addr) {
  return *((uint8_t *)localCopy(addr));
}

template <typename P> uint16_t OtherAddressSpace<P>::get16(pint_t addr) {
  return P::E::get16(*(uint16_t *)localCopy(addr));
}

template <typename P> uint32_t OtherAddressSpace<P>::get32(pint_t addr) {
  return P::E::get32(*(uint32_t *)localCopy(addr));
}

template <typename P> uint64_t OtherAddressSpace<P>::get64(pint_t addr) {
  return P::E::get64(*(uint64_t *)localCopy(addr));
}

template <typename P>
typename P::uint_t OtherAddressSpace<P>::getP(pint_t addr) {
  return P::getP(*(uint64_t *)localCopy(addr));
}

template <typename P>
uint64_t OtherAddressSpace<P>::getULEB128(pint_t &addr, pint_t end) {
  uintptr_t size = (end - addr);
  LocalAddressSpace::pint_t laddr = (LocalAddressSpace::pint_t) localCopy(addr);
  LocalAddressSpace::pint_t sladdr = laddr;
  uint64_t result = LocalAddressSpace::getULEB128(laddr, laddr + size);
  addr += (laddr - sladdr);
  return result;
}

template <typename P>
int64_t OtherAddressSpace<P>::getSLEB128(pint_t &addr, pint_t end) {
  uintptr_t size = (end - addr);
  LocalAddressSpace::pint_t laddr = (LocalAddressSpace::pint_t) localCopy(addr);
  LocalAddressSpace::pint_t sladdr = laddr;
  uint64_t result = LocalAddressSpace::getSLEB128(laddr, laddr + size);
  addr += (laddr - sladdr);
  return result;
}

template <typename P> void *OtherAddressSpace<P>::localCopy(pint_t addr) {
  // FIX ME
}

template <typename P>
bool OtherAddressSpace<P>::findFunctionName(pint_t addr, char *buf,
                                            size_t bufLen, unw_word_t *offset) {
  // FIX ME
}

/// unw_addr_space is the base class that abstract unw_addr_space_t type in
/// libunwind.h points to.
struct unw_addr_space {
  cpu_type_t cpuType;
  task_t taskPort;
};

/// unw_addr_space_i386 is the concrete instance that a unw_addr_space_t points
/// to when examining
/// a 32-bit intel process.
struct unw_addr_space_i386 : public unw_addr_space {
  unw_addr_space_i386(task_t task) : oas(task) {}
  OtherAddressSpace<Pointer32<LittleEndian> > oas;
};

/// unw_addr_space_x86_64 is the concrete instance that a unw_addr_space_t
/// points to when examining
/// a 64-bit intel process.
struct unw_addr_space_x86_64 : public unw_addr_space {
  unw_addr_space_x86_64(task_t task) : oas(task) {}
  OtherAddressSpace<Pointer64<LittleEndian> > oas;
};

/// unw_addr_space_ppc is the concrete instance that a unw_addr_space_t points
/// to when examining
/// a 32-bit PowerPC process.
struct unw_addr_space_ppc : public unw_addr_space {
  unw_addr_space_ppc(task_t task) : oas(task) {}
  OtherAddressSpace<Pointer32<BigEndian> > oas;
};

#endif // UNW_REMOTE

} // namespace libunwind

#endif // __ADDRESSSPACE_HPP__
