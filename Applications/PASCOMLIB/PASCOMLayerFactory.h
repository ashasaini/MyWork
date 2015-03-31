//Redmine-777
//REDMINE-780
//Redmine-783
// PASCOMLayerFactory.h: interface for the CPASCOMLayerFactory class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_PASCOM_LAYER_FACTORY_H__
#define __SM_PASCOM_LAYER_FACTORY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPASCOMVarFieldLayer;
//USHA TAREVIEW
class AFX_EXT_CLASS CPASCOMLayerFactory
{
public:

    static CPASCOMVarFieldLayer* GetPASCOMLayerInstance(const CString& f_str_appname, const CString& f_str_mode);
    static void DestroyPASCOMLayerInstance();

private:

    static CPASCOMVarFieldLayer* m_pascom_layer ;

    CPASCOMLayerFactory();
    virtual ~CPASCOMLayerFactory();

};
#endif // #ifndef __SM_PASCOM_LAYER_FACTORY_H__
