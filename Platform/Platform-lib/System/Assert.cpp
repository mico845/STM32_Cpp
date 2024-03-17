#include "platform-lib/System/Assert.hpp"
#include "Machine.hpp"

void Platform_AssertFailed(uint8_t* file, uint32_t line)
{
    STM32::cout << "Wrong parameters value: file " << (const char *)file << " on line " << line << "\r\n";
    while (true) { }
}

void Platform_AssertFailed(uint8_t* file, uint32_t line, const char *error)
{
    STM32::cout << error << "\r\n";
    STM32::cout << "Wrong parameters value: file " << (const char *)file << " on line " << line << "\r\n";
    while (true) { }
}

