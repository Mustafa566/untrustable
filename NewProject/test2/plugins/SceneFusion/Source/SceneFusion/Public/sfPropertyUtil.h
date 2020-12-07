/*************************************************************************
 *
 * KINEMATICOUP CONFIDENTIAL
 * __________________
 *
 *  Copyright (2017-2020) KinematicSoup Technologies Incorporated
 *  All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of KinematicSoup Technologies Incorporated and its
 * suppliers, if any.  The intellectual and technical concepts contained
 * herein are proprietary to KinematicSoup Technologies Incorporated
 * and its suppliers and may be covered by Canadian and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from KinematicSoup Technologies Incorporated.
 */
#pragma once

#include "sfValueProperty.h"
#include "sfStringProperty.h"
#include "sfNullProperty.h"
#include "sfListProperty.h"
#include "../Public/SceneFusion.h"
#include <CoreMinimal.h>
#include <Math/IntRect.h>

using namespace KS;
using namespace KS::SceneFusion2;

#define LOG_CHANNEL "sfPropertyUtil"

/**
 * Utility for converting between SF properties and common Unreal types.
 */
class sfPropertyUtil
{
public:
    /**
     * Constructs a property from a vector.
     *
     * @param   const FVector& value
     * @return  sfValueProperty::SPtr
     */
    static sfValueProperty::SPtr FromVector(const FVector& value)
    {
        return ToProperty(value);
    }

    /**
     * Converts a property to a vector.
     *
     * @param   sfProperty::SPtr propertyPtr
     * @return  FVector
     */
    static FVector ToVector(sfProperty::SPtr propertyPtr)
    {
        return FromProperty<FVector>(propertyPtr);
    }

    /**
     * Constructs a property from a rotator.
     *
     * @param   const FRotator& value
     * @return  sfValueProperty::SPtr
     */
    static sfValueProperty::SPtr FromRotator(const FRotator& value)
    {
        return ToProperty(value);
    }

    /**
     * Converts a property to a rotator.
     *
     * @param   sfProperty::SPtr propertyPtr
     * @return  FRotator
     */
    static FRotator ToRotator(sfProperty::SPtr propertyPtr)
    {
        return FromProperty<FRotator>(propertyPtr);
    }

    /**
     * Constructs a property from a quat.
     *
     * @param   const FQuat& value
     * @return  sfValueProperty::SPtr
     */
    static sfValueProperty::SPtr FromQuat(const FQuat& value)
    {
        return ToProperty(value);
    }

    /**
     * Converts a property to a quat.
     *
     * @param   sfProperty::SPtr propertyPtr
     * @return  FQuat
     */
    static FQuat ToQuat(sfProperty::SPtr propertyPtr)
    {
        return FromProperty<FQuat>(propertyPtr);
    }

    /**
     * Constructs a property from a box.
     *
     * @param   const FBox& value
     * @return  sfValueProperty::SPtr
     */
    static sfValueProperty::SPtr FromBox(const FBox& value)
    {
        return ToProperty(value);
    }

    /**
     * Converts a property to a box.
     *
     * @param   sfProperty::SPtr propertyPtr
     * @return  FBox
     */
    static FBox ToBox(sfProperty::SPtr propertyPtr)
    {
        return FromProperty<FBox>(propertyPtr);
    }

    /**
     * Constructs a property from a box2d.
     *
     * @param   const FBox2D& value
     * @return  sfValueProperty::SPtr
     */
    static sfValueProperty::SPtr FromBox2D(const FBox2D& value)
    {
        return ToProperty(value);
    }

    /**
     * Converts a property to a box2d.
     *
     * @param   sfProperty::SPtr propertyPtr
     * @return  FBox2D
     */
    static FBox2D ToBox2D(sfProperty::SPtr propertyPtr)
    {
        return FromProperty<FBox2D>(propertyPtr);
    }

    /**
     * Constructs a property from an int rect.
     *
     * @param   const FIntRect& value
     * @return  sfValueProperty::SPtr
     */
    static sfValueProperty::SPtr FromIntRect(const FIntRect& value)
    {
        return ToProperty(value);
    }

