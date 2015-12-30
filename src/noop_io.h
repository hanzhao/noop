#ifndef NOOP_IO_H
#define NOOP_IO_H

#include <iostream>
#include <string>


namespace noop {

std::ostream& operator <<(std::ostream& out, const std::u16string u16_str);
std::ostream& operator <<(std::ostream& out, const char16_t u16_chr);

namespace Encoding {

std::u16string UTF8ToUTF16(std::string u8_str);

} // namespace Encoding

} // namespace noop

#endif // NOOP_IO_H
