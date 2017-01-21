/*
* Copyright (c) 2016 Jesse Nicholson.
*
* This file is part of Http Filtering Engine.
*
* Http Filtering Engine is free software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or (at
* your option) any later version.
*
* In addition, as a special exception, the copyright holders give
* permission to link the code of portions of this program with the OpenSSL
* library.
*
* You must obey the GNU General Public License in all respects for all of
* the code used other than OpenSSL. If you modify file(s) with this
* exception, you may extend this exception to your version of the file(s),
* but you are not obligated to do so. If you do not wish to do so, delete
* this exception statement from your version. If you delete this exception
* statement from all source files in the program, then also delete it
* here.
*
* Http Filtering Engine is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
* Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with Http Filtering Engine. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once


#include "util/cb/EngineCallbackTypes.h"

#ifndef PVOID
typedef void* PVOID;
#endif

#ifdef HTTP_FILTERING_ENGINE_EXPORT
	#ifdef _MSC_VER
		#define HTTP_FILTERING_ENGINE_API __declspec(dllexport)
	#else
		#define HTTP_FILTERING_ENGINE_API __attribute__((visibility("default")))
	#endif // #ifdef _MSC_VER
#else
	#ifdef _MSC_VER
		#define HTTP_FILTERING_ENGINE_API __declspec(dllimport)
	#else
		#define HTTP_FILTERING_ENGINE_API
	#endif
#endif // #ifdef HTTP_FILTERING_ENGINE_EXPORT

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	/*
	struct HttpFilteringEngineCtl;
	typedef struct HttpFilteringEngineCtl HttpFilteringEngineCtl;
	typedef HttpFilteringEngineCtl* PVOID;
	typedef PVOID* PPVOID;
	*/
	
	//#pragma managed(push, off)

	/*
	struct HttpFilteringEngineCtl;
	typedef struct HttpFilteringEngineCtl* PVOID;
	typedef PVOID* PPVOID;
	*/
	//#pragma managed(pop)

	// Woa, major change in coding here. This guy can't even stay consistent with his coding
	// convention. Tsk Tsk, shake head, etc. Even though looking at those thin little underscores
	// are like razor blades sliding across my eyeballs as I scan the screen reading them, some of
	// the names used in the C API are very long. Very long names are the Achilles' heel of camel case.

	/// <summary>
	/// Creates a new instance of the HttpFilteringEngineCtl class, which manages the operation of
	/// the HTTP Filtering Engine.
	///
	/// It's a little messy having so many defined callbacks required at creation time, but there is
	/// a lot of information that the Engine can report and request, and is also multithreaded in
	/// nearly every aspect. Avoiding incurring the cost of synchronizing callbacks post creation is
	/// the reason for having this design here.
	/// </summary>
	/// <param name="firewallCb">
	/// A pointer to a method that is meant to determine if the supplied absolute binary path points
	/// to a binary that has been approved for internet access. Required.
	/// </param>
	/// <param name="caBundleAbsolutePath">
	/// An optional, but recommended, pointer to a string of the absolute path of a CA bundle to be
	/// used for upstream client contexts for certificate verification. If nullptr is supplied,
	/// caBundleAbsolutePathLength should be set to zero.
	/// </param>
	/// <param name="caBundleAbsolutePathLength">
	/// The length of the previous caBundleAbsolutePath string. Supply zero if nullptr was supplied
	/// to the caBundleAbsolutePath argument.
	/// </param>
	/// <param name="blockedHtmlPage">
	/// An optional, but recommended, pointer to a HTML string to display whenever a HTML payload is
	/// blocked.
	/// </param>
	/// <param name="blockedHtmlPageLength">
	/// The length of the previous blockedHtmlPage string. Supply zero if nullptr was supplied to the
	/// blockedHtmlPage argument.
	/// </param>
	/// <param name="httpListenerPort">
	/// The desired port for the proxy to listen for incoming plain TCP HTTP clients on. Recommended
	/// to supply a value of zero, as it is recommended to allow the OS to select an available port
	/// from the ephimeral port range.
	/// </param>
	/// <param name="httpsListenerPort">
	/// The desired port for the proxy to listen for incoming secure HTTP clients on. Recommended to
	/// supply a value of zero, as it is recommended to allow the OS to select an available port from
	/// the ephimeral port range.
	/// </param>
	/// <param name="numThreads">
	/// The number of thread to be run against the io_service that drives the proxy and all
	/// associated functionality, barring the platform dependent Diverter. If a value of zero is
	/// supplied, the value will be set internally to the number of logical cores on the device. Be
	/// advised that these threads are the same threads that execute the filtering functionality.
	/// </param>
	/// <param name="onClassify">
	/// A pointer to a method that can accept a byte array along with a mime-type and attempt to
	/// perform classification of the supplied binary content.
	/// </param>
	/// <param name="onInfo">
	/// A pointer to a method that can accept string informational data generated by the underlying
	/// Engine. This callback cannot be supplied post-construction.
	/// </param>
	/// <param name="onWarn">
	/// A pointer to a method that can accept string warning data generated by the underlying Engine.
	/// This callback cannot be supplied post-construction.
	/// </param>
	/// <param name="onError">
	/// A pointer to a method that can accept string error data generated by the underlying Engine.
	/// This callback cannot be supplied post-construction.
	/// </param>
	/// <param name="onRequestBlocked">
	/// A pointer to a method that can accept information about blocked requests generated by the
	/// underlying Engine. This callback cannot be supplied post-construction.
	/// </param>
	/// <param name="onElementsBlocked">
	/// A pointer to a method that can accept information about HTML elements removed by CSS selects,
	/// generated by the underlying Engine. This callback cannot be supplied post-construction.
	/// </param>
	/// <returns>
	/// A valid pointer to the created instance if the call succeeded, nullptr otherwise.
	/// </returns>
	extern HTTP_FILTERING_ENGINE_API PVOID fe_ctl_create(
		FirewallCheckCallback firewallCb,
		const char* caBundleAbsolutePath,
		uint32_t caBundleAbsolutePathLength,
		const char* blockedHtmlPage,
		uint32_t blockedHtmlPageLength,
		uint16_t httpListenerPort,
		uint16_t httpsListenerPort,
		uint32_t numThreads,
		ClassifyContentCallback onClassify,
		ReportMessageCallback onInfo,
		ReportMessageCallback onWarn,
		ReportMessageCallback onError,
		ReportBlockedRequestCallback onRequestBlocked,
		ReportBlockedElementsCallback onElementsBlocked
		);

	/// <summary>
	/// Destroys an existing Engine instance. If the Engine is running, it will be correctly shut
	/// down. Regardless of its state, the Engine instance pointed to will be destroyed and the
	/// supplied ptr argument will no longer be valid.
	/// </summary>
	/// <param name="ptr">
	/// A valid pointer to an existing Engine instance.
	/// </param>
	extern HTTP_FILTERING_ENGINE_API void fe_ctl_destroy(PVOID* ptr);

	/// <summary>
	/// Destroys an existing Engine instance. If the Engine is running, it will be correctly shut
	/// down. Regardless of its state, the Engine instance pointed to will be destroyed and the
	/// supplied ptr argument will no longer be valid. The difference with this version over
	/// the fe_ctl_destroy method is that it does not take a pointer to pointer, so users
	/// have to take care that they don't double free. This is unfortunately required for
	/// using correctly from C++ CLI.
	/// </summary>
	/// <param name="ptr">
	/// A valid pointer to an existing Engine instance.
	/// </param>
	extern HTTP_FILTERING_ENGINE_API void fe_ctl_destroy_unsafe(PVOID ptr);

	/// <summary>
	/// Begins intercepting and diverting HTTP/S traffic through the Engine.
	/// </summary>
	/// <param name="ptr">
	/// A valid pointer to an existing Engine instance.
	/// </param>
	extern HTTP_FILTERING_ENGINE_API const bool fe_ctl_start(PVOID ptr);

	/// <summary>
	/// Stops intercepting and diverting HTTP/S traffic through the Engine.
	/// </summary>
	/// <param name="ptr">
	/// A valid pointer to an existing Engine instance.
	/// </param>
	extern HTTP_FILTERING_ENGINE_API void fe_ctl_stop(PVOID ptr);

	/// <summary>
	/// Checks if the Engine is actively diverting and filtering HTTP/S traffic or not.
	/// </summary>
	/// <param name="ptr">
	/// A valid pointer to an existing Engine instance.
	/// </param>
	/// <returns>
	/// True if the Engine is actively diverting and filtering HTTP/S traffic, false otherwise.
	/// </returns>
	extern HTTP_FILTERING_ENGINE_API const bool fe_ctl_is_running(PVOID ptr);

	/// <summary>
	/// Gets the port that the Engine is listening on for diverted HTTP connections.
	/// </summary>
	/// <param name="ptr">
	/// A valid pointer to an existing Engine instance.
	/// </param>
	/// <returns>
	/// The port the Engine is listening on for diverted HTTP connections.
	/// </returns>
	extern HTTP_FILTERING_ENGINE_API uint16_t fe_ctl_get_http_listener_port(PVOID ptr);

	/// <summary>
	/// Gets the port that the Engine is listening on for diverted HTTPS connections.
	/// </summary>
	/// <param name="ptr">
	/// A valid pointer to an existing Engine instance.
	/// </param>
	/// <returns>
	/// The port the Engine is listening on for diverted HTTPS connections
	/// </returns>
	extern HTTP_FILTERING_ENGINE_API uint16_t fe_ctl_get_https_listener_port(PVOID ptr);

	/// <summary>
	/// Checks whether the queried option is enabled in the Engine. Options are specific, and
	/// library users should be provided with enumerations of these options in their respective
	/// languages so ensure correct functionality and clarity of intent for each option. Also, do
	/// not confuse options with categories. Options are preset values which govern specific,
	/// special functionality of the Engine. Categories are any non-zero user defined types that the
	/// Engine only uses to allow users to categorize rules loaded by the user.
	/// </summary>
	/// <param name="ptr">
	/// A valid pointer to an existing Engine instance.
	/// </param>
	/// <param name="optionId">
	/// The ID of the option to query.
	/// </param>
	/// <returns>
	/// True if the option queried is enabled, false otherwise.
	/// </returns>
	extern HTTP_FILTERING_ENGINE_API const bool fe_ctl_get_option(PVOID ptr, const uint32_t optionId);

	/// <summary>
	/// Sets whether the queried option is enabled in the Engine or not. Options are specific, and
	/// library users should be provided with enumerations of these options in their respective
	/// languages so ensure correct functionality and clarity of intent for each option. Also, do
	/// not confuse options with categories. Options are preset values which govern specific,
	/// special functionality of the Engine. Categories are any non-zero user defined types that the
	/// Engine only uses to allow users to categorize rules loaded by the user.
	/// </summary>
	/// <param name="ptr">
	/// A valid pointer to an existing Engine instance.
	/// </param>
	/// <param name="optionId">
	/// The ID of the option to change the value of.
	/// </param>
	/// <param name="val">
	/// The value to set for the supplied option.
	/// </param>
	extern HTTP_FILTERING_ENGINE_API void fe_ctl_set_option(PVOID ptr, const uint32_t optionId, const bool val);

	/// <summary>
	/// Sets whether the queried user defined category is enabled in the Engine or not. Categories
	/// are user defined numeric values which the engine is agnostic to the underlying meaning of
	/// specific values. The Engine uses these values to index rules loaded from various lists to
	/// simply provide the user the capability to turn certain lists on and off. The only
	/// restriction is that the user cannot supply a zero value for categories, as this is reserved
	/// by the Engine for the "unfiltered" category.
	/// 
	/// Also, do not confuse options with categories. Options are preset values which govern
	/// specific, special functionality of the Engine. Categories are any non-zero user defined
	/// types that the Engine only uses to allow users to categorize rules loaded by the user.
	/// </summary>
	/// <param name="ptr">
	/// A valid pointer to an existing Engine instance.
	/// </param>
	/// <param name="categoryId">
	/// The ID of the category to query. Must be non-zero. Supplying a zero value for the category
	/// will result in an immediate false return value.
	/// </param>
	/// <returns>
	/// True if the category queried is enabled, false otherwise.
	/// </returns>
	extern HTTP_FILTERING_ENGINE_API const bool fe_ctl_get_category(PVOID ptr, const uint8_t categoryId);

	/// <summary>
	/// Gets whether the queried user defined category is enabled in the Engine or not. Categories
	/// are user defined numeric values which the engine is agnostic to the underlying meaning of
	/// specific values. The Engine uses these values to index rules loaded from various lists to
	/// simply provide the user the capability to turn certain lists on and off. The only
	/// restriction is that the user cannot supply a zero value for categories, as this is reserved
	/// by the Engine for the "unfiltered" category.
	/// 
	/// Also, do not confuse options with categories. Options are preset values which govern
	/// specific, special functionality of the Engine. Categories are any non-zero user defined
	/// types that the Engine only uses to allow users to categorize rules loaded by the user.
	/// </summary>
	/// <param name="ptr">
	/// A valid pointer to an existing Engine instance.
	/// </param>
	/// <param name="categoryId">
	/// The ID of the category to change the value of. Must be non-zero. Supplying a zero value for
	/// the category will result in an immediate return, and no change will be made internally.
	/// </param>
	/// <param name="val">
	/// The value to set for the supplied category.
	/// </param>
	extern HTTP_FILTERING_ENGINE_API void fe_ctl_set_category(PVOID ptr, const uint8_t categoryId, const bool val);

	/// <summary>
	/// Attempts to have the Engine load an Adblock Plus formatted list containing filtering and
	/// hiding rules from the filesystem.
	/// </summary>
	/// <param name="ptr">
	/// A valid pointer to an existing Engine instance.
	/// </param>
	/// <param name="filePath">
	/// A pointer to a string containing the absolute path to the file to be loaded.
	/// </param>
	/// <param name="filePathLength">
	/// The total length of the supplied file path string.
	/// </param>
	/// <param name="listCategory">
	/// The category that the rules loaded from the list should be classified as belonging to. This
	/// is entirely user specified and the Engine is **mostly** agnostic to the meaning of these
	/// values. The value zero is reserved to represent the "unfiltered" category. Aside from this,
	/// whatever other value these categories are are has no bearing on internal functionality.
	/// </param>
	/// <param name="flushExisting">
	/// Whether or not to flush the existing entries in the category before loading new entries.
	/// </param>
	/// <param name="rulesLoaded">
	/// A pointer to set, if non-null, indicating the total number of rules successfully loaded and
	/// parsed from the source.
	/// </param>
	/// <param name="rulesFailed">
	/// A pointer to set, if non-null, indicating the total number of rules that failed to load and
	/// or be parsed from the source.
	/// </param>
	extern HTTP_FILTERING_ENGINE_API void fe_ctl_load_list_from_file(
		PVOID ptr, 
		const char* filePath, 
		const size_t filePathLength, 
		const uint8_t listCategory,
		const bool flushExisting,
		uint32_t* rulesLoaded,
		uint32_t* rulesFailed
		);

	/// <summary>
	/// Attempts to have the Engine treat the supplied string as an Adblock Plus formatted list
	/// containing filtering and hiding rules from the filesystem, parsing them from the string.
	/// </summary>
	/// <param name="ptr">
	/// A valid pointer to an existing Engine instance.
	/// </param>
	/// <param name="listString">
	/// A pointer to a string containing the Adblock Plus formatted list of filtering rules.
	/// </param>
	/// <param name="listStringLength">
	/// The total length of the supplied list string.
	/// </param>
	/// <param name="listCategory">
	/// The category that the rules loaded from the list should be classified as belonging to. This
	/// is entirely user specified and the Engine is **mostly** agnostic to the meaning of these
	/// values. The value zero is reserved to represent the "unfiltered" category. Aside from this,
	/// whatever other value these categories are are has no bearing on internal functionality.
	/// </param>
	/// <param name="flushExisting">
	/// Whether or not to flush the existing entries in the category before loading new entries.
	/// </param>
	/// <param name="rulesLoaded">
	/// A pointer to set, if non-null, indicating the total number of rules successfully loaded and
	/// parsed from the source.
	/// </param>
	/// <param name="rulesFailed">
	/// A pointer to set, if non-null, indicating the total number of rules that failed to load and
	/// or be parsed from the source.
	/// </param>
	extern HTTP_FILTERING_ENGINE_API void fe_ctl_load_list_from_string(
		PVOID ptr, 
		const char* listString, 
		const size_t listStringLength, 
		const uint8_t listCategory,
		const bool flushExisting,
		uint32_t* rulesLoaded,
		uint32_t* rulesFailed
		);

	/// <summary>
	/// Attempts to have the Engine load text triggers, separated by newline, from the supplied file
	/// path.
	/// </summary>
	/// <param name="ptr">
	/// A valid pointer to an existing Engine instance.
	/// </param>
	/// <param name="listString">
	/// A pointer to a string containing the path to the text triggers file to load.
	/// </param>
	/// <param name="listStringLength">
	/// The total length of the supplied file path string.
	/// </param>
	/// <param name="category">
	/// The category that the rules loaded from the list should be classified as belonging to. This
	/// is entirely user specified and the Engine is **mostly** agnostic to the meaning of these
	/// values. The value zero is reserved to represent the "unfiltered" category. Aside from this,
	/// whatever other value these categories are are has no bearing on internal functionality.
	/// </param>
	/// <param name="flushExisting">
	/// Whether or not to flush the existing entries in the category before loading new entries.
	/// </param>
	/// <param name="rulesLoaded">
	/// A pointer to set, if non-null, indicating the total number of rules successfully loaded and
	/// parsed from the source.
	/// </param>
	extern HTTP_FILTERING_ENGINE_API void fe_ctl_load_text_triggers_from_file(
		PVOID ptr,
		const char* filePath,
		const size_t filePathLength,
		const uint8_t category,
		const bool flushExisting,
		uint32_t* rulesLoaded
	);

	/// <summary>
	/// Attempts to have the Engine load text triggers, separated by newline, from the supplied
	/// string.
	/// </summary>
	/// <param name="ptr">
	/// A valid pointer to an existing Engine instance.
	/// </param>
	/// <param name="listString">
	/// A pointer to a string containing the text triggers to load.
	/// </param>
	/// <param name="listStringLength">
	/// The total length of the supplied text triggers string.
	/// </param>
	/// <param name="listCategory">
	/// The category that the rules loaded from the list should be classified as belonging to. This
	/// is entirely user specified and the Engine is **mostly** agnostic to the meaning of these
	/// values. The value zero is reserved to represent the "unfiltered" category. Aside from this,
	/// whatever other value these categories are are has no bearing on internal functionality.
	/// </param>
	/// <param name="flushExisting">
	/// Whether or not to flush the existing entries in the category before loading new entries.
	/// </param>
	/// <param name="rulesLoaded">
	/// A pointer to set, if non-null, indicating the total number of rules successfully loaded and
	/// parsed from the source.
	/// </param>
	extern HTTP_FILTERING_ENGINE_API void fe_ctl_load_text_triggers_from_string(
		PVOID ptr,
		const char* triggersString,
		const size_t triggersStringLength,
		const uint8_t category,
		const bool flushExisting,
		uint32_t* rulesLoaded
	);

	/// <summary>
	/// Gets the current root CA being used by the Engine, if any, in PEM format. Memory is allocated
	/// inside the function and the corresponding pointer is assigned to the bufferPP parameter. The
	/// user must call free() on the buffer in the event that the bufferSize parameter has a value
	/// greater than zero after the call.
	/// </summary>
	/// <param name="ptr">
	/// A valid pointer to an existing Engine instance.
	/// </param>
	/// <param name="bufferPP">
	/// A pointer to a char pointer to be populated by the operation.
	/// </param>
	/// <param name="bufferSize">
	/// A pointer to a size_t object that will hold the total number of elements in the populated
	/// array.
	/// </param>
	extern HTTP_FILTERING_ENGINE_API void fe_ctl_get_rootca_pem(PVOID ptr, char** bufferPP, size_t* bufferSize);

	/// <summary>
	/// Unloads any and all rules for the given category.
	/// </summary>
	/// <param name="ptr">
	/// A valid pointer to an existing Engine instance.
	/// </param>
	/// <param name="category">
	/// The category for which to unload any and all rules.
	/// </param>
	extern HTTP_FILTERING_ENGINE_API void fe_ctl_unload_rules_for_category(PVOID ptr, const uint8_t category);

	/// <summary>
	/// Unloads any and all text triggers for the given category.
	/// </summary>
	/// <param name="ptr">
	/// A valid pointer to an existing Engine instance.
	/// </param>
	/// <param name="category">
	/// The category for which to unload any and all text triggers.
	/// </param>
	extern HTTP_FILTERING_ENGINE_API void fe_ctl_unload_text_triggers_for_category(PVOID ptr, const uint8_t category);

#ifdef __cplusplus
};
#endif // __cplusplus