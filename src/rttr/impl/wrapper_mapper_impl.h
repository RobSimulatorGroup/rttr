/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014, 2015 Axel Menzel <info@axelmenzel.de>                       *
*                                                                                   *
*   This file is part of RTTR (Run Time Type Reflection)                            *
*   License: MIT License                                                            *
*                                                                                   *
*   Permission is hereby granted, free of charge, to any person obtaining           *
*   a copy of this software and associated documentation files (the "Software"),    *
*   to deal in the Software without restriction, including without limitation       *
*   the rights to use, copy, modify, merge, publish, distribute, sublicense,        *
*   and/or sell copies of the Software, and to permit persons to whom the           *
*   Software is furnished to do so, subject to the following conditions:            *
*                                                                                   *
*   The above copyright notice and this permission notice shall be included in      *
*   all copies or substantial portions of the Software.                             *
*                                                                                   *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
*   SOFTWARE.                                                                       *
*                                                                                   *
*************************************************************************************/

#ifndef RTTR_WRAPPER_MAPPER_IMPL_H_
#define RTTR_WRAPPER_MAPPER_IMPL_H_

#include "rttr/base/core_prerequisites.h"
#include "rttr/detail/misc/std_type_traits.h"
#include "rttr/detail/misc/utility.h"
#include <type_traits>

#include <memory>

namespace rttr
{

class type;

//////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct wrapper_mapper<std::shared_ptr<T>>
{
    typedef decltype(std::shared_ptr<T>().get()) wrapped_type;
    typedef std::shared_ptr<T> type;

    RTTR_INLINE static wrapped_type get(const type& obj)
    {
        return obj.get();
    }

    RTTR_INLINE static type create(const wrapped_type& t)
    {
        return type(t);
    }
};

//////////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct wrapper_mapper<std::reference_wrapper<T>>
{
    typedef decltype(std::declval<std::reference_wrapper<T>>().get()) wrapped_type;
    typedef std::reference_wrapper<T> type;

    RTTR_INLINE static wrapped_type get(const type& obj)
    {
        return obj.get();
    }

    RTTR_INLINE static type create(const wrapped_type& t)
    {
        return type(t);
    }
};

namespace detail
{

//////////////////////////////////////////////////////////////////////////////////////

template<typename T>
using wrapper_mapper_t = typename wrapper_mapper<typename remove_cv<typename std::remove_reference<T>::type
                                                                   >::type>::wrapped_type;

//////////////////////////////////////////////////////////////////////////////////////

template<typename T>
using is_wrapper = std::integral_constant<bool, !std::is_same<invalid_wrapper_type, 
                                                              wrapper_mapper_t< T >>::value>;

//////////////////////////////////////////////////////////////////////////////////////

template<typename T>
using wrapper_adress_return_type_t = typename raw_addressof_return_type< wrapper_mapper_t< T > >::type;

//////////////////////////////////////////////////////////////////////////////////////

template<typename T>
typename std::enable_if<is_wrapper<T>::value, wrapper_adress_return_type_t<T>>::type wrapped_raw_addressof(T& obj)
{
    using raw_wrapper_type = typename remove_cv<typename std::remove_reference<T>::type>::type;
    wrapper_mapper_t<T> value = wrapper_mapper<raw_wrapper_type>::get(obj);
    return raw_addressof(value);
}

//////////////////////////////////////////////////////////////////////////////////////

template<typename T>
typename std::enable_if<!is_wrapper<T>::value, T*>::type wrapped_raw_addressof(T& obj)
{
   return std::addressof(obj);
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

} // end namespace detail
} // end namespace rttr

#endif // RTTR_WRAPPER_MAPPER_IMPL_H_