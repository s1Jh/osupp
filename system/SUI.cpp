#include "SUI.hpp"

#include <utility>

#include "define.hpp"

#include "SUI/Button.hpp"
#include "SUI/Image.hpp"

NS_BEGIN

std::shared_ptr<SUI::BaseElement> SUI::NullElement;

SUI::BaseElement::BaseElement(
        std::string name,
        drect bounds
) : m_Bounds(bounds), m_sName(std::move(name)), m_eFlags(SUI_DEFAULT_FLAGS) {}

void SUI::BaseElement::update(float delta) {}


[[nodiscard]] drect SUI::BaseElement::getBounds() const {
    return m_Bounds;
}

[[nodiscard]] std::string SUI::BaseElement::getName() const {
    return m_sName;
}

[[nodiscard]] bool SUI::BaseElement::isVisible() const {
    return bool(m_eFlags & SUIFlags::Visible);
}

inline void SUI::BaseElement::hide() {
    m_eFlags &= ~SUIFlags::Visible;
}

inline void SUI::BaseElement::show() {
    m_eFlags |= SUIFlags::Visible;
}

void SUI::BaseElement::draw(Renderer &, const drect &bounds) {

}

void SUI::BaseElement::setBounds(drect n) {
    m_Bounds = n;
}

/*template<class T>
constexpr void MatchType(
        const std::string &name,
        const df2 &definition,
        SUI &ui,
        const SUI::FunctionMap &func_map
) {
    if (definition["class"].str() == name)
        ui.add<T>(definition, func_map);
}*/
/*SUI SUI::FromDefinitionFile(
        const DefinitionFile &definition,
        const FunctionMap &function_map
) {
    SUI ui;

    Log::Info("[UI PARSE] Parsing a UI definition");

    for (auto element_def: definition) {
        Log::Debug("[UI PARSE] Found element named: ", element_def.first);
        if (element_def.second.find("class") != element_def.second.end()) {
            Log::Debug("[UI PARSE] Class: ", element_def.second["class"].Str());

            std::string name = element_def.second["class"].Str();

            MatchType<SUIButton>("button", element_def.second, ui, function_map);
            MatchType<SUIImage>("image", element_def.second, ui, function_map);
        }
    }

    return ui;
}*/


void SUI::draw(Renderer& renderer) {
    for (auto &el: m_Elements)
        if (el->isVisible())
            el->draw(renderer, el->getBounds());
}

void SUI::update(float delta) {
    for (auto &el: m_Elements)
        el->update(delta);
}

NS_END