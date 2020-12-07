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
#include <ksMultiType.h>
#include "sfProperty.h"

namespace KS {
namespace SceneFusion2 {
    /**
     * A property that references a string. The string will be stored in the string table.
     */
    class EXTERNAL sfStringProperty :
        virtual public sfProperty
    {
    public:
        typedef std::shared_ptr<sfStringProperty> SPtr;

        /**
         * Static shared pointer constructor
         *
         * @param   const sfName& string
         * @return  SPtr
         */
        static SPtr Create(const sfName& string);

        /**
         * Destructor
         */
        virtual ~sfStringProperty() {};

        /**
         * Gets the string value.
         *
         * @return const sfName&
         */
        virtual const sfName& GetString() const = 0;

        /**
         * Sets the string value
         *
         * @param   const sfName& string
         */
        virtual void SetString(const sfName& string) = 0;
    };
} // SceneFusion2
} // KS