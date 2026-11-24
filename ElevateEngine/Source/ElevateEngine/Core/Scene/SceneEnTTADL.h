#pragma once

// GCC cannot find EnTT's hidden ADL comparison operators through imported
// module BMIs.  This declaration makes them visible in a translation unit that
// only sees EnTT types through modules.
namespace entt::internal
{
	template<typename Container>
	struct sparse_set_iterator;

	template<typename Container>
	[[nodiscard]] constexpr bool operator!=(
		const sparse_set_iterator<Container>& lhs,
		const sparse_set_iterator<Container>& rhs) noexcept;
}
