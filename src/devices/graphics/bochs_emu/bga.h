//
// Part of Metta OS. Check https://atta-metta.net for latest version.
//
// Copyright 2007 - 2017, Stanislav Karchebnyy <berkus@atta-metta.net>
//
// Distributed under the Boost Software License, Version 1.0.
// (See file LICENSE_1_0.txt or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "types.h"

class pci_device_t;

namespace graphics {

/**
 * Bochs BGA graphics adapter driver.
 *
 * Presently supports only LFB (see no reason to use banked modes).
 */
class bga
{
	void* lfb{(void*)0xe0000000};
	int xres_max{640}, yres_max{480}, bpp_max{8};

	void reg_write(int regno, uint16_t value);
	uint16_t reg_read(int regno);

	/**
	 * Obtain maximum supported resolution.
	 */
	void set_caps();

public:
	bga() {}

	/**
	 * Check for BGA support.
	 * @return true if BGA is available, false otherwise.
	 */
	bool is_available();
	void configure(pci_device_t* card);
	void init();
	void set_mode(int width, int height, int bpp);

	inline void* get_lfb() { return lfb; }
};

} // namespace graphics
