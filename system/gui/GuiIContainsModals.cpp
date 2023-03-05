// Copyright (c) 2023 sijh (s1Jh.199[at]gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//
// Created by sijh on 13.02.23.
//

#include "GuiIContainsModals.hpp"

#include "GuiModal.hpp"

namespace PROJECT_NAMESPACE {

namespace gui::detail
{

std::optional<std::reference_wrapper<Modal>> IContainsModals::addModal(const std::string &name, const df2 &def)
{
    if (!modals.contains(name)) {
        Modal mod;
        if (!mod.create(def)) {
            log::Error("An error occurred whilst creating modal ", name);
            return {};
        } else {
            modals[name] = mod;
        }
    }
    return modals.at(name);
}

std::optional<std::reference_wrapper<Modal>> IContainsModals::getModal(const std::string &name)
{
    if (modals.contains(name)) {
        return modals.at(name);
    }
    return {};
}

bool IContainsModals::updateModals(Root &root)
{
    bool ret = true;
    for (auto& mod : modals) {
        if (mod.second.isActive()) {
            ret = ret && mod.second.update(root);
        }
    }
    return ret;
}

bool IContainsModals::drawModals(video::LambdaRender &rnd)
{
    bool ret = true;
    for (auto& mod : modals) {
        if (mod.second.isActive()) {
            ret = ret && mod.second.draw(rnd);
        }
    }
    return ret;
}

int IContainsModals::createModalsFrom(const df2 &def)
{
    modals.clear();
    int cnt = 0;
    for (const auto& mod : def) {
        auto created = addModal(mod.first, mod.second);
        if (bool(created)) {
            cnt++;
        }
    }
    return cnt;
}

bool IContainsModals::activateModal(const std::string &name, bool instant)
{
    if (modals.contains(name)) {
        modals.at(name).activate(instant);
        return true;
    }
    return false;
}
bool IContainsModals::deactivateModal(const std::string &name, bool instant)
{
    if (modals.contains(name)) {
        modals.at(name).deactivate(instant);
        return true;
    }
    return false;
}

IContainsModals::ModalStorage &IContainsModals::getModals()
{
    return modals;
}

}

}
