/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef KBASE_SCOPED_HANDLE_H_
#define KBASE_SCOPED_HANDLE_H_

#include <Windows.h>

#include "kbase/basic_macros.h"

namespace kbase {

struct HandleTraits {
    using Handle = HANDLE;

    HandleTraits() = delete;

    ~HandleTraits() = delete;

    static Handle NullHandle() noexcept
    {
        return nullptr;
    }

    static bool IsValid(Handle handle) noexcept
    {
        return handle != nullptr && handle != INVALID_HANDLE_VALUE;
    }

    static void Close(Handle handle) noexcept
    {
        CloseHandle(handle);
    }
};

template<typename Traits>
class GenericScopedHandle {
public:
    using Handle = typename Traits::Handle;

    GenericScopedHandle() noexcept
        : handle_(Traits::NullHandle())
    {}

    explicit GenericScopedHandle(Handle handle) noexcept
        : handle_(handle)
    {}

    GenericScopedHandle(GenericScopedHandle&& other) noexcept
        : handle_(other.handle_)
    {
        other.handle_ = Traits::NullHandle();
    }

    ~GenericScopedHandle()
    {
        Close();
    }

    GenericScopedHandle& operator=(GenericScopedHandle&& rhs) noexcept
    {
        Close();

        handle_ = rhs.handle_;
        rhs.handle_ = Traits::NullHandle();

        return *this;
    }

    // A convenient way for destroying the object.
    GenericScopedHandle& operator=(nullptr_t) noexcept
    {
        Close();
        handle_ = Traits::NullHandle();

        return *this;
    }

    DISALLOW_COPY(GenericScopedHandle);

    explicit operator bool() const noexcept
    {
        return Traits::IsValid(handle_);
    }

    Handle get() const noexcept
    {
        return handle_;
    }

    Handle release() noexcept
    {
        Handle self = handle_;
        handle_ = Traits::NullHandle();

        return self;
    }

    void reset(Handle new_handle) noexcept
    {
        Close();
        handle_ = new_handle;
    }

    void reset(nullptr_t = nullptr) noexcept
    {
        Close();
        handle_ = Traits::NullHandle();
    }

    void swap(GenericScopedHandle& other) noexcept
    {
        using std::swap;
        swap(handle_, other.handle_);
    }

private:
    // In most cases, caller should update `handle_` after calling this function.
    void Close() noexcept
    {
        if (Traits::IsValid(handle_)) {
            Traits::Close(handle_);
        }
    }

private:
    Handle handle_;
};

// Specialized version for std::swap.
template<typename Traits>
void swap(GenericScopedHandle<Traits>& lhs, GenericScopedHandle<Traits>& rhs) noexcept
{
    lhs.swap(rhs);
}

using ScopedHandle = GenericScopedHandle<HandleTraits>;

}   // namespace kbase

#endif  // KBASE_SCOPED_HANDLE_H_
