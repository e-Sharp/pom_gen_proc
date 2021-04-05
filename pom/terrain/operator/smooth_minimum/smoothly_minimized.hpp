#pragma once

#include <pom/maths/operation/clamp.hpp>
#include <pom/maths/operation/lerp.hpp>

#include <tuple>

namespace pom::terrain {

template<typename TerrainA, typename TerrainB>
struct smoothly_minimized {
	smin smin = {};
	TerrainA a;
	TerrainB b;
};

template<typename TA, typename TB>
smoothly_minimized(smin, TA&&, TB&&) -> smoothly_minimized<TA, TB>;

template<typename TA, typename TB> constexpr
auto operator|(std::tuple<TA, TB>&& t, smin m) {
	return smoothly_minimized(
		std::move(m),
		std::get<0>(std::move(t)),
		std::get<1>(std::move(t)));
}

template<typename DeclContext, typename TA, typename TB> constexpr
vec3f recipe(DeclContext, color_, const smoothly_minimized<TA, TB>& m, vec2f coords) {
	auto [ca, ha] = decl::make<color_, height_>(m.a, coords);
	auto [cb, hb] = decl::make<color_, height_>(m.b, coords);
	auto t = maths::clamp((ha - hb) / (2.f * m.smin.range) + 0.5f, 0.f, 1.f);
	return maths::lerp(ca, cb, t);
}

template<typename DeclContext, typename TA, typename TB> constexpr
float recipe(DeclContext, height_, const smoothly_minimized<TA, TB>& m, vec2f coords) {
	auto [ha] = decl::make<height_>(m.a, coords);
	auto [hb] = decl::make<height_>(m.b, coords);
	auto t = maths::clamp((ha - hb) / (2.f * m.smin.range) + 0.5f, 0.f, 1.f);
	return maths::lerp(ha, hb, t) - m.smin.range * t * (1.f - t);
}

}
