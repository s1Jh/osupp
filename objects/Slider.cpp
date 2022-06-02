//
// Created by sijh on 30.05.22.
//

#include "Slider.h"

#include <utility>

namespace GAME_TITLE {

    void Slider::onUpdate(double delta) {

    }

    void Slider::onBegin() {

    }

    HitResult Slider::onFinish() {
        return HitResult::Hit50;
    }

    void Slider::onRaise() {

    }

    Slider::Slider(std::shared_ptr<ObjectTemplateSlider> t, BaseGameMode *g)
            : HitObject(std::move(t), g) {
        SOF = {session->getMap()->getCircleSize(), {objectTemplate->path.front().position}};
    }

    void Slider::onPress() {

    }

    void Slider::onDraw(Renderer &renderer) {
        for (auto & node : objectTemplate->path) {
            renderer.drawCircle({session->getMap()->getCircleSize(), node.position}, {.fillColor = TEAL});
        }

        for (auto & node : objectTemplate->path) {
            renderer.drawSegment(
                    node.connectingLine,
                    {.fillColor =ORANGE, .outlineWidth = 5}
            );
        }
    }




/*slider.vertices.setPrimitiveType(sf::PrimitiveType::Quads);

const float sliderThickness = 0.75f;

for (int i = 0; i < slider.points.size() - 1; )
{
    auto start = slider.points[i].position;
    auto end = slider.points[++i].position;

    sf::Vector2f normal = {
        (start.y - end.y),
        -(start.x - end.x),
    };

    normal /= mag(normal);
    normal *= sliderThickness;

    //sf::Vector2f size = (sf::Vector2f)slider_body.getTexture()->getSize();

    slider.vertices.append(sf::Vertex(start, { 0.0f, 0.0f }));
    slider.vertices.append(sf::Vertex(start + normal, { 1.f, 0.0f }));
    slider.vertices.append(sf::Vertex(end + normal, { 1.f, 1.f }));
    slider.vertices.append(sf::Vertex(end, { 0.0f, 1.f }));

    slider.vertices.append(sf::Vertex(start, { 0.0f,   0.0f }));
    slider.vertices.append(sf::Vertex(start - normal, { 1.f, 0.0f }));
    slider.vertices.append(sf::Vertex(end - normal, { 1.f,   1.f }));
    slider.vertices.append(sf::Vertex(end, { 0.0f, 1.f }));
}

// generate the slider vertices

for (int i = 0; i < slider.points.size() - 1; )
{
    auto& start = slider.points[i].position;
    auto& end = slider.points[++i].position;

    sf::Vector2f normal = {
        (start.x - end.x),
        -(start.y - end.y),
    };

    normal /= mag(normal);

    slider.vertices.clear();
    slider.vertices.setPrimitiveType(sf::PrimitiveType::Quads);

    slider.vertices.append(sf::Vertex(game2Screen(start + normal, ), sf::Color::Red));
    slider.vertices.append(sf::Vertex(game2Screen(start - normal), sf::Color::Red));
    slider.vertices.append(sf::Vertex(game2Screen(end + normal), sf::Color::Red));
    slider.vertices.append(sf::Vertex(game2Screen(end - normal), sf::Color::Red));
}*/

}