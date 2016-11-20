/*********************************
 *   File Creator: Wu Dayong
 *   Create Time: 2014-03-21
 *   Modification Time: 2016-04-19
 *
 *********************************/


#ifndef ICT_LA_PLATFORM_CORE_WORDSEG
#define ICT_LA_PLATFORM_CORE_WORDSEG

#include <vector>
#include <string>

namespace ICTLAP {
    namespace CORE {
        namespace WORDSEG {
            
            typedef void* WordSegApi;
            typedef struct WordLocation{
                int word_offset;
                int word_length;
            } WordLocation;

            typedef std::vector<WordLocation> WordSegRes;
            
            //Init First, returns a Handel
            WordSegApi WordSegInit(const char* wordSeg_data_path);
            
            //Get Result
            WordSegRes& WordSegment(WordSegApi h, const char* input_str, WordSegRes& res);

            //Get String Result
            bool WordSegmentStr(WordSegApi h, const char* input_str, std::vector<std::string>& res_words);

            //Exit Last
            void  WordSegExit(WordSegApi h);
            
            //Print Version Info
            void  WordSegVersion();
        }
    }
}
#endif
