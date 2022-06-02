//
// Created by sijh on 30.05.22.
//

#include "Slider.hpp"

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

        body.setAttributeDescriptors({
            AttributeType::Vec2,    // position
            AttributeType::Vec2     // UV
        });

        for (auto it = objectTemplate->path.begin();; it++)
        {
            auto start = it->position;
            auto nextIt = std::next(it);
            if (nextIt == objectTemplate->path.end())
                break;

            auto end = nextIt->position;

            fvec2d normal = {
                    (start.y - end.y),
                    -(start.x - end.x),
            };

            normal = Normalize(normal);
            normal *= session->getMap()->getCircleSize();

            //sf::Vector2f size = (sf::Vector2f)slider_body.getTexture()->getSize();

            auto v1 = start + normal;
            auto v2 = end + normal;
            auto v3 = start - normal;
            auto v4 = end - normal;

            auto offset = body.insertVertices({
                {start.x, start.y, 0.0f, 0.0f},
                {v1.x, v1.y, 1.0f, 0.0f},
                {v2.x, v2.y, 1.0f, 1.0f},
                {end.x, end.y, 0.0f, 1.0f},
                {v3.x, v3.y, 1.0f, 0.0f},
                {v4.x, v4.y, 1.0f, 1.0f},
            });

            body.insertIndices({
                0, 1, 3,
                3, 2, 1,
                4, 0, 3,
                3, 4, 5
            }, offset);
        }
        body.upload();
    }

    void Slider::onPress() {

    }

    void Slider::onDraw(Renderer &renderer) {
        const auto objectTransform = session->getObjectTransform();

        renderer.drawMesh(
            body, StandardResources::SliderShader,
            {},
            {{0, &StandardResources::SliderBody}},
            objectTransform
        );
        for (auto & node : objectTemplate->path) {
            renderer.drawCircle(
                    {session->getMap()->getCircleSize(), node.position},
                    {.fillColor = TEAL}, objectTransform);
        }

        for (auto & node : objectTemplate->path) {
            renderer.drawSegment(
                    node.connectingLine,
                    {.fillColor =ORANGE, .outlineWidth = 5},
                    objectTransform
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