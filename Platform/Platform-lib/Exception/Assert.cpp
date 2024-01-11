//
// Created by Whisky on 12/25/2023.
//

#include "Assert.hpp"
#include "Machine.hpp"


void Platform_AssertFailed(uint8_t* file, uint32_t line)
{
    cout << "Wrong parameters value: file " << (const char *)file << " on line " << line << "\r\n";
    while (true) { }
}

void Platform_AssertFailed(uint8_t* file, uint32_t line, const char *error)
{
    cout << error << "\r\n";
    cout << "Wrong parameters value: file " << (const char *)file << " on line " << line << "\r\n";
    while (true) { }
}

