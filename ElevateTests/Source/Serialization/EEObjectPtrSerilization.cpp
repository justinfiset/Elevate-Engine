#include <catch2/catch_test_macros.hpp>

#include <ElevateEngine/Core/EEObjectPtr.h>

using namespace Elevate;

class MockEEObject : public EEObject
{
public:
	int testInt = 123456;
};

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