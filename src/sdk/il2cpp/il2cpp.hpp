#pragma once

#include <memory/memory.hpp>
#include <sdk/offsets.hpp>

#include <print>

#define FIELD(func, type, offset)                                                   \
    type func {                                                                     \
        return memory->read<type>(reinterpret_cast<std::uintptr_t>(this) + offset); \
    }

#define FIELD_STR(func, offset)                                                                   \
    std::string func {                                                                            \
        auto ptr = memory->read<std::uintptr_t>(reinterpret_cast<std::uintptr_t>(this) + offset); \
        return memory->read_string(ptr);                                                          \
    }

struct Il2CppClass {
    FIELD_STR(name(), 0x10);
    FIELD_STR(namespaze(), 0x18);
};

struct Il2CppImageGlobalMetadata {
    FIELD(typeStart(), std::int32_t, 0x0);
};

struct Il2CppImage {
    FIELD_STR(name(), 0x0);
    FIELD(typeCount(), std::uint32_t, 0x18);
    FIELD(metadataHandle(), Il2CppImageGlobalMetadata*, 0x28);

    using classes_t = std::vector<std::pair<std::string, Il2CppClass*>>;

    classes_t get_classes() {
        classes_t ret = {};

        const auto base = memory->get_module("GameAssembly.dll").base;

        const auto type_info_table = memory->read<std::uintptr_t>(base + offsets::TYPE_INFO_TABLE);

        for (auto i = 0; i < this->typeCount(); i++) {
            const auto index = this->metadataHandle()->typeStart() + i;
            const auto current = memory->read<Il2CppClass*>(type_info_table + (index * sizeof(std::uintptr_t)));

            const auto name = std::format("{}.{}", current->namespaze(), current->name());
            ret.emplace_back(name, current);
        }

        return ret;
    }

    Il2CppClass* get_class(const std::string& req_class) {
        const auto classes = get_classes();

        const auto itterator = std::ranges::find(classes, req_class, &std::pair<std::string, Il2CppClass*>::first);
        return itterator == classes.cend() ? nullptr : itterator->second;
    }
};

struct Il2CppAssembly {
    FIELD(image(), Il2CppImage*, 0x0);
    FIELD_STR(name(), 0x18);

    using assemblies_t = std::vector<std::pair<std::string, Il2CppAssembly*>>;

    static assemblies_t get_assemblies() {
        assemblies_t ret = {};

        const auto base = memory->get_module("GameAssembly.dll").base;

        const auto array_begin = memory->read<std::uintptr_t>(base + offsets::ASSEMBLIES_BEGIN);
        const auto array_end = memory->read<std::uintptr_t>(base + offsets::ASSEMBLIES_END);

        for (auto current_ptr = array_begin; current_ptr < array_end; current_ptr += sizeof(std::uintptr_t)) {
            if (!current_ptr) {
                continue;
            }

            const auto current = memory->read<Il2CppAssembly*>(current_ptr);
            ret.emplace_back(current->name(), current);
        }

        return ret;
    }

    static Il2CppAssembly* get_assembly(const std::string& req_assembly) {
        const auto assebmlies = get_assemblies();

        const auto itterator = std::ranges::find(assebmlies, req_assembly, &std::pair<std::string, Il2CppAssembly*>::first);
        return itterator == assebmlies.cend() ? nullptr : itterator->second;
    }
};