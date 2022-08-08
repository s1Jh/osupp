/*******************************************************************************
 * Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#include "SUI.hpp"

#include <utility>

#include "define.hpp"

#include "SUI/Button.hpp"
#include "SUI/Image.hpp"

NS_BEGIN

std::shared_ptr<SUI::BaseElement> SUI::NullElement;

SUI::BaseElement::BaseElement(std::string name, drect bounds)
    : m_Bounds(bounds), m_sName(std::move(name)), m_eFlags(SUI_DEFAULT_FLAGS)
{}

void SUI::BaseElement::update(float delta)
{}

[[nodiscard]] drect SUI::BaseElement::getBounds() const
{ return m_Bounds; }

[[nodiscard]] std::string SUI::BaseElement::getName() const
{ return m_sName; }

[[nodiscard]] bool SUI::BaseElement::isVisible() const
{
    return bool(m_eFlags & SUIFlags::Visible);
}

inline void SUI::BaseElement::hide()
{ m_eFlags &= ~SUIFlags::Visible; }

inline void SUI::BaseElement::show()
{ m_eFlags |= SUIFlags::Visible; }

void SUI::BaseElement::draw(Renderer &, const drect &bounds)
{}

void SUI::BaseElement::setBounds(drect n)
{ m_Bounds = n; }

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

            MatchType<SUIButton>("button", element_def.second, ui,
function_map); MatchType<SUIImage>("image", element_def.second, ui,
function_map);
        }
    }

    return ui;
}*/

void SUI::draw(Renderer &renderer)
{
    for (auto &el: m_Elements)
        if (el->isVisible())
            el->draw(renderer, el->getBounds());
}

void SUI::update(float delta)
{
    for (auto &el: m_Elements)
        el->update(delta);
}

NS_END