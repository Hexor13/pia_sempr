#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <chrono>
#include <thread>

using namespace std;

// ---
// generace prekazek 
// ---

vector<sf::Sprite> generateNewObstacles(float size, sf::Texture& texture){
    vector<sf::Sprite> res;
    int obstacle_num = 5;
    for(int i=0; i<obstacle_num-1; i++){
        int obstacle_pos = rand() % obstacle_num;
        sf::Sprite obstacle;
        obstacle.setTexture(texture);
        obstacle.setScale(1, 1);
        obstacle.setPosition(obstacle_pos * size * 2, -2 * size);
        res.push_back(obstacle);
    }

    return res;
}

// ---
// kolize letadla a prekazek 
// --- 
//gen by chat 
bool isOverlapping(sf::Sprite sprite1, sf::Sprite sprite2){
    // 1st: check if rectangles of the sprite overlap
    if (!sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds())) {
        // If they collide, change the color of sprite1 to red
        sprite1.setColor(sf::Color::Red);
        return false;
    } 
    // 2nd: check if images of sprites overlap (rectangle overlaping is not precise)
    else{
        sf::FloatRect bounds1 = sprite1.getGlobalBounds();
        sf::FloatRect bounds2 = sprite2.getGlobalBounds();
        sf::Image image1 = sprite1.getTexture()->copyToImage();
        sf::Image image2 = sprite2.getTexture()->copyToImage();
        sf::Vector2i position1 = static_cast<sf::Vector2i>(sprite1.getPosition());
        sf::Vector2i position2 = static_cast<sf::Vector2i>(sprite2.getPosition());
        int left = std::max(static_cast<int>(bounds1.left), static_cast<int>(bounds2.left));
        int right = std::min(static_cast<int>(bounds1.left + bounds1.width), static_cast<int>(bounds2.left + bounds2.width));
        int top = std::max(static_cast<int>(bounds1.top), static_cast<int>(bounds2.top));
        int bottom = std::min(static_cast<int>(bounds1.top + bounds1.height), static_cast<int>(bounds2.top + bounds2.height));
        
        for (int x = left; x < right; ++x) {
            for (int y = top; y < bottom; ++y) {
                // Get the pixel color of sprite1 at (x, y)
                sf::Color color1 = image1.getPixel(x - position1.x, x - position1.x);

                // Get the pixel color of sprite2 at (x, y)
                sf::Color color2 = image2.getPixel(x - position2.x, y - position2.y);

                // Check if both pixels are not transparent (non-alpha = 255)
                if (color1.a > 0 && color2.a > 0) {
                    return true; // Collision detected
                }
            }
        }
    }
    return false;
}


