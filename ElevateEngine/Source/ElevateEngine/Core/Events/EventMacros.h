#pragma once

#define EVENT_CLASS_TYPE(type)                                                  \
    static EventType GetStaticType() { return EventType::type; }                \
    virtual EventType GetEventType() const override { return GetStaticType(); } \
    virtual const char *GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
    virtual int GetCategoryFlags() const override { return category; }
