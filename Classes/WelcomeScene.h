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
 Author         :	Kaven Feng
 Description	:	Welcome Scene
 History		:	2013, Initial implementation.
 */
#ifndef __KOGO_Studio_WelcomeScene__
#define __KOGO_Studio_WelcomeScene__

/// <description>
/// welcome page 
/// </description>
class WelcomeScene : public cocos2d::Layer
{
public:
	CREATE_FUNC(WelcomeScene);
    
protected:
    virtual bool init() override;
    void start(cocos2d::Object* pSender);
    // virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);    
};

#endif // __KOGO_Studio_WelcomeScene__

