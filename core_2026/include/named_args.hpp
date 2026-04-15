// 具名参数 named_args.hpp
// 通过 operator= 实现具名参数传递，支持默认值和参数检查
// 适用于 C++17 及以上
#pragma once

#include <type_traits>
#include <utility>

namespace named_args {

// ===================== Arg 定义 =====================

template<typename Tag, typename T>
struct Arg {
    using tag = Tag;
    T value;
};

template<typename T, typename = void>
struct is_arg : std::false_type {};

template<typename T>
struct is_arg<T, std::void_t<typename std::decay_t<T>::tag>> : std::true_type {};

template<typename T>
inline constexpr bool is_arg_v = is_arg<T>::value;

// ===================== 重载 operator= 以支持赋值传参 =====================

template<typename Tag>
struct ArgProxy {
    template<typename T>
    constexpr Arg<Tag, std::decay_t<T>> operator=(T&& v) const { // std::decay_t 将数组、函数、const退化为指针
        return {std::forward<T>(v)}; // 左值拷贝，右值move
    }
};

// ===================== 获取参数 =====================

// 查找 Tag 对应的值，否则返回默认值
template<typename Tag, typename Default, typename... Args>
constexpr auto get(Default&& def, Args&&... args) {
    using Ret = std::decay_t<Default>;
    Ret result = std::forward<Default>(def);

    ([&] {
        using ArgT = std::decay_t<Args>;
        if constexpr (is_arg_v<ArgT> && std::is_same_v<typename ArgT::tag, Tag>) {
            result = static_cast<Ret>(args.value);
        }
    }(), ...);

    return result;
}

// ===================== 检查参数是否存在 =====================

template<typename Tag, typename... Args>
constexpr bool has() {
    return ((is_arg_v<std::decay_t<Args>> && std::is_same_v<typename std::decay_t<Args>::tag, Tag>) || ...);
}

// ===================== 可选：强制参数存在 =====================

template<typename Tag, typename... Args>
constexpr auto require(Args&&... args) {
    static_assert(has<Tag, Args...>(), "Required named argument missing");
    
    // 从参数包中找到Tag对应的值
    auto result = get<Tag>(int{}, std::forward<Args>(args)...);
    return result;
}

} // namespace named_args
