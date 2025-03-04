/******************************************************************************
 * Copyright 2023 ETC Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************
 * This file is a part of RDM. For more information, go to:
 * https://github.com/ETCLabs/RDM
 *****************************************************************************/

/**
 * @file rdm/version.h
 * @brief Provides the current version of the RDM library.
 *
 * This file is provided for application use; the values defined in this file are not used
 * internally by the library.
 */

#ifndef RDM_VERSION_H_
#define RDM_VERSION_H_

/* clang-format off */

/** 
 * @defgroup rdm RDM
 * @brief Implementation of the RDM protocol.
 *
 * @{
 */

/**
 * @name RDM version numbers
 * @{
 */
#define RDM_VERSION_MAJOR 0 /**< The major version. */
#define RDM_VERSION_MINOR 3 /**< The minor version. */
#define RDM_VERSION_PATCH 0 /**< The patch version. */
#define RDM_VERSION_BUILD 3 /**< The build number. */
/**
 * @}
 */

/**
 * @name RDM version strings
 * @{
 */
#define RDM_VERSION_STRING      "0.3.0.3" /**< The 4-digit version string. */
#define RDM_VERSION_DATESTR     "27.Feb.2025" /**< The date this version was released (dd.Mm.yyyy). */
#define RDM_VERSION_COPYRIGHT   "Copyright 2025 ETC Inc." /**< The version's copyright string. */
#define RDM_VERSION_PRODUCTNAME "RDM" /**< The version's product name. */
/**
 * @}
 */

/**
 * @}
 */

#endif /* RDM_VERSION_H_ */
