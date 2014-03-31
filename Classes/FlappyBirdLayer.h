#include "cocos2d.h"
using namespace std;

/// <description>
/// flappy bird game layer
/// </description>
class FlappyBirdLayer :public cocos2d::Layer 
{
public:
    CREATE_FUNC(FlappyBirdLayer);
	static cocos2d::Scene* createScene();

public:
    /// init the layer
    virtual bool init() override;	
    /// timely update 
	virtual void update(float time) override;
    /// when the node is added into 
    virtual void onEnter() override;

    /// event reactions
	virtual void onTouchesEnded(const vector<Touch*>& touches, Event* event) override;
	virtual void onTouchesBegan(const vector<Touch*>& touches, Event* event) override;
    virtual bool onContactBegin(PhysicsContact& contact);

protected:
    /// <description>
    /// default constructors
    /// </description>
    FlappyBirdLayer();
    
    /// <description>
    /// set the score on the screen
    /// </description>
    void setScore(int s);
    
    /// <description>
    /// initialization works for each game begins
    /// </description>
    void gameStart();
    
    /// <description>
    /// the obstacles begin to emerge after tm seconds
    /// </description>
    void obstacleStart(float tm);
    
    /// <description>
    /// game restarts
    /// </description>
    void gameRestart();
    
    /// <description>
    /// clean up works for each game
    /// </description>
    void gameOver();
    
    /// <description>
    /// add one obstacle into the scene
    /// </description>
    void addObstacle(float tm);

    /// <description>
    /// Get the Hero bird
    /// </description>
    Sprite* Hero();

    /// <description>
    /// Get the terrain
    /// </description>
    Terrain* getTerrain();

    /// <description>
    /// Get the PhysicsWorld
    /// </description>
    PhysicsWorld* getPhysicsWorld();

private:
    PhysicsWorld* mpPhysicsWorld;
    Node*   obstacle;
	bool    isFlying;
	float   velocity;
	int     status;
	int     score;
	float   gravity;
};
