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

#include "cobs.h"

size_t cobs_encode(const uint8_t * const data_in, size_t length_in,
                   uint8_t * const data_out, const uint8_t reserved_value)
{
    uint8_t block_length = 1;
    size_t index_in;
    size_t index_out = length_in;
    size_t reserved_index;
    
    for (index_in = 0; index_in < length_in; index_in++)
    {
        if (data_in[index_in] == reserved_value)
            block_length = 1;
        else
        {
            if (++block_length == COBS_MAX_OFFSET)
            {
                ++index_out;
                block_length = 1;
            }
        }
    }

    --index_in;
    block_length = 0;
    reserved_index = find_reserved_rev(data_in, index_in, reserved_value);
    length_in = index_out + 1;

    while (index_in != SIZE_MAX)
    {
        if (data_in[index_in] == reserved_value)
        {
            if (block_length >= reserved_value)
                ++block_length;

            data_out[index_out] = block_length;
            block_length = 0;
            reserved_index = find_reserved_rev(data_in, index_in - 1,
                                               reserved_value);
        }
        else
        {
            if ((index_in - reserved_index) % (COBS_MAX_OFFSET - 1) == 0)
            {
                if (block_length >= reserved_value)
                    ++block_length;
                    
                data_out[index_out] = block_length;
                block_length = 0;
                --index_out;
            }
         
            data_out[index_out] = data_in[index_in];
            ++block_length;
        }

        --index_out;
        --index_in;
    }

    if (block_length >= reserved_value)
        ++block_length;

    data_out[0] = block_length;

    return length_in;
}

size_t cobs_decode(const uint8_t * const data_in, const size_t length_in,
                   uint8_t * const data_out, const uint8_t reserved_value)
{
    size_t reserved_index = data_in[0] + 1;
    size_t index_in;
    size_t index_out = 0;
    uint8_t valid_zero = (data_in[0] == COBS_MAX_OFFSET ? 0 : 1);
    
    if (data_in[0] >= reserved_value)
        --reserved_index;

    for (index_in = 1; index_in < length_in; index_in++)
    {
        if (index_in == reserved_index)
        {
            if (valid_zero)
            {
                reserved_index += data_in[index_in] + 1;
                valid_zero = (data_in[index_in] == COBS_MAX_OFFSET ? 0 : 1);
                data_out[index_out] = reserved_value;
                ++index_out;
            }
            else
            {
                reserved_index += data_in[index_in] + 1;
                valid_zero = (data_in[index_in] == COBS_MAX_OFFSET ? 0 : 1);
            }
            
            if (data_in[index_in] >= reserved_value)
                --reserved_index;
        }
        else
        {
            data_out[index_out] = data_in[index_in];
            ++index_out;
        }
    }
    
    return index_out;
}

size_t find_reserved_rev(const uint8_t * const data, size_t start,
                         const uint8_t reserved_value)
{
    while (data[start] != reserved_value && start != SIZE_MAX)
        --start;
    
    return start;
}

size_t cobs_size(const size_t length)
{
    return length + length / COBS_MAX_OFFSET + 1;
}

