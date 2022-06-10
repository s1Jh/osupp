#include "Curve.dpp"

#include "Util.hpp"
#include "Math.hpp"

#include <concepts>
#include <type_traits>
#include <list>

NS_BEGIN

    template<typename IteratorT, typename LengthT>
    requires IsCurveIterator<IteratorT> and std::floating_point<LengthT>
    Curve<IteratorT, LengthT>::Curve(const IteratorT _begin, const IteratorT _end) {
        setIteratorRange(_begin, _end);
    }

    template<typename IteratorT, typename LengthT>
    requires IsCurveIterator<IteratorT> and std::floating_point<LengthT>
    void Curve<IteratorT, LengthT>::setIteratorRange(const IteratorT _begin, const IteratorT _end) {
        begin = _begin;
        end = _end;

        update();
    }

    template<typename IteratorT, typename LengthT>
    requires IsCurveIterator<IteratorT> and std::floating_point<LengthT>
    void Curve<IteratorT, LengthT>::update() {
        length = 0.0;
        for (auto it = begin; it != std::prev(end); it++) {
            auto thisPosition = it->getPosition();
            auto nextPosition = std::next(it)->getPosition();
            length += Distance(thisPosition, nextPosition);
        }
    }

    template<typename IteratorT, typename LengthT>
    requires IsCurveIterator<IteratorT> and std::floating_point<LengthT>
    template <CurveType Type, typename TType> requires std::floating_point<TType>
    vec2d<TType> Curve<IteratorT, LengthT>::get(TType t) const {
        if (length < 0.0) return {0.0, 0.0};
        return CurveCalculationFunctor<Type>::calculate(*this, t);
    }

    template<typename IteratorT, typename LengthT>
    requires IsCurveIterator<IteratorT> and std::floating_point<LengthT>
    template <typename TType> requires std::floating_point<TType>
    vec2d<TType> Curve<IteratorT, LengthT>::get(CurveType type, TType t) const {
        switch (type) {
            case CurveType::Bezier: return this->get<CurveType::Bezier>(t);
            case CurveType::Straight: return this->get<CurveType::Straight>(t);
            case CurveType::Catmull: return this->get<CurveType::Catmull>(t);
            case CurveType::Circle: return this->get<CurveType::Circle>(t);
            case CurveType::SemiCircle: return this->get<CurveType::SemiCircle>(t);
            default: return {0, 0};
        }
    }

    template<typename IteratorT, typename LengthT>
    requires IsCurveIterator<IteratorT> and std::floating_point<LengthT>
    IteratorT Curve<IteratorT, LengthT>::getBegin() const {
        return begin;
    }

    template<typename IteratorT, typename LengthT>
    requires
    IsCurveIterator<IteratorT> and std::floating_point<LengthT>
    IteratorT Curve<IteratorT, LengthT>::getEnd() const {
        return end;
    }

    template<typename IteratorT, typename LengthT> requires
    IsCurveIterator<IteratorT> and std::floating_point<LengthT>
    LengthT Curve<IteratorT, LengthT>::getLength() const {
        return length;
    }

    template<typename IteratorT, typename LengthT> requires
    IsCurveIterator<IteratorT> and std::floating_point<LengthT>
    Curve<IteratorT, LengthT>::Curve() : begin(nullptr), end(nullptr), length(-1.0) {}

    template<typename TType, typename IteratorT, typename LengthT>
    requires std::floating_point<TType> and IsCurveIterator<IteratorT> and std::floating_point<LengthT>
    vec2d<TType> CurveCalculationFunctor<CurveType::Straight>::calculate(const Curve<IteratorT, LengthT> &curve, TType t) {
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
            // also check if we're on the last node of the path, as floating point inaccuracies may
            // cause this condition to fail
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
    requires std::floating_point<TType> and IsCurveIterator<IteratorT> and std::floating_point<LengthT>
    vec2d<TType> CurveCalculationFunctor<CurveType::Bezier>::calculate(const Curve<IteratorT, LengthT> &curve, TType t) {
        return bezierCalculate(curve.getBegin(), curve.getEnd(), t);
    }

    template<typename IteratorT, typename TType>
    requires IsCurveIterator<IteratorT> and std::floating_point<TType>
    vec2d<TType> CurveCalculationFunctor<CurveType::Bezier>::bezierCalculate(
            IteratorT begin, IteratorT end, TType t) {
        std::list<CurveNode> reduced;

        auto size = std::distance(begin, end);
        if (size <= 2) {
            const auto &A = begin->getPosition();
            const auto &B = std::prev(end)->getPosition();
            return BiLerp(A, B, (float) t);
        }

        for (auto it = begin; it != std::prev(end); it++) {
            const auto &firstPos = it->position;
            const auto &nextPos = std::next(it)->position;
            auto position = BiLerp(firstPos, nextPos, t);
            reduced.push_back({position});
        }
        return bezierCalculate(reduced.begin(), reduced.end(), t);
    }

    template<typename TType, typename IteratorT, typename LengthT>
    requires std::floating_point<TType> and IsCurveIterator<IteratorT> and std::floating_point<LengthT>
    vec2d<TType> CurveCalculationFunctor<CurveType::Catmull>::calculate(
            const Curve<IteratorT, LengthT> &curve, TType t) {
        return {0, 0};
    }

    template<typename TType, typename IteratorT, typename LengthT>
    requires std::floating_point<TType> and IsCurveIterator<IteratorT> and std::floating_point<LengthT>
    vec2d<TType> CurveCalculationFunctor<CurveType::Circle>::calculate(
            const Curve<IteratorT, LengthT> &curve, TType t) {
        return {0, 0};
    }

    template<typename TType, typename IteratorT, typename LengthT>
    requires std::floating_point<TType> and IsCurveIterator<IteratorT> and std::floating_point<LengthT>
    vec2d<TType> CurveCalculationFunctor<CurveType::SemiCircle>::calculate(
            const Curve<IteratorT, LengthT> &curve, TType t) {
        return {0, 0};
    }

    /*
template <typename IteratorT> requires IsCurveIterator<IteratorT>
template <typename T> requires std::floating_point<T>
vec2d<T> Curve<CurveType::Straight, IteratorT>::operator() (T t) {
    return {0.0, 0.0};
    /*t = Clamp(t, 0.0, 1.0);

    double coveredLength = 0.0;

    auto lastIt = std::prev(end, 2);
    for (auto it = begin; it != std::prev(end); it++) {
        const auto &node = *it;
        double nextCoveredLength = coveredLength + node.length;
        // also check if we're on the last node of the path, as floating point inaccuracies may
        // cause this condition to fail
        if (t <= nextCoveredLength / length || it == lastIt) {
            // we found the right segment, now interpolate
            // we interpolate between coveredLength and nextCoveredLength
            double factorStart = coveredLength / length;
            double segmentFactor = node.length / length;

            t -= factorStart;
            t /= segmentFactor;
            return BiLerp(node.position, std::next(it)->position, t);
        }
        coveredLength = nextCoveredLength;
    }
    // we should never get here
    log::error("Curve calculation fell through");
    return {0.0, 0.0};
}

template <typename IteratorT> requires IsCurveIterator<IteratorT>
void Curve<CurveType::Straight, IteratorT>::update()  {

}

template <typename IteratorT> requires IsCurveIterator<IteratorT>
template <typename T> requires std::floating_point<T>
vec2d<T> Curve<CurveType::Bezier, IteratorT>::operator() (T t)  {
    std::list<CurveNode> reduced;

    auto size = std::distance(begin, end);
    if (size <= 2) {
        const auto& A = begin->getPosition();
        const auto& B = std::prev(end)->getPosition();
        return BiLerp(A, B, (float)t);
    }

    for (auto it = begin; it != std::prev(end); it++) {
        const auto& firstPos = it->position;
        const auto& nextPos = std::next(it)->position;
        auto position = BiLerp(firstPos, nextPos, t);
        reduced.push_back({ position });
    }
    return this->operator()(reduced.begin(), reduced.end(), t);
}

template <typename IteratorT> requires IsCurveIterator<IteratorT>
void Curve<CurveType::Bezier, IteratorT>::update()  {

}

template <typename IteratorT> requires IsCurveIterator<IteratorT>
template <typename T> requires std::floating_point<T>
vec2d<T> Curve<CurveType::Circle, IteratorT>::operator() (T t)  {

    return {0.0, 0.0};
}

template <typename IteratorT> requires IsCurveIterator<IteratorT>
void Curve<CurveType::Circle, IteratorT>::update()  {

}

template <typename IteratorT> requires IsCurveIterator<IteratorT>
template <typename T> requires std::floating_point<T>
vec2d<T> Curve<CurveType::Catmull, IteratorT>::operator() (T t) {
    return {0.0, 0.0};
}

template <typename IteratorT> requires IsCurveIterator<IteratorT>
void Curve<CurveType::Catmull, IteratorT>::update()  {

}

template <typename IteratorT> requires IsCurveIterator<IteratorT>
template <typename T> requires std::floating_point<T>
vec2d<T> Curve<CurveType::Semicircle, IteratorT>::operator() (T t)  {
    return {0.0, 0.0};
}

template <typename IteratorT> requires IsCurveIterator<IteratorT>
void Curve<CurveType::Semicircle, IteratorT>::update()  {

}*/

NS_END