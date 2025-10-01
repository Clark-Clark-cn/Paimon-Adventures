#pragma once

#include "Animation.h"
#include "player.h"
#include "bullet.h"
#include <string>

extern bool debug;
extern IMAGE img_shadow_enemy;
extern Atlas atlas_boar_left;
extern Atlas atlas_boar_right;
extern Atlas atlas_snail_left;
extern Atlas atlas_snail_right;
extern Atlas atlas_bee_left;
extern Atlas atlas_bee_right;

extern Player* player;
extern std::vector<std::shared_ptr<Bullet>> bullets_custom;
extern int score;

static const float beeSpeed = config.get("enemy.bee.speed").asFloat();
static const int beeHp = config.get("enemy.bee.hp").asInt();
static const float boarSpeed = config.get("enemy.boar.speed").asFloat();
static const int boarHp = config.get("enemy.boar.hp").asInt();
static const float snailSpeed = config.get("enemy.snail.speed").asFloat();
static const int snailHp = config.get("enemy.snail.hp").asInt();

class Enemy{
    Vector2 position;
    Vector2 velocity{0,0};
    Vector2 size;
    bool isFacingRight=false;
    Animation leftAnim;
    Animation rightAnim;
    Animation* currentAnim=&leftAnim;
    enum SpawnSide{
        LEFT,
        RIGHT,
        TOP,
        BOTTOM
	};
	int spawnSide = rand()%4;
    enum Type{
        BOAR=0,
        SNAIL=1,
        BEE
    };
    int type=rand()%2==0?(rand()%2==0?BEE:SNAIL):BOAR;
    int hp=type==BEE?beeHp:(type==BOAR?boarHp:snailHp);
    float speed=type==BEE?beeSpeed:(type==BOAR?boarSpeed:snailSpeed);
    bool canAttack=true;
    Timer attackTimer;
    bool drawIMG=true;
    Timer drawSwitchTimer;
public:
    bool alive=true;
    Enemy(){
		Vector2 pos;
        switch (spawnSide)
        {
	        case LEFT:
                pos = { -50.0f, (float)(rand() % 720) };
				break;
            case RIGHT:
                pos = { 1280.0f + 50.0f, (float)(rand() % 720) };
                break;
            case TOP:
                pos = { (float)(rand() % 1280), -50.0f };
                break;
            case BOTTOM:
                pos = { (float)(rand() % 1280), 720.0f + 50.0f };
                break;
        }
        position=pos;
        size={(float)atlas_boar_left.getImage(0)->getwidth(), (float)atlas_boar_left.getImage(0)->getheight()};
        if(type==SNAIL){
            size={(float)atlas_snail_left.getImage(0)->getwidth(), (float)atlas_snail_left.getImage(0)->getheight()};
            leftAnim.setAtlas(&atlas_snail_left);
            leftAnim.setFrameTime(100);
            leftAnim.setLoop(true);
            rightAnim.setAtlas(&atlas_snail_right);
            rightAnim.setFrameTime(100);
            rightAnim.setLoop(true);
        }else if(type==BOAR){
            leftAnim.setAtlas(&atlas_boar_left);
            leftAnim.setFrameTime(50);
            leftAnim.setLoop(true);
            rightAnim.setAtlas(&atlas_boar_right);
            rightAnim.setFrameTime(50);
            rightAnim.setLoop(true);
        }else{
            leftAnim.setAtlas(&atlas_bee_left);
            leftAnim.setFrameTime(50);
            leftAnim.setLoop(true);
            rightAnim.setAtlas(&atlas_bee_right);
            rightAnim.setFrameTime(50);
            rightAnim.setLoop(true);
        }
        static int beeAttackInterval=config.get("enemy.bee.attackInterval").asInt();
        static int boarAttackInterval=config.get("enemy.boar.attackInterval").asInt();
        static int snailAttackInterval=config.get("enemy.snail.attackInterval").asInt();
        attackTimer.setInterval(type==BEE?beeAttackInterval:
            (type==BOAR?boarAttackInterval:snailAttackInterval));
        attackTimer.setLoop(false);
        attackTimer.setCallback([this](){
            canAttack=true;
            drawIMG=true;
            drawSwitchTimer.setEnabled(false);
        });
        drawSwitchTimer.setInterval(50);
        drawSwitchTimer.setLoop(true);
        drawSwitchTimer.setCallback([this](){
            drawIMG=!drawIMG;
        });
        attackTimer.setEnabled(false);
        drawSwitchTimer.setEnabled(false);
    }
    ~Enemy()=default;
    void Draw() {
        static int shadow_offset_y=config.get("enemy.shadow_offset_y").asInt();
        Vector2 pos_shadow;
        pos_shadow.x=position.x+size.x/2-img_shadow_enemy.getwidth()/2;
        pos_shadow.y=position.y+size.y-shadow_offset_y;
        if(drawIMG){
            putimage(pos_shadow, &img_shadow_enemy);
            currentAnim->draw(position);
        }
        if(debug){
            setlinecolor(RED);
            rectangle(position.x-size.x/2, position.y-size.y/2, position.x+size.x/2, position.y+size.y/2);
        }
    }
    void Update(unsigned int deltaTime) {
		attackTimer.update(deltaTime);
        drawSwitchTimer.update(deltaTime);
        velocity = { 0,0 };
        Vector2 toPlayer = player->getPosition() - position;

        velocity = toPlayer.normalized() * speed;
        if (toPlayer.x > 0) isFacingRight = true;
        else if (toPlayer.x < 0) isFacingRight = false;
        position+=velocity*deltaTime;
        currentAnim=isFacingRight?&rightAnim:&leftAnim;
        currentAnim->update(deltaTime);
        if(isCollisionPlayer(*player)&&player->canAttackNow()){
            mciSendString(L"play hurt from 0", NULL, 0, NULL);
            static int beeDamage=config.get("enemy.bee.damage").asInt();
            static int boarDamage=config.get("enemy.boar.damage").asInt();
            static int snailDamage=config.get("enemy.snail.damage").asInt();
            int damage=type==BEE?beeDamage:(type==BOAR?boarDamage:snailDamage);
            player->addHP(-damage);
            if(type==BEE)speed=speed*0.8f;
        }
        if(canAttack){
            for(auto& b: bullets_around){
                if(b->isActive() && isCollisionBullet(*b)){
                    mciSendString(L"play hit from 0", NULL, 0, NULL);
                    b->hit();
                    hp--;
                    if(hp<=0){
                        alive=false;
                        score++;
                    }
                    canAttack=false;
                    attackTimer.setEnabled(true);
                    drawSwitchTimer.setEnabled(true);
                    attackTimer.reset();
                    drawSwitchTimer.reset();
                    break;
                }
            }
            for(auto& b: bullets_custom){
                if(b->isActive() && isCollisionBullet(*b)){
                    mciSendString(L"play hit from 0", NULL, 0, NULL);
                    b->hit();
                    hp--;
                    if(hp<=0){
                        alive=false;
                        score++;
                    }
                    canAttack=false;
                    attackTimer.setEnabled(true);
                    drawSwitchTimer.setEnabled(true);
                    attackTimer.reset();
                    drawSwitchTimer.reset();
                    break;
                }
            }
        }
    }
    bool isCollisionPlayer(const Player& p) const {
        Vector2 checkPos = position + size / 2;
        bool isCollideX = checkPos.x >= p.getPosition().x && checkPos.x <= p.getPosition().x + p.getSize().x;
        bool isCollideY = checkPos.y >= p.getPosition().y && checkPos.y <= p.getPosition().y + p.getSize().y;
        return isCollideX && isCollideY;
	}
    bool isCollisionBullet(const Bullet& b) const {
        bool isCollideX = b.getPosition().x >= position.x && b.getPosition().x <= position.x + size.x;
        bool isCollideY = b.getPosition().y >= position.y && b.getPosition().y <= position.y + size.y;
        return isCollideX && isCollideY;
    }
};