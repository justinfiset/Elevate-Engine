#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "../Mocks/MockEEObject.h"

import Elevate;
using namespace Elevate;

TEST_CASE("EEObject String Serialization and Roundtrip", "[Serialization][PropertySet][JSONSerializer][String]")
{
	auto originalObj = std::make_shared<MockEEObject>();

	originalObj->testStruct.testStructString = "ElevateEngineRocks";

	TypeLayout originalLayout = originalObj->GetLayout();
	PropertySet originalProps = originalLayout.CaptureState();

	JsonSerializer serializer;
	ByteBuffer buffer;
	REQUIRE(serializer.Serialize(originalProps, buffer));

	std::string jsonStr = ByteUtils::ToString(buffer);
	CAPTURE(jsonStr);

	CHECK(jsonStr.find("\"testStructString\":\"ElevateEngineRocks\"") != std::string::npos);

	PropertySet deserializedProps;
	REQUIRE(serializer.Deserialize(buffer, deserializedProps));

	auto newObj = std::make_shared<MockEEObject>();
	REQUIRE(newObj->testStruct.testStructString == "qwerty");

	TypeLayout newLayout = newObj->GetLayout();
	newLayout.ApplyState(deserializedProps);

	CHECK(newObj->testStruct.testStructString == "ElevateEngineRocks");
}

TEST_CASE("EEObject JSON Roundtrip and SetFromProperties", "[Serialization][PropertySet][TypeLayout][JsonSerializer]")
{
	auto originalObj = std::make_shared<MockEEObject>();
	originalObj->testInt = 999999;
	originalObj->testFloat = 42.42f;
	originalObj->testStruct.testStructInt = 456;

	TypeLayout originalLayout = originalObj->GetLayout();
	PropertySet originalProps = originalLayout.CaptureState();

	JsonSerializer serializer;
	ByteBuffer buffer;
	REQUIRE(serializer.Serialize(originalProps, buffer));

	std::string jsonStr = ByteUtils::ToString(buffer);
	CAPTURE(jsonStr);

	PropertySet deserializedProps;
	REQUIRE(serializer.Deserialize(buffer, deserializedProps));

	auto newObj = std::make_shared<MockEEObject>();
	REQUIRE(newObj->testInt == 123456);
	REQUIRE(newObj->testFloat == 321.123f);
	REQUIRE(newObj->testStruct.testStructInt == 123);

	TypeLayout newLayout = newObj->GetLayout();
	newLayout.ApplyState(deserializedProps);

	CHECK(newObj->testInt == 999999);
	CHECK(std::abs(newObj->testFloat - 42.42f) < 0.0001f);
	CHECK(newObj->testStruct.testStructInt == 456);
}

TEST_CASE("EEObject JSON Serilization", "[Serialization][PropertySet][JsonSerializer]")
{
	auto objPtr(std::make_shared<MockEEObject>());
	Elevate::TypeLayout layout = objPtr->GetLayout();
	Elevate::PropertySet res = layout.CaptureState();

	Elevate::ByteBuffer outBuffer;
	Elevate::JsonSerializer serializer;
	bool success = serializer.Serialize(res, outBuffer);
	REQUIRE(success);

	std::string json = Elevate::ByteUtils::ToString(outBuffer);
	CAPTURE(json);

	CHECK(json.find("\"testInt\":123456") != std::string::npos);
	CHECK(json.find("\"testStructInt\":123") != std::string::npos);
	CHECK(json.find("\"testStructString\":\"qwerty\"") != std::string::npos);
}
