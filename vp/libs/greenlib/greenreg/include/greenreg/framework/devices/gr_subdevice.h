/*
Copyright (c) 2008, Intel Corporation
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// ChangeLog GreenSocs
// 2009-01-21 Christian Schroeder: changed namespace
// 2009-01-29 Christian Schroeder: renamings
// 2009-01-29 FURTHER CHANGES SEE SVN LOG!


#ifndef _GR_SUBDEVICE_H_
#define _GR_SUBDEVICE_H_

#include <boost/config.hpp>
#include <systemc>

#include "greenreg/internal/device/subdevice_container.h"
#include "greenreg/sysc_ext/kernel/gr_attribute_type_container.h"
#include "greenreg/sysc_ext/kernel/gr_switch_container.h"

namespace gs {
namespace reg {

////////////////////////////////////////////
/// gr_subdevice
/// represents a sub block of logic to its parent
/// which can either be another gr_subdevice or
/// a gr_device.  All of the containers accessable
/// from the device are accessible here via a
/// a layer of transparent wrappers.
/// @author 
/// @since 
////////////////////////////////////////////
//template< class USER_MODULE>
class gr_subdevice : public gs::reg::subdevice
{
public:

// allow gr_device to define it's own processes
//	SC_HAS_PROCESS( USER_MODULE);

	////////////////////////////////////////////
	/// gr_subdevice
	/// explicit constructor for a subdevice
	///
	/// @param _name ? name of module
	/// @param _parent ? parent of this subdevice, FORCED pass by reference, MUST EXIST
	////////////////////////////////////////////
	explicit gr_subdevice( sc_core::sc_module_name _name, I_device & _parent)
	: subdevice( _name, _parent)
	{
	}

	virtual ~gr_subdevice()
	{
	}

	////////////////////////////////////////////
	/// kind
	/// implements kind for the user...
	///
	/// @return string container full templated device name
	///
	/// @see sc_object
	////////////////////////////////////////////
	virtual const char * kind() const
	{
		std::stringstream ss;
		ss << "gr_subdevice<" << basename() << ">";
		return( ss.str().c_str()) ;
	}

// data members
public:

protected:

};

} // end namespace gs:reg
} // end namespace gs:reg

#endif /*_GR_SUBDEVICE_H_*/
