#ifndef _OMEN_PATTERN_H_
#define _OMEN_PATTERN_H_

#include <vector>
#include "DataType.h"
using namespace WeiboTopic_ICT;


namespace omen_pattern
{
    /*
     * \class > OmenPattern
     * \brief > omen detect patterns
     * \date > 2016/10
     * \author > zhounan(zhounan@software.ict.ac.cn)
     */
    class OmenPattern
    {
        public:

            /*
             * \fn > InitPatternData
             * \brief > init omen pattern data
             * \param[in] rTriggerPath > trigger word path
             * \param[in] rActionPath > action word path
             * \param[out] rErrInfo > error info if the function succeed it will be empty
             * \ret bool > whether function succeed
             * \date > 2016/10
             * \author > zhounan(zhounan@software.ict.ac.cn)
             */
            static bool InitPatternData(const string &rTriggerPath, const string &rActionPath, string &rErrInfo);


            /*
             * \fn > WordCollocationPattern
             * \brief > pattern of verb collocation
             * \param[in] rCorpus > corpus to filter
             * \param[out] rRes > result
             * \param[out] rErrInfo > error info
             * \ret bool > whether function succeed
             * \date > 2016/10
             * \author > zhounan(zhounan@software.ict.ac.cn)
             */
            static bool WordCollocationPattern(vector<pstWeibo> &rCorpus, vector<pstWeibo> &rRes, string &rErrInfo);


        private:
            /*
             * \fn > __LoadWordTable
             * \brief > load pattern keywords
             * \param[in] rPath > rPath
             * \param[out] vWords > put word to the vector
             * \param[out] rErrInfo > error info
             * \ret bool > whether function succeed
             * \date > 2016/10
             * \author > zhounan(zhounan@software.ict.ac.cn)
             */
            static bool __LoadWordTable(const string &rPath, vector<string> &vWords, string &rErrInfo);


            /*
             * \fn > __DocWordCollocation
             * \brief > word collocation pattern for one doc
             * \param[in] sText > doc text
             * \ret bool > whether th doc match patterns
             * \date > 2016/10
             * \author > zhounan(zhounan@software.ict.ac.cn)
             */
            static bool __DocWordCollocation(const string sText);


            /*
             * \fn > __SentenceSegment
             * \brief > cut text to sentences
             * \param[in] sText > doc text
             * \param[out] vSents > setences
             * \date > 2016/10
             * \author > zhounan(zhounan@software.ict.ac.cn)
             */
            static void __SentenceSegment(const string &sText, vector<string> &vSents);


        private:
            // .pattern triggers
            static vector<string> sm_vTriggers;

            // pattern actions
            static vector<string> sm_vActions;


    };
}

#endif
