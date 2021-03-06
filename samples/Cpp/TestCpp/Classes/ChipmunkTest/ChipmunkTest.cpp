//
// Accelerometer + Chipmunk physics + multi touches example
// a cocos2d example
// http://www.cocos2d-x.org
//

#include "ChipmunkTest.h"


enum {
    kTagParentNode = 1,
};

enum {
    Z_PHYSICS_DEBUG = 100,
};

// callback to remove Shapes from the Space

ChipmunkTestLayer::ChipmunkTestLayer()
{
#if CC_ENABLE_CHIPMUNK_INTEGRATION      
    // enable events
    setTouchEnabled(true);
    setAccelerometerEnabled(true);

    // title
    LabelTTF *label = LabelTTF::create("Multi touch the screen", "Marker Felt", 36);
    label->setPosition(Point( VisibleRect::center().x, VisibleRect::top().y - 30));
    this->addChild(label, -1);

    // reset button
    createResetButton();

    // init physics
    initPhysics();

#if 1
    // Use batch node. Faster
    SpriteBatchNode *parent = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 100);
    _spriteTexture = parent->getTexture();
#else
    // doesn't use batch node. Slower
    _spriteTexture = TextureCache::getInstance()->addImage("Images/grossini_dance_atlas.png");
    Node *parent = Node::create();
#endif
    addChild(parent, 0, kTagParentNode);

    addNewSpriteAtPosition(Point(200,200));

    // menu for debug layer
    MenuItemFont::setFontSize(18);
    MenuItemFont *item = MenuItemFont::create("Toggle debug", CC_CALLBACK_1(ChipmunkTestLayer::toggleDebugCallback, this));

    Menu *menu = Menu::create(item, NULL);
    this->addChild(menu);
    menu->setPosition(Point(VisibleRect::right().x-100, VisibleRect::top().y-60));

    scheduleUpdate();
#else
    LabelTTF *pLabel = LabelTTF::create("Should define CC_ENABLE_CHIPMUNK_INTEGRATION=1\n to run this test case",
                                            "Arial",
                                            18);
    Size size = Director::getInstance()->getWinSize();
    pLabel->setPosition(Point(size.width/2, size.height/2));
    
    addChild(pLabel);
    
#endif
    
}

void ChipmunkTestLayer::toggleDebugCallback(Object* pSender)
{
#if CC_ENABLE_CHIPMUNK_INTEGRATION
    _debugLayer->setVisible(! _debugLayer->isVisible());
#endif
}

ChipmunkTestLayer::~ChipmunkTestLayer()
{
    // manually Free rogue shapes
    for( int i=0;i<4;i++) {
        cpShapeFree( _walls[i] );
    }

    cpSpaceFree( _space );

}

void ChipmunkTestLayer::initPhysics()
{
#if CC_ENABLE_CHIPMUNK_INTEGRATION    
    // init chipmunk
    //cpInitChipmunk();

    _space = cpSpaceNew();

    _space->gravity = cpv(0, -100);

    //
    // rogue shapes
    // We have to free them manually
    //
    // bottom
    _walls[0] = cpSegmentShapeNew( _space->staticBody,
        cpv(VisibleRect::leftBottom().x,VisibleRect::leftBottom().y),
        cpv(VisibleRect::rightBottom().x, VisibleRect::rightBottom().y), 0.0f);

    // top
    _walls[1] = cpSegmentShapeNew( _space->staticBody, 
        cpv(VisibleRect::leftTop().x, VisibleRect::leftTop().y),
        cpv(VisibleRect::rightTop().x, VisibleRect::rightTop().y), 0.0f);

    // left
    _walls[2] = cpSegmentShapeNew( _space->staticBody,
        cpv(VisibleRect::leftBottom().x,VisibleRect::leftBottom().y),
        cpv(VisibleRect::leftTop().x,VisibleRect::leftTop().y), 0.0f);

    // right
    _walls[3] = cpSegmentShapeNew( _space->staticBody, 
        cpv(VisibleRect::rightBottom().x, VisibleRect::rightBottom().y),
        cpv(VisibleRect::rightTop().x, VisibleRect::rightTop().y), 0.0f);

    for( int i=0;i<4;i++) {
        _walls[i]->e = 1.0f;
        _walls[i]->u = 1.0f;
        cpSpaceAddStaticShape(_space, _walls[i] );
    }

    // Physics debug layer
    _debugLayer = PhysicsDebugNode::create(_space);
    this->addChild(_debugLayer, Z_PHYSICS_DEBUG);
#endif
}

