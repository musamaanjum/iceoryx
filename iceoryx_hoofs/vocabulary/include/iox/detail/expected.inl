// Copyright (c) 2019 by Robert Bosch GmbH. All rights reserved.
// Copyright (c) 2021 - 2023 by Apex.AI Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0
#ifndef IOX_HOOFS_VOCABULARY_EXPECTED_INL
#define IOX_HOOFS_VOCABULARY_EXPECTED_INL

#include "iox/expected.hpp"

namespace iox
{
template <typename T, typename>
detail::success<void> ok()
{
    return detail::success<void>{};
}

template <typename T, typename>
detail::success<T> ok(const T& value)
{
    return detail::success<T>{value};
}

template <typename T, typename, typename>
detail::success<T> ok(T&& value)
{
    return detail::success<T>{std::forward<T>(value)};
}

template <typename T, typename... Targs, typename>
detail::success<T> ok(Targs&&... args)
{
    return detail::success<T>{std::forward<Targs>(args)...};
}

template <typename T>
detail::error<T> err(const T& value)
{
    return detail::error<T>{value};
}

template <typename T, typename>
detail::error<T> err(T&& value)
{
    return detail::error<T>{std::forward<T>(value)};
}

template <typename T, typename... Targs>
detail::error<T> err(Targs&&... args)
{
    return detail::error<T>{std::forward<Targs>(args)...};
}

template <typename ValueType, typename ErrorType>
inline expected<ValueType, ErrorType>::expected(const detail::success<ValueType>& successValue) noexcept
    : m_store(in_place, successValue.value)
{
}

template <typename ValueType, typename ErrorType>
inline expected<ValueType, ErrorType>::expected(detail::success<ValueType>&& successValue) noexcept
    : m_store(in_place, std::move(successValue.value))
{
}

template <typename ValueType, typename ErrorType>
inline expected<ValueType, ErrorType>::expected(const detail::error<ErrorType>& errorValue) noexcept
    : m_store(unexpect, errorValue.value)
{
}

template <typename ValueType, typename ErrorType>
inline expected<ValueType, ErrorType>::expected(detail::error<ErrorType>&& errorValue) noexcept
    : m_store(unexpect, std::move(errorValue.value))
{
}

template <typename ValueType, typename ErrorType>
inline expected<ValueType, ErrorType>::expected(expected<ValueType, ErrorType>&& rhs) noexcept
    : m_store{std::move(rhs.m_store)}
{
}

template <typename ValueType, typename ErrorType>
template <typename... Targs>
inline expected<ValueType, ErrorType>::expected(in_place_t, Targs&&... args) noexcept
    : m_store(in_place, std::forward<Targs>(args)...)
{
}

template <typename ValueType, typename ErrorType>
template <typename... Targs>
inline expected<ValueType, ErrorType>::expected(unexpect_t, Targs&&... args) noexcept
    : m_store(unexpect, std::forward<Targs>(args)...)
{
}

template <typename ValueType, typename ErrorType>
inline expected<ValueType, ErrorType>&
expected<ValueType, ErrorType>::operator=(expected<ValueType, ErrorType>&& rhs) noexcept
{
    // AXIVION Next Construct AutosarC++19_03-M0.1.2, AutosarC++19_03-M0.1.9, FaultDetection-DeadBranches : False positive. Check needed to avoid self assignment.
    if (this != &rhs)
    {
        m_store = std::move(rhs.m_store);
    }
    return *this;
}

template <typename ValueType, typename ErrorType>
template <typename... Targs>
inline expected<ValueType, ErrorType> expected<ValueType, ErrorType>::create_value(Targs&&... args) noexcept
{
    return expected{in_place, std::forward<Targs>(args)...};
}

template <typename ValueType, typename ErrorType>
template <typename... Targs>
inline expected<ValueType, ErrorType> expected<ValueType, ErrorType>::create_error(Targs&&... args) noexcept
{
    return expected{unexpect, std::forward<Targs>(args)...};
}

template <typename ValueType, typename ErrorType>
inline expected<ValueType, ErrorType>::operator bool() const noexcept
{
    return !has_error();
}

template <typename ValueType, typename ErrorType>
inline bool expected<ValueType, ErrorType>::has_error() const noexcept
{
    return m_store.has_error();
}

template <typename ValueType, typename ErrorType>
inline ErrorType&& expected<ValueType, ErrorType>::get_error() && noexcept
{
    return std::move(get_error());
}

template <typename ValueType, typename ErrorType>
inline ErrorType& expected<ValueType, ErrorType>::get_error() & noexcept
{
    // AXIVION Next Construct AutosarC++19_03-A5.2.3 : const cast to avoid code duplication
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    return const_cast<ErrorType&>(const_cast<const expected<ValueType, ErrorType>*>(this)->get_error());
}

template <typename ValueType, typename ErrorType>
inline const ErrorType& expected<ValueType, ErrorType>::get_error() const& noexcept
{
    cxx::ExpectsWithMsg(has_error(), "Trying to access an error but a value is stored!");
    return m_store.error_unchecked();
}

template <typename ValueType, typename ErrorType>
template <typename U>
inline enable_if_non_void_t<U>&& expected<ValueType, ErrorType>::value() && noexcept
{
    return std::move(value());
}

template <typename ValueType, typename ErrorType>
template <typename U>
inline const enable_if_non_void_t<U>& expected<ValueType, ErrorType>::value() const& noexcept
{
    cxx::ExpectsWithMsg(!has_error(), "Trying to access a value but an error is stored!");
    return m_store.value_unchecked();
}

template <typename ValueType, typename ErrorType>
template <typename U>
inline enable_if_non_void_t<U>& expected<ValueType, ErrorType>::value() & noexcept
{
    // AXIVION Next Construct AutosarC++19_03-A5.2.3 : const cast to avoid code duplication
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    return const_cast<ValueType&>(const_cast<const expected<ValueType, ErrorType>*>(this)->value());
}

template <typename ValueType, typename ErrorType>
template <typename U>
inline enable_if_non_void_t<U>* expected<ValueType, ErrorType>::operator->() noexcept
{
    return &value();
}

template <typename ValueType, typename ErrorType>
template <typename U>
inline const enable_if_non_void_t<U>* expected<ValueType, ErrorType>::operator->() const noexcept
{
    // AXIVION Next Construct AutosarC++19_03-A5.2.3 : const_cast avoids code duplication, is safe since the
    // constness of the return value is restored
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    return const_cast<const ValueType*>(const_cast<expected*>(this)->operator->());
}

template <typename ValueType, typename ErrorType>
template <typename U>
inline enable_if_non_void_t<U>& expected<ValueType, ErrorType>::operator*() noexcept
{
    return value();
}

template <typename ValueType, typename ErrorType>
template <typename U>
inline const enable_if_non_void_t<U>& expected<ValueType, ErrorType>::operator*() const noexcept
{
    // AXIVION Next Construct AutosarC++19_03-A5.2.3 : const_cast avoids code duplication and is safe here, since the
    // constness of the return value is restored
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    return const_cast<const ValueType&>(const_cast<expected*>(this)->operator*());
}

// AXIVION Next Construct AutosarC++19_03-A13.5.2, AutosarC++19_03-A13.5.3: see doxygen brief section in header
template <typename ValueType, typename ErrorType>
inline expected<ValueType, ErrorType>::operator expected<void, ErrorType>() const noexcept
{
    if (has_error())
    {
        return err(m_store.error_unchecked());
    }
    return expected<void, ErrorType>(in_place);
}

template <typename ValueType, typename ErrorType>
template <typename U>
inline optional<enable_if_non_void_t<U>> expected<ValueType, ErrorType>::to_optional() const noexcept
{
    optional<enable_if_non_void_t<U>> returnValue;
    if (!has_error())
    {
        returnValue.emplace(value());
    }
    return returnValue;
}

template <typename ValueType, typename ErrorType>
inline constexpr bool operator==(const expected<ValueType, ErrorType>& lhs,
                                 const expected<ValueType, ErrorType>& rhs) noexcept
{
    if (lhs.has_error() != rhs.has_error())
    {
        return false;
    }
    if (lhs.has_error() && rhs.has_error())
    {
        return lhs.m_store.error_unchecked() == rhs.m_store.error_unchecked();
    }
    return detail::compare_expected_value<ValueType, ErrorType>::is_same_value_unchecked(lhs.m_store, rhs.m_store);
}

template <typename ValueType, typename ErrorType>
inline constexpr bool operator!=(const expected<ValueType, ErrorType>& lhs,
                                 const expected<ValueType, ErrorType>& rhs) noexcept
{
    return !(lhs == rhs);
}
} // namespace iox

#endif // IOX_HOOFS_VOCABULARY_EXPECTED_INL