    /**
     * Converts a property to an int rect.
     *
     * @param   sfProperty::SPtr propertyPtr
     * @return  FIntRect
     */
    static FIntRect ToIntRect(sfProperty::SPtr propertyPtr)
    {
        return FromProperty<FIntRect>(propertyPtr);
    }

    /**
     * Constructs a property from a string.
     *
     * @param   const FString& value
     * @return  sfStringProperty::SPtr
     */
    static sfStringProperty::SPtr FromString(const FString& value)
    {
        return sfStringProperty::Create(TCHAR_TO_UTF8(*value));
    }

    /**
     * Converts a property to a string.
     *
     * @param   sfProperty::SPtr propertyPtr
     * @return  FString
     */
    static FString ToString(sfProperty::SPtr propertyPtr)
    {
        if (propertyPtr == nullptr || propertyPtr->Type() != sfProperty::STRING)
        {
            return "";
        }
        sfStringProperty::SPtr stringPtr = propertyPtr->AsString();
        return FString(UTF8_TO_TCHAR(stringPtr->GetString()->c_str()));
    }

    /**
     * Constructs a property from an FText.
     *
     * @param   const FText& value
     * @return  sfProperty::SPtr
     */
    static sfProperty::SPtr FromText(const FText& value)
    {
        if (value.IsEmpty())
        {
            return sfNullProperty::Create();
        }
        sfListProperty::SPtr listPtr = sfListProperty::Create();
        if (value.IsCultureInvariant())
        {
            listPtr->Add(FromString(*value.ToString()));
        }
        else if (value.IsFromStringTable())
        {
            FName table;
            FString key;
            if (FTextInspector::GetTableIdAndKey(value, table, key))
            {
                listPtr->Add(FromString(*table.ToString()));
                listPtr->Add(FromString(*key));
            }
        }
        else
        {
            listPtr->Add(FromString(*FTextInspector::GetNamespace(value).Get("")));
            listPtr->Add(FromString(*FTextInspector::GetKey(value).Get("")));
            listPtr->Add(FromString(*value.ToString()));
        }
        if (listPtr->Size() == 0)
        {
            return sfNullProperty::Create();
        }
        return listPtr;
    }

    /**
     * Converts a property to an FText.
     *
     * @param   sfProperty::SPtr propertyPtr
     * @return  FText
     */
    static FText ToText(sfProperty::SPtr propertyPtr)
    {
        if (propertyPtr->Type() == sfProperty::NUL)
        {
            return FText::FromString("");
        }
        sfListProperty::SPtr listPtr = propertyPtr->AsList();
        if (listPtr->Size() == 1)
        {
            // CultureInvariant = true, stringIds = text
            FString text = ToString(listPtr->Get(0));
            return FText::AsCultureInvariant(text);
        }
        FString key = ToString(listPtr->Get(1));
        if (listPtr->Size() == 2)
        {
            // stringIds = table, key
            FName table = FName(*ToString(listPtr->Get(0)));
            return FText::FromStringTable(table, key);
        }
        else if (listPtr->Size() == 3)
        {
            // stringIds = namespace, key, text
            FString nameSpace = ToString(listPtr->Get(0));
            FString sourceString = ToString(listPtr->Get(2));
            return FText::ChangeKey(nameSpace, key, FText::FromString(sourceString));
        }
        return FText::FromString("");
    }

private:
    /**
     * Constructs a property from a T.
     *
     * @param   const T& value
     * @return  sfValueProperty::SPtr
     */
    template<typename T>
    static sfValueProperty::SPtr ToProperty(const T& value)
    {
        const uint8_t* temp = reinterpret_cast<const uint8_t*>(&value);
        ksMultiType multiType(ksMultiType::BYTE_ARRAY, temp, sizeof(T), sizeof(T));
        return sfValueProperty::Create(std::move(multiType));
    }

    /**
     * Converts a property to T.
     *
     * @param   sfProperty::SPtr propertyPtr
     * @return  T
     */
    template<typename T>
    static T FromProperty(sfProperty::SPtr propertyPtr)
    {
        if (propertyPtr == nullptr || propertyPtr->Type() != sfProperty::VALUE)
        {
            return T();
        }
        sfValueProperty::SPtr valuePtr = propertyPtr->AsValue();
        return *(reinterpret_cast<const T*>(valuePtr->GetValue().GetData().data()));
    }
};

#undef LOG_CHANNEL