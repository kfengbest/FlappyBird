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
Description	:	adapter for Cocos2DX with Naga Lib
History		:	2012, Initial implementation.
*/
#ifndef ___Naga_Adaptors_h__
#define ___Naga_Adaptors_h__

USING_NS_CC;

NAMESPACE_NAGA_BEGIN

    template <typename T>
        class PointerTrait<T, 
                typename STD enable_if<STD is_base_of<cocos2d::Object, T>::value>::type>
        {
        public:
            static INLINE void AddRef(T* pointer) 
            {
                if (pointer) {
                    pointer->retain();
                }
            }

            static INLINE void ReleaseRef(T* pointer) 
            {
                if (pointer) {
                    pointer->release();
                }           
            }
        };

    template <typename T>
        cocos2d::Scene* CreateScene()
        {
            auto scene = cocos2d::Scene::create();

            // 'layer' is an autorelease object
            auto layer = T::create();

            // add layer as a child to scene
            scene->addChild(layer);

            return scene;
        }

    template <typename _Ty>
        struct _CreatePolicy<_Ty, 
            typename std::enable_if<std::is_convertible<_Ty,cocos2d::Object>::value>::type>
        {
            static _Ty * Create() 
            {
                return _Ty::create();
            }
        };

NAMESPACE_NAGA_END

#endif // ___Naga_Adaptors_h__
