#include "Impl.h"
#include "FlappyBirdLayer.h"

USING_NS_CC;
using namespace std;

FlappyBirdLayer::FlappyBirdLayer()
    : obstacle(nullptr)
    , mpPhysicsWorld(nullptr)
    , isFlying(false)
    , velocity(-2)
    , status(GAME_STATUS_RESTART)
    , score(0)
    , gravity(0.2f)
{
}

Scene* FlappyBirdLayer::createScene()
{
	auto scene = Scene::createWithPhysics();
    auto world = scene->getPhysicsWorld();
    if (world)
    {
        world->setGravity(Vect(0,-700));
#ifdef _DEBUG
        world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#endif 
    }

    auto layer = FlappyBirdLayer::create();
	scene->addChild(layer);
	return scene;
}

bool FlappyBirdLayer::init()
{
	if (!Layer::init())
		return false;
    
     return true;
}
 
void FlappyBirdLayer::onEnter() 
{
    Layer::onEnter();
    auto size = Director::getInstance()->getVisibleSize();

    // create background 
    auto bkg = Sprite::create(bird_bg);
    this->addChild(bkg,DEPTH_SKY, TAG_SKY);
    bkg->setPosition(size.width/2, size.height/2);
       
    // ground object
    {
        auto ground = Sprite::create(bird_ground);
        Size cSize = ground->getContentSize();
        ground->setPosition(size.width/2,cSize.height/2);

        auto body = PhysicsBody::createBox(cSize);        
        body->setDynamic(false);        
        ground->setPhysicsBody(body);
        body->setCollisionBitmask(1);
        this->addChild(ground, DEPTH_FOREGROUND, TAG_GROUND);
    }
   
    // Game Over logo
    {
        auto overLogo = Sprite::create(bird_gameover);
        overLogo->setPosition(Point(size.width / 2, size.height / 2 + overLogo->getContentSize().height));
        overLogo->setVisible(false);
        this->addChild(overLogo, DEPTH_UI, TAG_GAMEOVER);
    }
    
    // score
    auto score = LabelBMFont::create("0", score_font);
    score->setPosition(Point(size.width / 2, size.height / 4 * 3));
    addChild(score, DEPTH_UI, TAG_SCORE);
    score->setVisible(false);
    
    // Obstacles
    obstacle = Node::create();
    this->addChild(obstacle, DEPTH_GAME_LAYER, TAG_OBSTACLE);

    // create the terrain the left 
    {
        auto terrain = Terrain::create();
        //this->addChild(terrain, DEPTH_BACKGROUND, TAG_TERRAIN);
    }

    // touch
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    // for touch events 
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesEnded = CC_CALLBACK_2(FlappyBirdLayer::onTouchesEnded, this);
    listener->onTouchesBegan = CC_CALLBACK_2(FlappyBirdLayer::onTouchesBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    // for physic events
    auto contact = EventListenerPhysicsContact::create();
    contact->onContactBegin = CC_CALLBACK_1(FlappyBirdLayer::onContactBegin,this);
    dispatcher->addEventListenerWithSceneGraphPriority(contact, this);

    // init status
    status = GAME_STATUS_RESTART;
}

/// <description>
/// Get the Hero bird
/// </description>
Sprite* FlappyBirdLayer::Hero()
{
    auto hero = (Sprite*)this->getChildByTag(TAG_HERO);
    if (hero == nullptr)
    {
        auto size = Director::getInstance()->getVisibleSize();
        hero = Sprite::create(bird_hero);
        hero->setPosition(size.width / 3, size.height*0.8);
        hero->setRotation(0);
        hero->setVisible(true);

        Animation* an = Animation::create();
        an->addSpriteFrameWithFile(bird_hero);
        an->addSpriteFrameWithFile(bird_hero2);
        an->addSpriteFrameWithFile(bird_hero3);
        an->setDelayPerUnit(0.5f / 3.0f);
        Animate* anim = Animate::create(an);
        hero->runAction(RepeatForever::create(anim));

        auto body = PhysicsBody::createBox(hero->getContentSize());
        body->setDynamic(true);
        body->setCollisionBitmask(1);
        hero->setPhysicsBody(body);
        this->addChild(hero, DEPTH_GAME_LAYER, TAG_HERO);
    }

    return hero;
}

/// <description>
/// get the terrain object
/// </description>
Terrain* FlappyBirdLayer::getTerrain()
{
    return (Terrain*)this->getChildByTag(TAG_TERRAIN);
}

/// <description>
/// Get the PhysicsWorld
/// </description>
PhysicsWorld* FlappyBirdLayer::getPhysicsWorld()
{
    if (!mpPhysicsWorld) {
        mpPhysicsWorld = this->getScene()->getPhysicsWorld();
    }
    return mpPhysicsWorld;
}

/// <description>
/// update the render world 
/// </description>
void FlappyBirdLayer::update(float time)
{
    /// update the Box2D world 
    Layer::update(time);

	auto size = Director::getInstance()->getVisibleSize();
    //cocos2d::log("time=%f", time);    
	switch (status)
	{
	case GAME_STATUS_PLAYING:
        {
            float x = Hero()->getPositionX();
            int heroX  = x - Hero()->getContentSize().width;
            Rect rHero = ((Sprite*)Hero())->getBoundingBox();

            /// update the score and obstacle positions
            auto obs = obstacle->getChildren();
            std::for_each(obs.rbegin(),obs.rend(), [&](Node* n)
            {
                Sprite* s = (Sprite*)n;
                
                int oPosX = s->getPositionX();
                int oNextX= oPosX - 3;
                s->setPositionX(oNextX);
                if (oPosX >= heroX && oNextX<= heroX)
                {
                    score++;    /// score counts for both up & down
                }
                
                // remove the obstacle if it is completely out of screen
                if (oNextX < - s->getContentSize().width / 2)
                {                                        
                    obstacle->removeChild(s);
                }
            });
            
            getTerrain()->setOffsetX(-x);
            setScore(score/2);
            break;
        }

	case GAME_STATUS_GAME_OVER:
        {
            // do nothing here
            break;
        }

	case GAME_STATUS_RESTART:
        {
            // do nothing here
            break;
        }
	}
}

/// <description>
/// add one obstacle into the scene
/// </description>
void FlappyBirdLayer::addObstacle(float tm)
{
    Size size = Director::getInstance()->getWinSize();
    
	auto sprite = Sprite::create(bird_obstacle_up);
	Size spriteSize = sprite->getContentSize();	
    auto body = PhysicsBody::createBox(spriteSize);
    body->setDynamic(false);
    //body->setVelocity(Vect(-3,0));
    sprite->setPhysicsBody(body);
    body->setCollisionBitmask(1);
    //obstacle->setLocalZOrder(DEPTH_GAME_LAYER);
    obstacle->addChild(sprite);
    
	auto sprite2 = Sprite::create(bird_obstacle_down);
	Size spriteSize2 = sprite->getContentSize();    
    body = PhysicsBody::createBox(spriteSize2);
    body->setDynamic(false);
    //body->setVelocity(Vect(-3,0));
    body->setCollisionBitmask(1);
    sprite2->setPhysicsBody(body);
    //obstacle->setLocalZOrder(DEPTH_GAME_LAYER);
    obstacle->addChild(sprite2);
    
    int offsetY = spriteSize.height / 4;
	int maxUpY = size.height + offsetY;
    int minUpY = size.height - offsetY;
	int y1 = CCRANDOM_0_1()*(maxUpY - minUpY) + minUpY;
    
    /// the gap is the distance between up obstacle and down obstacle
    int gap= spriteSize.height/3;
    int y2 = y1 - spriteSize.height/2 - gap - spriteSize2.height/2;
    float offsetX = 2.f;
	sprite->setPosition(Point(size.width + spriteSize.width/2 + offsetX, y1));
	sprite2->setPosition(Point(size.width + spriteSize2.width/2 + offsetX, y2));
}

/// <description>
/// set the score on the screen
/// </description>
void FlappyBirdLayer::setScore(int s)
{
    auto scoreSprite = (LabelBMFont*)this->getChildByTag(TAG_SCORE);
    char szScore[8] = {0};
    snprintf(szScore,sizeof(szScore)/sizeof(szScore[0])-1,"%d",s);
    scoreSprite->setString(szScore);
}

/// <description>
/// initialization works for each game begins
/// </description>
void FlappyBirdLayer::gameStart()
{
    this->getChildByTag(TAG_SCORE)->setVisible(true);
    Hero()->setVisible(true);
    Hero()->getPhysicsBody()->setDynamic(true);
    Hero()->setRotation(0.f);
    status = GAME_STATUS_PLAYING;
    
    scheduleOnce(schedule_selector(FlappyBirdLayer::obstacleStart),2.0);
    // reschedule update
    scheduleUpdate();
    
    isFlying = false;
    score    = 0;
    setScore(score);
    velocity = -3;
}

/// <description>
/// the obstacles begin to emerge after tm seconds
/// </description>
void FlappyBirdLayer::obstacleStart(float tm)
{
    this->schedule(schedule_selector(FlappyBirdLayer::addObstacle),1.0f);
}

/// <description>
/// game restarts
/// </description>
void FlappyBirdLayer::gameRestart()
{
    // set the status to restart
    status = GAME_STATUS_RESTART;
    
    // remove all obstacles
    auto obs = obstacle->getChildren();
    std::for_each(obs.rbegin(),obs.rend(), [&](Node* n) {
        obstacle->removeChild(n);
    });
    //obstacle->removeAllChildren();
        
    Size size = Director::getInstance()->getVisibleSize();
    this->removeChild(Hero());
   
    // show logo
    this->getChildByTag(TAG_GAMEOVER)->setVisible(false);
    // hide score
    this->getChildByTag(TAG_SCORE)->setVisible(false);
}

/// <description>
/// clean up works for each game
/// </description>
void FlappyBirdLayer::gameOver()
{
    /// set the status to GameOver
    status = GAME_STATUS_GAME_OVER;
    /// stop the timer for adding obstacles
    this->unschedule(schedule_selector(FlappyBirdLayer::addObstacle));
    /// stop the timer for update
    this->unscheduleUpdate();
    
    setScore(score/2);
    /// show the gameover flag
    this->getChildByTag(TAG_GAMEOVER)->setVisible(true);
}

void FlappyBirdLayer::onTouchesBegan(const vector<Touch*>& touches, Event* event)
{
    if (status == GAME_STATUS_RESTART)
    {
        gameStart();
    }
	else if (status == GAME_STATUS_PLAYING)
	{
		isFlying = true;
        Hero()->getPhysicsBody()->setVelocity(Vect(0,300));
	}
	else if (status == GAME_STATUS_GAME_OVER)
	{
        gameRestart();
	}
}

void FlappyBirdLayer::onTouchesEnded(const vector<Touch*>& touches, Event* event)
{
	if (status == GAME_STATUS_PLAYING)
	{
		isFlying = false;
	}
}

bool FlappyBirdLayer::onContactBegin(PhysicsContact& contact)
{
   Node* pNodeA = contact.getShapeA()->getBody()->getNode();
   Node* pNodeB = contact.getShapeB()->getBody()->getNode();
   if (pNodeA == Hero() || pNodeB == Hero())
   {
       gameOver();
   }

   return true;
}
