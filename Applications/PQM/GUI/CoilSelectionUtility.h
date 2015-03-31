//TMSC-REDMINE-783
//TMSC-REDMINE-781
//TMSC-REDMINE-780
// CoilSelectionUtility.h: interface for the CCoilSelectionUtility class.
//
//////////////////////////////////////////////////////////////////////
//Redmine-775

//Redmine-777
#ifndef __SM_COIL_SELECTION_UTILITY_H__
#define __SM_COIL_SELECTION_UTILITY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <PQM/CoilDefinitions.h>
#include <PQM/CDSCommonStructures.h>

#include "InterfaceClasses.h"

using namespace MPlus::EH;
enum CoilTypes { CoilType_G, CoilType_L, CoilType_A};

enum GCoilItems { GCoilItems_1 = 0 , GCoilItems_TOTAL};
enum LCoilItems { LCoilItems_1 = 0 , LCoilItems_2 , LCoilItems_TOTAL};
enum LCoilButtonType { LCoilCoilButtons, LCoilPositionButtons};

enum ACoilItems { ACoilItems_1 = 0 , ACoilItems_2, ACoilItems_3, ACoilItems_4, ACoilItems_5, ACoilItems_6, ACoilItems_7 , ACoilItems_TOTAL};
enum ACoilButtonType { ACoilPortButtons, ACoilCoilButtons, ACoilPositionButtons};


enum MarkerSignEnum { MarkerSign_Plus = 0 , MarkerSign_Minus , MarkerSign_None, MarkerSign_Invalid };

enum COIL_SEL_MODE { COIL_SEL_NORMAL, COIL_SEL_CDS } ;

typedef struct CoilSelectionConfig_s {
    int 		pqmMode;
} CoilSelectionConfig_t, *CoilSelectionConfig_p;

typedef struct CoilSelectionGlobal_s {


    struct {
        int 					sft_mode;		/* sft_mode */
        int 					numChannels;	/* Maxumum Coil element */
        int 					VAPChannels;	/* number of VAP unit */
        int 					channelPerVFEP;
    } coil;

    int 						message;

    CoilSelectionConfig_t		config;

public:
    CoilSelectionGlobal_t();
} CoilSelectionGlobal_t;


class CUIClassInterfaceForCoilSelDialog
{

public :
    virtual void UpdateWarningMsgWindowWith(const int f_msgstring_id) = 0;
    virtual void UpdateMsgWindowWithMsgOK() = 0;
    virtual void UiCheckCombine() = 0;
    virtual void CoilSelectionUiMessage(const int f_message_id) = 0 ;
    virtual void SelectCoilUiSetSelectColorForIndex
    (const int f_coil_buttonindex) = 0 ;

    virtual void SelectCoilUiSetDefaultColorForIndex
    (const int f_coil_buttonindex) = 0 ;

    virtual void SelectCoilSetWarningButtonColorForIndex
    (const int f_coil_buttonindex) = 0 ;

    virtual void SelectCoilUiSetLableForSelectionCoilIndex
    (const int f_coil_buttonindex, const CString& f_lable) = 0;

    virtual void SelectCoilUiSetSensitiveButton(
        const int f_coil_buttonindex,
        const bool f_enable
    ) = 0 ;

    virtual void SectionCoilSetSelectColorForButton
    (const int f_section_buttonindex, const bool f_redraw_button = false) = 0;

    virtual void SectionCoilSetDefaultColorForButton
    (const int f_section_buttonindex, const bool f_redraw_button = false) = 0;

    virtual void SectionCoilSetWarningButtonColor
    (const int f_section_buttonindex) = 0;

    virtual void SectionCoilSetLableForButton
    (const int f_section_buttonindex, const CString& f_lable) = 0;

    virtual void SectionCoilUpdateSectionSize
    (const int f_section_buttonindex, CoilSelectionSectionButton_t* f_section_btn) = 0;


