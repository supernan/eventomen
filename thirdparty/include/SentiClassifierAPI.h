/*!
 *  \file	SentiClassifierAPI.h
 *  \brief  面向新闻评论的倾向性分析组件
 *  \date	2013-8-28
 *  \author 徐学可(xuxueke@software.ict.ac.cn)
 */
#ifndef __ICT_SENTI_CLASSIFIER_API_H__
#define __ICT_SENTI_CLASSIFIER_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#define ICT_SENTI_CLF_API __declspec(dllexport)
#else
#define ICT_SENTI_CLF_API
#endif

#define MAX_LABEL_LEN 255


    typedef void SENTI_CLASSIFIER_HANDLER;

    /*!
    *	\fn	int Senti_Classifier_Init(const char* config_file, SENTI_CLASSIFIER_HANDLER **h_clf);
    *	\brief 倾向性分类器句柄初始化
    *   \param [in] config_file 配置文件
    *   \param [out] h_clf 倾向性分类器句柄
    *	\return	成功返回 1，否则返回 0
    *	\author xxk
    *	\date 2013-8-28
    */
    ICT_SENTI_CLF_API int Senti_Classifier_Init(const char *config_file, SENTI_CLASSIFIER_HANDLER **h_clf);


    /*!
    *	\fn	int Senti_Classifier_Exit(CLASSIFIER_HANDLE *h_clf);
    *	\brief 分类器句柄释放
    *   \param [in] h_clf 倾向性分类器句柄
    *	\return	成功返回 1，否则返回 0
    *	\author xxk
    *	\date 2013-8-28
    */
    ICT_SENTI_CLF_API int Senti_Classifier_Exit(SENTI_CLASSIFIER_HANDLER *h_clf);



    /*!
    * \fn int Senti_Classify_Perform(SENTI_CLASSIFIER_HANDLER *h_clf, const char* text, double* score);
    * \brief 执行分类
    * \param [in] text 输入文本
    * \param [in] h_clf 倾向性分类器句柄
    * \param [out] score 倾向性分类得分
    * \return	成功返回 1，否则返回 0
    * \date 2013-8-28
    *  \author xxk
    */
    ICT_SENTI_CLF_API int Senti_Classify_Perform(SENTI_CLASSIFIER_HANDLER *h_clf, const char *text, double *score);


#ifdef __cplusplus
}
#endif

#endif

