#ifndef _FEAT_ENGINE_H_
#define _FEAT_ENGINE_H_

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <sstream>
#include "basictools.h"
#include "DataType.h"

using namespace WeiboTopic_ICT;
using namespace std;

#define BASIC_TF_FEAT 0
#define TIME_KEY_TF_FEAT 1

namespace feature_engine
{
    /*
     * \struct > featureEngineParam
     * \brief > params of engine
     * \date  > 2016/10
     * \author > zhounan(zhounan@software.ict.ac.cn)
     */
    struct featureEngineParam
    {
        set<string> stopWords;
    };


    /*
     * \class > CFeatureEngine
     * \brief > Feature Engine Class
     * \date  > 2016/10
     * \author > zhounan(zhounan@software.ict.ac.cn)
     */
    class IFeatureEngine
    {
        public:

            /*
            * \fn > FeatExtractByController
            * \brief > extract feature by feature engine
            * \param [in] rFeatID > feature id used this id to decide which function is going to use
            * \param [in] vWords > word segment result like w1/t1 w2/t2 ....
            * \param [out] sErInfo > record erro info
            * \param [out] pDoc > record features in doc object
            * \ret bool > whether function is succeed
            * \date  > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            static bool FeatExtractByController(const int &rFeatID, const vector<string> &vWords,
                                                pstWeibo pDoc, string &sErInfo);

            /*
            * \fn > InitFeatureEngine
            * \brief > initialize engine
            * \param [in] param > params needed by engine
            * \ret bool > whether function is succeed
            * \date  > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            static bool InitFeatureEngine(const featureEngineParam &param);

        private:
            /*
            * \fn > FeatExtractByWords
            * \brief > extract feature by words and tags
            * \param [in] vWords > word segment result like w1/t1 w2/t2 ....
            * \param [out] sErInfo > record erro info
            * \param [out] pDoc > record features in doc object
            * \ret bool > whether function is succeed
            * \date  > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            static bool __FeatExtractByWords(const vector<string> &vWords, pstWeibo pDoc, string &sErInfo);


            /*
            * \fn > FeatExtractByTimeKey
            * \brief > extract feature by words and tags
            * \param [in] vWords > word segment result like w1/t1 w2/t2 ....
            * \param [out] sErInfo > record erro info
            * \param [out] pDoc > record features in doc object
            * \ret bool > whether function is succeed
            * \date  > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            static bool __FeatExtractByTimeKey(const vector<string> &vWords, pstWeibo pDoc, string &sErInfo);

            // stop words
            static set<string> sm_StopWords;

            // postag blacklist
            static vector<string> sm_PosTagBlacklist;



    };
}



#endif
