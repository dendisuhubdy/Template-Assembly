#pragma once

/**
    Bytes of byte data.
*/
template <char... chars>
struct ByteString {
    static constexpr size_t size = sizeof...(chars);
    static const char data[sizeof...(chars)];
};

template <char... chars>
const char ByteString<chars...>::data[] = { chars... };

/**
    Interface for a type that can be converted to a byte string.
*/
template <typename>
struct ToBytes;

template <char... characters>
struct ToBytes<ByteString<characters...>> {
    using type = ByteString<characters...>;
};

template <typename x>
using to_bytes = typename ToBytes<x>::type;


/**
*/
template <typename l, typename r>
struct BytesAdd;

template <char... ls, char... rs>
struct BytesAdd<ByteString<ls...>, ByteString<rs...>> {
    using type = ByteString<ls..., rs...>;
};

template <typename l, typename r>
using string_add = typename BytesAdd<to_bytes<l>, to_bytes<r>>::type;

/**
*/
template <typename...>
struct BytesJoin {
    using type = ByteString<>;
};

template <typename x, typename... xs>
struct BytesJoin<x, xs...> {
    using type = string_add<x, typename BytesJoin<xs...>::type>;
};

template <typename... args>
using string_join = typename BytesJoin<args...>::type;


/**
    Generate a byte string of `c` repeated `s` times.
*/
template <size_t s, char c>
struct BytesGen {
    using type = string_add<ByteString<c>, typename BytesGen<s - 1, c>::type>;
};

template <char c>
struct BytesGen<0, c> {
    using type = ByteString<>;
};

template <size_t s, char c>
using bytes_gen = typename BytesGen<s, c>::type;

/**
    Left pad a byte string with zeros.
*/
template <size_t size, typename s>
using bytes_pack = string_add<bytes_gen<size - s::size, '\0'>, s>;





template <size_t count, unsigned x>
struct IntToBytes {
    using type = string_add<
        ByteString<static_cast<char>(x & 0xff)>,
        typename IntToBytes<count - 1, (x >> 8)>::type>;
};

template <unsigned x>
struct IntToBytes<0, x> {
    using type = ByteString<>;
};

template <size_t count, typename x>
using to_string = typename IntToBytes<count, x::value>::type;

