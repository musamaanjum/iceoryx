// Copyright (c) 2019 - 2020 by Robert Bosch GmbH. All rights reserved.
// Copyright (c) 2020 - 2023 by Apex.AI Inc. All rights reserved.
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
#ifndef IOX_HOOFS_VOCABULARY_EXPECTED_HPP
#define IOX_HOOFS_VOCABULARY_EXPECTED_HPP

#include "iox/attributes.hpp"
#include "iox/detail/expected_helper.hpp"
#include "iox/functional_interface.hpp"
#include "iox/optional.hpp"

#include <utility>

namespace iox
{
/// @brief helper struct to create an expected which is signalling success more easily
/// @param T type which the success helper class should contain
/// @code
///     expected<int, float> callMe() {
///         //...
///         return success<int>(55);
///     }
/// @endcode
template <typename T = void>
struct success
{
    /// @brief constructor which creates a success helper class by copying
    ///         the value of t
    /// @param[in] t value which should be later stored in an expected
    explicit success(const T& t) noexcept;

    /// @brief constructor which creates a success helper class by moving
    ///         the value of t
    /// @param[in] t value which should be later moved into an expected
    explicit success(T&& t) noexcept;
    template <typename... Targs>

    /// @brief constructor which creates a success helper class by forwarding
    ///         arguments to the constructor of T
    /// @param[in] args... arguments which will be perfectly forwarded to the
    ///                     constructor
    explicit success(Targs&&... args) noexcept;

    T value;
};

/// @brief helper struct to handle 'void' value type specialization
/// @code
///     expected<void, float> callMe() {
///         //...
///         return success<>();
///     }
/// @endcode
template <>
struct success<void>
{
    // dummy value
    bool value{true};
};

/// @brief helper struct to create an expected which is signalling an error more easily
/// @param T type which the success helper class should contain
/// @code
///     expected<void, float> callMe() {
///         //...
///         return error<float>(12.34f);
///     }
/// @endcode
template <typename T>
struct error
{
    /// @brief constructor which creates a error helper class by copying
    ///         the value of t
    /// @param[in] t value which should be later stored in an expected
    explicit error(const T& t) noexcept;

    /// @brief constructor which creates a error helper class by moving
    ///         the value of t
    /// @param[in] t value which should be later moved into an expected
    explicit error(T&& t) noexcept;

    /// @brief constructor which creates a error helper class by forwarding
    ///         arguments to the constructor of T
    /// @param[in] args... arguments which will be perfectly forwarded to the
    ///                     constructor
    template <typename... Targs>
    explicit error(Targs&&... args) noexcept;

    T value;
};

/// @brief helper trait for SFINEA to disable specific functions for 'void' value type
/// @tparam ValueType type of the value which can be stored in the expected
template <typename ValueType>
using ExpectedValueTypeNonVoid = typename std::enable_if<!std::is_void<ValueType>::value, ValueType>::type;

/// @brief Implementation of the C++23 expected class which can contain an error or a success value
/// @param ValueType type of the value which can be stored in the expected
/// @param ErrorType type of the error which can be stored in the expected
template <typename ValueType, typename ErrorType>
class IOX_NO_DISCARD expected final : public FunctionalInterface<expected<ValueType, ErrorType>, ValueType, ErrorType>
{
  public:
    /// @brief default ctor is deleted since you have to clearly state if the
    ///         expected contains a success value or an error value
    expected() = delete;

    /// @brief the copy constructor calls the copy constructor of the contained success value
    ///         or the error value - depending on what is stored in the expected
    expected(const expected&) noexcept = default;

    /// @brief the move constructor calls the move constructor of the contained success value
    ///         or the error value - depending on what is stored in the expected
    /// @note The move c'tor does not explicitly invalidate the moved-from object but relies on the move c'tor of
    /// ValueType or ErrorType to correctly invalidate the stored object
    expected(expected&& rhs) noexcept;

    /// @brief Creates an expected which is signaling success and perfectly forwards
    ///        the args to the constructor of ValueType
    /// @tparam Targs is the template parameter pack for the perfectly forwarded arguments
    /// @param[in] in_place_t compile time variable to distinguish between constructors with certain behavior
    /// @param[in] args... arguments which will be forwarded to the 'ValueType' constructor
    template <typename... Targs>
    explicit expected(in_place_t, Targs&&... args) noexcept;

    /// @brief Creates an expected which is signaling an error and perfectly forwards
    ///        the args to the constructor of ErrorType
    /// @tparam Targs is the template parameter pack for the perfectly forwarded arguments
    /// @param[in] unexpect_t compile time variable to distinguish between constructors with certain behavior
    /// @param[in] args... arguments which will be forwarded to the 'ErrorType' constructor
    template <typename... Targs>
    explicit expected(unexpect_t, Targs&&... args) noexcept;

