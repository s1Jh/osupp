#pragma once

#include "DefinitionFile.hpp"
#include "Rect.hpp"
#include "SUI.hpp"
#include "Sprite.hpp"
#include "assets/AssetTexture.hpp"

#include <memory>
#include <string>
#include <unordered_map>

class SUIImage: public SUI::BaseElement
{
public:
    SUIImage(const std::string &name, const drect &rect,
             std::shared_ptr<Texture> tex = nullptr);

    SUIImage(const DefinitionFile &def,
             std::unordered_map<std::string, SUI::CallbackFunction> function_map);

    Sprite &GetSprite();

protected:
    void Draw(const drect &bounds) override;

private:
    Sprite m_Sprite;
};
