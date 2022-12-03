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

#include "Curve.dpp"

#include "Math.hpp"
#include "Util.hpp"

#include <concepts>
#include <list>
#include <type_traits>

NS_BEGIN

namespace math
{

template<typename IteratorT, typename LengthT>
requires IsCurveIterator<IteratorT> and std::floating_point<LengthT>
Curve<IteratorT, LengthT>::Curve(const IteratorT _begin, const IteratorT _end)
{
    setIteratorRange(_begin, _end);
}

template<typename IteratorT, typename LengthT>
requires IsCurveIterator<IteratorT> and std::floating_point<LengthT>
void Curve<IteratorT, LengthT>::setIteratorRange(const IteratorT _begin,
                                                 const IteratorT _end)
{
    begin = _begin;
    end = _end;

    update();
}

template<typename IteratorT, typename LengthT>
requires IsCurveIterator<IteratorT> and std::floating_point<LengthT>
void Curve<IteratorT, LengthT>::update()
{
    length = 0.0;
    for (auto it = begin; it != std::prev(end); it++) {
        auto thisPosition = it->getPosition();
        auto nextPosition = std::next(it)->getPosition();
        length += Distance(thisPosition, nextPosition);
    }
}

template<typename IteratorT, typename LengthT> requires IsCurveIterator<IteratorT> and std::floating_point<LengthT>

template<CurveType Type, typename TType>
requires std::floating_point<TType> vec2d<TType>
Curve<IteratorT, LengthT>::get(TType t)
const
{
    if (length < 0.0)
        return {0.0, 0.0};
    return CurveCalculationFunctor<Type>::calculate(*this, t);
}

template<typename IteratorT, typename LengthT> requires IsCurveIterator<IteratorT> and std::floating_point<LengthT>

template<typename TType>
requires std::floating_point<TType> vec2d<TType>
Curve<IteratorT, LengthT>::get(CurveType type, TType t)
const
{
    switch (type) {
        case CurveType::BEZIER:return this->get<CurveType::BEZIER>(t);
        case CurveType::STRAIGHT:return this->get<CurveType::STRAIGHT>(t);
        case CurveType::CATMULL:return this->get<CurveType::CATMULL>(t);
        case CurveType::CIRCLE:return this->get<CurveType::CIRCLE>(t);
        case CurveType::SEMI_CIRCLE:return this->get<CurveType::SEMI_CIRCLE>(t);
        default:return {0, 0};
    }
}

template<typename IteratorT, typename LengthT>
requires IsCurveIterator<IteratorT> and std::floating_point<LengthT>
IteratorT Curve<IteratorT, LengthT>::getBegin()
const
{ return begin; }

template<typename IteratorT, typename LengthT>
requires IsCurveIterator<IteratorT> and std::floating_point<LengthT>
IteratorT Curve<IteratorT, LengthT>::getEnd()
const
{ return end; }

template<typename IteratorT, typename LengthT>
requires IsCurveIterator<IteratorT> and std::floating_point<LengthT>
LengthT Curve<IteratorT, LengthT>::getLength()
const
{ return length; }

template<typename IteratorT, typename LengthT>
requires IsCurveIterator<IteratorT> and std::floating_point<LengthT>
Curve<IteratorT, LengthT>::Curve()
    : length(-1.0)
{}

template<typename TType, typename IteratorT, typename LengthT>
requires std::floating_point<TType> and IsCurveIterator<IteratorT> and
    std::floating_point<LengthT>
vec2d<TType> CurveCalculationFunctor<CurveType::STRAIGHT>::calculate(
    const Curve<IteratorT, LengthT> &curve, TType t)
{
    t = Clamp(t, 0.0, 1.0);

    auto length = curve.getLength();
    auto begin = curve.getBegin();
    auto end = curve.getEnd();

    double coveredLength = 0.0;
    auto lastIt = std::prev(end, 2);
    for (auto it = begin; it != end; it++) {
        auto position = it->getPosition();
        auto nextPosition = std::next(it)->getPosition();

        auto nodeLength = Distance(position, nextPosition);

        double nextCoveredLength = coveredLength + nodeLength;
        // also check if we're on the last node of the path, as floating point
        // inaccuracies may cause this condition to fail
        if (t <= nextCoveredLength / length || it == lastIt) {
            // we found the right segment, now interpolate
            // we interpolate between coveredLength and nextCoveredLength
            double factorStart = coveredLength / length;
            double segmentFactor = nodeLength / length;

            t -= factorStart;
            t /= segmentFactor;
            return BiLerp(position, nextPosition, t);
        }
        coveredLength = nextCoveredLength;
    }
    // we should never get here
    log::error("Curve calculation fell through");
    return {0.0, 0.0};
}

template<typename TType, typename IteratorT, typename LengthT>
requires std::floating_point<TType> and IsCurveIterator<IteratorT> and
    std::floating_point<LengthT>
vec2d<TType> CurveCalculationFunctor<CurveType::BEZIER>::calculate(
    const Curve<IteratorT, LengthT> &curve, TType t)
{
    auto copy = std::vector<typename IteratorT::value_type>{curve.getBegin(), curve.getEnd()};

    auto nt = 1.0 - t;

    while (copy.size() > 1) {
        for (int i = 0, e = copy.size() - 1; i < e; i++) {
            copy[i].position = copy[i].position * nt + copy[i + 1].position * t;
        }
        copy.erase(std::prev(copy.end()));
    }
    return copy[0].position;
}

template<typename TType, typename IteratorT, typename LengthT>
requires std::floating_point<TType> and IsCurveIterator<IteratorT> and
    std::floating_point<LengthT>
vec2d<TType> CurveCalculationFunctor<CurveType::CATMULL>::calculate(
    const Curve<IteratorT, LengthT> &, TType)
{
    return {0, 0};
}

template<typename TType, typename IteratorT, typename LengthT>
requires std::floating_point<TType> and IsCurveIterator<IteratorT> and
    std::floating_point<LengthT>
vec2d<TType> CurveCalculationFunctor<CurveType::CIRCLE>::calculate(
    const Curve<IteratorT, LengthT> &, TType)
{
    return {0, 0};
}

template<typename TType, typename IteratorT, typename LengthT>
requires std::floating_point<TType> and IsCurveIterator<IteratorT> and
    std::floating_point<LengthT>
vec2d<TType> CurveCalculationFunctor<CurveType::SEMI_CIRCLE>::calculate(
    const Curve<IteratorT, LengthT> &, TType)
{
    return {0, 0};
}

}

NS_END