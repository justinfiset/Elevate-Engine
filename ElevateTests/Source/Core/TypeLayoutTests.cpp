#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <typeindex>

#include "../Mocks/MockEEObject.h"

import Elevate;

bool HasProperty(const Elevate::PropertySet& props, std::string_view name)
{
	return std::ranges::any_of(props, [&](const auto& p) { return p.Name == name; });
}

namespace
{
	// Capture -> Serialize -> Deserialize -> Apply onto a fresh object.
	// Takes a "setup" callable that mutates the original, and returns the restored object.
	template <typename T, typename Setup>
	std::shared_ptr<T> RoundTripT(Setup&& setup)
	{
		auto original = std::make_shared<T>();
		setup(*original);

		PropertySet props = original->GetLayout().CaptureState();

		JsonSerializer serializer;
		ByteBuffer buffer;
		REQUIRE(serializer.Serialize(props, buffer));
		CAPTURE(ByteUtils::ToString(buffer));

		PropertySet loaded;
		REQUIRE(serializer.Deserialize(buffer, loaded));

		auto restored = std::make_shared<T>();
		restored->GetLayout().ApplyState(loaded);
		return restored;
	}

	// Shorthand for the base mock object
	template <typename Setup>
	std::shared_ptr<MockEEObject> RoundTrip(Setup&& setup)
	{
		return RoundTripT<MockEEObject>(std::forward<Setup>(setup));
	}
}

// ---------------------------------------------------------------------------
// Layout / PropertySet generation
// ---------------------------------------------------------------------------

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

TEST_CASE("Derived layout contains base and derived fields", "[Serialization][TypeLayout][Inheritance]")
{
	auto obj = std::make_shared<MockEEDerivedObject>();
	PropertySet props = obj->GetLayout().CaptureState();

	CHECK(HasProperty(props, "m_guid"));
	CHECK(HasProperty(props, "testInt"));
	CHECK(HasProperty(props, "derivedInt"));
	CHECK(HasProperty(props, "outer"));
}

// ---------------------------------------------------------------------------
// Round trips, one per type
// ---------------------------------------------------------------------------

TEST_CASE("Nested struct round trip changes every field", "[Serialization][TypeLayout]")
{
	// Arrange
	auto original = std::make_shared<MockEEObject>();
	original->testInt = 1;
	original->testFloat = 2.5f;
	original->testStruct.testStructInt = 456;
	original->testStruct.testStructString = "changed";

	// Act
	PropertySet props = original->GetLayout().CaptureState();
	JsonSerializer serializer;
	ByteBuffer buffer;
	REQUIRE(serializer.Serialize(props, buffer));
	CAPTURE(ByteUtils::ToString(buffer));

	PropertySet loaded;
	REQUIRE(serializer.Deserialize(buffer, loaded));

	auto restored = std::make_shared<MockEEObject>();
	restored->GetLayout().ApplyState(loaded);

	// Assert
	CHECK(restored->testInt == 1);
	CHECK(restored->testFloat == 2.5f);
	CHECK(restored->testStruct.testStructInt == 456);
	CHECK(restored->testStruct.testStructString == "changed");
}

TEST_CASE("Round trip: int", "[Serialization][RoundTrip][int]")
{
	// Note: no value equal to the default (123456), otherwise a missing restore would go unnoticed
	const int value = GENERATE(0, 1, -1, 42, std::numeric_limits<int>::max(), std::numeric_limits<int>::min());
	CAPTURE(value);

	auto restored = RoundTrip([&](MockEEObject& o) { o.testInt = value; });

	CHECK(restored->testInt == value);
}

TEST_CASE("Round trip: float", "[Serialization][RoundTrip][float]")
{
	const float value = GENERATE(0.0f, -0.0f, 1.0f, -2.5f, 3.14159274f, 1e-7f, 1e30f,
		std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest());
	CAPTURE(value);

	auto restored = RoundTrip([&](MockEEObject& o) { o.testFloat = value; });

	// A JSON round trip must preserve the exact value (float -> text -> float)
	CHECK(restored->testFloat == value);
}

TEST_CASE("Round trip: float preserves the sign of zero", "[Serialization][RoundTrip][float]")
{
	// -0.0f == 0.0f is true, so the previous test cannot detect a lost sign
	auto restored = RoundTrip([](MockEEObject& o) { o.testFloat = -0.0f; });

	CHECK(std::signbit(restored->testFloat));
}

TEST_CASE("Round trip: non-finite floats (documents current behavior)", "[Serialization][RoundTrip][float]")
{
	// Many JSON writers emit null or reject NaN/inf. Decide what you want and pin it here.
	const float value = GENERATE(std::numeric_limits<float>::infinity(),
		-std::numeric_limits<float>::infinity(),
		std::numeric_limits<float>::quiet_NaN());
	CAPTURE(value);

	auto restored = RoundTrip([&](MockEEObject& o) { o.testFloat = value; });

	if (std::isnan(value))
	{
		CHECK(std::isnan(restored->testFloat));
	}
	else
	{
		CHECK(restored->testFloat == value);
	}
}

