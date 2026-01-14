/* Start Header ************************************************************************/
/*!
\file		Map.cpp
\author		Vu Phan Hung
\par		phanhung.vu@digipen.edu
\date		09/03/2025
\brief		This file implements

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "GameDefn.hpp"

void Map::Enter()
{
    Object2D::Enter();
}

void Map::PreUpdate()
{
    Object2D::PreUpdate();
}

void Map::Update()
{
    Object2D::Update();
}

void Map::PostUpdate()
{
    Object2D::PostUpdate();
}

//void Map::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
//{
//    //no idea what to put here
// //probably the seed
//}

void Map::Exit()
{
    Object2D::Exit();
}

//change maxStep to increase/decrease the max for height changes in subsequent randomly generated platform
std::vector<Map::MapObject> Map::CreateSeed(int xMin, int xMax, int yMin, int yMax,int minStep, int maxStep, int numObjects, float objectSizeX, float objectSizeY)
{
	std::vector<Map::MapObject> mapObjects;

    int previousY = yMin - 1; // Start below the minimum Y value

    std::srand(static_cast<unsigned int>(std::time(0) * AEGetTime(NULL) * AERandFloat() + 0.01f));   //magic number

    for (int i = 0; i < numObjects; i++) {  //AAAAAAAAAAAAAAAAAAAAAAAAAA
        if (previousY > yMax)      //if last object > max height dont spawn
        {
            std::cout << i << " objects spawned\n";
            break;
        }

        //random platform x and y coord
        int x = xMin + (std::rand() % (xMax - xMin + 1)); // Random x between xMin and xMax
        std::cout << " rand " << std::rand() << " ymax " << yMax << " previousY " << previousY << "\n";
        int step = minStep++ + (std::rand() % maxStep);           // Random Y step
        int y = previousY + step;                        // Increment Y by the step size
        if (y > yMax) break;                          // Stop if this y is past yMax
        
        // Random platform scale
        float minScaleX = objectSizeX;
        float maxScaleX = objectSizeY;
        float scaleX = minScaleX + ((float)rand() / RAND_MAX) * (maxScaleX - minScaleX);
        float scaleY = 20.0f;
        AEVec2 scale_v{ scaleX,scaleY };

        s32 rand = (std::rand() % 20);

        EnemyType enemy_type{ ENEMY_NONE };

        switch (rand) {
        case ENEMY_NONE:
            break;
        case ENEMY_TYPE_1:
            enemy_type = ENEMY_TYPE_1;
            break;
        case ENEMY_TYPE_2:
            enemy_type = ENEMY_TYPE_2;
            break;
        case ENEMY_TYPE_3:
            enemy_type = ENEMY_TYPE_3;
            break;
        case ENEMY_POWERUP:
            enemy_type = ENEMY_POWERUP;
            break;
        default:
            break;
        }

        PlatformType platform_type{ PLATFORM_NORMAL };

        if (rand < 2) {
            platform_type = PLATFORM_SPIKE;
        }

        mapObjects.emplace_back(x, y, scale_v, enemy_type, platform_type);                     // Create a MapObject and add it to the map
        previousY = y;                                   // Update the previous Y value
    }

    for (int i = 0; i < mapObjects.size(); i++)
    {
        if (i != 0)
        {
            std::cout << "Object at (x: " << mapObjects.at(i).x << ", y: " << mapObjects.at(i).y << " which is " << (mapObjects.at(i).y - mapObjects.at(i - 1).y) << " higher" << ")\n";
        }
        else
        {
            std::cout << "Object at (x: " << mapObjects.at(i).x << ", y: " << mapObjects.at(i).y << ")\n";
        }
    }

    return mapObjects;
}

//change maxStep to increase/decrease the max for height changes in subsequent randomly generated platform
std::vector<Map::MapObject> Map::CreateSeed(int xMin, int xMax, int yMin, int yMax, int minStep, int maxStep, int numObjects, float objectSizeX, float objectSizeY, unsigned int seed)
{
    std::vector<Map::MapObject> mapObjects;

    int previousY = yMin - 1; // Start below the minimum Y value

    std::srand(seed);

    for (int i = 0; i < numObjects; i++) {  //AAAAAAAAAAAAAAAAAAAAAAAAAA
        if (previousY == yMax)      //if last object > max height - step dont spawn FIX THISSSSSSSSS
        {
            std::cout << i << " objects spawned\n";
            break;
        }

        //random platform x and y coord
        int x = xMin + (std::rand() % (xMax - xMin + 1)); // Random x between xMin and xMax
        std::cout << " rand " << std::rand() << " ymax " << yMax << " previousY " << previousY << "\n";
        int step = minStep++ + (std::rand() % maxStep);           // Random Y step
        int y = previousY + step;                        // Increment Y by the step size
        if (y > yMax) break;                          // Stop if this y is past yMax

        // Random platform scale
        float minScaleX = objectSizeX;
        float maxScaleX = objectSizeY;
        float scaleX = minScaleX + ((float)rand() / RAND_MAX) * (maxScaleX - minScaleX);
        float scaleY = 20.0f;
        AEVec2 scale_v{ scaleX,scaleY };

        s32 rand = (std::rand() % 20);

        EnemyType enemy_type{ ENEMY_NONE };

        switch (rand) {
        case ENEMY_NONE:
            break;
        case ENEMY_TYPE_1:
            enemy_type = ENEMY_TYPE_1;
            break;
        case ENEMY_TYPE_2:
            enemy_type = ENEMY_TYPE_2;
            break;
        case ENEMY_TYPE_3:
            enemy_type = ENEMY_TYPE_3;
            break;
        case ENEMY_POWERUP:
            enemy_type = ENEMY_POWERUP;
            break;
        default:
            break;
        }

        PlatformType platform_type{ PLATFORM_NORMAL };

        if (rand < 2) {
            platform_type = PLATFORM_SPIKE;
        }

        mapObjects.emplace_back(x, y, scale_v, enemy_type, platform_type);                   // Create a MapObject and add it to the map
        previousY = y;                                   // Update the previous Y value
    }

    for (int i = 0; i < mapObjects.size(); i++)
    {
        if (i != 0)
        {
            std::cout << "Object at (x: " << mapObjects.at(i).x << ", y: " << mapObjects.at(i).y << " which is " << (mapObjects.at(i).y - mapObjects.at(i - 1).y) << " higher" << ")\n";
        }
        else
        {
            std::cout << "Object at (x: " << mapObjects.at(i).x << ", y: " << mapObjects.at(i).y << ")\n";
        }
    }

    return mapObjects;
}

void Map::CreateMap(const std::vector<Map::MapObject>& map)
{
    // Create a parent object to hold all map objects
    auto mapParent = std::make_shared<Object2D>();
    mapParent->SetName("MapObjects");

    // For each map object, create a corresponding RectPlayer in the scene
    for (const auto& obj : map) {
        auto rectPlayer = std::make_shared<RectPlayer>();
        rectPlayer->SetName("MapObject");
        rectPlayer->SetPosition({ static_cast<f32>(obj.x), static_cast<f32>(obj.y) });

        rectPlayer->SetScale({ obj.s.x, obj.s.y });
        rectPlayer->SetColor(0x0000FF00);        // Default green color

        if (obj.x < -200.f) {
            auto leaf = std::make_shared<ImageRenderer2D>();
            leaf->SetSpaceType(WORLD_SPACE);
            leaf->LoadTexture("Assets/LeftLeaf.png");
            leaf->SetScale({ obj.s.x, obj.s.x });
            rectPlayer->AddChild(leaf);
        }
        else if (obj.x > 200.f) {
            auto leaf = std::make_shared<ImageRenderer2D>();
            leaf->SetSpaceType(WORLD_SPACE);
            leaf->LoadTexture("Assets/RightLeaf.png");
            leaf->SetScale({ obj.s.x, obj.s.x });
            rectPlayer->AddChild(leaf);
        }
        else {
            auto leaf = std::make_shared<ImageRenderer2D>();
            leaf->SetSpaceType(WORLD_SPACE);
            leaf->LoadTexture("Assets/MiddleLeaf.png");
            leaf->SetScale({ obj.s.x * 1.5f, obj.s.x });
            rectPlayer->AddChild(leaf);
        }

        switch (obj.enemy_type) {
        case ENEMY_NONE: {

            break;
        }
        case ENEMY_TYPE_1: {
            // Display and do collision with enemy 1 (red enemy)
            std::shared_ptr<EnemyBasic1> enemy1 = std::make_shared <EnemyBasic1>();
            enemy1->SetScale({ 60.0f, 60.0f });
            enemy1->SetPosition({ static_cast<f32>(obj.x), static_cast<f32>(obj.y) + obj.s.y / 2.0f + enemy1->GetScale().y / 2.0f });	// Need to change pos (detection at start of game)
            enemy1->SetName("Enemy 1");
            GetWorldBranch()->AddChild(enemy1);

            std::shared_ptr<RectPlayer> rect_enemy1 = std::make_shared <RectPlayer>();
            rect_enemy1->SetName("Enemy 1");
            rect_enemy1->SetScale({ enemy1->GetScale().x, enemy1->GetScale().y });
            rect_enemy1->SetPosition({ 0.0f, 0.0f });
            rect_enemy1->SetColor(0x00FF0000);
            enemy1->AddChild(rect_enemy1);

            std::shared_ptr<ImageRenderer2D> enemyMesh1 = std::make_shared <ImageRenderer2D>();
            enemyMesh1->SetName("BeeEnemy");
            enemyMesh1->SetScale({ enemy1->GetScale().x, enemy1->GetScale().y });
            enemyMesh1->SetPosition({ enemy1->GetPosition()});
            enemyMesh1->LoadTexture("Assets/enemyBee.png");
            enemyMesh1->SetSpaceType(WORLD_SPACE);
            enemy1->AddChild(enemyMesh1);
            break;
        }
        case ENEMY_TYPE_2: {
            // Display and do collision with enemy 2 (green enemy)
            std::shared_ptr<EnemyBasic2> enemy2 = std::make_shared <EnemyBasic2>();
            enemy2->SetName("Enemy 2");
            enemy2->SetScale({ 45.0f, 45.0f });
            enemy2->SetPosition({ static_cast<f32>(obj.x), static_cast<f32>(obj.y) + obj.s.y / 2.0f + enemy2->GetScale().y / 2.0f });	// Need to change pos (detection at start of game)
            GetWorldBranch()->AddChild(enemy2);

            std::shared_ptr<RectPlayer> rect_enemy2 = std::make_shared <RectPlayer>();
            rect_enemy2->SetName("Enemy 2 Rect");
            rect_enemy2->SetScale({ enemy2->GetScale().x, enemy2->GetScale().y });
            rect_enemy2->SetPosition({ 0.0f, 0.0f });
            rect_enemy2->SetColor(0x0000FF00);
            enemy2->AddChild(rect_enemy2);

            std::shared_ptr<ImageRenderer2D> enemyMesh2 = std::make_shared <ImageRenderer2D>();
            enemyMesh2->SetName("DragonEnemy");
            enemyMesh2->SetScale({ enemy2->GetScale().x, enemy2->GetScale().y });
            enemyMesh2->SetPosition({ enemy2->GetPosition() });
            enemyMesh2->LoadTexture("Assets/enemyDragonfly.png");
            enemyMesh2->SetSpaceType(WORLD_SPACE);
            enemy2->AddChild(enemyMesh2);
            break;
        }
        case ENEMY_TYPE_3: {
            // Display and do collision with enemy 3 (blue enemy)
            std::shared_ptr<EnemyBasic3> enemy3 = std::make_shared <EnemyBasic3>();
            enemy3->SetScale({ 50.0f, 50.0f });
            enemy3->SetPosition({ static_cast<f32>(obj.x), static_cast<f32>(obj.y) + obj.s.y / 2.0f + enemy3->GetScale().y / 2.0f });	// Need to change pos (detection at start of game)
            enemy3->SetName("Enemy 3");
            GetWorldBranch()->AddChild(enemy3);

            std::shared_ptr<RectPlayer> rect_enemy3 = std::make_shared <RectPlayer>();
            rect_enemy3->SetName("Enemy 3 Rect");
            rect_enemy3->SetScale({ enemy3->GetScale().x, enemy3->GetScale().y });
            rect_enemy3->SetPosition({ 0.0f, 0.0f });
            rect_enemy3->SetColor(0x000000FF);
            enemy3->AddChild(rect_enemy3);

            std::shared_ptr<ImageRenderer2D> enemyMesh3 = std::make_shared <ImageRenderer2D>();
            enemyMesh3->SetName("ChickenEnemy");
            enemyMesh3->SetScale({ enemy3->GetScale().x, enemy3->GetScale().y });
            enemyMesh3->SetPosition({ enemy3->GetPosition() });

            int side = enemy3->GetOffSide();
            if (!side) {
                enemyMesh3->LoadTexture("Assets/enemyChickenLeft.png");
            }
            else {
                enemyMesh3->LoadTexture("Assets/enemyChickenRight.png");
            }

            enemyMesh3->SetSpaceType(WORLD_SPACE);
            enemy3->AddChild(enemyMesh3);
            break;
        }
        case ENEMY_POWERUP: {
            std::shared_ptr<ImageRenderer2D> powerup = std::make_shared<ImageRenderer2D>();
            powerup->SetScale({ 50.f, 50.f });
            powerup->SetPosition({ static_cast<f32>(obj.x), static_cast<f32>(obj.y) + obj.s.y + powerup->GetScale().y / 2.f });
            powerup->SetName("POWERUP");
            powerup->SetSpaceType(WORLD_SPACE);
            powerup->LoadTexture("Assets/PowerUpBox.png");
            GetWorldBranch()->AddChild(powerup);
            break;
        }
        default:
            break;
        }



        GetWorldBranch()->AddChild(rectPlayer);

        if (obj.platform_type == PLATFORM_SPIKE) {
            auto spike = std::make_shared<RectPlayer>();
            spike->SetName("SPIKE");
            spike->SetPosition({ rectPlayer->GetLocalPosition().x, rectPlayer->GetLocalPosition().y - obj.s.y });
            spike->SetScale({ obj.s.x, obj.s.y });
            spike->SetColor(0x4FFF0000);
            spike->SetTransparency(0.f);
            GetWorldBranch()->AddChild(spike);

            auto spike_sprite = std::make_shared<ImageRenderer2D>();
            spike_sprite->SetName("SPIKESPRITE");
            spike_sprite->SetScale({ obj.s.x, obj.s.y });
            spike_sprite->LoadTexture("Assets/Spike.png");
            spike_sprite->SetSpaceType(WORLD_SPACE);
            spike_sprite->SetTiling(static_cast<s32>((obj.s.x / 2.f) / obj.s.y), 1);
            spike->AddChild(spike_sprite);
        }

        std::cout << "================ Spawned Map! =================" << std::endl;
    }
}
