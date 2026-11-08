#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "../Mocks/MockArrayEEObject.h"

import Elevate;
using namespace Elevate;

TEST_CASE("EEObject Array of Structs JSON Roundtrip", "[Serialization][PropertySet][JsonSerializer][Array]")
{
	auto originalObj = std::make_shared<MockArrayEEObject>();

	originalObj->projectList.emplace_back("ElevateLauncher", 101);
	originalObj->projectList.emplace_back("SandboxGame", 202);

	TypeLayout originalLayout = originalObj->GetLayout();
	PropertySet originalProps = originalLayout.CaptureState();

	JsonSerializer serializer;
	ByteBuffer buffer;
	REQUIRE(serializer.Serialize(originalProps, buffer));

	std::string jsonStr = ByteUtils::ToString(buffer);
	CAPTURE(jsonStr);

	CHECK(jsonStr.find("\"projectList\"") != std::string::npos);
	CHECK(jsonStr.find("\"ElevateLauncher\"") != std::string::npos);
	CHECK(jsonStr.find("\"SandboxGame\"") != std::string::npos);

	PropertySet deserializedProps;
	REQUIRE(serializer.Deserialize(buffer, deserializedProps));

	auto newObj = std::make_shared<MockArrayEEObject>();
	REQUIRE(newObj->projectList.empty());

	TypeLayout newLayout = newObj->GetLayout();
	newLayout.ApplyState(deserializedProps);

	REQUIRE(newObj->projectList.size() == 2);
	CHECK(newObj->projectList[0].itemName == "ElevateLauncher");
	CHECK(newObj->projectList[0].itemID == 101);
	CHECK(newObj->projectList[1].itemName == "SandboxGame");
	CHECK(newObj->projectList[1].itemID == 202);
}
