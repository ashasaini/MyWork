// NodeCreate.cpp: implementation of the NodeCreate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NodeCreate.h"
#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// int NodeCreate::mask = 0;
NodeCreate::NodeCreate()
{
    /*TIPREP_TRACE(NodeCreate::NodeCreate);*/

}

NodeCreate::~NodeCreate()
{
    /*TIPREP_TRACE(NodeCreate::~NodeCreate);*/

}
/*************************************************************
* Method:       pCopyNode
* Description:  This function is used to copy a node recursively.
* Parameter:    VfDBHandle_t vfd_dst : destination study file handle.
* Parameter:    VfPathElem_t vf_dst_path : destination path
* Parameter:    int vf_dst_depth : destination protocol depth
* Parameter:    VfDBHandle_t vfd_src : Source study file handle
* Parameter:    VfPathElem_t vf_src_path : source path
* Parameter:    int vf_src_depth : source protocol depth
* Parameter:    int image_group : image group
* Parameter:    int group : level
* Parameter:    int subgrp : sub level
* Returns:      status_t : E_NO_ERROR/others...
*************************************************************/
status_t NodeCreate::pCopyNode(VfDBHandle_t vfd_dst,
                   VfPathElem_t vf_dst_path[],
                   int          vf_dst_depth,
                   VfDBHandle_t vfd_src,
                   VfPathElem_t vf_src_path[],
                   int          vf_src_depth,
                   int          image_group,
                   int          group,
                   int          subgrp)
{
    TIPREP_TRACE(NodeCreate::pCopyNode,MRTraceMarker3);
    
    try{
        
        VfPathElem_t    vf_dst_path_save[MAX_DEPTH];
        VfPathElem_t    vf_src_path_save[MAX_DEPTH];
        VfFieldSpec_t   *vf_dst_req = NULL; 
        VfFieldSpec_t   *vf_src_req = NULL; 
        int         vf_dst_req_len = 0;    
        int         vf_src_req_len = -1;    
        int         vf_num = 0;     
        VfName_t        subtrees[MAX_DEPTH];
        int         num_subtree = 0;    
        time_t      vf_tm = 0;      
        register int    i = 0;      
        status_t        vfStat = E_NO_ERROR;    
        status_t        status = E_NO_ERROR;    
                                                /* パスを保存する。バーフィールドライブラリの中にはパスをバッファと
                                                * 使用し、パスの内容を変更する曲者がいるようだ。今は、大丈夫かも知
                                                * れないが不安なので保険のためにデータをセーブしておく。
                                                * 再帰呼び出しを使用しなければ、このようなことをする必要もないのだ
                                                * が…
        */
        memcpy(vf_dst_path_save, vf_dst_path, sizeof(VfPathElem_t) * vf_dst_depth);
        memcpy(vf_src_path_save, vf_src_path, sizeof(VfPathElem_t) * vf_src_depth);
        /* フィールドデータのリストを取得する。この段階ではデータ
        * (vf_src_req->buffer) は入らない。
        */
        vfStat = VfGetAllFields(vfd_src, vf_src_path, vf_src_depth, NULL,
            &vf_src_req, &vf_src_req_len);
        if_error(vfStat) {
            return(vfStat);     
        }
        
        if(vf_src_req_len == 0) {       
            return(E_NO_ERROR);     
        }
        
        vf_dst_req = (VfFieldSpec_t *)malloc(sizeof(VfFieldSpec_t) *
            vf_src_req_len);
        
        if(vf_dst_req == NULL) {    
            return(E_ERROR);        
        }
        
        /* 実際に VfGetFields() をする際のオプションをセットする。
        * サブツリーの場合(VFT_SUBTREE)、後でノードを作成するためにバッファに
        * コピーしておく。
        */
        num_subtree = 0;
        
        //vf_dst_req_len = 0;
        for(i = 0; i < vf_src_req_len; i++) {
            if(vf_src_req[i].type == VFT_SUBTREE) {
                if(image_group == 0 &&
                    vf_src_req[i].name == SVN_ST_PROCESSED) {
                    /* 画像ノードをコピーしない。*/
                } else {
                    subtrees[num_subtree] = vf_src_req[i].name;
                    num_subtree++;
                }
            } else {
                //if (vf_src_req[i].name != SVN_PLN_LDB_MAPPED_ID)  
                {
                    VFF_SET_ARGSG(vf_dst_req[vf_dst_req_len],
                        vf_src_req[i].name,
                        vf_src_req[i].type,
                        vf_src_req[i].size,
                        NULL,
                        VFO_ALLOCATE | VFO_VARIABLE_LEN,
                        vf_src_req[i].group);
                    vf_dst_req_len++;
                }
            }
        }
        
        if(vf_dst_req_len > 0) {
            vfStat = VfGetFields(vfd_src, vf_src_path, vf_src_depth,
                vf_dst_req, vf_dst_req_len, &vf_num);
            if_error(vfStat) {
                return(vfStat);     
            }
            
            /* フィールドのデータを吐き出すためにオプションをセットし直す。
            */
            for(i = 0; i < vf_dst_req_len; i++) {
                /* group >0 は画像データのはず… */
                vf_dst_req[i].group = group;
                vf_dst_req[i].subgrp = subgrp;
                //  if (vf_dst_req[i].name != SVN_PLN_LDB_MAPPED_ID) 
                {
                    
                    
                    VFF_SET_ARGSGS(vf_dst_req[i],
                        vf_dst_req[i].name, 
                        vf_dst_req[i].type,
                        vf_dst_req[i].size,
                        vf_dst_req[i].buffer,
                        VFO_REPLACE,
                        vf_dst_req[i].group,
                        vf_dst_req[i].subgrp);
                } 
                // else 
                // { 
                //                 char * buf = "     ";
                //                 VFF_SET_ARGSGS(vf_dst_req[i],
                //                     vf_dst_req[i].name,
                //                     vf_dst_req[i].type,
                //                  //   vf_dst_req[i].size,
                //                  strlen(buf),
                //                     buf,
                //                     VFO_REPLACE,
                //                     vf_dst_req[i].group,
                //                            vf_dst_req[i].subgrp);
                
                //   bool flag = true; //jus to debug
                // vf_dst_req[i].buffer = "";
                // vf_dst_req[i].size = strlen("");
                // }
            }
            
            /* フィールドのデータを吐き出す。
            */
            vfStat = VfPutFields(vfd_dst, vf_dst_path, vf_dst_depth,
                vf_dst_req, vf_dst_req_len, &vf_num, &vf_tm);
            if_error(vfStat) {
                return(vfStat);     
            }
            
            /* バッファ領域を解放する。
            */
            for(i = 0; i < vf_dst_req_len; i++) {
                if(vf_dst_req[i].buffer != NULL) {
                    free(vf_dst_req[i].buffer);
                    vf_dst_req[i].buffer = NULL;
                }
            }
        }
        
        /* フィールド用バッファをクリアする。
        */
        free(vf_src_req);
        free(vf_dst_req);
        vf_src_req = NULL;      
        vf_dst_req = NULL;      
        
                                /* サブツリーを作成する。再帰呼出しされる点に注意！！
        */
        for(i = 0; i < num_subtree; i++) {
            memcpy(vf_src_path, vf_src_path_save,
                sizeof(VfPathElem_t) * vf_src_depth);
            memcpy(vf_dst_path, vf_dst_path_save,
                sizeof(VfPathElem_t) * vf_dst_depth);
            status = pCopySubtree(vfd_dst,
                vf_dst_path,
                vf_dst_depth,
                vfd_src,
                vf_src_path,
                vf_src_depth,
                image_group,
                subtrees[i], group, subgrp);
            if_error(status) {
                return(status);     
            }
        }
        
        //vilas_review_29-11-2012..return 'status' avoid hard coding
        return(E_NO_ERROR);     
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in Node Create Class"), __LINE__, __FILE__);
        return E_ERROR;
    }
}
/*************************************************************
* Method:       pCopySubtree
* Description:  This function is used To create a subtree recursively.
* Parameter:    VfDBHandle_t vfd_dst : destination study file handle
* Parameter:    VfPathElem_t vf_dst_path : destination path
* Parameter:    int vf_dst_depth : destination depth
* Parameter:    VfDBHandle_t vfd_src : source study file handle
* Parameter:    VfPathElem_t vf_src_path : source path
* Parameter:    int vf_src_depth : source depth
* Parameter:    int image_group : image group
* Parameter:    VfName_t subtree_name : level in protocol
* Parameter:    int group :
* Parameter:    int subgrp :
* Returns:      status_t : E_NO_ERROR/others...
*************************************************************/
status_t NodeCreate::pCopySubtree(VfDBHandle_t  vfd_dst,
                      VfPathElem_t  vf_dst_path[],
                      int       vf_dst_depth,
                      VfDBHandle_t  vfd_src,
                      VfPathElem_t  vf_src_path[],
                      int       vf_src_depth,
                      int       image_group,
                      VfName_t      subtree_name, int group, int subgrp)
{
    TIPREP_TRACE(NodeCreate::pCopySubtree,MRTraceMarker3);

    try{
        VfPathElem_t    vf_dst_path_save[MAX_DEPTH];
        VfPathElem_t    vf_src_path_save[MAX_DEPTH];
        register int    i = 0;      
        int         total = 0;      
        int         active = -1;    
        //vilas_review_29-11-2012..use only one status variable
        status_t        status = E_NO_ERROR;    
        status_t        vfStat = E_NO_ERROR;    
        BITFLD_DECL(l_mask,   MAX_BITFLD);
        /* ノードの数を取得する。
        */
        total = MAX_BITFLD;
        BITFLD_INIT(l_mask, MAX_BITFLD, 0);
        vfStat = VfScanSubtree(vfd_src, vf_src_path, vf_src_depth,
            subtree_name, l_mask, &total, &active);
        if_error(vfStat) {
            return(vfStat);     
        }
        
        /* フィールドはあるがノードが存在しない場合正常終了を返す
        */
        if(active == 0) {       
            return(E_NO_ERROR);      
        }
        
        /* ノードを作成する。この段階ではノードの歯抜けが存在しない
        * (total == active) という前提で、取得したノード数を全て作成する。
        *
        * しかしtotal == active ということはありえない。
        * 実際にDeleteSubtreeするとTotal != activeになる。
        * よってActiveなノード数作成することにする。　（V6.00）
        * ここではノードは作らずにSubtreeのみ作成する。
        * 実際のBranchはノードコピーする際に追加作成する。
        */
        vfStat = VfCreateSubtree(vfd_dst, vf_dst_path, vf_dst_depth,
            subtree_name, -1);
        
        if(vfStat != E_EXISTS) {
            if_error(vfStat) {
                return(vfStat);     
            }
        }
        
        /* パスを保存する。バーフィールドライブラリの中にはパスをバッファと
        * 使用し、パスの内容を変更する曲者がいるようだ。今は、大丈夫かも知
        * れないが不安なので保険のためにデータをセーブしておく。
        * 再帰呼び出しを使用しなければ、このようなことをする必要もないのだ
        * が…
        */
        vf_dst_path[vf_dst_depth].subtree_name = subtree_name;
        vf_src_path[vf_src_depth].subtree_name = subtree_name;
        vf_dst_depth++;
        vf_src_depth++;
        memcpy(vf_dst_path_save, vf_dst_path, sizeof(VfPathElem_t) * vf_dst_depth);
        memcpy(vf_src_path_save, vf_src_path, sizeof(VfPathElem_t) * vf_src_depth);
        
        /* 作成したノードに対してフィールドコピー関数をコールする。
        * 再帰的にコールされ点に注意！！！
        */
        for(i = 0; i < total; i++) {
            if(BITFLD_ON(l_mask, i)) { /* add. V5.00 */
                memcpy(vf_dst_path, vf_dst_path_save,
                    sizeof(VfPathElem_t) * vf_dst_depth);
                memcpy(vf_src_path, vf_src_path_save,
                    sizeof(VfPathElem_t) * vf_src_depth);
                vf_dst_path[vf_dst_depth - 1].index = i;
                vf_src_path[vf_src_depth - 1].index = i;
                /* Activeなブランチだけを追加する V6.00 */
                status = VfAddBranch(vfd_dst, vf_dst_path, vf_dst_depth);
                //vilas_review_29-11-2012..multiple status are returned use all
                if_error(status) {
                    DB_PRINTF("E", ("VfAddBranch status : %d\n", status));
                    return(status);     
                }
                status = pCopyNode(vfd_dst,
                    vf_dst_path,
                    vf_dst_depth,
                    vfd_src,
                    vf_src_path,
                    vf_src_depth,
                    image_group,
                    group,
                    subgrp);
                if_error(status) {
                    DB_PRINTF("E", ("pCopyNode status = %d\n", status));
                    return(status);     
                }
            }
        }
        
        return(E_NO_ERROR);     
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in Node Create Class"), __LINE__, __FILE__);
        return E_ERROR;
    }
}


///////////////////////////////////////////////////////////////////////////////
// End of file
//////////////