    /// @brief calls the destructor of the success value or error value - depending on what
    ///         is stored in the expected
    ~expected() noexcept = default;

    /// @brief  calls the copy assignment operator of the contained success value
    ///         or the error value - depending on what is stored in the expected
    expected& operator=(const expected&) noexcept;

    /// @brief  calls the move assignment operator of the contained success value
    ///         or the error value - depending on what is stored in the expected
    /// @note The move assignment operator does not explicitly invalidate the moved-from object but relies on the move
    /// assignment operator of ValueType or ErrorType to correctly invalidate the stored object
    expected& operator=(expected&& rhs) noexcept;

    /// @brief  constructs an expected which is signaling success and uses the value
    ///         provided by successValue to copy construct its success value
    /// @param[in] successValue value which will be stored in the expected
    //
    // we would like to use 'return ok(myValue)' with an implicit
    // conversion to return an expected easily
    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    expected(const success<ValueType>& successValue) noexcept;

    /// @brief  constructs an expected which is signaling success and uses the value
    ///         provided by successValue to move construct its success value
    /// @param[in] successValue value which will be moved into the expected
    //
    // we would like to use 'return ok(myValue)' with an implicit
    // conversion to return an expected easily
    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    expected(success<ValueType>&& successValue) noexcept;

    /// @brief  constructs an expected which is signaling an error and stores the
    ///         error value provided by errorValue
    /// @param[in] errorValue error value which will be stored in the expected
    ///
    // we would like to use 'return err(myErrorValue)' with an implicit
    // conversion to return an expected easily
    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    expected(const error<ErrorType>& errorValue) noexcept;

    /// @brief  constructs an expected which is signaling an error and stores the
    ///         error value provided by errorValue
    /// @param[in] errorValue error value which will be moved into the expected
    ///
    // we would like to use 'return err(myErrorValue)' with an implicit
    // conversion to return an expected easily
    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    expected(error<ErrorType>&& errorValue) noexcept;

    /// @brief  creates an expected which is signaling success and perfectly forwards
    ///         the args to the constructor of ValueType
    /// @param[in] args... arguments which will be forwarded to the ValueType constructor
    /// @return expected signalling success
    template <typename... Targs>
    static expected create_value(Targs&&... args) noexcept;

    /// @brief  creates an expected which is signaling an error and perfectly forwards
    ///         the args to the constructor of ErrorType
    /// @param[in] args... arguments which will be forwarded to the ErrorType constructor
    /// @return expected signalling error
    template <typename... Targs>
    static expected create_error(Targs&&... args) noexcept;

    // AXIVION Next Construct AutosarC++19_03-A13.5.3: Implementation is inspired from std::expected
    /// @brief  returns true if the expected does not contain an error otherwise false
    /// @return bool which contains true if the expected contains an error
    explicit operator bool() const noexcept;

    /// @brief  returns true if the expected contains an error otherwise false
    /// @return bool which contains true if the expected contains an error
    bool has_error() const noexcept;

    /// @brief  returns a reference to the contained error value, if the expected
    ///         does not contain an error the error handler is called
    /// @return reference to the internally contained error
    ErrorType& get_error() & noexcept;

    /// @brief  returns a const reference to the contained error value, if the expected
    ///         does not contain an error the error handler is called
    /// @return const reference to the internally contained error
    const ErrorType& get_error() const& noexcept;

    /// @brief  returns a rvalue reference to the contained error value, if the expected
    ///         does not contain an error the error handler is called
    /// @return rvalue reference to the internally contained error
    ErrorType&& get_error() && noexcept;

    /// @brief  returns a reference to the contained success value, if the expected
    ///         does not contain a success value the error handler is called
    /// @tparam U helper temlate parameter for SFINEA
    /// @return reference to the internally contained value
    /// @note this only works for non void ValueTypes
    template <typename U = ValueType>
    ExpectedValueTypeNonVoid<U>& value() & noexcept;

    /// @brief  returns a const reference to the contained success value, if the expected
    ///         does not contain a success value the error handler is called
    /// @tparam U helper temlate parameter for SFINEA
    /// @return const reference to the internally contained value
    /// @note this only works for non void ValueTypes
    template <typename U = ValueType>
    const ExpectedValueTypeNonVoid<U>& value() const& noexcept;

    /// @brief  returns a reference to the contained success value, if the expected
    ///         does not contain a success value the error handler is called
    /// @tparam U helper temlate parameter for SFINEA
    /// @return rvalue reference to the internally contained value
    template <typename U = ValueType>
    ExpectedValueTypeNonVoid<U>&& value() && noexcept;

