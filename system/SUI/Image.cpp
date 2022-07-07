#include "Image.hpp"

#include "AssetManager.hpp"

Sprite &SUIImage::GetSprite()
{ return m_Sprite; }

void SUIImage::Draw(const drect &bounds)
{
    DrawSprite(m_Sprite, GetBounds().position, m_Sprite.getScale());
}

SUIImage::SUIImage(const std::string &name, const drect &rect,
                   std::shared_ptr<Texture> tex)
    : BaseElement(name, rect), m_Sprite(tex)
{
    m_Sprite.setSize(rect.size);
}

SUIImage::SUIImage(
    const DefinitionFile &def,
    std::unordered_map<std::string, SUI::CallbackFunction> function_map)
    : BaseElement(def.Name(), {})
{
    m_Sprite.setTexture(AssetManager::GetAsset<Texture>(def["texture"].Str()));

    drect bounds = {m_Sprite.getSize(), def["position"].Vec()};
    SetBounds(bounds);
}
