#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "../Mocks/MockEEObject.h"

import Elevate;

bool HasProperty(const Elevate::PropertySet& props, std::string_view name)
{
	return std::ranges::any_of(props, [&](const auto& p) { return p.Name == name; });
}

TEST_CASE("TypeLayout::CaptureState with mock object", "[Serialization][TypeLayout][PropertySet]")
{
	// Arrange
	auto objPtr(std::make_shared<MockEEObject>());

	// Act
	TypeLayout layout = objPtr->GetLayout();
	PropertySet res = layout.CaptureState(); // Build the propertyset from the layout

	// Assert
	SECTION("Validate that all the types are correctly reflected")
	{
		auto& objectType = Elevate::TypeRegistry::GetEntry<MockEEObject>().type;
		CHECK(objectType == std::type_index(typeid(MockEEObject)));

		auto& structType = Elevate::TypeRegistry::GetEntry<MockEEStruct>().type;
		CHECK(structType == std::type_index(typeid(MockEEStruct)));
	}

	SECTION("Validation of the generated class layout")
	{
		REQUIRE(!layout.GetFields().empty());

		std::set<std::string> names;
		for (const auto& f : layout.GetFields())
		{
			names.insert(f.name);
		}

		CHECK(names.contains("m_guid"));
		CHECK(names.contains("testInt"));
		CHECK(names.contains("testFloat"));
		CHECK(names.contains("testStruct"));
	}

	SECTION("Validation of the generated PropertySet")
	{
		REQUIRE(!res.empty());

		CHECK(HasProperty(res, "m_guid"));
		CHECK(HasProperty(res, "testInt"));
		CHECK(HasProperty(res, "testFloat"));
		CHECK(HasProperty(res, "testStruct"));
	}

	SECTION("PropertySet matches the layout")
	{
		REQUIRE(res.size() == layout.GetFieldCount());

		for (const auto& f : layout.GetFields())
		{
			CHECK(HasProperty(res, f.name));
		}
	}
}
