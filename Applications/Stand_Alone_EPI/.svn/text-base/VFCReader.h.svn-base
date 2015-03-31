// VFCReader.h: interface for the CVFCReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VFCREADER_H__D3E9FEDF_6159_417A_BD15_E4C5F42EB234__INCLUDED_)
#define AFX_VFCREADER_H__D3E9FEDF_6159_417A_BD15_E4C5F42EB234__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef NORMAL
#define NORMAL          0
#endif

#ifndef ERROR
#define ERROR           -1
#endif

#include "VFChandler.h"
#define SCAN_KIND_NORMAL    0
#define SCAN_KIND_GATE      1
#define SCAN_KIND_GATE_MPHASE   2
#define SCAN_KIND_DYNAMIC   3

#define PROCESSED_TREE      1
#define RAW_DATA_TREE       0

#define MAXPROTS        512
#define MAXOBJECT       256
#define MAXIMAGE_INFO_LEN   256
#define MAXIMAGE_LIST_LEN   256

typedef struct StudyProtocolData_ { /*  STUB */
    int             protocol_num;
    int             ImageType;
    char                *seqname;
    char                *comment;
    float               Slise_thickness;
    float               Slise_gap;
    float               ro_Res;
    float               pe_Res;
    float               ro_Fov;
    float               pe_Fov;
    float               se_Fov;
    float               ro_Matrix;
    float               pe_Matrix;
    int             wrap;
    float               te;
    float               tr;
    float               ti;
    float               nex;
    struct StudyProtocolData_   *next;
} StudyProtocolData_t;

typedef struct data_path_ {
    struct data_path_   *prev;
    int         protocol;
    int         subprotocol;
    int         processed;
    int         slicegroupe;
    int         echo;
    int         slice;
    int         position;
    int         image;
    int         slice_of_3d;
    int         read_size;
    int         pe_size;
    int         type_of_data;
    int         type_of_time;
    int         tree_type;
    int         form_of_data;
    struct data_path_   *next;
} Data_path_t;

class CVFCReader : public CVFChandler
{
public:
    CVFCReader();
    virtual ~CVFCReader();
    void checkFunction();
    int OpenDBFile();
    int CloseDBFile();
    status_t Disp_node_count();
    char** getData() const;
    int getTotalRawNodes() const;
    int getNumberofChannel(int &f_nProtocolNo, int &f_nTotalChannel);
    int dumpRawdata(CString &f_csPath, const int &f_nDataForm,
                    double *&f_dData, CString &f_csFileName,
                    int &f_nFileSize, int &f_nChannelSelection);
    int protTraverse(const int &f_PortIdx, int &f_nSubProtNo, int &f_nSubGrpNo, int &f_nSliceNo, int &f_nSliceImgNo);
    int getRawData(VfPathElem_t vfPath[],
                   int &f_nDataHeight,
                   int &f_nDataWidth,
                   int &f_nMultiplier,
                   const int &f_nChannelNo,
                   int &f_nDataTypeSize,
                   int &f_nNumRepetition,
                   void *&f_cRawData, long &f_nImgRawDataSize, int &f_nDataType);
    int getTotalChannel() const {
        return m_nTotalChannel;
    }
    int MakeProcessImageData(double * inBuf , int numdata, double *&outBuf);
    int MakeImaginaryData(double * inBuf , int numdata, double *&outBuf);
    int MakeRealData(double * inBuf , int numdata, double *&outBuf);
    int MakePhaseData(double * inBuf , int numdata , double *&outBuf);
    int MakeMagnitudeData(double * inBuf , int numdata , double *&outBuf);
    int getProtocolInfo(protInfo_t *&l_protData, int &f_nTotalProtocol);
    int vftoDUMPDATATYPE(void *dataBuffer, void *dftBuf, int data_type, int numPoints);
    int rawDataCheck();
    int protocolCheckEPI(int f_nProtIdx);
    status_t GetSeqName(int f_protidx, CString &seqname);
    int DumpProcessedData(CString &f_csPath, int &f_height,
                          double *&f_dData, CString &f_csFileName, int &f_nFileSize, int &f_width);
    double* ReadImageData(int &all_data , int &read_dim, double &img_scale);
    int getProtocolData(VfPathElem_t f_pathP[], int &f_nNumRepetition, int &f_nDataForm,
                        int &f_nDataType, int &f_nDataHeight, int &f_nDataWidth);
    double* ReadRawData(int &all_data , int &read_dim, double &img_scale, int &f_nDataType, BOOL f_bIntDataReq = FALSE, int f_nChannelNum = 0);
    int vftointgr(void *DataBuffer, void *dftBuf, int data_type, int numPoints);

private:
    void CleanUp();
    void Disp_node_count_unset(Data_path_t  *path_now);
    int Disp_node_count_set(VfDBHandle_t    studyHandle,     /*VfHndle*/
                            Data_path_t **path_now, /*path array*/
                            int     *index_num, /*total index*/
                            int     depth,      /*start depth*/
                            VfPathElem_t    *sarch_path /*use path element*/
                           );
    int Disp_node_count_set2(VfDBHandle_t   studyHandle,     /*VfHndle*/
                             Data_path_t **path_now,     /*path array*/
                             int     *index_num, /*total index*/
                             int     depth,      /*start depth*/
                             VfPathElem_t    *sarch_path /*use path element*/
                            );
    int Disp_Make_list(
        int     num ,
        Data_path_t *Path_index_list);
    void UpdatePathIndexNow();
    void UpdateRawPath();
    void SetSelectedPositionData(CString &cs);
    void GetDataType(int &f_datatype);
    void GetDataForm(int &f_dataForm);
    int GetSubNodes(VfPathElem_t f_pathP[], int f_curLevel, int &f_ntotalsubnodes);
    int getImageSize(const int &f_nDataType,
                     const int &f_nDataForm,                     
                      int &f_nDataTypeSize, 
                      int &f_nMultiplier);

    int getProtAcqOrder(int *f_narray, int &f_nacqOrderSize);
    int CheckIsProtocolScanned(int &f_nprotIndx);
    int getLabelAlpha(int protIndx, CString &l_csStr);
    BOOL IsProcessed(CString &cs);
    void UpdateImagePath();
    int getProcessImgDataDimentions(int &f_nDataHeight,int &f_nDataWidth);
private:
    Data_path_t     *m_Path_index_top;
    //VfDBHandle_t  m_studyHandle;
    int             m_Path_index_num;
    Data_path_t     *m_Path_index_now;
    char** m_data;
    int m_TotalRawNodes;
    int m_selected_Image_Posotion;
    int m_SelectedChannel;
    int m_nTotalChannel;
//Data members
};

#endif // !defined(AFX_VFCREADER_H__D3E9FEDF_6159_417A_BD15_E4C5F42EB234__INCLUDED_)
