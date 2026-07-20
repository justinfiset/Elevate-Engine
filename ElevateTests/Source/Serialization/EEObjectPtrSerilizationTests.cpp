#include <catch2/catch_test_macros.hpp>

#include <ElevateEngine/Core/Data.h>
#include <ElevateEngine/Core/Byte.h>

#include <ElevateEngine/Core/EEObject.h>
#include <ElevateEngine/Core/EEObjectPtr.h>
#include <ElevateEngine/Core/Reflection.h>

#include <ElevateEngine/Core/TypeLayout.h>
#include <ElevateEngine/Serialization/ObjectPropertyField.h>

using namespace Elevate;

struct MockEEStruct
{
    BEGIN_STRUCT(MockEEStruct)
    int testStructInt = 123;
    PROPERTY(testStructInt)
    END_STRUCT()
};

class MockEEObject : public EEObject
{
public:
    BEGIN_OBJECT(MockEEObject)

    int testInt = 123456;
    PROPERTY(testInt)

    float testFloat = 321.123f;
    PROPERTY(testFloat)

    MockEEStruct testStruct;
    PROPERTY(testStruct);

    END_OBJECT()

    MockEEObject() = default;
    virtual ~MockEEObject() = default;
};

TEST_CASE("TypeLayout::CaptureState with mock object", "[Serialization][TypeLayout]") {
    auto objPtr (std::make_shared<MockEEObject>());
    TypeLayout layout = objPtr->GetLayout();
    PropertySet res = layout.CaptureState(); // Build the propertyset from the layout
    
    SECTION("Validate that all the types are correctly reflected") {
        auto& objectType = Elevate::TypeRegistry::GetEntry<MockEEObject>().type;
        CHECK(objectType == std::type_index(typeid(MockEEObject)));

        auto& structType = Elevate::TypeRegistry::GetEntry<MockEEStruct>().type;
        CHECK(structType == std::type_index(typeid(MockEEStruct)));
    }

    SECTION("Validation of the generated class layout") {
        REQUIRE(!layout.GetFields().empty());
        REQUIRE(layout.GetFieldCount() == 3);
    }

    SECTION("Validation of the generated PropertySet") {
        // The propertyset should contain multiple properties
        REQUIRE(!res.empty());
        REQUIRE(res.size() == 3);
    }
}

TEST_CASE("EEObjectPtr serlization is not empty", "[Serialization]") {
    Elevate::EEObjectPtr objPtr(std::make_shared<MockEEObject>());
    Elevate::ByteBuffer guid = objPtr.Serialize();
	REQUIRE(!guid.empty());
}

TEST_CASE("EEObjectPtr serialization ensures uniqueness", "[Serialization]") {
    Elevate::EEObjectPtr ptr1 = std::make_shared<MockEEObject>();
    Elevate::EEObjectPtr ptr2 = std::make_shared<MockEEObject>();

    Elevate::ByteBuffer buffer1 = ptr1.Serialize();
    Elevate::ByteBuffer buffer2 = ptr2.Serialize();

    REQUIRE(!buffer1.empty());
    REQUIRE(!buffer2.empty());

    REQUIRE(buffer1 != buffer2);
}

TEST_CASE("EEObjectPtr serialization gives the same as the EEObject's guid.") {
    Elevate::EEObjectPtr objPtr(std::make_shared<MockEEObject>());
    Elevate::ByteBuffer guid = objPtr.Serialize();
    REQUIRE(!guid.empty());

    std::string objGuid = objPtr->GetGuid().ToString();
    std::erase(objGuid, '-');
    REQUIRE(Elevate::ByteUitls::ToHexString(guid) == objGuid);
}