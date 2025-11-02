#include "World.h"
#include "Painter.h"
#include "Point.h"
#include "Dust.h"
#include "Physics.h"
#include <fstream>
#include <random>

// Длительность одного тика симуляции.
// Подробнее см. update()
// Изменять не следует
static constexpr double timePerTick = 0.001;

/**
 * Конструирует объект мира для симуляции
 * @param worldFilePath путь к файлу модели мира
 */
World::World(const std::string& worldFilePath) {

    std::ifstream stream(worldFilePath);
    /**
     * TODO: хорошее место для улучшения.
     * Чтение границ мира из модели
     * Обратите внимание, что здесь и далее мы многократно
     * читаем в объект типа Point, последовательно
     * заполняя координаты x и у. Если что-то делаем
     * многократно - хорошо бы вынести это в функцию
     * и не дублировать код...
     */
    stream >> topLeft >> bottomRight;
    physics.setWorldBox(topLeft, bottomRight);

    /**
     * TODO: хорошее место для улучшения.
     * (x, y) и (vx, vy) - составные части объекта, также
     * как и (red, green, blue). Опять же, можно упростить
     * этот код, научившись читать сразу Point, Color...
     */


    // Здесь не хватает обработки ошибок, но на текущем
    // уровне прохождения курса нас это устраивает
    while (stream.peek(), stream.good()) {
        Point center;
        Velocity velocity;
        double radius;
        Color color;
        bool isCollidable;
        stream >> center >> velocity  >> color >> radius >> std::boolalpha >> isCollidable;

        // TODO: место для доработки.
        // Здесь не хватает самого главного - создания
        // объекта класса Ball со свойствами, прочитанными
        // выше, и его помещения в контейнер balls

        // После того как мы каким-то образом
        // сконструируем объект Ball ball;
        // добавьте его в конец контейнера вызовом
        // balls.push_back(ball);

        Ball ball(center, velocity, radius, color, isCollidable);
        balls.push_back(ball);
}

 

physics.setCollisionCallback([this](Point p, Color){
        std::uniform_real_distribution<double> angleDist(0, 2*M_PI);
        std::uniform_real_distribution<double> speedDist(800, 1000);
        for(int i = 0; i < 10; i++){
            Dust d;
            d.pos = p;
            d.c = Color(0.0, 0.0, 255.0);
            double angle = angleDist(gen);
            double speed = speedDist(gen);
            d.velocity = Point(std::cos(angle) * speed, std::sin(angle)* speed);
            DustPar.push_back(d);
             }
        });
    
    }

/// @brief Отображает состояние мира
void World::show(Painter& painter) const {
    // Рисуем белый прямоугольник, отображающий границу
    // мира
    painter.draw(topLeft, bottomRight, Color(1, 1, 1));

        for(auto d : DustPar){
        d.draw(painter);
    }

    // Вызываем отрисовку каждого шара
    for (const Ball& ball : balls) {
        ball.draw(painter);
    }


}

/// @brief Обновляет состояние мира
void World::update(double time) {
    /**
     * В реальном мире время течет непрерывно. Однако
     * компьютеры дискретны по своей природе. Поэтому
     * симуляцию взаимодействия шаров выполняем дискретными
     * "тиками". Т.е. если с момента прошлой симуляции
     * прошло time секунд, time / timePerTick раз обновляем
     * состояние мира. Каждое такое обновление - тик -
     * в physics.update() перемещаем шары и обрабатываем
     * коллизии - ситуации, когда в результате перемещения
     * один шар пересекается с другим или с границей мира.
     * В общем случае время не делится нацело на
     * длительность тика, сохраняем остаток в restTime
     * и обрабатываем на следующей итерации.
     */

    // учитываем остаток времени, который мы не "доработали" при прошлом update
    time += restTime;
    const auto ticks = static_cast<size_t>(std::floor(time / timePerTick));
    restTime = time - double(ticks) * timePerTick;

    physics.update(balls, ticks);

    for(size_t i = 0; i < ticks; ++i){
        for(auto& d : DustPar){
            d.lifetime -= timePerTick;
            if(d.lifetime > 0){
                d.pos = d.pos + d.velocity * timePerTick;
            }
        }
    }

    std::vector<Dust> alive;
    for(auto& d : DustPar){
        if(d.lifetime > 0){
            alive.push_back(d);
        }
    }
    DustPar = alive;
}