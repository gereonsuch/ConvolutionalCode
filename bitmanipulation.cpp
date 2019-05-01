/******************************************************************************
 *                                                                            *
 * MIT License                                                                *
 *                                                                            *
 * Copyright 2019 Gereon Such                                                 *
 *                                                                            *
 * Permission is hereby granted, free of charge, to any person obtaining      *
 * a copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including        *
 * without limitation the rights to use, copy, modify, merge, publish,        *
 * distribute, sublicense, and/or sell copies of the Software, and to         *
 * permit persons to whom the Software is furnished to do so, subject         *
 * to the following conditions:                                               *
 *                                                                            *
 * The above copyright notice and this permission notice shall be             *
 * included in all copies or substantial portions of the Software.            *
 *                                                                            *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,            *
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES            *
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                   *
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS        *
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN         *
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN          *
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. *
 ******************************************************************************/

#include "bitmanipulation.h"

unsigned long numbits(unsigned long val){
    /** \brief get the number of bits set in the given value
     * 0 returns 0; 1 returns 1; 2,3 returns 2, 4,5,6,7 returns 3; and so on.
     *
     * @param[in] val Value, of which the set number of bits is returned.
     */
    unsigned int n=0;

    while(val!=0){
        val>>=1;
        n++;
    }
    return n;
}

unsigned long bitsequence_from_vector(std::vector<char> seq){
    /** \brief Write the bits in seq to an unsigned long container.
     * More than 64 Bits result in an overflow of the container.
     *
     * @param[in] seq Sequence of Bits. The values 0,1, ascii '0' and ascii '1' are allowed. Other values are skipped.
     */
    unsigned long bits=0;
    for(char b: seq){
        if(b==1 || b=='1'){
            bits<<=1; //shift old values so the lsb is free
            bits^=1; //set lsb to current bit
        }else if(b==0 || b=='0')
            bits<<=1; //no need to set lsb, just shift. the zero is set autmatically
        else continue; //skip if bit is unknown.
    }
    return bits;
}

char xor_all_bits(unsigned long val){
    /** \brief Returns the xor of all bits
     * Get the XOR product of all bits in an unsigned long value. This is equal to the modulo 2 of the hamming weight, but it is not calculated through hamming weight(number of ones in a bitsequence).
     *
     * @param[in] val Input value to get the XOR product.
     */
    //unsigned long = 64 bit
    val^=val>>32;
    val^=val>>16;
    val^=val>>8;
    val^=val>>4;
    val^=val>>2;
    val^=val>>1;
    return val&1;
}

std::vector<char> takeskip(std::vector<char> &vec, unsigned long take, unsigned long skip, unsigned long offset){
    /** \brief Takes and skips items from the given vector and returns the result
     *
     * \param[in] vec    Input items
     * \param[in] take   number of samples to take
     * \param[in] skip   number of samples to skip after the taken
     * \param[in] offset initial offset samples(init skip)
     */
    std::vector<char> output;
    output.reserve(vec.size());

    if(take==0 && skip==0) return output; //invalid input, return empty.

    while(offset<vec.size()){
        for(unsigned long i=0;i<take;i++)
            output.push_back(vec[offset++]);
        offset+=skip;
    }

    return output;
}
