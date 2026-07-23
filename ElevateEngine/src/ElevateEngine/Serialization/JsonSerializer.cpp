#include "JsonSerializer.h"
#include <variant>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace Elevate
{
    template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
    template<class... Ts> overload(Ts...) -> overload<Ts...>;

    rapidjson::Value SerializeValue(const PropertyValue& value, rapidjson::Document::AllocatorType& allocator);

    void SerializeRecursive(const PropertySet& fields, rapidjson::Value& parentObj, rapidjson::Document::AllocatorType& allocator)
    {
        for (const auto& property : fields)
        {
            rapidjson::Value key(property.Name.c_str(), allocator);
            rapidjson::Value val = SerializeValue(property.Value, allocator);
            parentObj.AddMember(key, val, allocator);
        }
    }

    rapidjson::Value SerializeValue(const PropertyValue& value, rapidjson::Document::AllocatorType& allocator)
    {
        return std::visit(overload{
            [&](bool v) { return rapidjson::Value(v); },
            [&](int64_t v) { return rapidjson::Value(v); },
            [&](double v) { return rapidjson::Value(v); },
            [&](const std::string& v) {
                return rapidjson::Value(v.c_str(), allocator);
            },
            [&](const ByteBuffer& v) {
                return rapidjson::Value(
                    reinterpret_cast<const char*>(v.data()),
                    static_cast<rapidjson::SizeType>(v.size()),
                    allocator
                );
            },
            [&](const PropertyContainer& v) {
                bool isArrayContainer = !v.Children.empty() && v.Children[0].Name.rfind("[", 0) == 0;

                if (isArrayContainer)
                {
                    rapidjson::Value jsonArray(rapidjson::kArrayType);
                    for (const auto& elem : v.Children)
                    {
                        jsonArray.PushBack(SerializeValue(elem.Value, allocator), allocator);
                    }
                    return jsonArray;
                }
                else
                {
                    rapidjson::Value childObj(rapidjson::kObjectType);
                    SerializeRecursive(v.Children, childObj, allocator);
                    return childObj;
                }
            }
            }, value);
    }

    bool JsonSerializer::Serialize(const PropertySet& fields, ByteBuffer& outBuffer) const
    {
        rapidjson::Document doc;
        doc.SetObject();

        SerializeRecursive(fields, doc, doc.GetAllocator());

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);

        const std::byte* dataPtr = reinterpret_cast<const std::byte*>(buffer.GetString());
        outBuffer.assign(dataPtr, dataPtr + buffer.GetSize());
        return true;
    }

    // Déclaration anticipée
    void ParseJsonValue(const rapidjson::Value& val, PropertyField& prop, const std::string& currentPath, uint16_t currentDepth);

    void DeserializeRecursive(const rapidjson::Value& jsonObj, PropertySet& outFields, const std::string& parentPath, uint16_t currentDepth)
    {
        if (!jsonObj.IsObject()) return;

        for (auto it = jsonObj.MemberBegin(); it != jsonObj.MemberEnd(); ++it)
        {
            std::string name = it->name.GetString();
            const auto& val = it->value;

            PropertyField prop;
            prop.Name = name;
            prop.Path = parentPath.empty() ? name : parentPath + "/" + name;
            prop.Depth = currentDepth;

            ParseJsonValue(val, prop, prop.Path, currentDepth);
            outFields.push_back(prop);
        }
    }

    void ParseJsonValue(const rapidjson::Value& val, PropertyField& prop, const std::string& currentPath, uint16_t currentDepth)
    {
        if (val.IsBool())
        {
            prop.Type = EngineDataType::Bool;
            prop.Value = val.GetBool();
        }
        else if (val.IsInt64() || val.IsInt() || val.IsUint() || val.IsUint64())
        {
            prop.Type = EngineDataType::Int;
            prop.Value = static_cast<int64_t>(val.GetInt64());
        }
        else if (val.IsDouble())
        {
            prop.Type = EngineDataType::Double;
            prop.Value = val.GetDouble();
        }
        else if (val.IsString())
        {
            prop.Type = EngineDataType::String;
            prop.Value = std::string(val.GetString());
        }
        else if (val.IsObject())
        {
            PropertyContainer container;
            DeserializeRecursive(val, container.Children, currentPath, currentDepth + 1);
            prop.Type = EngineDataType::Custom; // Important pour que le système identifie le conteneur d'objet
            prop.Value = container;
        }
        else if (val.IsArray())
        {
            PropertyContainer container;
            PropertySet arraySet;

            for (rapidjson::SizeType i = 0; i < val.Size(); ++i)
            {
                const auto& elemVal = val[i];
                PropertyField elemProp;
                std::string indexStr = "[" + std::to_string(i) + "]";

                elemProp.Name = indexStr;
                elemProp.Path = currentPath + indexStr;
                elemProp.Depth = currentDepth + 1;

                ParseJsonValue(elemVal, elemProp, elemProp.Path, currentDepth + 1);
                arraySet.push_back(elemProp);
            }

            container.Children = arraySet;
            prop.Type = EngineDataType::Array;
            prop.Value = container;
        }
    }

    bool JsonSerializer::Deserialize(const ByteBuffer& data, PropertySet& outFields)
    {
        if (data.empty()) return false;

        std::string jsonStr(reinterpret_cast<const char*>(data.data()), data.size());

        rapidjson::Document doc;
        doc.Parse(jsonStr.c_str());

        if (doc.HasParseError() || !doc.IsObject())
        {
            return false;
        }

        outFields.clear();

        // Utilise la même logique récursive pour la racine afin d'éviter le code en double
        DeserializeRecursive(doc, outFields, "", 0);

        return true;
    }
}