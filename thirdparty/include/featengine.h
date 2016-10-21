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
            * \fn > FeatExtractByWords
            * \brief > extract feature by words and tags
            * \param [in] vWords > word segment result like w1/t1 w2/t2 ....
            * \param [out] sErInfo > record erro info
            * \param [out] pDoc > record features in doc object
            * \ret bool > whether function is succeed
            * \date  > 2016/10
            * \author > zhounan(zhounan@software.ict.ac.cn)
            */
            static bool FeatExtractByWords(const vector<string> &vWords, pstWeibo pDoc, string &sErInfo);


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
            static set<string> sm_StopWords;



    };
}



#endif
