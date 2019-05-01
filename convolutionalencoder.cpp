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

#include "convolutionalencoder.h"

void ConvolutionalEncoder::set_generator(const std::vector<unsigned long> &G){
    /** \brief Sets the Generators for a given code.
     * Examples:
     *  - (5,7)   is 1/2 code with first generator 0b101 and second 0b111
     *  - (1,1,1) is a systematic, memoryless third order repetition code.
     *
     * \param[in] G Generator for a convolutional code. The least significant bit implies the direct tap(before the memory).
     */
    d_generator.clear();
    for(unsigned long g: G){
        if(g==0) continue; //

        d_generator.push_back(g);
    }
}

void ConvolutionalEncoder::set_generator(const std::vector<std::vector<char> > &G){
    /** \brief Sets the Generators for a given code.
     * Examples:
     *  - ((1,0,1),(1,1,1))   is 1/2 code with first generator 0b101 and second 0b111
     *  - ((1),(1),(1)) is a systematic, memoryless third order repetition code.
     *
     * \param[in] G Generator for a convolutional code. The least significant bit implies the direct tap(before the memory). For the inner chars 0, 1, ascii '0' and ascii '1' are valid.
     */
    std::vector<unsigned long> generator;
    generator.reserve(G.size());

    for(const std::vector<char> &g: G)
        generator.push_back(bitsequence_from_vector(g));

    this->set_generator(generator);
}

unsigned long ConvolutionalEncoder::get_annihilator(){
    /** \brief Returns the annihilator polynomial from the currently set generator.
     *
     * The annihilator is a "1/1 code", i.e. a pure convolution, which can be applied on the coded bits. For the correct offset, the result will even out, i.e. every Kth bits will be 0, where
     * K is the number of generators in the convolutional code. Construction of annihilator for Generators G, for example, G = {37,11} = {100101, 1011} as follows:
     *  100101
     *  001011   <- 1) pad zeros
     *
     *  100101
     *  001011   <- 2) start 2nd generator MSB
     *  *
     *
     *  100101   <- 3) take 1st generator MSG
     *  001011
     *  *
     *
     * proceed like this with all bit'columns' and add these to a binary number. The result is the annihilator polynom. In this example, the annihilator is anni = 010010011011 = 1179.
     */
    if(d_generator.size()==0)
        return 0;

    unsigned long anni=0;
    for(unsigned long i=get_constraintlen();i!=0;i--)
        for(unsigned long k=d_generator.size();k!=0;k--){
            anni<<=1;
            anni^=(d_generator[k-1]>>(i-1))&1;
        }
    return anni;
}

unsigned long ConvolutionalEncoder::get_memory(){
    /** \brief Returns the memory of the Convolutional encoder */

    unsigned long memlen=0;
    for(unsigned long g: d_generator)
        memlen=std::max(memlen, g==0 ? 0 : numbits(g)-1); //g==0 would lead to overflow with -1, thus it is excluded.
    return memlen;
}

std::vector<char> ConvolutionalEncoder::operator()(char bit){
    /** \brief Pushes a new bit through the encoder and returns the coded bits.
     *
     * @param[in] bit Input bit, 0, 1, ascii '0' and ascii '1' are valid.
     */
    if(bit==1 || bit=='1'){
        d_mem<<=1;
        d_mem^=1;
    }else if(bit==0 || bit=='0'){
        d_mem<<=1;
    }else return {}; //return empty if input is invalid

    std::vector<char> result;
    result.reserve(d_generator.size());

    for(unsigned long g: d_generator)
        result.push_back(xor_all_bits(d_mem&g));

    return result;
}

void ConvolutionalEncoder::operator() (std::vector<char> &output, char bit){
    /** \brief Convenience alias for operator(char bit)
     *
     * \param[out] output Output sequence, where the bits are appended.
     * \param[in]  bit    Input bit, 0, 1, ascii '0' and ascii '1' are valid.
     */
    std::vector<char> result=(*this)(bit);

    output.insert(output.end(), result.begin(), result.end());
}

void ConvolutionalEncoder::operator()(std::vector<char> &output, const std::vector<char> &bits){
    /** \brief Pushes a new bit through through encoder and pushes the encoded bits to output.
     *
     * \param[out] output Output sequence, where the bits are appended.
     * \param[in]  bits   Input bits, 0, 1, ascii '0' and ascii '1' are valid. Other values are discarded without error!
     */

    //allocate enough memory in output sequence for all encoded bits _additionally_
    output.reserve(output.size() + bits.size()*this->K());

    for(char bit: bits){
        if(bit==1 || bit=='1')      bit=1;
        else if(bit==0 || bit=='0') bit=0;
        else continue; //skip values that do not match for performance reasons.

        d_mem<<=1;
        d_mem^=bit;

        for(unsigned long g: d_generator)
            output.push_back(xor_all_bits(d_mem&g));
    }
}