    virtual void SectionCoilSetSensitiveButton(
        const int f_section_buttonindex,
        const bool f_enable
    ) = 0;

    virtual void SectionCoilEnableDisableButton(
        const int f_section_buttonindex,
        const bool f_enable
    ) = 0;

    virtual void SelectCoilEnableDisableButton(
        const int f_select_buttonindex,
        const bool f_enable
    ) = 0;

    virtual int GetGUISectionWidthDefault() const = 0;

    virtual COUCH_POSITION GetCouchAxis() const = 0;
} ;


class CUIClassInterfaceForCoilListDialog
{

public :
    virtual void CheckCombineFailedFor(const int select_coil_index)const = 0;
    virtual void UiSetMenuWarningColorForCouch(const ACoilButtonType f_acoilbuttontype, const ACoilItems f_acoilitems) = 0;
    virtual void UiSetMenuDefaultColorForCouch(const ACoilButtonType f_acoilbuttontype, const ACoilItems f_acoilitems) = 0;
} ;



//class CCoilSelWin;
class CCoilSelectionUtility : private MR::ACQ::SM::COMMON::NonCopyable<CCoilSelectionUtility>
{
public:

    CCoilSelectionUtility();
    virtual ~CCoilSelectionUtility();

    status_t CheckCombine();
    status_t CheckPosition(const CoilSelectionSelectCoil_t* f_coil)const;
    int ComputePosition(const int location, const int position)const;
    int ComputePosition(const int position)const;
    status_t CoilCheckList(const BOOL f_flag);
    void CoilSetCoilButton(const bool f_offset = false, const int f_step = -1, const bool f_ignore_pos_change = false);
    void CoilMapCoilButton(const bool f_is_move_coil_btn = false);
    void ProcessShowCoilMsg(VftCoilListInfo_t* f_show_coil);
    void ProcessSelectCoilMsg(const coilSelect_t* f_select_coil);
    bool ProcessConnectCoilMsg(VftCoilListInfo_t* f_connect_coil, CString& error_message) ;
    BOOL CoilCompareList();
    void InitUtilityFlags();
    void CoilSelectionClearPqmListCoilMenu();

    void CoilSelectionCoilSetFreePosition(const int direction, const int	step, const bool f_is_move_coil_btn = false);
    void ResetAllMarkerSigns();
    void UiSetToPqmSelectCoil();
    void SetSelectCoil(const CoilSelectionSelectCoil_t* f_coil_selection, const int f_index);
    bool IsLinkedSectionOfGroupedCoil(const CoilSelectionSectionButton_t* const lhs,
                                      const CoilSelectionSectionButton_t* const rhs) const;
    bool IsCoilApplied()const;
    bool HasDisallowableCombineCoil()const;

    static MPLUSERRORUSERRESPONSE_e DisplayMessageinWarningDialog(
        const CString& f_warn_message,
        const int f_warn_buttonstype,
        const int f_warn_level = SCAN_WARN_INFORMATION,
        const int f_warn_dialogtype = SCAN_WARN_SYSMODAL
    ) ;
    static int GetBaseIndexFor(const CoilTypes coiltypes) ;
    static int GetBaseIndexForCoilButtonFor(const CoilTypes coiltypes) ;


    DllT* GetHead()const;
    void SetHead(DllT* f_head);

    DllT* GetTrestleHead()const;
    void SetTrestleHead(DllT* f_trestle_head);

    DllT* GetCouchHead(const int f_index)const;
    void SetCouchHead(DllT* f_couch_head, const int index);

    DllT* GetLinearHead(const int f_index)const;
    void SetLinearHead(DllT* f_linear_head, const int index);

    VftCoilListInfo_t* GetConnectCoil()const;
    void SetConnectCoil(const VftCoilListInfo_t* f_connect_coil)const;

    VftCoilListInfo_t* GetShowCoilList()const;
    void SetShowCoilList(const VftCoilListInfo_t* f_show_coil_list)const;

