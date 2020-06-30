#include "..\include\Exception.h"

MeyaS::Exception::Exception():reason(std::string("The author is a lazy dog so the reason is omitted")) {}

MeyaS::Exception::Exception(std::string reason):reason(std::move(reason)) {
}

const char *MeyaS::Exception::what() const noexcept {
    return this->reason.c_str();
}
