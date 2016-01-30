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

#include "BaseInMemoryCertificateStore.hpp"

namespace te
{
	namespace httpengine
	{
		namespace mitm
		{
			namespace secure
			{
				
				/// <summary>
				/// The WindowsInMemoryCertificateStore offers no functionality over the
				/// BaseInMemoryCertificateStore class, except to implement the platform specific,
				/// pure virtual functions declared in BaseInMemoryCertificateStore.
				/// </summary>
				class WindowsInMemoryCertificateStore : public BaseInMemoryCertificateStore
				{

				public:

					/// <summary>
					/// Default constructor, delegates to the parameterized constructure which takes
					/// country code, organization name and common name, with default values. Be
					/// advised that the constructor that this delegates to can throw.
					/// 
					/// Automatically writes the CA certificate generated at construction to a .pem
					/// file with the same name as was supplied for the CA Common Name property.
					/// </summary>
					WindowsInMemoryCertificateStore();

					/// <summary>
					/// Constructs a new WindowsInMemoryCertificateStore and generates a self signed CA
					/// certificate, storing the generated EVP_PKEY and X509 structures in the
					/// m_thisCaKeyPair and m_thisCa members respectively. This constructor invokes
					/// members that can throw.
					/// 
					/// Automatically writes the CA certificate generated at construction to a .pem
					/// file with the same name as was supplied for the CA Common Name property.
					/// </summary>
					/// <param name="countryCode">
					/// The country code for the self signed CA to be generated.
					/// </param>
					/// <param name="organizationName">
					/// The organization name for the self signed CA to be generated.
					/// </param>
					/// <param name="commonName">
					/// The common name for the self signed CA to be generated.
					/// </param>
					WindowsInMemoryCertificateStore(
						const std::string& countryCode,
						const std::string& organizationName,
						const std::string& commonName
						);

					virtual ~WindowsInMemoryCertificateStore();

					/// <summary>
					/// Attempts to install the current temporary root CA certificate for
					/// transparent filtering to the appropriate OS specific filesystem certificate
					/// store. This must be overridden in an os specific derrived class.
					/// 
					/// This method is assumed to throw in all derrived types, so runtime_errors
					/// need to be expected and correctly handled.
					/// </summary>
					/// <returns>
					/// True if the operation succeeded and the current temporary root CA
					/// certificate was installed to the appropriate OS filesystem certificate
					/// store. False otherwise.
					/// </returns>
					virtual bool EstablishOsTrust();

					/// <summary>
					/// Searches the OS filesystem certificate store for any installed root CA
					/// certificates generated by this program and deletes them. Although there
					/// isn't any harm in not doing this, not doing this would make an awful mess,
					/// potentially building up useless CA certificates in the client's OS. The
					/// private key is of course never stored with the certificates, but rather is
					/// kept in memory. This is just about cleaning up our garbage. This must be
					/// overridden in an os specific derrived class.
					/// 
					/// This method is assumed to throw in all derrived types, so runtime_errors
					/// need to be expected and correctly handled.
					/// </summary>
					virtual void RevokeOsTrust();

				};

			} /* namespace secure */
		} /* namespace mitm */
	} /* namespace httpengine */
} /* namespace te */
