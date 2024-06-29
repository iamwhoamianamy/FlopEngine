#pragma once

namespace flp::traits
{

template<typename T> struct remove_all            { using type = T;                    };
template<typename T> struct remove_all<T const>   { using type = remove_all<T>::type;  };
template<typename T> struct remove_all<T*>        { using type = remove_all<T>::type;  };
template<typename T> struct remove_all<T const*>  { using type = remove_all<T*>::type; };
template<typename T> struct remove_all<T* const>  { using type = remove_all<T*>::type; };
template<typename T> struct remove_all<T&>        { using type = remove_all<T>::type;  };
template<typename T> struct remove_all<T const&>  { using type = remove_all<T>::type;  };
template<typename T> struct remove_all<T&&>       { using type = remove_all<T>::type;  };
template<typename T> struct remove_all<T const&&> { using type = remove_all<T>::type;  };

template<typename T>
using remove_all_t = remove_all<T>::type;

} // namespace flp::traits
