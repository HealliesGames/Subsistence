#include "Scene.h"

Scene::Scene( )
{

}

Scene::~Scene()
{
    //dtor
}

bool Scene::Menu( sf::RenderWindow &window)
{
    sf::Clock waitTime;

    bool started = false;


    enum textType {
        CREDITS,
        TITLE,
        START,
        FULLSCREEN,
        ITCHIO,
        QUIT,
        MADEFOR
    };


    int selected = 0;
    sf::Text texts[7];

    for(int i = 0; i < sizeof(texts)/sizeof(texts[0]); i++ ) {
        texts[i].setFont(font);
        texts[CREDITS].setColor(sf::Color::White);
        if(i >= 2 && i <= 5)
            texts[i].setCharacterSize(38);
    }

    texts[CREDITS].setCharacterSize(16);
    texts[CREDITS].setString("HEALLIESGAMES");
    texts[CREDITS].setPosition(SCENE_WIDTH / 2, 64);
    texts[CREDITS].setOrigin(texts[CREDITS].getLocalBounds().width / 2, texts[CREDITS].getLocalBounds().height / 2);

    texts[TITLE].setCharacterSize(96);
    texts[TITLE].setString("SUBSISTENCE");
    texts[TITLE].setPosition(SCENE_WIDTH / 2, 96);
    texts[TITLE].setOrigin(texts[TITLE].getLocalBounds().width / 2, texts[TITLE].getLocalBounds().height / 2);

    texts[START].setString("START");
    texts[START].setPosition(SCENE_WIDTH / 2 - 64, SCENE_HEIGHT / 2);
    texts[START].setOrigin(texts[START].getLocalBounds().left, texts[START].getLocalBounds().height / 2);

    texts[FULLSCREEN].setString("FULLSCREEN");
    texts[FULLSCREEN].setPosition(SCENE_WIDTH / 2 - 64, SCENE_HEIGHT / 2 + 45);
    texts[FULLSCREEN].setOrigin(texts[FULLSCREEN].getLocalBounds().left, texts[FULLSCREEN].getLocalBounds().height / 2);

    texts[ITCHIO].setString("ITCH.IO");
    texts[ITCHIO].setPosition(SCENE_WIDTH / 2 - 64, SCENE_HEIGHT / 2 + 45 * 2);
    texts[ITCHIO].setOrigin(texts[ITCHIO].getLocalBounds().left , texts[ITCHIO].getLocalBounds().height / 2);

    texts[QUIT].setString("QUIT");
    texts[QUIT].setPosition(SCENE_WIDTH / 2 - 64, SCENE_HEIGHT / 2 + 45 * 3);
    texts[QUIT].setOrigin(texts[QUIT].getLocalBounds().left , texts[QUIT].getLocalBounds().height / 2);

    texts[MADEFOR].setCharacterSize(30);
    texts[MADEFOR].setString("MADE FOR OLCCODEJAM 2019");
    texts[MADEFOR].setPosition(32, SCENE_HEIGHT - 64);


    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();


            if (event.type == sf::Event::KeyPressed && !started) {
                switch(event.key.code){

                case sf::Keyboard::Up:
                    snSelection.play();

                    if(selected == 0)
                        selected = 3;
                    else
                        selected --;
                    break;

                case sf::Keyboard::Down:
                    snSelection.play();
                    if(selected == 3)
                        selected = 0;
                    else
                        selected ++;
                    break;

                case sf::Keyboard::Enter:
                    switch(selected) {
                    case 0:
                        started = true;
                        snStart.play();
                    break;

                    case 1:
                        if(!fulls) {
                            window.create(sf::VideoMode(SCENE_WIDTH, SCENE_HEIGHT), "Subsistence", sf::Style::Fullscreen);
                            fulls = true;
                        }
                        else{
                            window.create(sf::VideoMode(SCENE_WIDTH, SCENE_HEIGHT), "Subsistence");
                            fulls = false;
                        }
                    break;

                    case 2:
                        system("start https://healliesgames.itch.io");
                    break;

                    case 3:
                        return true;
                    break;

                    }
                    break;
                }
            }
        }

        if(!started)
            waitTime.restart().asSeconds();
        else
            if(waitTime.getElapsedTime().asSeconds() >= 2)
                return false;



                switch(selected) {
                case 0:
                texts[START].setString("> START");
                texts[FULLSCREEN].setString("FULLSCREEN");
                texts[ITCHIO].setString("ITCH.IO");
                texts[QUIT].setString("QUIT");
                    break;

                case 1:
                texts[START].setString("START");
                texts[FULLSCREEN].setString("> FULLSCREEN");
                texts[ITCHIO].setString("ITCH.IO");
                texts[QUIT].setString("QUIT");
                    break;

                case 2:
                texts[START].setString("START");
                texts[FULLSCREEN].setString("FULLSCREEN");
                texts[ITCHIO].setString("> ITCH.IO");
                texts[QUIT].setString("QUIT");
                    break;

                case 3:
                texts[START].setString("START");
                texts[FULLSCREEN].setString("FULLSCREEN");
                texts[ITCHIO].setString("ITCH.IO");
                texts[QUIT].setString("> QUIT");
                    break;

                }

        window.clear();

        if(!started)
            for(int i = 0; i < sizeof(texts)/sizeof(texts[0]); i++ )
                window.draw(texts[i]);

        window.display();
    }
}

