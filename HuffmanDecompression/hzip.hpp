#include <string>
#include <vector>
#include <map>
#include <queue>

typedef unsigned short hchar_t;
typedef unsigned int hfreq_t;
typedef std::map<hchar_t, hfreq_t> frmap_t;
typedef std::map<hchar_t,std::string> hmap_t;
typedef char bit_t;

const hchar_t PSEUDO_EOF = 256;
const hchar_t NOT_A_CHAR = 257; // Write to internal nodes
const bit_t MAX_NUMBER_OF_BITS = 8;

std::string stobyte(std::string* sBits);

struct NODE
{
	hchar_t _character;
	hfreq_t _freq;

	NODE* _lChild;
	NODE* _rChild;

	NODE(hchar_t character, hfreq_t freq):
		_character(character),_freq(freq),
		_lChild(nullptr),_rChild(nullptr){}

	~NODE(){
		delete _lChild;
		delete _rChild;
	}

};

struct Comparator
{
	bool operator()(NODE* lChild,NODE* rChild)
	{
		return lChild->_freq > rChild->_freq;	
	}

};

class HZip
{
	public:
		Hzip();
	/*
    	* Build a frequency table from input string
    	* and return a frequency map.
    	*/
		frmap_t buildFrequencyTable(const std::string& input);

		/*Build a Huffman tree from frequency table and return the root.
		*/
		NODE* buildEncodingTree(frmap_t freqTable);

		/*Get encoded map from the root of encoded tree	and reurn encoded map.
		* */
		hmap_t getEncodedMap(NODE* encodingTree);
		/*Read characters from input string and write bytes to output
		* used for compression*/
		void encodeData(std::string& input, const frmap_t freqMap, hmap_t encodedMap, std::vector<std::string>& output);

		/*
		* Construct frequency table from the header 
		* of the input string.
		* ------------------------------------------
		* Used for decompression
		*/
		frmap_t headerProcessing(std::string& input);

	/*
		* Read bits by bits from the input string
		* and write character to the output.
		* ----------------------------------------
		* Used for decompression
		*/
		void decodeData(std::string& input, NODE* encodingTree, std::string& output);

	protected:
		  /*
		* Build Huffman encoding map.
		* ----------------------------
		* Recursive approach.
		*/
		void buildEncodingMap(NODE* encodingTree, std::string sCode);
		 hmap_t p_encodedMap;
	private:
    	std::priority_queue<NODE*, std::vector<NODE*>, Comparator> m_minHeap;
};