TEST_CASE("Round trip: string (inside the nested struct)", "[Serialization][RoundTrip][string]")
{
	const std::string value = GENERATE(
		std::string{},
		std::string{ "simple" },
		std::string{ "with spaces and accents \xC3\xA9\xC3\xA0\xC3\xBC" },
		std::string{ "quotes \" and backslash \\ and slash /" },
		std::string{ "line\nbreak\tand tab" });

	CAPTURE(value);

	auto restored = RoundTrip([&](MockEEObject& o) { o.testStruct.testStructString = value; });

	CHECK(restored->testStruct.testStructString == value);
}

TEST_CASE("Round trip: int (inside the nested struct)", "[Serialization][RoundTrip][int][struct]")
{
	const int value = GENERATE(0, -1, 456, std::numeric_limits<int>::max(), std::numeric_limits<int>::min());
	CAPTURE(value);

	auto restored = RoundTrip([&](MockEEObject& o) { o.testStruct.testStructInt = value; });

	CHECK(restored->testStruct.testStructInt == value);
}

TEST_CASE("Round trip: nested struct", "[Serialization][RoundTrip][struct]")
{
	auto restored = RoundTrip([](MockEEObject& o)
		{
			o.testStruct.testStructInt = 456;
			o.testStruct.testStructString = "changed";
		});

	CHECK(restored->testStruct.testStructInt == 456);
	CHECK(restored->testStruct.testStructString == "changed");
}

TEST_CASE("Round trip: struct nested two levels", "[Serialization][RoundTrip][struct]")
{
	auto restored = RoundTripT<MockEEDerivedObject>([](MockEEDerivedObject& o)
		{
			o.outer.outerInt = 11;
			o.outer.inner.testStructInt = 22;
			o.outer.inner.testStructString = "deep";
		});

	CHECK(restored->outer.outerInt == 11);
	CHECK(restored->outer.inner.testStructInt == 22);
	CHECK(restored->outer.inner.testStructString == "deep");
}

TEST_CASE("Round trip: inherited and derived fields", "[Serialization][RoundTrip][Inheritance]")
{
	auto restored = RoundTripT<MockEEDerivedObject>([](MockEEDerivedObject& o)
		{
			o.testInt = 5;       // from MockEEObject
			o.testFloat = 6.5f;  // from MockEEObject
			o.derivedInt = 77;   // from MockEEDerivedObject
		});

	CHECK(restored->testInt == 5);
	CHECK(restored->testFloat == 6.5f);
	CHECK(restored->derivedInt == 77);
}

TEST_CASE("Round trip: unmodified fields keep their default values", "[Serialization][RoundTrip]")
{
	const MockEEObject defaults;

	auto restored = RoundTrip([](MockEEObject& o) { o.testInt = 42; });

	CHECK(restored->testInt == 42);
	CHECK(restored->testFloat == defaults.testFloat);
	CHECK(restored->testStruct.testStructInt == defaults.testStruct.testStructInt);
	CHECK(restored->testStruct.testStructString == defaults.testStruct.testStructString);
}

// ---------------------------------------------------------------------------
// Partial state and error handling
// ---------------------------------------------------------------------------

TEST_CASE("ApplyState with a partial PropertySet only touches present fields", "[Serialization][RoundTrip][Partial]")
{
	auto original = std::make_shared<MockEEObject>();
	original->testInt = 1;
	original->testFloat = 2.5f;

	PropertySet props = original->GetLayout().CaptureState();
	std::erase_if(props, [](const auto& p) { return p.Name == "testFloat"; });

	auto target = std::make_shared<MockEEObject>();
	const float defaultFloat = target->testFloat;
	target->GetLayout().ApplyState(props);

	CHECK(target->testInt == 1);
	CHECK(target->testFloat == defaultFloat);
}

TEST_CASE("ApplyState ignores unknown properties", "[Serialization][RoundTrip][Partial]")
{
	auto original = std::make_shared<MockEEObject>();
	original->testInt = 3;

	PropertySet props = original->GetLayout().CaptureState();
	REQUIRE(!props.empty());
	auto extra = props.front();
	extra.Name = "doesNotExist";
	props.push_back(extra);

	auto target = std::make_shared<MockEEObject>();
	REQUIRE_NOTHROW(target->GetLayout().ApplyState(props));
	CHECK(target->testInt == 3);
}

TEST_CASE("Deserialize rejects malformed input without crashing", "[Serialization][Json][Errors]")
{
	JsonSerializer serializer;

	const std::string garbage = GENERATE(
		std::string{},
		std::string{ "not json" },
		std::string{ "{" },
		std::string{ "[1, 2," });
	CAPTURE(garbage);

	ByteBuffer buffer;
	buffer.assign(reinterpret_cast<const std::byte*>(garbage.data()),
		reinterpret_cast<const std::byte*>(garbage.data()) + garbage.size());

	PropertySet loaded;
	CHECK_FALSE(serializer.Deserialize(buffer, loaded));
}
