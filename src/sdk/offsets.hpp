#pragma once

#include <cstdint>

namespace offsets {
    constexpr auto ASSEMBLIES_BEGIN = 0x593D8B8; // il2cpp_domain_get_assemblies -> first call -> lea rax, offset
    constexpr auto ASSEMBLIES_END = ASSEMBLIES_BEGIN + sizeof(std::uintptr_t);
    constexpr auto TYPE_INFO_TABLE = 0x593C1B0; // 48 89 05 59 C8 73 05 -> mov offset, rax
} // namespace offsets