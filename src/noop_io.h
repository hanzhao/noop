#ifndef NOOP_IO_H
#define NOOP_IO_H

#include <noop.h>


namespace noop {

std::ostream& operator <<(std::ostream& out, const String u32_str);
std::ostream& operator <<(std::ostream& out, const Char u32_chr);

namespace Encoding {

String UTF8ToUTF32(std::string u8_str);
std::string UTF32ToUTF8(String u32_str);

} // namespace Encoding

} // namespace noop

#endif // NOOP_IO_H