    /// @brief dereferencing operator which returns a reference to the contained
    ///         success value. if the expected contains an error the error handler is called
    /// @tparam U helper temlate parameter for SFINEA
    /// @return reference to the contained value
    /// @note this only works for non void ValueTypes
    /// @code
    ///     expected<int, float> frodo(ok(45));
    ///     *frodo += 12;
    ///     IOX_LOG(INFO) << *frodo; // prints 57
    /// @endcode
    template <typename U = ValueType>
    ExpectedValueTypeNonVoid<U>& operator*() noexcept;

    /// @brief dereferencing operator which returns a reference to the contained
    ///         success value. if the expected contains an error the error handler is called
    /// @tparam U helper temlate parameter for SFINEA
    /// @return const reference to the contained value
    /// @note this only works for non void ValueTypes
    /// @code
    ///     expected<int, float> frodo(ok(45));
    ///     *frodo += 12;
    ///     IOX_LOG(INFO) << *frodo; // prints 57
    /// @endcode
    template <typename U = ValueType>
    const ExpectedValueTypeNonVoid<U>& operator*() const noexcept;

    /// @brief arrow operator which returns the pointer to the contained success value
    ///         if the expected contains an error the error handler is called
    /// @tparam U helper temlate parameter for SFINEA
    /// @return pointer of type ValueType to the contained value
    /// @note this only works for non void ValueTypes
    /// @code
    ///     expected<std::vector<int>, int> holyPiotr(ok<std::vector<int>>({1,2,3}));
    ///     holyPiotr->push_back(4);
    /// @endcode
    template <typename U = ValueType>
    ExpectedValueTypeNonVoid<U>* operator->() noexcept;

    /// @brief arrow operator which returns the pointer to the contained success value
    ///         if the expected contains an error the the error handler is called
    /// @tparam U helper temlate parameter for SFINEA
    /// @return pointer of type const ValueType to the contained value
    /// @note this only works for non void ValueTypes
    /// @code
    ///     expected<std::vector<int>, int> holyPiotr(ok<std::vector<int>>({1,2,3}));
    ///     holyPiotr->push_back(4);
    /// @endcode
    template <typename U = ValueType>
    const ExpectedValueTypeNonVoid<U>* operator->() const noexcept;

    /// @brief conversion operator to a 'void' value type expected which can be useful
    ///         if you would like to return only the success of a function
    /// @return converts an expected which can contain a value and an error to an
    ///         expected which contains only an error
    /// @code
    ///     expected<int, int> someErrorProneFunction(){}
    ///
    ///     expected<void, int> isItSuccessful() {
    ///         return someErrorProneFunction();
    ///     }
    /// @endcode
    //
    // AXIVION Next Construct AutosarC++19_03-A13.5.2 , AutosarC++19_03-A13.5.3: see doxygen brief section
    // template <typename E>
    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    operator expected<void, ErrorType>() const noexcept;

    /// @brief conversion operator to an optional.
    /// @tparam U helper temlate parameter for SFINEA
    /// @return optional containing the value if the expected contains a value, otherwise a nullopt
    /// @note this only works for non void ValueTypes
    template <typename U = ValueType>
    optional<ExpectedValueTypeNonVoid<U>> to_optional() const noexcept;

    template <typename T, typename E>
    friend constexpr bool ::iox::operator==(const expected<T, E>&, const expected<T, E>&) noexcept;

  private:
    detail::expected_storage<ValueType, ErrorType> m_store;
};

/// @brief equality check for two distinct expected types
/// @tparam ValueType type of the value stored in the expected
/// @tparam ErrorType type of the error stored in the expected
/// @param[in] lhs left side of the comparison
/// @param[in] rhs right side of the comparison
/// @return true if the expecteds are equal, otherwise false
template <typename ValueType, typename ErrorType>
constexpr bool operator==(const expected<ValueType, ErrorType>& lhs,
                          const expected<ValueType, ErrorType>& rhs) noexcept;

/// @brief inequality check for two distinct expected types
/// @tparam ValueType type of the value stored in the expected
/// @tparam ErrorType type of the error stored in the expected
/// @param[in] lhs left side of the comparison
/// @param[in] rhs right side of the comparison
/// @return true if the expecteds are not equal, otherwise false
template <typename ValueType, typename ErrorType>
constexpr bool operator!=(const expected<ValueType, ErrorType>& lhs,
                          const expected<ValueType, ErrorType>& rhs) noexcept;

} // namespace iox

#include "iox/detail/expected.inl"

#endif // IOX_HOOFS_VOCABULARY_EXPECTED_HPP
