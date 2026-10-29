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

struct MockEEOuterStruct
{
	BEGIN_STRUCT(MockEEOuterStruct);
	MockEEStruct inner;
	PROPERTY(inner);
	int outerInt = 7;
	PROPERTY(outerInt);
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

class MockEEDerivedObject : public MockEEObject
{
public:
	using Super = MockEEObject;

	BEGIN_OBJECT(MockEEDerivedObject);
	int derivedInt = 999;
	PROPERTY(derivedInt);
	MockEEOuterStruct outer;
	PROPERTY(outer);
	END_OBJECT();
};
