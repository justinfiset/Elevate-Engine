#include "Reflection.h"

import Elevate;
using namespace Elevate;

struct MockEEStruct
{
    BEGIN_STRUCT(MockEEStruct);
    int testStructInt = 123;
    PROPERTY(testStructInt);
    std::string testStructString = "qwerty";
    PROPERTY(testStructString);
    END_STRUCT();
};

class MockEEObject : public EEObject
{
public:
    using Super = EEObject;

    BEGIN_OBJECT(MockEEObject);

    int testInt = 123456;
    PROPERTY(testInt);

    float testFloat = 321.123f;
    PROPERTY(testFloat);

    MockEEStruct testStruct;
    PROPERTY(testStruct);
    ;

    END_OBJECT();

    MockEEObject() = default;
    virtual ~MockEEObject() = default;
};