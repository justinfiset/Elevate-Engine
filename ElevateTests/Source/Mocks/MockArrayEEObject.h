#include "Reflection.h"

import Elevate;
using namespace Elevate;

struct MockSubStruct
{
    BEGIN_STRUCT(MockSubStruct);
    std::string itemName = "DefaultItem";
    PROPERTY(itemName);
    int itemID = 42;
    PROPERTY(itemID);
    END_STRUCT();

    MockSubStruct() = default;
    MockSubStruct(std::string name, int id) : itemName(std::move(name)), itemID(id) {}
};

class MockArrayEEObject : public EEObject
{
public:
    using Super = EEObject;

    BEGIN_OBJECT(MockArrayEEObject);

    std::vector<MockSubStruct> projectList;
    PROPERTY(projectList);

    END_OBJECT();

    MockArrayEEObject() = default;
    virtual ~MockArrayEEObject() = default;
};
