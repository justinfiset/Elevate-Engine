#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <ElevateEngine/Core/Data.h>
#include <ElevateEngine/Core/Byte.h>
#include <ElevateEngine/Core/EEObject.h>
#include <ElevateEngine/Core/EEObjectPtr.h>
#include <ElevateEngine/Core/Reflection.h>
#include <ElevateEngine/Core/TypeLayout.h>
#include <ElevateEngine/Serialization/PropertyField.h>
#include <ElevateEngine/Serialization/JsonSerializer.h>

using namespace Elevate;

struct MockSubStruct
{
    BEGIN_STRUCT(MockSubStruct)
    std::string itemName = "DefaultItem";
    PROPERTY(itemName)
    int itemID = 42;
    PROPERTY(itemID)
    END_STRUCT()

    MockSubStruct() = default;
    MockSubStruct(std::string name, int id) : itemName(std::move(name)), itemID(id) {}
};

class MockArrayEEObject : public EEObject
{
public:
    using Super = EEObject;

    BEGIN_OBJECT(MockArrayEEObject)

    std::vector<MockSubStruct> projectList;
    PROPERTY(projectList)

    END_OBJECT()

    MockArrayEEObject() = default;
    virtual ~MockArrayEEObject() = default;
};

TEST_CASE("EEObject Array of Structs JSON Roundtrip", "[Serialization][PropertySet][JSONSerializer][Array]") {
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