bool Scene::Play( sf::RenderWindow &window ) {

    msAtmosphere.setVolume(50);

    int pToReach = 3000;
    bool started = false, isYes = true;
    int score = 0, multiplier = 1;
    slowDown = false;
    gameOver = false;
    showRestart = false;
    rebuilding = false;
    spawnAsteroidSize = 160;

    sf::Text txScore, txDestroy, txRestart, txYes, txNo, txRecord;

    txScore.setPosition(32, 16);
    txScore.setFont(font);

    txRecord.setPosition(32, 48);
    txRecord.setFont(font);
    txRecord.setColor(sf::Color::White);
    txRecord.setString( "RECORD: " + std::to_string(LoadRecord()) );


    txDestroy.setFont(font);
    txDestroy.setCharacterSize(100);
    txDestroy.setString("DESTROY");
    txDestroy.setPosition(SCENE_WIDTH / 2, SCENE_HEIGHT / 2);
    txDestroy.setOrigin(txDestroy.getLocalBounds().width / 2, txDestroy.getLocalBounds().height / 2);
    txDestroy.setColor(sf::Color::White);

    txRestart.setFont(font);
    txRestart.setCharacterSize(64);
    txRestart.setString("RESTART?");
    txRestart.setPosition(SCENE_WIDTH / 2, SCENE_HEIGHT / 2 -80);
    txRestart.setOrigin(txRestart.getLocalBounds().width / 2, txRestart.getLocalBounds().height / 2 );
    txRestart.setColor(sf::Color::White);

    txYes.setFont(font);
    txYes.setCharacterSize(64);
    txYes.setString("YES");
    txYes.setPosition(SCENE_WIDTH / 2 - 64, SCENE_HEIGHT / 2);

    txYes.setColor(sf::Color::White);

    txNo.setFont(font);
    txNo.setCharacterSize(64);
    txNo.setString("NO");
    txNo.setPosition(SCENE_WIDTH / 2 - 64, SCENE_HEIGHT / 2 + 80);
    txNo.setColor(sf::Color::White);


    sf::Clock* waitTime = new sf::Clock();
    sf::Clock clock, rebuildingTime, multiplierTime;
    sf::Time deltaTime;

    Ship* s = new Ship( SCENE_WIDTH / 2, SCENE_HEIGHT / 1.2);
    Upgrade* u = new Upgrade( Upgrade::SHOOTANDPOINT);

    sf::VertexArray spawnAsteroidSizeShape;
    spawnAsteroidSizeShape.resize(8);
    spawnAsteroidSizeShape[0].position = sf::Vector2f(0, -spawnAsteroidSize);
    spawnAsteroidSizeShape[1].position = sf::Vector2f(-spawnAsteroidSize/1.5, -spawnAsteroidSize/1.5);
    spawnAsteroidSizeShape[2].position = sf::Vector2f(-spawnAsteroidSize, 0);
    spawnAsteroidSizeShape[3].position = sf::Vector2f(-spawnAsteroidSize / 1.5, spawnAsteroidSize / 1.5);
    spawnAsteroidSizeShape[4].position = sf::Vector2f(0,  spawnAsteroidSize);
    spawnAsteroidSizeShape[5].position = sf::Vector2f(+spawnAsteroidSize / 1.5, spawnAsteroidSize / 1.5);
    spawnAsteroidSizeShape[6].position = sf::Vector2f(+spawnAsteroidSize, 0);
    spawnAsteroidSizeShape[7].position = sf::Vector2f(+spawnAsteroidSize/ 1.5, -spawnAsteroidSize / 1.5);

    std::vector<Asteroid*> aVec;
    Asteroid* a = new Asteroid( SCENE_WIDTH / 2, SCENE_HEIGHT / 3.5, spawnAsteroidSizeShape);
    aVec.push_back(a);

    std::vector<Bullet*> bVec;

    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();


            if (event.type == sf::Event::KeyPressed && showRestart) {
                switch(event.key.code){

                case sf::Keyboard::Up:
                    snSelection.play();
                    isYes = !isYes;
                    break;

                case sf::Keyboard::Down:
                    snSelection.play();
                    isYes = !isYes;
                    break;

                case sf::Keyboard::Enter:

                    switch(isYes) {
                    case 0:
                    msAtmosphere.stop();
                    for(int i = 0; i < aVec.size(); i++)
                        delete aVec[i];

                    for(int i = 0; i < bVec.size(); i++)
                        delete bVec[i];
                    delete s, u;
                    return false;

                    break;

                    case 1:
                        for(int i = 0; i < aVec.size(); i++)
                            delete aVec[i];

                        for(int i = 0; i < bVec.size(); i++)
                            delete bVec[i];
                        delete s, u;
                        return true;
                    break;

                    }
                    break;
                }
            }

        }


        if(multiplier == 1) {
            multiplierTime.restart().asSeconds();
        } else {
            if(multiplierTime.getElapsedTime().asSeconds() >= 7) {
                s->shootUpgrade = false;
                multiplier = 1;
            }
        }

        if(!rebuilding)
            rebuildingTime.restart().asSeconds();
        else {
            if(rebuildingTime.getElapsedTime().asSeconds() >= 5) {

                for(int i = 0; i < aVec.size(); i++) {
                    delete aVec[i];
                }
                aVec.clear();

                spawnAsteroidSize += 8;
                spawnAsteroidSizeShape[0].position = sf::Vector2f(0, -spawnAsteroidSize);
                spawnAsteroidSizeShape[1].position = sf::Vector2f(-spawnAsteroidSize/1.5, -spawnAsteroidSize/1.5);
                spawnAsteroidSizeShape[2].position = sf::Vector2f(-spawnAsteroidSize, 0);
                spawnAsteroidSizeShape[3].position = sf::Vector2f(-spawnAsteroidSize / 1.5, spawnAsteroidSize / 1.5);
                spawnAsteroidSizeShape[4].position = sf::Vector2f(0,  spawnAsteroidSize);
                spawnAsteroidSizeShape[5].position = sf::Vector2f(+spawnAsteroidSize / 1.5, spawnAsteroidSize / 1.5);
                spawnAsteroidSizeShape[6].position = sf::Vector2f(+spawnAsteroidSize, 0);
                spawnAsteroidSizeShape[7].position = sf::Vector2f(+spawnAsteroidSize/ 1.5, -spawnAsteroidSize / 1.5);

                Asteroid* a = new Asteroid( SCENE_WIDTH / 2, SCENE_HEIGHT / 3.5, spawnAsteroidSizeShape);
                aVec.push_back(a);

                rebuilding = false;
            }
        }

        if(started) {
            if(!slowDown) {
                if(msAtmosphere.getStatus() != sf::Music::Playing &&
                   snStart.getStatus() != sf::Sound::Playing)
                    msAtmosphere.play();

                u->Update(deltaTime.asSeconds() );
            }

            deltaTime = clock.getElapsedTime();

            if(!gameOver && !slowDown)
            {
                s->Update(deltaTime.asSeconds());

                if(!rebuilding) {
                    s->HasCollided( aVec );
                    s->Shoot( bVec );
                }

                if(DistancePoint( s->GetPosition(), u->GetPosition() ) < u->radius*2 && u->active) {
                    snUpgrade.play();

                    switch(u->type) {
                        case Upgrade::SHOOTANDPOINT:
                            s->shootUpgrade = true;
                            multiplier = 2;
                        break;

                        case Upgrade::REBUILD:

                            for(int i = 0; i < aVec.size(); i++) {
                                aVec[i]->rebuildPoint = sf::Vector2f(SCENE_WIDTH / 2, SCENE_HEIGHT / 3.5);
                            }

                            for(int i = 0; i < bVec.size(); i++) {
                                delete bVec[i];
                                bVec.erase( bVec.begin() + i );
                            }

                            rebuilding = true;
                        break;
                    }

                    u->JumpToEdge();
                    u->active = false;
                }
            }

            s->Death(deltaTime.asSeconds());

            for(int i = 0; i < bVec.size(); i++)
            {
                bVec[i]->Update(deltaTime.asSeconds());
                if(bVec[i]->OutScene())
                {
                    delete bVec[i];
                    bVec.erase( bVec.begin() + i );
                }

            }

            for(int i = 0; i < aVec.size(); i++)
            {
                aVec[i]->Update(deltaTime.asSeconds());

                int cBullet = aVec[i]->BulletCollision( bVec, deltaTime.asSeconds());

                if(cBullet != NOT_SETTED)
                {

                    if(!slowDown)
                    {
                        aVec[i]->GetSplitPoint( bVec[cBullet]->GetPosition(), bVec[cBullet]->GetTrajectory());

                        if(DistancePoint(bVec[cBullet]->GetPosition(), aVec[i]->_splitPointA) > DistancePoint(bVec[cBullet]->GetPosition(), aVec[i]->_splitPointB)) {
                            bVec[cBullet]->dPoint = aVec[i]->_splitPointA;
                            bVec[cBullet]->startColPoint = aVec[i]->_splitPointB;
                        }
                        else {
                            bVec[cBullet]->dPoint = aVec[i]->_splitPointB;
                            bVec[cBullet]->startColPoint = aVec[i]->_splitPointA;
                        }


                        bVec[cBullet]->collide = true;
                        bVec[cBullet]->SetPosition( bVec[cBullet]->startColPoint);
                        slowDown = true;
                        msAtmosphere.pause();
                    }

                    if(DistancePoint(bVec[cBullet]->GetPosition(), bVec[cBullet]->dPoint ) <= (bulletSpeed/2) * deltaTime.asSeconds())
                    {
                        slowDown = false;
                        delete bVec[cBullet];
                        bVec.erase( bVec.begin() + cBullet);
                        score += 100 * multiplier;
                        if(score >= pToReach) {
                            u->active = true;
                            u->JumpToEdge();
                            pToReach += 3000;
                        }

                        Asteroid* a = new Asteroid( aVec[i]->GetPosition().x, aVec[i]->GetPosition().y, aVec[i]->Split( ));
                        aVec.push_back(a);

                    }
                }
            }
        } else {

            if(waitTime->getElapsedTime().asSeconds() >= 3) {
                started = true;
                delete waitTime;
            }
        }

        clock.restart().asSeconds();

        window.clear();


        u->Draw(window);
        s->Draw(window);

        for(int i = 0; i < aVec.size(); i++)
            aVec[i]->Draw( window );

        for(int i = 0; i < bVec.size(); i++)
            bVec[i]->Draw( window );

        if(multiplier > 1) {
        txScore.setColor( sf::Color(255, 128, 0));
        txScore.setString( "SCORE: " + std::to_string(score) + " x" + std::to_string(multiplier));
        } else {
        txScore.setColor( sf::Color::White);
        txScore.setString( "SCORE: " + std::to_string(score));
        }
        window.draw(txScore);
        window.draw(txRecord);


        if(LoadRecord() < score && gameOver)
            SaveRecord(score);

        if(showRestart) {

            window.draw( txRestart);
            window.draw( txYes );
            window.draw( txNo );

            if(isYes) {
                txYes.setString( "> YES");
                txNo.setString( "NO");
            } else {
                txYes.setString( "YES");
                txNo.setString( "> NO");
            }

        }

        if(!started) {
            if(waitTime->getElapsedTime().asSeconds() >= 1.5) {
                window.draw(txDestroy);
            }
        }

        window.display();
    }
}
