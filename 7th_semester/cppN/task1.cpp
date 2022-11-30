template <char... c>
struct Tstring {
    static constexpr char value[sizeof...(c) + 1] = { c..., '\0' };
    static constexpr int size = sizeof...(c);

    template <char... T>
    constexpr bool operator=(const Tstring<T...>& u) const {
        return true;
    }

};

template <char... c, char... T>
constexpr Tstring<c..., T...> operator+(const Tstring<c...> K, const Tstring<T...> U) {
    return Tstring<c..., T...>();
}

template<char... c>
constexpr const char Tstring<c...>::value[sizeof...(c) + 1];

template <typename T, T... c>
constexpr Tstring<c...> operator""_s() { 
    return Tstring<c...>();
}

int main() {
    constexpr auto hello = "hello"_s + " world"_s;
    static_assert(hello = "hello world"_s);
    return 0;
}