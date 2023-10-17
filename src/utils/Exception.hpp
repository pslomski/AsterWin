#pragma once

#include <exception>
#include <stdexcept>

class EGenericError : public std::runtime_error
{
public:
    EGenericError(const char* const& what) : std::runtime_error(what) {}
};

class ECannotCreateFont : public EGenericError
{
public:
    ECannotCreateFont(const char* const& what) : EGenericError(what) {}
};