void ChipmunkTestLayer::update(float delta)
{
    // Should use a fixed size step based on the animation interval.
    int steps = 2;
    float dt = Director::getInstance()->getAnimationInterval()/(float)steps;

    for(int i=0; i<steps; i++){
        cpSpaceStep(_space, dt);
    }
}

void ChipmunkTestLayer::createResetButton()
{
    MenuItemImage *reset = MenuItemImage::create("Images/r1.png", "Images/r2.png", CC_CALLBACK_1(ChipmunkTestLayer::reset, this));

    Menu *menu = Menu::create(reset, NULL);

    menu->setPosition(Point(VisibleRect::center().x, VisibleRect::bottom().y + 30));
    this->addChild(menu, -1);
}

void ChipmunkTestLayer::reset(Object* sender)
{
    Scene* s = new ChipmunkAccelTouchTestScene();
    ChipmunkTestLayer* child = new ChipmunkTestLayer();
    s->addChild(child);
    child->release();
    Director::getInstance()->replaceScene(s);
    s->release();
}

void ChipmunkTestLayer::addNewSpriteAtPosition(Point pos)
{
#if CC_ENABLE_CHIPMUNK_INTEGRATION    
    int posx, posy;

    Node *parent = getChildByTag(kTagParentNode);

    posx = CCRANDOM_0_1() * 200.0f;
    posy = CCRANDOM_0_1() * 200.0f;

    posx = (posx % 4) * 85;
    posy = (posy % 3) * 121;


    int num = 4;
    cpVect verts[] = {
        cpv(-24,-54),
        cpv(-24, 54),
        cpv( 24, 54),
        cpv( 24,-54),
    };

    cpBody *body = cpBodyNew(1.0f, cpMomentForPoly(1.0f, num, verts, cpvzero));

    body->p = cpv(pos.x, pos.y);
    cpSpaceAddBody(_space, body);

    cpShape* shape = cpPolyShapeNew(body, num, verts, cpvzero);
    shape->e = 0.5f; shape->u = 0.5f;
    cpSpaceAddShape(_space, shape);

    PhysicsSprite *sprite = PhysicsSprite::createWithTexture(_spriteTexture, Rect(posx, posy, 85, 121));
    parent->addChild(sprite);

    sprite->setCPBody(body);
    sprite->setPosition(pos);
#endif
}

void ChipmunkTestLayer::onEnter()
{
    Layer::onEnter();
}

void ChipmunkTestLayer::ccTouchesEnded(Set* touches, Event* event)
{
    //Add a new body/atlas sprite at the touched location

    for( auto &item: *touches)
    {
        Touch* touch = static_cast<Touch*>(item);

        Point location = touch->getLocation();

        addNewSpriteAtPosition( location );
    }
}

void ChipmunkTestLayer::didAccelerate(Acceleration* pAccelerationValue)
{
    static float prevX=0, prevY=0;

#define kFilterFactor 0.05f

    float accelX = (float) pAccelerationValue->x * kFilterFactor + (1- kFilterFactor)*prevX;
    float accelY = (float) pAccelerationValue->y * kFilterFactor + (1- kFilterFactor)*prevY;

    prevX = accelX;
    prevY = accelY;

    Point v = Point( accelX, accelY);
    v = v * 200;
    _space->gravity = cpv(v.x, v.y);
}

void ChipmunkAccelTouchTestScene::runThisTest()
{
    Layer* pLayer = new ChipmunkTestLayer();
    addChild(pLayer);
    pLayer->release();

    Director::getInstance()->replaceScene(this);
}

