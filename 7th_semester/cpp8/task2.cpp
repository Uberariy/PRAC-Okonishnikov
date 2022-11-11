#include <iostream>
#include <memory>

template<typename T1, typename...>
struct are_same;

template<typename T1, typename T2>
struct are_same<T1, T2> : std::integral_constant<bool, std::is_same<std::decay_t<T1>, std::decay_t<T2>>::value> {};

template<typename T1, typename T2, typename... Targs>
struct are_same<T1, T2, Targs...> : std::integral_constant<bool, std::is_same<std::decay_t<T1>, std::decay_t<T2>>::value && 
                                                           are_same<T1, Targs...>::value> {};

template<typename... Targs>
constexpr bool are_same_v = are_same<Targs...>::value;

int main() {
    static_assert(are_same_v<int, int32_t, signed int>, "compile assert");
    return 0;
}