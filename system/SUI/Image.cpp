#include "Image.hpp"

#include "AssetManager.hpp"

Sprite & SUIImage::GetSprite()
{
    return m_Sprite;
}

void SUIImage::Draw ( const drect& bounds )
{
    DrawSprite ( m_Sprite, GetBounds().position, m_Sprite.GetScale() );
}

SUIImage::SUIImage (
    const std::string& name,
    const drect& rect,
    std::shared_ptr<Texture> tex
) : BaseElement ( name, rect ),
    m_Sprite ( tex )
{
    m_Sprite.SetSize ( rect.size );
}

SUIImage::SUIImage (
    const DefinitionFile& def,
    std::unordered_map<std::string, SUI::CallbackFunction> function_map
) : BaseElement ( def.Name(), {} )
{
    m_Sprite.SetTexture ( AssetManager::GetAsset<Texture> ( def["texture"].Str() ) );

    drect bounds =
    {
        m_Sprite.GetSize(),
        def["position"].Vec()
    };
    SetBounds ( bounds );
}