    coilSelect_t GetToPqmSelectCoil()const;
    void SetToPqmListCoil();

    COIL_SEL_MODE GetCoilSelMode() const ;	//Patni-Hemant/2010May6/Added/JaFT/SM_PH3_CDR-CDS_CDR_11
    void SetCoilSelMode(const COIL_SEL_MODE f_coil_selection_mode);

    CPoint GetPressPoint();
    void SetPressPoint(const CPoint& f_press_point);

    CPoint GetReleasePoint();
    void SetReleasePoint(const CPoint& f_release_point);

    CoilSelectionSelectCoil_t* GetSelectedCoil(const int f_index);
    void SetSelectedCoil(const CoilSelectionSelectCoil_t* f_coil_selection, const int f_index);

    CoilSelectionSelectCoil_t* GetAppliedCoil(const int f_index);
    void SetAppliedCoil(const bool f_cancel = false);

    VftCoilListInfo_t* GetterToPqmListCoil();
    VftCoilCoordinateListInfo_t* GetPqmCordinateListInfo();

    CoilSelectionSectionButton_t* GetSectionButton(const int f_index);
    CoilSelectionSectionButton_t* GetCoilButton(const int f_index);
    MarkerSignEnum GetMarkerSignForIndex(const int f_coil_button_index)const;
    CoilSelectionGlobal_t* GetCoilSelectionGlobal() ;
    CoilSelectionSelectCoil_t* GetSelectedCoil();
    int SetPqmListCoil(VftCoilListInfo_t* f_list_coil, const BOOL f_flag);
    void SetPQMMode(const int f_mode);
    void SetUIClassInterfaceForCoilSelDialog(CUIClassInterfaceForCoilSelDialog* uiclassinterfaceforcoilseldialog) ;
    void SetUIClassInterfaceForCoilListDialog(CUIClassInterfaceForCoilListDialog* coillistdialog_interface) ;
    void SetCoilChannels(const int f_VAP_channels, const int f_VFEP_channels);
    bool SetPortForLinerCoil(CoilSelectionSelectCoil_t* f_coil = NULL);
    void SetBeforePoint(const CPoint& f_before_point);
    void SetSelectCoilList(const coilSelect_t* f_select_coil_list)const;
    void SetCoordinateList(const VftCoilCoordinateListInfo_t* f_coordinate_list);

    sCSConfigParams* GetCSConfigParams() ;
    void SetCSConfigParams(const sCSConfigParams& f_scsconfigparams);

    float GetTotalLablesOccupied(const CoilSelectionSelectCoil_t* const f_coil_struct) const;

private:

    sCSConfigParams m_coilsel_configparams;
    bool m_has_disallowable_combine_coil;

    COIL_SEL_MODE m_coil_selection_mode ;
    MarkerSignEnum                  m_marker_sign[MAX_COIL_BUTTON];	/*/marker signs .. index according to m_coil_button*/
    CoilSelectionGlobal_t           m_coil_selection_global;
    VftCoilListInfo_t*               m_connect_coil;
    VftCoilListInfo_t*               m_show_coil_list;
    coilSelect_t*                    m_select_coil_list;
    CoilSelectionCoil_t*				m_coil_infomation[10];
    CoilSelectionSelectCoil_t		m_applied_coil[MAX_PORT];/*applied coil*/
    CoilSelectionSelectCoil_t		m_selected_coil[MAX_PORT];/*Selected Coil*/
    CoilSelectionSectionButton_t	m_section_button[MAX_SECTION_BUTTON];	/*/Section Button (34 NO.s)*/
    CoilSelectionSectionButton_t	m_coil_button[MAX_COIL_BUTTON];
    VftCoilListInfo_t	            m_to_pqm_list_coil;
    VftCoilCoordinateListInfo_t     m_list_coil_coordinate;
    coilSelect_t 	 	            m_to_pqm_select_coil;

    CPoint m_release_point;
    CPoint m_press_point;
    CPoint m_before_point;

