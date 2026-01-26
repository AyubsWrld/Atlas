#pragma once
#include<type_traits> 

template<typename T> 
requires std::is_enum_v<T>
constexpr T operator|(T EnumeratorA, T EnumeratorB) noexcept
{
    using   TUnderlying     = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<TUnderlying>(EnumeratorA) | static_cast<TUnderlying>(EnumeratorB));
}

template<typename T> 
requires std::is_enum_v<T>
constexpr T operator&(T a, T b) noexcept {
    using U = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<U>(a) & static_cast<U>(b));
}

template<typename T> 
requires std::is_enum_v<T>
constexpr T operator^(T a, T b) noexcept {
    using U = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<U>(a) ^ static_cast<U>(b));
}

template<typename T> 
requires std::is_enum_v<T>
constexpr T operator~(T a) noexcept {
    using U = std::underlying_type_t<T>;
    return static_cast<T>(~static_cast<U>(a));
}

template<typename T> 
requires std::is_enum_v<T>
constexpr T& operator|=(T& a, T b) noexcept {
    return a = a | b;
}

template<typename T> 
requires std::is_enum_v<T>
constexpr T& operator&=(T& a, T b) noexcept {
    return a = a & b;
}
