#pragma once

#include <cstdint>

namespace offsets {
    constexpr auto ASSEMBLIES_BEGIN = 0x69D86C0; // il2cpp_domain_get_assemblies -> mov rax, offset
    constexpr auto ASSEMBLIES_END = ASSEMBLIES_BEGIN + sizeof(std::uintptr_t);
    constexpr auto TYPE_INFO_TABLE = 0x69D8998; // 48 89 05 ? ? ? ? 48 8B 05 ? ? ? ? 8B 48 -> mov offset, rax
} // namespace offsets