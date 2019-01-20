// Shmurho
//
// Shmurho - Copyright (C) 2017-2019 Stanislav Demyanovich
//
// This software is provided 'as-is', without any express or
// implied warranty. In no event will the authors be held
// liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute
// it freely, subject to the following restrictions:
//
// 1.  The origin of this software must not be misrepresented;
// you must not claim that you wrote the original software.
// If you use this software in a product, an acknowledgment
// in the product documentation would be appreciated but
// is not required.
//
// 2.  Altered source versions must be plainly marked as such,
// and must not be misrepresented as being the original software.
//
// 3.  This notice may not be removed or altered from any
// source distribution.

/// \file
/// \brief Resources parcel implementation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --


#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Resource/JSONFile.h>
#include <Urho3D/Resource/ResourceCache.h>

#include "Parcel.hpp"


using namespace Urho3D;

namespace Shmurho
{
namespace Preload
{


void Parcel::RegisterObject(Context* context)
{
    context->RegisterFactory<Parcel>();
}

Parcel::Parcel(Context* context)
: Resource(context)
{
}

bool Parcel::BeginLoad(Deserializer& source)
{
    String extension = GetExtension(source.GetName());

    bool success = false;

    if (extension == ".json")
    {
        success = BeginLoadJSON(source);
    }
    else
    {
        success = BeginLoadXML(source);
    }

    if (!success)
    {
        GetSubsystem<Log>()->Write(LOG_ERROR,
                                   "Failed to load parcel file: " + source.GetName());
    }
    else
    {
        if (GetName().Empty())
        {
            SetName(source.GetName());
        }
    }

    return (success);
}

bool Parcel::EndLoad()
{
    return (true);
}

void Parcel::AddResource(Urho3D::StringHash type, const Urho3D::String& name) noexcept
{
    assert(!name.Empty());
    if (name.Empty())
    {
        return;
    }

    auto listIt = resRefLists_.Begin();
    for (; listIt != resRefLists_.End(); ++ listIt)
    {
        if (listIt->type_ == type)
        {
            break;
        }
    }
    if (listIt == resRefLists_.End())
    {
        resRefLists_.Push(ResourceRefList{type});
        listIt = --(resRefLists_.End());
    }
    listIt->names_.Push(name);
}

bool Parcel::Empty() const noexcept
{
    if (resRefLists_.Empty())
    {
        return true;
    }
    bool empty = true;
    for (auto listIt = resRefLists_.Begin(); listIt != resRefLists_.End(); ++ listIt)
    {
        if (!listIt->names_.Empty())
        {
            empty = false;
            break;
        }
        if (!empty)
        {
            break;
        }
    }
    return empty;
}

bool Parcel::BeginLoadXML(Deserializer& source)
{
    assert(false && "Parcel loading from XML is NOT IMPLEMENTED");
    return (false);
}

/// Loading JSON of the following form:
///
/// {
///     "resource-lists" : [
///         {
///             "type"  : "Texture2D",
///             "names" : [
///                 "Textures/PreloaderBackground.jpeg",
///                 "Textures/PreloaderForeground.png",
///                 "Textures/OneMoreTexture.png"
///             ]
///         },
///         {
///             "type"  : "Font",
///             "names" : [
///                 "Fonts/Anonimous Pro.ttf"
///             ]
///         },
///         {
///             "type"  : "XMLFile",
///             "names" : [
///                 "UI/DefaultStyle.xml"
///             ]
///         },
///         {
///             "type"  : "Shmurho::Parcel::Parcel",
///             "names" : [
///                 "Parcels/CommonResources.json",
///                 "Parcels/CommonUIResources.json",
///                 "Parcels/StartMenuResources.json"
///             ]
///         }
///     ]
/// }
///

bool Parcel::BeginLoadJSON(Deserializer& source)
{
    auto log = GetSubsystem<Log>();

    bool success = false;

    // Load JSON itself
    SharedPtr<JSONFile> jsonFile(new JSONFile(context_));
    success = jsonFile->Load(source);

    if (success) // if JSON loaded, then load the parcel data from JSON
    {
        const JSONValue& rootVal = jsonFile->GetRoot();

        const JSONArray& typeArray = rootVal.Get("resource-lists").GetArray();

        resRefLists_.Reserve(typeArray.Size());
        for (unsigned i = 0; i < typeArray.Size(); ++i)
        {
            const JSONValue& typeVal = typeArray[ i ];
            String typeStr = typeVal.Get("type").GetString();
            StringHash type(typeStr);

            if (context_->GetTypeName(type) != typeStr)
            {
                log->Write(Urho3D::LOG_ERROR,
                           ToString("Failed to parse parcel file '%s'.",
                                    typeStr.CString()));
                continue;
            }

            auto listsIt = resRefLists_.Begin();

            for (; listsIt != resRefLists_.End(); ++listsIt)
            {
                if (listsIt->type_ == type)
                {
                    break;
                }
            }

            if (listsIt == resRefLists_.End())
            {
                resRefLists_.Insert(listsIt, ResourceRefList(type));
            }

            const JSONArray& nameArray = typeVal.Get("names").GetArray();

            for (unsigned i = 0; i < nameArray.Size(); ++i)
            {
                const JSONValue& nameVal = nameArray[ i ];
                listsIt->names_.Push(nameVal.GetString());
            }
        }
    }
    else
    {
        log->Write(LOG_ERROR,
                   ToString("Failed to open parcel file '%s'.",
                            source.GetName().CString()));
    }

    return (success);
}


} // namespace Preload
} // namespace Shmurho
