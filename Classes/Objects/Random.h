/*
Copyright 2012 NAGA.  All Rights Reserved.

The source code contained or described herein and all documents related
to the source code ("Material") are owned by NAGA or its suppliers or 
licensors.  Title to the Material remains with NAGA or its suppliers and 
licensors.  The Material is protected by worldwide copyright laws and 
treaty provisions.  No part of the Material may be used, copied, reproduced, 
modified, published, uploaded, posted, transmitted, distributed, or 
disclosed in any way without NAGA's prior express written permission.

No license under any patent, copyright, trade secret or other
intellectual property right is granted to or conferred upon you by
disclosure or delivery of the Materials, either expressly, by
implication, inducement, estoppel or otherwise.  Any license under such
intellectual property rights must be express and approved by NAGA in
writing.
*/

/*
Author		:	Yu Li
Description	:	Randomizers for various of types 
History		:	2014, Initial implementation.
*/
#ifndef __KOGO_Random_H__
#define __KOGO_Random_H__

#include <utility>
#include <random>
#include "cocos2d.h"

namespace detail 
{
    template <typename _Ty, typename = void>
    struct random_traits{ };

    template <typename _Ty> 
    struct random_traits<_Ty, 
        typename std::enable_if<std::is_integral<_Ty>::value>::type>
    {
        typedef std::mt19937 engine_type;
        typedef std::uniform_int_distribution<_Ty> distribution_type;
        typedef typename distribution_type::result_type result_type;

        static inline /*const_expr*/ result_type Min() { return 0; }
        static inline /*const_expr*/ result_type Max() { return 9; }
    };

    template <typename _Ty> 
    struct random_traits<_Ty, 
        typename std::enable_if<std::is_floating_point<_Ty>::value>::type>
    {
        typedef std::mt19937 engine_type;
        typedef std::uniform_real_distribution<_Ty> distribution_type;
        typedef typename distribution_type::result_type result_type;

        static inline /*const_expr*/ result_type Min() { return 0.0f; }
        static inline /*const_expr*/ result_type Max() { return 1.0f; }
    };
};

template <typename _Ty>
    class randomizer 
    {       
        typedef detail::random_traits<_Ty> Traits;
        typedef typename Traits::result_type result_type;
        typedef typename Traits::distribution_type distribution_type;
        typedef typename Traits::engine_type engine_type;

        engine_type engine;
        distribution_type dist;
    public:
        randomizer(_Ty m=Traits::Min(), _Ty n=Traits::Max()) 
            : engine(std::random_device()()), dist(m,n){}

        inline result_type operator() () {
            return dist(engine);
        }
    };

template <>
    class randomizer<cocos2d::Color4F>
    {        
        typedef cocos2d::Color4F result_type;

        typedef detail::random_traits<int> Traits;
        typedef Traits::distribution_type distribution_type;
        typedef Traits::engine_type engine_type;

        engine_type engine;
        distribution_type dist;

    public:
        randomizer() 
            : engine(std::random_device()()), dist(0,256){}

        inline result_type operator() () 
        {
            const int minSum = 450;
            const int minDelta = 150;
            int r, g, b, min, max;            
            while (true) 
            {
                r = dist(engine);
                g = dist(engine);
                b = dist(engine);
                min = std::min(std::min(r, g), b);
                max = std::max(std::max(r, g), b);
                if (max-min < minDelta) 
                    continue;
                if (r+g+b < minSum) 
                    continue;
                break;
            }

            return result_type(r/255.f, g/255.f, b/255.f, 1.f);
        }
    };

#endif // __KOGO_Random_H__