int main(){
    // ---
    // Inicializace textur pro hru
    // ---

    string plane_filename = "resources\\pngwing.com.png";
    string obstacle_filename = "resources\\pngegg.png";
    string background_filename = "resources\\space.jpg";
    string gameover_filename = "resources\\game_over.jpg";
    string heart_filename = "resources\\heart.png";
    string start_filename = "resources\\start_screen_real.png";
    //string start_filename = "resources\\start_screen_CZ.png"; // for czech(Easter Egg)+ zmena scale v nastaveni
    //string start_filename = "resources\\start_screen_RUS.png"; // отсыл очка (Easter Egg)
    sf::Texture start_texture;
    if (!start_texture.loadFromFile(start_filename))
    {
        cout << "image " << start_filename << " can not be loaded. Exiting..." << endl;
        return 1;
    }
    sf::Texture plane_texture;
    if (!plane_texture.loadFromFile(plane_filename))
    {
        cout << "image " << plane_filename << " can not be loaded. Exiting..." << endl;
        return 1;
    }
    sf::Texture obstacle_texture;
    if (!obstacle_texture.loadFromFile(obstacle_filename))
    {
        cout << "image " << obstacle_filename << " can not be loaded. Exiting..." << endl;
        return 1;
    }
    sf::Texture background_texture;
    if (!background_texture.loadFromFile(background_filename))
    {
        cout << "image " << background_filename << " can not be loaded. Exiting..." << endl;
        return 1;
    }
    sf::Texture gameover_texture;
    if (!gameover_texture.loadFromFile(gameover_filename))
    {
        cout << "image " << gameover_filename << " can not be loaded. Exiting..." << endl;
        return 1;
    }
    sf::Texture heart_texture;
    if (!heart_texture.loadFromFile(heart_filename))
    {
        cout << "image " << heart_filename << " can not be loaded. Exiting..." << endl;
        return 1;
    }
    heart_texture.setRepeated(true);

    // ---
    // nastaveni objektu
    // ---

    sf::Sprite background_sprite;
    background_sprite.setTexture(background_texture);
    background_sprite.setScale(0.85, 0.85);


    sf::Sprite gameover_sprite;
    gameover_sprite.setTexture(gameover_texture);
    float scale  = 0.85;
    gameover_sprite.setScale(0.65, 0.65);
    gameover_sprite.setPosition(0, 300);
    int invincible_timer = 120;
    int invincible_now = 0;

    vector<sf::Sprite> obstacles = generateNewObstacles(50, obstacle_texture);
    int add_obstacles_timer = 120;
    int add_obstacles_now = 0;

    sf::Sprite plane_sprite;
    plane_sprite.setTexture(plane_texture);
    // plane_sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
    // plane_sprite.setScale(0.15, 0.15);
    plane_sprite.setPosition(200, 500);
    sf::Vector2f position_plane;
    float plane_speed = 5.0;
    float obstacle_speed = 10.0;

    int lives = 3;
    sf::Sprite hearts_sprite;
    hearts_sprite.setTextureRect(sf::IntRect(0, 0, lives*32, 32));
    hearts_sprite.setPosition(404, 0);
    hearts_sprite.setTexture(heart_texture);

    sf::Sprite start_sprite;
    start_sprite.setTexture(start_texture);
    start_sprite.setScale(1, 1); //for real
    //start_sprite.setScale(0.55, 0.8); //for easter egg

    // ---
    // herni okno
    // ---

    auto window = sf::RenderWindow({500, 1080}, "SpaceJet Game");
    window.setFramerateLimit(60);
     bool gameStarted = false;
    while (window.isOpen() && !gameStarted) {
        window.clear();
        window.draw(start_sprite); 
        window.display();

        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                gameStarted = true; 
            }
        }
    }
    // ---
    // herni logika
    // ---

    while (window.isOpen())
    {
        window.clear();
        window.draw(background_sprite);
        if(add_obstacles_now >= add_obstacles_timer){
            add_obstacles_now = 0;//Reset časovač pro překážky
            vector<sf::Sprite> new_obstacles = generateNewObstacles(50, obstacle_texture);
            obstacles.insert(obstacles.end(), new_obstacles.begin(), new_obstacles.end());
        }
        add_obstacles_now++;
        bool collided = false;
        
        for(int i=0; i<obstacles.size(); i++){
            if(obstacles[i].getPosition().y > 1080){
                obstacles.erase(obstacles.begin() + i);
                i--;
                continue;
            }
            if(invincible_now >= invincible_timer){
                if(isOverlapping(plane_sprite, obstacles[i])){
                    obstacles[i].setColor(sf::Color::Red);
                    collided = true;
                }
            }
            //Posun prekazky dolu
            obstacles[i].move(0, obstacle_speed);
            window.draw(obstacles[i]);
        }

// Zpracovani kolize
        if(collided){
            lives--;
            hearts_sprite.setTextureRect(sf::IntRect(0, 0, lives*32, 32));
            hearts_sprite.setPosition(404, 0);

            invincible_now = 0;
            plane_sprite.setPosition(200, 500);
            if(lives <= 0){
                window.draw(gameover_sprite);
                window.display();
                break;
            }
        }
        if(invincible_now < invincible_timer){
            invincible_now++;
        }
        window.draw(hearts_sprite);
        for (auto event = sf::Event(); window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

// Zpracovani pohybu 
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && plane_sprite.getPosition().y > 0) {
    plane_sprite.move(0, -plane_speed);  
        }
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S)|| sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  && plane_sprite.getPosition().y < 950) {
            plane_sprite.move(0, plane_speed);   
        }
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))&& plane_sprite.getPosition().x > 0) {
            plane_sprite.move(-plane_speed, 0);  
        }
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))&& plane_sprite.getPosition().x < 420) {
            plane_sprite.move(plane_speed, 0);   
        }

        if(invincible_now%10 < 5){
            window.draw(plane_sprite);
        }
        window.display();
    }

    while(window.isOpen()){
        for (auto event = sf::Event(); window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
    }
    return 0;
}
