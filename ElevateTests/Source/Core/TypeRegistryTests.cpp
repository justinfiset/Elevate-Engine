#include <catch2/catch_test_macros.hpp>

#include <ElevateEngine/Core/Reflection.h>

using namespace Elevate;

class MockEEObject : public EEObject
{
public:
    using Super = EEObject;
    BEGIN_OBJECT(MockEEObject)
    int testInt = 123456;
    PROPERTY(testInt)
    END_OBJECT()
};

TEST_CASE("TypeRegistry must contain the entry for a user defined EEObject.", "[Core][Reflection]") {
    REQUIRE(Elevate::TypeRegistry::GetEntries().contains(typeid(MockEEObject)));
}

TEST_CASE("TypeRegistry Entry must contain valid informations.", "[Core][Reflection]") {
    auto& entry = TypeRegistry::GetEntry<MockEEObject>();
    REQUIRE(entry.name == "Mock EE Object");
    REQUIRE(entry.type == typeid(MockEEObject));
}
