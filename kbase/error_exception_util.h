/*
 @ Kingsley Chen
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef KBASE_ERROR_EXCEPTION_UTIL_H_
#define KBASE_ERROR_EXCEPTION_UTIL_H_

#include <sstream>
#include <string>
#include <stdexcept>

#include "kbase/strings/string_format.h"

namespace kbase {

#define GUARANTOR_A(x) GUARANTOR_OP(x, B)
#define GUARANTOR_B(x) GUARANTOR_OP(x, A)
#define GUARANTOR_OP(x, next) \
    GUARANTOR_A.current_value(#x, (x)).GUARANTOR_ ## next

#define MAKE_GUARANTOR(exp) kbase::Guarantor(exp, __FILE__, __LINE__)

#define ENSURE(exp)                                                       \
    if ((exp)) ;                                                          \
    else                                                                  \
        MAKE_GUARANTOR(#exp).GUARANTOR_A     
        /*throw std::runtime_error("bla");*/ 

class Guarantor {
public:
    Guarantor(const char* msg, const char* file_name, int line)
    {
        std::string context  
            = StringPrintf("Failed: %s\nFile: %s Line: %d\nCurrent Variables:\n",
                         msg, file_name, line);
        exception_desc_ << context;
    }

    ~Guarantor() = default;

    Guarantor(const Guarantor&) = delete;
    Guarantor& operator=(const Guarantor&) = delete;

    //Guarantor& context(const char* file_name, int line);

    template<typename T>
    Guarantor& current_value(const char* name, const T& value)
    {
        exception_desc_ << name << " = " << value << "\n";
        return *this;
    }

    // stubs
    Guarantor& GUARANTOR_A = *this;
    Guarantor& GUARANTOR_B = *this;

    std::string exception_description() const
    {
        return exception_desc_.str();
    }

private:
    std::ostringstream exception_desc_;
};

// This class automatically retrieves the last error code of the calling thread when
// constructing an instance, and stores the value internally.
class LastError {
public:
    LastError();

    unsigned long last_error_code() const;

    // Since the description of the error is intended for programmers only, the
    // function insists on using English as its dispalying language.
    std::wstring GetVerboseMessage() const;

private:
    unsigned long error_code_;
};

class Win32Exception : public std::runtime_error {
public:
    Win32Exception(unsigned long last_error, const std::string& message);
    
    unsigned long error_code() const;

private:
    unsigned long error_code_;
};

// Throws a Win32Exception if |expression| is true.
// This function internally displays description of the last error, which means that
// ex.what() does return text like "user_message (descption_of_last_error)"
void ThrowLastErrorIf(bool expression, const std::string& user_message);

}   // namespace kbase

#endif  // KBASE_ERROR_EXCEPTION_UTIL_H_