    CUIClassInterfaceForCoilSelDialog* m_uiclassinterfaceforcoilseldialog ;
    CUIClassInterfaceForCoilListDialog* m_coillistdialog_interface ;
    int	m_select_count;
    DllT*            m_head;
    DllT*            m_coil_trestle_head;
    DllT*            m_coil_couch_head[7];
    DllT*            m_coil_liner_head[2];
    BOOL        m_compare_flag;
    BOOL        m_list_flag;

    void searchSelPosition(const int f_location)const;
    status_t CheckPort()const;
    CoilSelectionCoil_t* SearchCoil(
        const int f_id,
        const char f_port,
        const char f_channel,
        const BOOL f_flag
    );

    void OffsetButtonAsPerSize(CoilSelectionSelectCoil_t* f_selected_coil, CoilSelectionSectionButton_t* f_section_btn, const bool f_offset);
    BOOL CompareListCoil();
    int ChartoInt(const char c);
    CString CommunicationCoilMessage(const CString& f_war_msg);
    void CoilSelectionUiStringCat(char* f_main_string, char* f_add, char* f_out_string);
    void CoilSelectionUiErrorNAVI(char* pString, char* add, DWORD f_erro_type = MPlus::EH::Crucial);
    bool CoilSelectionOKCancel(const bool f_ok);
    void coilSelectionCheckOkCB();
    void coilSelectionCheckCancelCB()const;
    void UpdateMsgWindowWithMsgOK()const;
    bool IsValidCoilIndex(const int f_index)const ;
    void CoilSelectionUiMessage(const int f_message_id)const;
    void SelectCoilUiSetSelectColorForIndex(const int f_coil_buttonindex)const;
    void SelectCoilSetWarningButtonColorForIndex(const int f_coil_buttonindex)const ;
    void SelectCoilUiSetLableForSelectionCoilIndex(const int f_coil_buttonindex, const CString& f_lable)const;
    void SelectCoilUiSetSensitiveButton(const int f_coil_buttonindex, const bool f_enable)const ;
    void SelectCoilUiSetDefaultColorForIndex(const int f_coil_buttonindex)const ;
    void SectionCoilSetSelectColorForButton(const int f_section_buttonindex)const ;
    void SectionCoilSetDefaultColorForButton(const int f_section_buttonindex)const ;
    void SectionCoilSetWarningButtonColor(const int f_section_buttonindex)const ;
    void SectionCoilSetLableForButton(const int f_section_buttonindex, const CString& f_lable)const;
    void SectionCoilUpdateSectionSize(const int f_section_buttonindex, CoilSelectionSectionButton_t* f_section_btn)const;
    void SectionCoilSetSensitiveButton(const int f_section_buttonindex, const bool f_enable)const ;
    void SectionCoilEnableDisableButton(const int f_section_buttonindex, const bool f_enable)const ;
    void SelectCoilEnableDisableButton(const int f_select_buttonindex, const bool f_enable)const ;
    void ResetMarkerSign(const int f_index) ;
    BOOL UiSetPqmSelectCoil();
    bool HasCoilSameSectionIdWithLinkedId(const CoilSelectionSectionButton_t* const f_section_btn) const;
    void GetCoilPortString(const CoilSelectionCoil_t* f_coil_p, char* f_port_string);
    int SetPqmListCoilMenu(
        VftCoilListInfo_t*	 l_list_coil,
        CoilSelectionSelectCoil_t* f_coil,
        const int					f_cnt,
        int*					f_no,
        const BOOL                f_flag
    );

    static int GetConvertedCoilButtonIndexForSelectedCoilIndex(const int f_selected_coil_index);

    coilSelect_t* GetSelectCoilList()const;

    void SetDisallowableCombineCoil(const bool f_has_disallowable_combine_coil);
    void SetMarkerSign(const int f_index, const MarkerSignEnum f_markersignenum) ;
};

#endif // #ifndef __SM_COIL_SELECTION_UTILITY_H__
