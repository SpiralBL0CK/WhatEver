#include "hzip.hpp"

std::string stobyte(std::string& sBits)
{
    std::string sBytes;

    std::size_t numberOfBits = 0;
    bit_t bit = 0;

     /* Iterate through string of bits 
    * If there are not enough 8 bits (to construct 1 byte) 
    * when reaching the end of string bits then
    * 0 will be filled. That's the purpose of "+ numberOfBits"
    */

    for(std::size_t i = 0; i < sBits.size();++i)
    {
        (sBits[i] == '0') ? (bit = 0): (bit = 1);

        // Initialise byteChar once
        static char byteChar = 0;

        byteChar = (byteChar << 1) | bit;
        ++numberOfBits;
        if (numberOfBits == MAX_NUMBER_OF_BITS) {
            // then add that byte to the string
            sBytes += byteChar;
        
            // and reset byteChar and numberOfBits for the next iteration
            byteChar = 0;
            numberOfBits = 0;
        }
    }
    return sBytes;
}

Hzip::HZip()
{}

frmap_t
HZip::buildFrequencyTable(const std::string& input)
{
    frmap_t freqTable;

    for (std::size_t i = 0; i < input.size(); ++i)
        ++freqTable[hchar_t(input[i])]; // cast input[i] from char to hchar_t 

    // push a PSEDO_EOF to frequency table with freq = 1
    freqTable[PSEUDO_EOF] = 1;

    return freqTable;
}

NODE* HZip::buildEncodingTree(frmap_t freqTable)
{
     // push each char and its frequency to min heap
    for (auto it = freqTable.begin(); it != freqTable.end(); ++it)
        m_minHeap.push(new NODE(it->first, it->second));

    // push until there is just 1 node left (root)
    while (m_minHeap.size() > 1) {
        NODE* lChild  = m_minHeap.top();
         m_minHeap.pop();

        // get next least frequency node
        NODE* rChild = m_minHeap.top();
        m_minHeap.pop();
        /*
        * Make a parent node with
        * key is a char or a NOT_A_CHAR which indicates an internal node
        * value is the sum of 2 least frequency
        */
        NODE* parent = new NODE(NOT_A_CHAR, lChild->_freq + rChild->_freq);

        // Link to its children
        parent->_lChild = lChild;
        parent->_rChild = rChild;

        m_minHeap.push(parent);
    }
    // Top of min heap is the root of built tree
    return m_minHeap.top();
}

int main()
{
    return 0;
}