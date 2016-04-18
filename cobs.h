/* COBS
 * Copyright (C) 2016 Nicola Corna <nicola@corna.info>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

/**
 * cobs_encode() - encode an array using COBS
 * @data_in:        input data
 * @length_in:      input data length
 * @data_out:       output data (can point to the same data as data_in)
 * @reserved_value: value reserved, to be removed from the array (typically 0)
 *
 * Return: output data length
 */
size_t cobs_encode(const uint8_t * const data_in, size_t length_in,
                   uint8_t * const data_out, const uint8_t reserved_value);

/**
 * cobs_decode() - decode a COBS-encoded array
 * @data_in:        input data
 * @length_in:      input data length
 * @data_out:       output data (can point to the same data as data_in)
 * @reserved_value: value reserved, to be removed from the array (typically 0)
 *
 * Return: output data length
 */
size_t cobs_decode(const uint8_t * const data_in, const size_t length_in,
                   uint8_t * const data_out, const uint8_t reserved_value);

/**
 * cobs_size() - worst-case size of a COBS-encoded array
 * @length:         input data length
 *
 * Return: worst-case data length
 */
size_t cobs_size(const size_t length);

/**
 * find_reserved_rev() - search for a value in an array backwards (internal use)
 * @data:           input data
 * @start:          start position
 * @reserved_value: value to be found
 *
 * Return: SIZE_MAX if reserved_value wasn't found, its index otherwise
 */
size_t find_reserved_rev(const uint8_t * const data, size_t start,
                         const uint8_t reserved_value);
