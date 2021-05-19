#pragma once
#include <string_view>
#include <vector>
#include <Windows.h>


namespace memory_utils
{
	template<typename T>
	T* capture_interface(std::string_view module_name, std::string_view interface_name)
	{
		typedef void* (*interface_type)(const char* name, int ret);
		const auto temp = reinterpret_cast<interface_type>(GetProcAddress(GetModuleHandle(module_name.data()), "CreateInterface"));
		return static_cast<T*>(temp(interface_name.data(), 0));
	}

	inline std::uint8_t* pattern_scanner(const std::string_view module_name, const std::string_view signature) noexcept
	{
		auto* const module_handle = GetModuleHandleA(module_name.data());

        if (!module_handle)
            return nullptr;

        static auto pattern_to_byte = [](const std::string_view pattern) {
            auto bytes = std::vector<int>();
            auto* const start = const_cast<char*>(pattern.data());
            auto* const end = const_cast<char*>(pattern.data()) + std::strlen(pattern.data());

            for (auto* current = start; current < end; ++current) {
                if (*current == '?') {
                    ++current;

                    if (*current == '?')
                        ++current;

                    bytes.push_back(-1);
                }
                else {
                    bytes.push_back(std::strtoul(current, &current, 16));
                }
            }
            return bytes;
        };

        auto* const dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
        auto* const nt_headers =
            reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

        const auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
        auto pattern_bytes = pattern_to_byte(signature);
        auto* const scan_bytes = reinterpret_cast<std::uint8_t*>(module_handle);

        const auto s = pattern_bytes.size();
        auto* const d = pattern_bytes.data();

        for (auto i = 0ul; i < size_of_image - s; ++i) {
	        auto found = true;

            for (auto j = 0ul; j < s; ++j) {
                if (scan_bytes[i + j] != d[j] && d[j] != -1) {
                    found = false;
                    break;
                }
            }
            if (found)
                return &scan_bytes[i];
        }

        return nullptr;
	}

	template<typename t>
	t& get_offset(void* ptr, DWORD offset)
	{
        return *reinterpret_cast<t*>((uintptr_t)ptr + offset);
	}
}