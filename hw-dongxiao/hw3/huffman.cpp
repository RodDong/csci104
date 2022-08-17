#include <iostream>
#include <fstream>
#include <stdexcept>
#include <iomanip>
#include <algorithm>
#include "huffman.h"
using namespace std;

// Complete
RawTextVector AsciiHexIO::read(const char* filename)
{
    RawTextVector text;
    ifstream ifile(filename);
    if(ifile.fail()){
        throw std::invalid_argument("Bad input filename");
    }
    while(!ifile.fail()) {
        char c;
        string word;
        for(int i=0; i < 4; i++){
            ifile >> c;
            if(ifile.fail()){
                if(i > 0) {
                    throw std::runtime_error(
                        "Bad file format...did not find multiple of 4 hex characters");
                }
                else {
                    break; // normal exit
                }
            }
            else {
                word += c;
            }
        }
        if(word.size() == 4) {
            text.push_back(word);
        }
    }
    ifile.close();
    return text;
}

// Complete
void AsciiHexIO::write(const char* filename, const RawTextVector&  text)
{
    const size_t NUM_WORDS_PER_LINE = 8;
    ofstream ofile(filename);
    if(ofile.fail()){
        throw std::runtime_error("Unable to open output file");
    }
    size_t i = 0;
    for( const auto & word : text ){
        ofile << word;
        if(i % NUM_WORDS_PER_LINE != NUM_WORDS_PER_LINE-1 ){
            ofile << " ";
        }
        else {
            ofile << endl;
        }
        i++;
    }
    ofile.close();
}

// Complete
void AsciiHuffmanIo::writeCodedText(
    const char* filename,
    const CompressedData& inText,
    const CodeKeyMap& code)
{
    ofstream ofile(filename);
    ofile << code.size() << endl;
    // copy from map to vector so we can sort
    typedef pair<string,string> StrStrPair;
    vector< StrStrPair > codesToSort(code.begin(), code.end());
    std::sort(
        codesToSort.begin(), codesToSort.end(),
        [] (const StrStrPair& s1, const StrStrPair& s2) -> bool
            { return (s1.first.size() < s2.first.size()) ||
                     (s1.first.size() == s2.first.size() && s1.first < s2.first); } );
    for(const auto& pair : codesToSort) {
        ofile << pair.first << " " << pair.second << endl;
    }
    ofile << inText << endl;
    ofile.close();
}

// To be completed
void AsciiHuffmanIo::readCodedText(
    const char* filename,
    CompressedData& outText,
    CodeKeyMap& code)
{
    // clear old contents of the output parameters that this function
    // should fill in
    code.clear();
    outText.clear();

    // Complete the code below
    ifstream ifile(filename);
    if(ifile.fail()){
        throw runtime_error("Cannot open file");
    }
    size_t len;
    ifile >> len;
    while(len--){
        string c, k;
        ifile >> c >> k;
        code[c] = k;
    }
    ifile >> outText;
}

// To be completed
void HuffmanCoder::compress(
    const RawTextVector& inText,
    CompressedData& outText,
    CodeKeyMap& codes)
{
    // Clear old contents of the output parameters that this function should fill in
    codes.clear();
    outText.clear();
    KeyCodeMap keys;
    // Add your code here
    KeyFrequencyMap count;
    for(RawTextVector::const_iterator it=inText.begin(); it!=inText.end(); ++it)
        count[*it]++;
    Heap<HeapItem> heap;
    for(KeyFrequencyMap::iterator it=count.begin(); it!=count.end(); ++it){
        HeapItem temp;
        temp.total = it->second;
        temp.keys.insert(it->first);
        heap.push(temp);
    }
    while(true){
        HeapItem k1=heap.top();
        heap.pop();
        if(heap.empty()){
            heap.push(k1);
            break;
        }
        HeapItem k2=heap.top();
        heap.pop();
        for(set<string>::iterator it=k1.keys.begin(); it!=k1.keys.end(); ++it)
            keys[*it] = string("0") + keys[*it];
        for(set<string>::iterator it=k2.keys.begin(); it!=k2.keys.end(); ++it)
            keys[*it] = string("1") + keys[*it];
        k1.total += k2.total;
        k1.keys.insert(k2.keys.begin(), k2.keys.end());
        heap.push(k1);
    }
    for(KeyCodeMap::iterator it=keys.begin(); it!=keys.end(); ++it)
        codes[it->second] = it->first;
    for(RawTextVector::const_iterator it=inText.begin(); it!=inText.end(); ++it)
        outText += keys[*it];
}

// To be completed
void HuffmanCoder::decompress(
    const CompressedData& inText,
    const CodeKeyMap& codes,
    RawTextVector& outText)
{
    // Clear old contents of the output parameter
    outText.clear();
    // Add your code here
    string inputCode;

    // create the decompressed text by reading one char at a time and,
    // since Huffman codes are prefix codes, once the string matches
    // a key in the code map, just substitute the word and start again
    for(string::const_iterator it=inText.begin(); it!=inText.end(); ++it){
        inputCode+=(*it);
        CodeKeyMap::const_iterator res = codes.find(inputCode);
        if(res!=codes.end()){
            outText.push_back(res->second);
            inputCode.clear();
        }
    }
}

// Complete
double HuffmanCoder::ratio(const RawTextVector& inText, const CompressedData& outText, const CodeKeyMap& codes)
{
    // 2 bytes per entry
    double rawSize = 2 * inText.size();
    // each character in outText is a bit so convert to bytes
    double compressedSize = (outText.size() + 7) / 8;
    for(const auto& pair : codes) {
        compressedSize += 2 + pair.first.size()+1; // 2 bytes for 16-bit val + string of 1,0 + null char
    }
    cout << "Original size (bytes): " << rawSize << endl;

    cout << "Compressed size (bytes): " << compressedSize << endl;
    double compressionRatio = rawSize / compressedSize;
    cout << "Compression ratio: " << setprecision(2) << fixed << compressionRatio << endl;
    return compressionRatio;
}
