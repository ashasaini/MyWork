// ScrPadHandler.cpp: implementation of the CScrPadHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScrPadHandler.h"
////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////////////////////


/*************************************************************
* Method:    	CScrPadHandler
* Description: 	Constructor
* Returns:   	NA
*************************************************************/
CScrPadHandler::CScrPadHandler() 
:m_nProtocolindex(0)
{	
	
}

/**
Method Name: Destructor
Description: Deletes object and clears up memory
@param: none
@return: Not applicable
*/

/*************************************************************
* Method:    	~CScrPadHandler
* Description: 	Destructor 
* Returns:   	
*************************************************************/

CScrPadHandler::~CScrPadHandler()
{
	//
}

/*************************************************************
* Method:    	CreateScrPad
* Description: 	This function is used for creating shared memory.
* Parameter: 	char * f_csScratchPadName : Name of the scratch pad 
*               to be created
* Parameter: 	char * f_csStudyFileName : name of study file whose 
*               data will be copied to scratch pad
* Parameter: 	int f_nprot_idx : protocol Number
* Returns:   	int
*************************************************************/

int CScrPadHandler::CreateScrPad(char * f_csScratchPadName , 
						 char * f_csStudyFileName, 
						 int f_nprot_idx)
{	
    VfDBHandle_t    vfd;

	try {
		if (NULL == f_csScratchPadName)
		{
			printf("SCRATCHPAD_NAME_ERROR\n");
			return(E_ERROR);
		}
		
		if (NULL == f_csStudyFileName)
		{
			printf("STUDYFILE_NAME_ERROR\n");
			return(E_ERROR);
		}
		if (f_nprot_idx < 0)
		{
			printf("PROTOCOL_INDEX_ERROR\n");
			return(E_ERROR);
		}
		
		strcpy(m_csScrPadName, f_csScratchPadName);
		strcpy(m_csStudyFileName, f_csStudyFileName);
		
		m_nProtocolindex = f_nprot_idx;
		
		if (-1 == unlink(m_csScrPadName))
		{
			printf("UNLINK_ERROR_NO = %d\n",errno);
			//return(E_ERROR);
		}
        FILE* fp = fopen(m_csScrPadName, "a");
        
        if (fp)
            fclose(fp);

		
		VfDBName_t      vfName;
		vfName.filename = m_csStudyFileName;//m_csScrPadName;
		
		
		if(E_NO_ERROR != VfOpen(VFFT_FILE, vfName, O_CREAT | O_RDWR, &vfd))
		{
			printf("ERROR in VfOpen(%s) (%d,%s).\n",vfName.filename, 
                   __LINE__, __FILE__);
			//printf("STUDYFILE_OPEN_ERROR = %s\n",m_csScrPadName);
			return(E_ERROR);
		}
		
		if(E_NO_ERROR != VuiCopyProtocolToFile_(vfd, 0,const_cast<char*>(m_csScrPadName) 
                 , 0, 0, NULL))
		{
			VfClose(vfd);
			printf("SCRATCHPAD_CREATION_ERROR\n");
            unlink(m_csScrPadName);
			return(E_ERROR);
		}
	}
	catch (...)
	{
		if(vfd)
		{
			VfClose(vfd);
		}
		return -1;
	}
	return E_NO_ERROR;
}

/*************************************************************
* Method:    	DeleteScrPad
* Description: 	This function is used for deleting shared memory
* Returns:   	int
*************************************************************/

int CScrPadHandler::DeleteScrPad()
{
	
	
	VfDBName_t		vf_name;
    VfDBHandle_t	vfd_dst;
	
	vf_name.filename = m_csStudyFileName;
    if_error(VfOpen(VFFT_FILE, vf_name, O_RDWR, &vfd_dst))
    {
		printf("ERROR in VfOpen(%s) (%d,%s).\n",vf_name.filename,
            __LINE__, __FILE__);
        return(E_ERROR);
    }
	
	if(E_NO_ERROR != VuiCopyProtocolFromFile(vfd_dst, m_nProtocolindex,
        m_csScrPadName,0, 0, NULL))
	{
		VfClose(vfd_dst);
		printf("SCRATCHPAD_DELETION_ERROR\n");
		return(E_ERROR);
	}
	
    VfClose(vfd_dst);	
	return E_NO_ERROR;
}

/*************************************************************
* Method:    	GetSeqName
* Description: 	This function is used for retrieving the sequence name.
* Parameter: 	char * f_csSeqName : name of sequence(out param)
* Returns:   	int
*************************************************************/

int CScrPadHandler::GetSeqName(char * f_csSeqName)
{
    
	if (NULL == f_csSeqName)
	{
		printf("SEQUENCE_NAME_ERROR\n");
		return E_ERROR;
	}
	VfDBHandle_t    vfd;
    VfDBName_t      vfName;	
	
    vfName.filename = m_csScrPadName;
    
    if(E_NO_ERROR != VfOpen(VFFT_FILE, vfName , O_RDONLY, &vfd))
    {
		printf("ERROR in VfOpen(%s) (%d,%s).\n",vfName.filename, 
            __LINE__, __FILE__);
        return(E_ERROR);
    }
	
    int				l_nArgn = 0;
	VfFieldSpec_t	varfield[5];
	char			l_csSeqTemplate[200];       
	
    VFF_SET_ARGS(	varfield[l_nArgn], 
		SVN_PLN_TEMPLATE_SEQ, 
		SVT_PLN_TEMPLATE_SEQ,
		sizeof(l_csSeqTemplate) - 1, 
		l_csSeqTemplate, VFO_VARIABLE_LEN);
    l_nArgn++;
	
    VfPathElem_t    path[1];
	int				l_nNum_found = 0;            
	
    path[0].subtree_name = SVD_ST_PROTOCOL;
    path[0].index = 0;
    if(E_NO_ERROR != VfGetFields (vfd, path,SVD_ST_PROTOCOL,varfield,
        l_nArgn, &l_nNum_found))
	{
		printf("ERROR in VfGetFields(%s) (%d,%s).\n",vfName.filename,
            __LINE__, __FILE__);
		return(E_ERROR);
		
	}
	
    if (l_nNum_found != 1)
    {
        return(E_ERROR);
    }
	
    l_nArgn = 0;
    l_csSeqTemplate[varfield[l_nArgn].size] = '\0'; // terminate
    l_nArgn++;
	
    strcpy(f_csSeqName, l_csSeqTemplate);
	
    if(vfd)
    {
        VfClose(vfd);
    }
	return E_NO_ERROR;
}

/*************************************************************
* Method:    	ReleaseHandle
* Description: 	This function is used for deleting self.
* Returns:   	int
*************************************************************/

int CScrPadHandler::ReleaseHandle()
{
    try
    {
        delete this;
    }
    catch (...)
    {
        //log error
        return -1;
        
    }
    
    return 0;
}

/*********************************************************************
 * Description: プロトコルノードをファイルにコピーする。
 * Assumptions:
 * Side Effects:
 * Return Value:
 *********************************************************************/
status_t CScrPadHandler::VuiCopyProtocolToFile_(VfDBHandle_t vfd_src,
                                                int		vf_src_index,
                                                char		*dst_file,
                                                int		vf_dst_index,
                                                int		image_group,
			                                    int		*protocol)
{
    VfDBHandle_t	vfd_dst;

    if((vfd_dst = VuiOpen(dst_file,O_RDWR|O_TRUNC)) == NULL) 
   	return E_ERROR;

    if_error(vuiCopyProtocol_(vfd_dst,vf_dst_index,vfd_src,vf_src_index,
			     image_group,protocol))
	return E_ERROR;

    VuiClose(vfd_dst);

    return E_NO_ERROR;
}

/*********************************************************************
* Description: プロトコルノードをコピーする。
* Assumptions:
 * Side Effects:
 * Return Value: E_NO_ERROR/others...
 *********************************************************************/
status_t CScrPadHandler::vuiCopyProtocol_(VfDBHandle_t	vfd_dst,
				int		vf_dst_index,
				VfDBHandle_t	vfd_src,
				int		vf_src_index,
				int		image_group,
				int		*protocol)
{
    VfPathElem_t	vf_dst_path[MAX_DEPTH],vf_src_path[MAX_DEPTH];
    int			vf_dst_depth,vf_src_depth;
    int			i,total,active;
    status_t		status;
    status_t		vfStat;
    BITFLD_DECL(mask,	MAX_BITFLD);

    /* エラーの場合この値が返る */
    if(protocol != NULL) *protocol = -1;

	memset( vf_dst_path , 0 , sizeof(vf_dst_path) ) ;
	memset( vf_src_path , 0 , sizeof(vf_src_path) ) ;

    vf_src_path[0].subtree_name = SVN_ST_PROTOCOL;
    vf_src_path[0].index	= vf_src_index;
    vf_dst_path[0].subtree_name = SVN_ST_PROTOCOL;
    vf_dst_path[0].index	= vf_dst_index;
    vf_dst_depth = vf_src_depth = SVD_ST_PROTOCOL;

    /* ターゲットノードの存在を確認するために VfScanSubtree をコールする。
     */
    total = MAX_BITFLD;
    BITFLD_INIT(mask,MAX_BITFLD,0);
    vfStat = VfScanSubtree(vfd_dst,vf_dst_path,SVD_ST_STUDY,
			   SVN_ST_PROTOCOL,mask,&total,&active);
    if(vfStat == E_EXISTS)
    {
#if 1
	/* VfCreateSubtree を引数 -1 でコールすると枝無しのノード
	 * を作成することができる。
	 * VfDeleteBranch を連続的にコールするとlibvfが異常終了する
	 * 場合がある。
	 */
	vfStat = VfCreateSubtree(vfd_dst,vf_dst_path,SVD_ST_STUDY,
				 SVN_ST_PROTOCOL,-1);
	if_error(vfStat)
	{
	    return vfStat;
	}
	vf_dst_path[0].index = vf_dst_index;
	active = total = 0;
#else
	/* サブツリーが存在しない場合、サブツリーを１つ作成し、すぐに
	 * 削除する。後で、指定ノードを VfAddBranch() するために、
	 * ブランチ_0 を作成し、即削除する。
	 */
	vfStat = VfCreateSubtree(vfd_dst,vf_dst_path,SVD_ST_STUDY,
				 SVN_ST_PROTOCOL,1);
	if_error(vfStat)
	{
	    return vfStat;
	}
	/* ブランチ_0 を削除する。
	 */
	vf_dst_path[0].index = 0;
	vfStat = VfDeleteBranch(vfd_dst,vf_dst_path,SVD_ST_PROTOCOL);
	if_error(vfStat)
	{
	    return vfStat;
	}
	vf_dst_path[0].index = vf_dst_index;
	active = total = 0;
#endif
    }
    else if(vfStat != E_NO_ERROR)
    {
	return vfStat;
    }

    /* ターゲットノードが存在するかどうかを確認する。
     */
    if(vf_dst_index < 0)
    {
	/* もし vf_dst_index が -1 なら、空いているノードをセットする。
	 */
	if(active == total)	  /* 全てのノードが埋まっている */
	    vf_dst_index = total; /* 0 origin であるため total
				   * は最後のノードの次を意味する。
				   */
	else
	{
	    for(i = 0;i < total;i++)
	    {
		if(!BITFLD_ON(mask,i))	/* 空いているノードを探す */
		{
		    vf_dst_index = i;
		    break;
		}
	    }
	    /* このパスは意味がないはず、万が一のためにこのコードを残す。
	     */
	    if(vf_dst_index < 0)
		vf_dst_index = total;
	}
	vf_dst_path[0].index	= vf_dst_index;
    }
    else if(vf_dst_index < total && BITFLD_ON(mask,vf_dst_index))
    {
	/* ターゲットノードが存在する場合、そのブランチを削除する。
	 * 後で、VfAddBranch() をするがこの操作で前に存在したデータ
	 * を削除することを目的としている。
	 */
	vfStat = VfDeleteBranch(vfd_dst,vf_dst_path,SVD_ST_PROTOCOL);
	if_error(vfStat)
	{
	    return vfStat;
	}
    }

    /* ターゲットノードを追加する。
     */
    vfStat = VfAddBranch(vfd_dst,vf_dst_path,SVD_ST_PROTOCOL);
    if_error(vfStat)
    {
	return vfStat;
    }
    status = vuiCopyNode_(vfd_dst,
			 vf_dst_path,
			 vf_dst_depth,
			 vfd_src,
			 vf_src_path,
			 vf_src_depth,
			 image_group);

    /* V5.00 Speeder Map の場合は subProtocol を外す  */
    vuiCheckSpeeder_(vfd_dst, vf_dst_index) ;

    /* V5.00 3D-locator AutoProEdit を外す  */
    vuiDelAutoProEdit_(vfd_dst, vf_dst_index) ;

    /* V5.00 InjectTime  を外す  */
    vuiDelInjectTime_(vfd_dst, vf_dst_index) ;

    /* V5.10 SPEEDER CORRECT_PARAMS  を外す  */
    vuiDelCorrect_params_(vfd_dst, vf_dst_index) ;

    /* V5.10 SPEEDER CORRECT_PARAMS  を外す  */
    vuiDelPVCMB_params_(vfd_dst, vf_dst_index) ;

    /*** V9.01 位相補正PreScanパラメータを外す ***/
    vuiDelPhaseCorrection_params_(vfd_dst, vf_dst_index);

    vuiDelMoveCouchMethod_(vfd_dst, vf_dst_index);

    /*** JET 縮退情報を外す ***/
	vuiDelDegeneracy_(vfd_dst, vf_dst_index);

	/*** SYMC_HOST情報を外す ***/
	vuiDelSyncHost_(vfd_dst, vf_dst_index);

	/*** CDS情報を外す ***/
	vuiDelCds_(vfd_dst, vf_dst_index);

	/*** プレスキャン寝台位置情報を外す ***/
	vuiDelPrePos_(vfd_dst, vf_dst_index);

    if(protocol != NULL)
	*protocol = vf_dst_index;

    
    return status;
}


/*********************************************************************
 * Description: 再帰的にノードをコピーする。
 * Assumptions:
 * Side Effects:
 * Return Value: E_NO_ERROR/others...
 *********************************************************************/
 status_t CScrPadHandler::vuiCopyNode_(VfDBHandle_t	vfd_dst,
			    VfPathElem_t	vf_dst_path[],
			    int			vf_dst_depth,
			    VfDBHandle_t	vfd_src,
			    VfPathElem_t	vf_src_path[],
			    int			vf_src_depth,
			    int			image_group)
{
    VfPathElem_t	vf_dst_path_save[MAX_DEPTH];
    VfPathElem_t	vf_src_path_save[MAX_DEPTH];
    VfFieldSpec_t	*vf_dst_req = NULL;
    VfFieldSpec_t	*vf_src_req = NULL;
    int			vf_dst_req_len = 0;
    int			vf_src_req_len = 0;
    int			vf_num = 0;
    VfName_t		subtrees[MAX_DEPTH];
    int			num_subtree = 0;
    time_t		vf_tm = 0;
    register int	i = 0;
    status_t		vfStat = 0;
    status_t		status = 0;

    /* パスを保存する。バーフィールドライブラリの中にはパスをバッファと
     * 使用し、パスの内容を変更する曲者がいるようだ。今は、大丈夫かも知
     * れないが不安なので保険のためにデータをセーブしておく。
     * 再帰呼び出しを使用しなければ、このようなことをする必要もないのだ
     * が…
     */
    memcpy(vf_dst_path_save,vf_dst_path,sizeof(VfPathElem_t) * vf_dst_depth);
    memcpy(vf_src_path_save,vf_src_path,sizeof(VfPathElem_t) * vf_src_depth);

    /* フィールドデータのリストを取得する。この段階ではデータ
     * (vf_src_req->buffer) は入らない。
     */
    vfStat = VfGetAllFields(vfd_src,vf_src_path,vf_src_depth,NULL,
			    &vf_src_req,&vf_src_req_len);
    if_error(vfStat)
    {
	return vfStat;
    }

    if(vf_src_req_len == 0) return E_NO_ERROR;
    
    vf_dst_req = (VfFieldSpec_t *)malloc(sizeof(VfFieldSpec_t) *
					 vf_src_req_len);
    ZeroMemory(vf_dst_req,sizeof(VfFieldSpec_t) *vf_src_req_len);

    if(vf_dst_req == NULL)
	return E_ERROR;
    
    /* 実際に VfGetFields() をする際のオプションをセットする。
     * サブツリーの場合(VFT_SUBTREE)、後でノードを作成するためにバッファに
     * コピーしておく。
     */
    num_subtree = 0;
    vf_dst_req_len = 0;
    for(i = 0;i < vf_src_req_len;i++)
    {
	if(vf_src_req[i].type == VFT_SUBTREE)
	{
	    if(image_group == 0 &&
	       vf_src_req[i].name == SVN_ST_PROCESSED)
	    {
		/* 画像ノードをコピーしない。*/
	    }
	    else
	    {
		subtrees[num_subtree] = vf_src_req[i].name;
		num_subtree++;
	    }
	}
	else if((vf_src_depth == SVD_ST_IMAGE &&
		(vf_src_req[i].name == SVN_GEN_DATA 
		||(vf_src_req[i].name >= SVN_GEN_DATA1 &&
		 vf_src_req[i].name <= SVN_GEN_DATA127))) 
		 ||	/* MRS データはコピーしない */
		((vf_src_req[i].name >= SVN_MRS_REF_RAW_DATA &&
		  vf_src_req[i].name <= SVN_MRS_REF_RAW_DATA7 ) ||
		 (vf_src_req[i].name >= SVN_MRS_REF_RAW_DATA8 &&
		  vf_src_req[i].name <= SVN_MRS_REF_RAW_DATA127)||
		 (vf_src_req[i].name >= SVN_MRS_REF_WSIGNAL  &&
		  vf_src_req[i].name <= SVN_MRS_REF_WSIGNAL7 ) ||
		 (vf_src_req[i].name >= SVN_MRS_REF_WSIGNAL8 &&
		  vf_src_req[i].name <= SVN_MRS_REF_WSIGNAL127)||
		 vf_src_req[i].name == SVN_MRS_WATER_DATA  ||
		 vf_src_req[i].name == SVN_MRS_FS_DATA		 ) 
		 || /*	   位相補正生データはコピーしない V9.01 */
		 (vf_src_req[i].name == SVN_GEN_PHASE_CORRECTION_DATA))
	{
	    /* PAC のエレメントが増えるたびに条件を追加しないとダメ */
	    /* V9.00では128chまで拡張されたが、GEN_DATAに関しては連番で発番
	       されているので、個々のチェックではなく、範囲でチェックするようにした。*/
	    /* 生データはコピーしない。*/;
	}
	else
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

    if(vf_dst_req_len > 0)
    {
	vfStat = VfGetFields(vfd_src,vf_src_path,vf_src_depth,
			     vf_dst_req,vf_dst_req_len,&vf_num);
	if_error(vfStat)
	{
	    return vfStat;
	}
	
	/* フィールドのデータを吐き出すためにオプションをセットし直す。
	 */
	for(i = 0;i < vf_dst_req_len;i++)
	{
	    /* group >0 は画像データのはず… */
	    if(vf_dst_req[i].group > 0)
		vf_dst_req[i].group = image_group;
	    VFF_SET_ARGSG(vf_dst_req[i],
			  vf_dst_req[i].name,
			  vf_dst_req[i].type,
			  vf_dst_req[i].size,
			  vf_dst_req[i].buffer,
			  VFO_REPLACE,
			  vf_dst_req[i].group);
	}
	/* フィールドのデータを吐き出す。
	 */
	vfStat = VfPutFields(vfd_dst,vf_dst_path,vf_dst_depth,
			     vf_dst_req,vf_dst_req_len,&vf_num,&vf_tm);
	if_error(vfStat)
	{
	    return vfStat;
	}
	/* バッファ領域を解放する。
	 */
	for(i = 0;i < vf_dst_req_len;i++)
	{
	    if(vf_dst_req[i].buffer != NULL)
	    {
		//free(vf_dst_req[i].buffer);
		vf_dst_req[i].buffer = NULL;
	    }
	}
    }
    /* フィールド用バッファをクリアする。
     */
    //free(vf_src_req);
    if(vf_dst_req){
        free(vf_dst_req);
    }
    vf_src_req = vf_dst_req = NULL;
    
    /* サブツリーを作成する。再帰呼出しされる点に注意！！
     */
    for(i = 0;i < num_subtree;i++)
    {
	memcpy(vf_src_path,vf_src_path_save,
	       sizeof(VfPathElem_t) * vf_src_depth);
	memcpy(vf_dst_path,vf_dst_path_save,
	       sizeof(VfPathElem_t) * vf_dst_depth);

	status = vuiCopySubtree_(vfd_dst,
				vf_dst_path,
				vf_dst_depth,
				vfd_src,
				vf_src_path,
				vf_src_depth,
				image_group,
				subtrees[i]);
	if_error(status)
	    return status;
    }
    return E_NO_ERROR;
}


/*********************************************************************
 * Description: サブツリーを再帰的に作成する。
 * Assumptions:
 * Side Effects:
 * Return Value: E_NO_ERROR/others...
 *********************************************************************/
status_t CScrPadHandler::vuiCopySubtree_(VfDBHandle_t	vfd_dst,
			       VfPathElem_t	vf_dst_path[],
			       int		vf_dst_depth,
			       VfDBHandle_t	vfd_src,
			       VfPathElem_t	vf_src_path[],
			       int		vf_src_depth,
			       int		image_group,
			       VfName_t		subtree_name)
{
    VfPathElem_t	vf_dst_path_save[MAX_DEPTH];
    VfPathElem_t	vf_src_path_save[MAX_DEPTH];
    register int	i;
    int			total,active;
    status_t		status;
    status_t		vfStat;
    BITFLD_DECL(mask,	MAX_BITFLD);
    DB_FUNC_ENTER("[vuiCopySubtree]:");

    /* ノードの数を取得する。
     */
    total = MAX_BITFLD;
    BITFLD_INIT(mask,MAX_BITFLD,0);
    vfStat = VfScanSubtree(vfd_src,vf_src_path,vf_src_depth,
			   subtree_name,mask,&total,&active);
	if_error(vfStat) 
    {
	return vfStat;
	}
    /* フィールドはあるがノードが存在しない場合正常終了を返す
     */
    if(active == 0) return E_NO_ERROR;
  

    /* ノードを作成する。この段階ではノードの歯抜けが存在しない
     * (total == active) という前提で、取得したノード数を全て作成する。
     *
     * しかしtotal == active ということはありえない。
	 * 実際にDeleteSubtreeするとTotal != activeになる。
	 * よってActiveなノード数作成することにする。　（V6.00）
     * ここではノードは作らずにSubtreeのみ作成する。  
	 * 実際のBranchはノードコピーする際に追加作成する。
	 */

	vfStat = VfCreateSubtree(vfd_dst,vf_dst_path,vf_dst_depth,
			     subtree_name,-1);
	if ( vfStat != E_EXISTS)
	{
		if_error(vfStat)
		{
			return vfStat;
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
    memcpy(vf_dst_path_save,vf_dst_path,sizeof(VfPathElem_t) * vf_dst_depth);
    memcpy(vf_src_path_save,vf_src_path,sizeof(VfPathElem_t) * vf_src_depth);

    /* 作成したノードに対してフィールドコピー関数をコールする。
     * 再帰的にコールされ点に注意！！！
     */
    for(i = 0;i < total;i++)
    {
	if(BITFLD_ON(mask,i))	/* add. V5.00 */
	{
		memcpy(vf_dst_path,vf_dst_path_save,
			sizeof(VfPathElem_t) * vf_dst_depth);
		memcpy(vf_src_path,vf_src_path_save,
			sizeof(VfPathElem_t) * vf_src_depth);
		vf_dst_path[vf_dst_depth - 1].index = i;
		vf_src_path[vf_src_depth - 1].index = i;

		/* Activeなブランチだけを追加する V6.00 */
	    status = VfAddBranch(vfd_dst,vf_dst_path,vf_dst_depth);
		if_error(status)
		{
			return status;
		}

		status = vuiCopyNode_(vfd_dst,
			     vf_dst_path,
			     vf_dst_depth,
			     vfd_src,
			     vf_src_path,
			     vf_src_depth,
			     image_group);
		if_error(status)
			return status;
	}
    }
    return E_NO_ERROR;
}



/*********************************************************************
 * Description: SPEEDER MAP 撮影の場合はSUB_PROTOCOLを削除する。
 *              Dynamic 方式になったのでDynamicFlagも削除する。　V6.20
 * Assumptions:
 * Side Effects:
 * Return Value: E_NO_ERROR/others...
 *********************************************************************/
status_t	CScrPadHandler::vuiCheckSpeeder_(VfDBHandle_t vfd, int   protocol)
{

    VfFieldSpec_t       fieldSpec[6];
    int                 n , numDone;
    int			total_time, speeder_type,mode;
    int                 total,active;
    time_t              ts; 
    VfPathElem_t        path[5];
    status_t            sts, status  ;

    BITFLD_DECL(mask,MAX_BITFLD);
    
    status = E_NO_ERROR ;
    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = protocol;
    path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    path[1].index = 0;

    n = 0 ;
    VFF_SET_ARGS(fieldSpec[n],SVN_PLN_SPEEDER_TYPE,VFT_INT32,
                sizeof(speeder_type),&speeder_type,0L);       n++;

    status = VfGetFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec, n, (int*)&numDone);

    if ( n == numDone && speeder_type == VFC_SPEEDER_TYPE_SENSITIVITY ) 
    {
	/* V6.20からDynamic方式になった。　
	　 DynamicFlagを削除する。
	   スキャンタイムをもどす。
	   またCOIL切り替えはPV+MVの方式をそのまま使用する。
	   各種TBL類はそちらの処理{vuiDelPVCMB_params()}で削除する
	*/
	n = 0 ;
	VFF_SET_ARGS(fieldSpec[n],SVN_PLN_SUBPROTOCOL_TIME,VFT_INT32,
			sizeof(total_time),&total_time,0L);       n++;
	status = VfGetFields(vfd, path, SVD_ST_SUBPROTOCOL, fieldSpec, n, (int*)&numDone);
	if ( n == numDone )
	{
		n = 0 ;
		VFF_SET_ARGS(fieldSpec[n],SVN_PLN_TOTAL_TIME,VFT_INT32,
			sizeof(total_time),&total_time,VFO_REPLACE);       n++;
		status = VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec, n, (int*)&numDone, &ts);

	}

	mode = FALSE ;
	n = 0 ;
	VFF_SET_ARGS(fieldSpec[n],SVN_PLN_DYNAMIC_SCAN ,VFT_BOOL,
			sizeof(mode),&mode,VFO_REPLACE);       n++;
	VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec, n, (int*)&numDone, &ts);

    	memset(mask,0,sizeof(mask));
    	total = MAX_BITFLD;
    	active = 0;
	sts = VfScanSubtree(vfd, path,SVD_ST_PROTOCOL,SVN_ST_SUBPROTOCOL,
					mask,&total,&active);

	if ( sts == E_NO_ERROR && active > 1 )
	{
	    int flag = FALSE ;
	    for( n = 0 ; n < total ; n++ )
	    {
		if ( flag == TRUE && BITFLD_ON(mask,n))
		{
		    path[1].subtree_name = SVN_ST_SUBPROTOCOL;
		    path[1].index = n ;
		    status = VfDeleteBranch(vfd,  path , SVD_ST_SUBPROTOCOL) ;
		    if ( status != E_NO_ERROR )
		    	break ;
		}

		if(BITFLD_ON(mask,n))	/* add. V5.00 */
		    flag = TRUE ;
	    }
	    if ( status == E_NO_ERROR )
	    {
		path[1].subtree_name = SVN_ST_SUBPROTOCOL;
		path[1].index = 0;
		n = 0 ;
		VFF_SET_ARGS(fieldSpec[n],SVN_PLN_SUBPROTOCOL_TIME,VFT_INT32,
				sizeof(total_time),&total_time,0L);       n++;
		status = VfGetFields(vfd, path, SVD_ST_SUBPROTOCOL, fieldSpec, n, (int*)&numDone);

		if ( n == numDone )
		{
			n = 0 ;
			VFF_SET_ARGS(fieldSpec[n],SVN_PLN_TOTAL_TIME,VFT_INT32,
	     			sizeof(total_time),&total_time,VFO_REPLACE);       n++;
			status = VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec, n, (int*)&numDone, &ts);

		}
 	    }
	}
    }
    else
      status = E_ERROR ;

    return status ;
}

/*********************************************************************
 * Description: 3D-locator AutoProEdit を外す
 * Assumptions:
 * Side Effects:
 * Return Value: E_NO_ERROR/others...
 *********************************************************************/
 void	CScrPadHandler::vuiDelAutoProEdit_(VfDBHandle_t vfd, int   protocol)
{

    VfFieldSpec_t       fieldSpec[1];
    int                 n , numDone;
    int32_t			mode ;
    time_t              ts; 
    VfPathElem_t        path[5];

    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = protocol;
    path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    path[1].index = 0;

    n = 0 ;
    VFF_SET_ARGS(fieldSpec[n],SVN_PLN_AUTO_PROEDIT_FLAG,VFT_INT32,
		 sizeof(mode),&mode,0L);       n++;
    VfGetFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec, n, (int*)&numDone);
    if ( n == numDone  && mode == VFC_AUTO_PROEDIT_3DLOCATOR )
    {
	mode = VFC_AUTO_PROEDIT_NONE ;
	n = 0 ;
	VFF_SET_ARGS(fieldSpec[n],SVN_PLN_AUTO_PROEDIT_FLAG,VFT_INT32,
			 sizeof(mode),&mode,VFO_DELETE);       n++;
	VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec, n, (int*)&numDone, &ts);
    }

}

/*********************************************************************
 * Description: INJECT TIME を外す
 * Assumptions:
 * Side Effects:
 * Return Value: E_NO_ERROR/others...
 *********************************************************************/
 void	CScrPadHandler::vuiDelInjectTime_(VfDBHandle_t vfd, int   protocol)
{

    VfFieldSpec_t       fieldSpec[1];
    int                 n , numDone;
    int32_t		tm ;
    time_t              ts; 
    VfPathElem_t        path[5];

    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = protocol;
    path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    path[1].index = 0;

    n = 0 ;
    VFF_SET_ARGS(fieldSpec[n],SVN_EXE_INJECT_TIME,VFT_TIME,
		 sizeof(tm),&tm,0L);       n++;
    VfGetFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec, n, (int*)&numDone);
    if ( n == numDone  )
    {
	tm = 0 ;
	n = 0 ;
	VFF_SET_ARGS(fieldSpec[n],SVN_EXE_INJECT_TIME,VFT_TIME,
			 sizeof(tm),&tm,VFO_DELETE);       n++;
	VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec, n, (int*)&numDone, &ts);
    }

}

/*********************************************************************
 * Description: Speeder SVN_EXE_PAC_PHASE_CORRECT_PARAMS
 			SVN_EXE_PAC_GAIN_CORRECT_PARAMS
		を外す
 * Assumptions:
 * Side Effects:
 * Return Value: E_NO_ERROR/others...
 *********************************************************************/
 void	CScrPadHandler::vuiDelCorrect_params_(VfDBHandle_t vfd, int   protocol)
{

    VfFieldSpec_t       fieldSpec[4];
    VfFieldSpec_t       fieldSpec1[1];
    int                 n , numDone;
    time_t              ts; 
    VfPathElem_t        path[5];

    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = protocol;

    n = 0 ;
    VFF_SET_ARGS(fieldSpec[n],SVN_EXE_PAC_PHASE_CORRECT_PARAMS,VFT_FLT32,
    		 0, NULL, VFO_VARIABLE_LEN); n++;
    VFF_SET_ARGS(fieldSpec[n],SVN_EXE_PAC_GAIN_CORRECT_PARAMS,VFT_FLT32,
    		 0, NULL, VFO_VARIABLE_LEN); n++;
    VfGetFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec, n, (int*)&numDone);

    if ( fieldSpec[0].status == E_NO_ERROR )
    {
	n = 0 ;
	VFF_SET_ARGS(fieldSpec1[n],SVN_EXE_PAC_PHASE_CORRECT_PARAMS,VFT_FLT32,
			 fieldSpec[0].size,NULL,VFO_DELETE);       n++;
	VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec1, n, (int*)&numDone, &ts);
    }
    if ( fieldSpec[1].status == E_NO_ERROR )
    {
	n = 0 ;
	VFF_SET_ARGS(fieldSpec1[n],SVN_EXE_PAC_GAIN_CORRECT_PARAMS,VFT_FLT32,
			 fieldSpec[1].size,NULL,VFO_DELETE);       n++;
	VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec1, n, (int*)&numDone, &ts);
    }
}

/*********************************************************************
 * Description: PV+MB   SVN_EXE_CHANGED_RECEIVE_COIL
 			SVN_EXE_PAC_PATTERN_TBL
 			SVN_EXE_RECVR_COIL_TBL
 			SVN_EXE_RECVR_COILNAME_TBL
 			SVN_EXE_CHANNEL_GAIN_RATIO_TBL
			SVN_EXE_PAC_PHASE_CORRECT_PARAMS_TBL
			SVN_EXE_PAC_GAIN_CORRECT_PARAMS_TBL
		を外す
 * Assumptions:
 * Side Effects:
 * Return Value: E_NO_ERROR/others...
 *********************************************************************/
 void	CScrPadHandler::vuiDelPVCMB_params_(VfDBHandle_t vfd, int   protocol)
{

    VfFieldSpec_t       fieldSpec[8];
    VfFieldSpec_t       fieldSpec1[8];
    int                 n , numDone;
    time_t              ts; 
    VfPathElem_t        path[5];

    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = protocol;

    n = 0 ;
    VFF_SET_ARGS(fieldSpec[n],SVN_EXE_CHANGED_RECEIVE_COIL,VFT_INT32,
    		 0, NULL, VFO_VARIABLE_LEN); n++;
    VFF_SET_ARGS(fieldSpec[n],SVN_EXE_PAC_PATTERN_TBL,VFT_INT32,
    		 0, NULL, VFO_VARIABLE_LEN); n++;
    VFF_SET_ARGS(fieldSpec[n],SVN_EXE_RECVR_COIL_TBL,VFT_INT32,
    		 0, NULL, VFO_VARIABLE_LEN); n++;
    VFF_SET_ARGS(fieldSpec[n],SVN_EXE_RECVR_COILNAME_TBL,VFT_STRING,
    		 0, NULL, VFO_VARIABLE_LEN); n++;
    VFF_SET_ARGS(fieldSpec[n],SVN_EXE_CHANNEL_GAIN_RATIO_TBL,VFT_FLT32,
    		 0, NULL, VFO_VARIABLE_LEN); n++;
    VFF_SET_ARGS(fieldSpec[n],SVN_EXE_PAC_PHASE_CORRECT_PARAMS_TBL,VFT_FLT32,
    		 0, NULL, VFO_VARIABLE_LEN); n++;
    VFF_SET_ARGS(fieldSpec[n],SVN_EXE_PAC_GAIN_CORRECT_PARAMS_TBL,VFT_FLT32,
    		 0, NULL, VFO_VARIABLE_LEN); n++;
    VfGetFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec, n, (int*)&numDone);

    if ( fieldSpec[0].status == E_NO_ERROR )
    {
	n = 0 ;
        VFF_SET_ARGS(fieldSpec1[n],SVN_EXE_CHANGED_RECEIVE_COIL,VFT_INT32,
			 fieldSpec[0].size,NULL,VFO_DELETE);       n++;
	VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec1, n, (int*)&numDone, &ts);
    }
    if ( fieldSpec[1].status == E_NO_ERROR )
    {
	n = 0 ;
    	VFF_SET_ARGS(fieldSpec1[n],SVN_EXE_PAC_PATTERN_TBL,VFT_INT32,
			 fieldSpec[1].size,NULL,VFO_DELETE);       n++;
	VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec1, n, (int*)&numDone, &ts);
    }
    if ( fieldSpec[2].status == E_NO_ERROR )
    {
	n = 0 ;
    	VFF_SET_ARGS(fieldSpec1[n],SVN_EXE_RECVR_COIL_TBL,VFT_INT32,
			 fieldSpec[3].size,NULL,VFO_DELETE);       n++;
	VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec1, n, (int*)&numDone, &ts);
    }
    if ( fieldSpec[3].status == E_NO_ERROR )
    {
	n = 0 ;
    	VFF_SET_ARGS(fieldSpec1[n],SVN_EXE_RECVR_COILNAME_TBL,VFT_STRING,
			 fieldSpec[3].size,NULL,VFO_DELETE);       n++;
	VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec1, n, (int*)&numDone, &ts);
    }
    if ( fieldSpec[4].status == E_NO_ERROR )
    {
	n = 0 ;
    	VFF_SET_ARGS(fieldSpec1[n],SVN_EXE_CHANNEL_GAIN_RATIO_TBL,VFT_FLT32,
			 fieldSpec[4].size,NULL,VFO_DELETE);       n++;
	VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec1, n, (int*)&numDone, &ts);
    }
    if ( fieldSpec[5].status == E_NO_ERROR )
    {
	n = 0 ;
    	VFF_SET_ARGS(fieldSpec1[n],SVN_EXE_PAC_PHASE_CORRECT_PARAMS_TBL,VFT_FLT32,
			 fieldSpec[5].size,NULL,VFO_DELETE);       n++;
	VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec1, n, (int*)&numDone, &ts);
    }
    if ( fieldSpec[6].status == E_NO_ERROR )
    {
	n = 0 ;
    	VFF_SET_ARGS(fieldSpec1[n],SVN_EXE_PAC_GAIN_CORRECT_PARAMS_TBL,VFT_FLT32,
			 fieldSpec[6].size,NULL,VFO_DELETE);       n++;
	VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec1, n, (int*)&numDone, &ts);
    }
}

/*********************************************************************
 * Description: 位相補正プリスキャン用の不要フィールド削除
			/1.x/2.0/ SVN_EXE_ANGLE_OFFSET
 			/1.x/2.0/ SVN_EXE_RO_TUNE_VALUE
 			/1.x/2.0/ SVN_EXE_PHASE_CORRECTION_PRESCAN_STATUS
 			/1.x/2.0/ SVN_GEN_PHASE_CORRECTION_DATA
			/1.x/2.0/ SVN_EXE_PHASE_CORRECTION_PRESCAN_SCAN_START
		を外す
 * Assumptions:　　V9.01
 * Side Effects:
 * Return Value: E_NO_ERROR/others...
 *********************************************************************/
 void	CScrPadHandler::vuiDelPhaseCorrection_params_(VfDBHandle_t vfd, int   protocol)
{

    VfFieldSpec_t       fieldSpec[8];
    VfFieldSpec_t       fieldSpec1[8];
    int                 n , numDone;
    time_t              ts; 
    VfPathElem_t        path[5];

    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = protocol;
    path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    path[1].index = 0;

    /********* SUBPROTOCOLノードの情報削除 *********/
    n = 0 ;
    VFF_SET_ARGS(fieldSpec[n],SVN_EXE_ANGLE_OFFSET,VFT_FLT32,
    		 0, NULL, VFO_VARIABLE_LEN); n++;
    VFF_SET_ARGS(fieldSpec[n],SVN_EXE_RO_TUNE_VALUE,VFT_FLT32,
    		 0, NULL, VFO_VARIABLE_LEN); n++;
    VFF_SET_ARGS(fieldSpec[n],SVN_EXE_PHASE_CORRECTION_PRESCAN_STATUS,VFT_INT32,
    		 0, NULL, VFO_VARIABLE_LEN); n++;
    VFF_SET_ARGS(fieldSpec[n],SVN_EXE_PHASE_CORRECTION_PRESCAN_SCAN_START,VFT_TIME,
    		 0, NULL, VFO_VARIABLE_LEN); n++;
    VfGetFields(vfd, path, SVD_ST_SUBPROTOCOL, fieldSpec, n, (int*)&numDone);

    n = 0 ;
    if ( fieldSpec[0].status == E_NO_ERROR )
    {
        VFF_SET_ARGS(fieldSpec1[n],SVN_EXE_ANGLE_OFFSET,VFT_FLT32,
			 fieldSpec[0].size,NULL,VFO_DELETE);       n++;
    }
    if ( fieldSpec[1].status == E_NO_ERROR )
    {
    	VFF_SET_ARGS(fieldSpec1[n],SVN_EXE_RO_TUNE_VALUE,VFT_FLT32,
			 fieldSpec[1].size,NULL,VFO_DELETE);       n++;
    }
    if ( fieldSpec[2].status == E_NO_ERROR )
    {
    	VFF_SET_ARGS(fieldSpec1[n],SVN_EXE_PHASE_CORRECTION_PRESCAN_STATUS,VFT_INT32,
			 fieldSpec[2].size,NULL,VFO_DELETE);       n++;
    }
    if ( fieldSpec[3].status == E_NO_ERROR )
    {
    	VFF_SET_ARGS(fieldSpec1[n],SVN_EXE_PHASE_CORRECTION_PRESCAN_SCAN_START,VFT_TIME,
			 fieldSpec[3].size,NULL,VFO_DELETE);       n++;
    }
    if( n )
    {
	VfPutFields(vfd, path, SVD_ST_SUBPROTOCOL, fieldSpec1, n, (int*)&numDone, &ts);
    }

}

/*********************************************************************
 * Description: DEGENERACY を外す
 * Assumptions:
 * Side Effects:
 * Return Value: E_NO_ERROR/others...
 *********************************************************************/
 void	CScrPadHandler::vuiDelDegeneracy_(VfDBHandle_t vfd, int   protocol)
{

    VfFieldSpec_t       fieldSpec[3];
    VfFieldSpec_t       fieldSpec1[1];
    int                 n , numDone;
//     int32_t             numSection = 0;
    time_t              ts; 
    VfPathElem_t        path[5];

    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = protocol;

    n = 0 ;
    VFF_SET_ARGS(fieldSpec[n],SVN_EXE_NUM_PAC_DEGENERACY,SVT_EXE_NUM_PAC_DEGENERACY,
    		 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN); n++;
    VFF_SET_ARGS(fieldSpec[n],SVN_PLN_SECTION_RECVR_COIL_SIGNAL_DEGENERACY,SVT_PLN_SECTION_RECVR_COIL_SIGNAL_DEGENERACY,
    		 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN); n++;
    VFF_SET_ARGS(fieldSpec[n],SVN_EXE_DEGENERACY_PAC_MATRIX,SVT_EXE_DEGENERACY_PAC_MATRIX,
    		 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN); n++;
    VfGetFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec, n, (int*)&numDone);

    if ( fieldSpec[0].status == E_NO_ERROR )
    {
	n = 0 ;
        VFF_SET_ARGS(fieldSpec1[n],SVN_EXE_NUM_PAC_DEGENERACY,SVT_EXE_NUM_PAC_DEGENERACY,
			 fieldSpec[0].size,NULL,VFO_DELETE);       n++;
	VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec1, n, (int*)&numDone, &ts);
    }
    if ( fieldSpec[1].status == E_NO_ERROR )
    {
	n = 0 ;
    	VFF_SET_ARGS(fieldSpec1[n],SVN_PLN_SECTION_RECVR_COIL_SIGNAL_DEGENERACY,SVT_PLN_SECTION_RECVR_COIL_SIGNAL_DEGENERACY,
			 fieldSpec[1].size,NULL,VFO_DELETE);       n++;
	VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec1, n, (int*)&numDone, &ts);
	}
    if ( fieldSpec[2].status == E_NO_ERROR )
    {
	n = 0 ;
    	VFF_SET_ARGS(fieldSpec1[n],SVN_EXE_DEGENERACY_PAC_MATRIX,SVT_EXE_DEGENERACY_PAC_MATRIX,
			 fieldSpec[2].size,NULL,VFO_DELETE);       n++;
	VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec1, n, (int*)&numDone, &ts);
	}

}

/*********************************************************************
<<<<<<< vuiCopy.c
 * Description: SYNC_HOST を外す
 * Assumptions:
 * Side Effects:
 * Return Value: E_NO_ERROR/others...
 *********************************************************************/
 void	CScrPadHandler::vuiDelSyncHost_(VfDBHandle_t vfd, int   protocol)
{

    VfFieldSpec_t       fieldSpec[1];
    VfFieldSpec_t       fieldSpec1[1];
    int                 n , numDone;
//     int32_t             numSection = 0;
    time_t              ts; 
    VfPathElem_t        path[5];

    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = protocol;

    n = 0 ;
    VFF_SET_ARGS(fieldSpec[n],SVN_DCM_SYNC_HOST,SVT_DCM_SYNC_HOST,
    		 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN); n++;
    VfGetFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec, n, (int*)&numDone);

    if ( fieldSpec[0].status == E_NO_ERROR )
    {
	n = 0 ;
        VFF_SET_ARGS(fieldSpec1[n],SVN_DCM_SYNC_HOST,SVT_DCM_SYNC_HOST,
			 fieldSpec[0].size,NULL,VFO_DELETE);       n++;
	VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec1, n, (int*)&numDone, &ts);
    }

}

/*********************************************************************
 * Description: CDS を外す
 * Assumptions:
 * Side Effects:
 * Return Value: E_NO_ERROR/others...
 *********************************************************************/
 void	CScrPadHandler::vuiDelCds_(VfDBHandle_t vfd, int   protocol)
{
	VfFieldSpec_t       fieldSpec[7];
	VfFieldSpec_t       fieldSpec1[7];
	int                 n , numDone;
// 	int32_t             numSection = 0;
	time_t              ts; 
	VfPathElem_t        path[5];

	path[0].subtree_name = SVN_ST_PROTOCOL;
	path[0].index = protocol;

	n = 0 ;

	VFF_SET_ARGS(fieldSpec[n],SVN_PLN_CDS_STATUS,SVT_PLN_CDS_STATUS,
			 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN); n++;
	VFF_SET_ARGS(fieldSpec[n],SVN_PLN_CDS_COIL_COORDINATE,SVT_PLN_CDS_COIL_COORDINATE,
			 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN); n++;

	VFF_SET_ARGS(fieldSpec[n],SVN_EXE_CDS_COIL_LOCATION,SVT_EXE_CDS_COIL_LOCATION,
			 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN); n++;

	VFF_SET_ARGS(fieldSpec[n],SVN_PLN_REPETITIONS_CDS,SVT_PLN_REPETITIONS_CDS,
			 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN); n++;
	VFF_SET_ARGS(fieldSpec[n],SVN_PLN_NUM_SECTION_RECVR_COIL_CDS,SVT_PLN_NUM_SECTION_RECVR_COIL_CDS,
			 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN); n++;
	VFF_SET_ARGS(fieldSpec[n],SVN_PLN_SECTION_RECVR_COIL_CDS,SVT_PLN_SECTION_RECVR_COIL_CDS,
			 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN); n++;
	VFF_SET_ARGS(fieldSpec[n],SVN_PLN_SECTION_RECVR_COIL_SIGNAL_CDS,SVT_PLN_SECTION_RECVR_COIL_SIGNAL_CDS,
			 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN); n++;

	VfGetFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec, n, (int*)&numDone);

	n = 0 ;
	if ( fieldSpec[0].status == E_NO_ERROR )
	{
	    VFF_SET_ARGS(fieldSpec1[n],SVN_PLN_CDS_STATUS,SVT_PLN_CDS_STATUS,
			 fieldSpec[0].size,NULL,VFO_DELETE);       n++;
	}
	if ( fieldSpec[1].status == E_NO_ERROR )
	{
		VFF_SET_ARGS(fieldSpec1[n],SVN_PLN_CDS_COIL_COORDINATE,SVT_PLN_CDS_COIL_COORDINATE,
			 fieldSpec[1].size,NULL,VFO_DELETE);       n++;
	}
	if ( fieldSpec[2].status == E_NO_ERROR )
	{
		VFF_SET_ARGS(fieldSpec1[n],SVN_EXE_CDS_COIL_LOCATION,SVT_EXE_CDS_COIL_LOCATION,
			 fieldSpec[2].size,NULL,VFO_DELETE);       n++;
	}
	if ( fieldSpec[3].status == E_NO_ERROR )
	{
		VFF_SET_ARGS(fieldSpec1[n],SVN_PLN_REPETITIONS_CDS,SVT_PLN_REPETITIONS_CDS,
			 fieldSpec[3].size,NULL,VFO_DELETE);       n++;
	}
	if ( fieldSpec[4].status == E_NO_ERROR )
	{
		VFF_SET_ARGS(fieldSpec1[n],SVN_PLN_NUM_SECTION_RECVR_COIL_CDS,SVT_PLN_NUM_SECTION_RECVR_COIL_CDS,
			 fieldSpec[4].size,NULL,VFO_DELETE);       n++;
	}
	if ( fieldSpec[5].status == E_NO_ERROR )
	{
		VFF_SET_ARGS(fieldSpec1[n],SVN_PLN_SECTION_RECVR_COIL_CDS,SVT_PLN_SECTION_RECVR_COIL_CDS,
			 fieldSpec[5].size,NULL,VFO_DELETE);       n++;
	}
	if ( fieldSpec[6].status == E_NO_ERROR )
	{
		VFF_SET_ARGS(fieldSpec1[n],SVN_PLN_SECTION_RECVR_COIL_SIGNAL_CDS,SVT_PLN_SECTION_RECVR_COIL_SIGNAL_CDS,
			 fieldSpec[6].size,NULL,VFO_DELETE);       n++;
	}
	if( n )
	{
		VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec1, n, (int*)&numDone, &ts);
	}
}

/*********************************************************************
 * Description: プレスキャン寝台情報 を外す
 * Assumptions:
 * Side Effects:
 * Return Value: E_NO_ERROR/others...
 *********************************************************************/
 void	CScrPadHandler::vuiDelPrePos_(VfDBHandle_t vfd, int   protocol)
{
	VfFieldSpec_t       fieldSpec[2];
	VfFieldSpec_t       fieldSpec1[2];
	int                 n , numDone;
// 	int32_t             numSection = 0;
	time_t              ts; 
	VfPathElem_t        path[1];

	path[0].subtree_name = SVN_ST_PROTOCOL;
	path[0].index = protocol;

	n = 0 ;

	VFF_SET_ARGS(fieldSpec[n],SVN_EXE_RFL_COUCH,SVT_EXE_RFL_COUCH,
			 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN); n++;
	VFF_SET_ARGS(fieldSpec[n],SVN_EXE_CFA_COUCH,SVT_EXE_CFA_COUCH,
			 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN); n++;

	VfGetFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec, n, (int*)&numDone);

	n = 0 ;
	if ( fieldSpec[0].status == E_NO_ERROR )
	{
	    VFF_SET_ARGS(fieldSpec1[n],SVN_EXE_RFL_COUCH,SVT_EXE_RFL_COUCH,
			 fieldSpec[0].size,NULL,VFO_DELETE);       n++;
	}
	if ( fieldSpec[1].status == E_NO_ERROR )
	{
		VFF_SET_ARGS(fieldSpec1[n],SVN_EXE_CFA_COUCH,SVT_EXE_CFA_COUCH,
			 fieldSpec[1].size,NULL,VFO_DELETE);       n++;
	}
	if( n )
	{
		VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec1, n, (int*)&numDone, &ts);
	}
}

/*********************************************************************
 * Description: 寝台移動モードの削除
 * Assumptions:
 * Side Effects:
 * Return Value: E_NO_ERROR/others...
 *********************************************************************/
 void	CScrPadHandler::vuiDelMoveCouchMethod_(VfDBHandle_t vfd, int   protocol)
{

    VfFieldSpec_t       fieldSpec[1];
    int                 n , numDone;
    int32_t		mode , tm ;
    time_t              ts; 
    VfPathElem_t        path[5];

    ZeroMemory(path,     sizeof(path));
    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = protocol;

    n = 0 ;
    VFF_SET_ARGS(fieldSpec[n],SVN_EXE_MOVE_COUCH_METHOD,VFT_INT32,
		 sizeof(mode),&mode,0L);       n++;
    VfGetFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec, n, (int*)&numDone);

    if ( n == numDone && fieldSpec[0].status == E_NO_ERROR )
    {
	tm = 0 ;
	n = 0 ;
	numDone = 0 ;
	VFF_SET_ARGS(fieldSpec[n],SVN_EXE_MOVE_COUCH_METHOD,VFT_INT32,
			 sizeof(mode),&mode,VFO_DELETE);       n++;
	VfPutFields(vfd, path, SVD_ST_PROTOCOL, fieldSpec, n, (int*)&numDone, &ts);

    }
}
/*********************************************************************
 * Description:
 * Assumptions:
 * Side Effects:
 * Return Value:
 *********************************************************************/
status_t CScrPadHandler::VuiCopyProtocol_(VfDBHandle_t	vfd,
			 int		vf_src_index,
			 int		vf_dst_index,
			 int		image_group,
			 int		*protocol)
{

    if_error(vuiCopyProtocol_(vfd,vf_dst_index,vfd,vf_src_index,
			     image_group,protocol))
	return E_ERROR;

    return E_NO_ERROR;
}

/*********************************************************************
 * Description: ?????????????????????
 * Assumptions:
 * Side Effects:
 * Return Value: E_NO_ERROR/others...
 *********************************************************************/
status_t CScrPadHandler::VuiCopyProtocolFromFile_(VfDBHandle_t	vfd_dst,
				 int		vf_dst_index,
				 char		*src_file,
				 int		vf_src_index,
				 int		image_group,
				 int		*protocol)
{
    VfDBHandle_t	vfd_src;

    if((vfd_src = VuiOpen(src_file,O_RDWR)) == NULL)
	return E_ERROR;

    if_error(vuiCopyProtocol_(vfd_dst,vf_dst_index,vfd_src,vf_src_index,
			     image_group,protocol))
	return E_ERROR;

    VuiClose(vfd_src);

    return E_NO_ERROR;
}

int CScrPadHandler::createNewProtocol( CString f_csStudyFileName,
                                      int f_nSrcProt,
                                      int &f_nDstProt )
{
    VfDBHandle_t vfd;
    char l_cpStudyfilename[128];
    sprintf(l_cpStudyfilename, "%s", f_csStudyFileName);
    vfd = VuiOpen(l_cpStudyfilename,O_RDWR);
    if (vfd == NULL)
    {
        return E_ERROR;
    }
    int l_nStatus = VuiCopyProtocol_(vfd,f_nSrcProt,-1,0,&f_nDstProt);

    if (l_nStatus != E_NO_ERROR)
    {
        return E_ERROR;
    }
    if(f_nDstProt != -1) {
        VuiAddToAcqOrder_(vfd, f_nDstProt);
    } else {
        return -1;
    }    
    return E_NO_ERROR;

}
/*********************************************************************
 * Description: Append one more protocol branch to our current list.
 * Assumptions:
 * Side Effects:
 * Return Value:
 *		E_NO_ERROR = successfully done.
 *		E_ERROR    = memory allocation error or unable to
 *			     write to the study.
 *********************************************************************/
status_t CScrPadHandler::VuiAddToAcqOrder_(VfDBHandle_t	studyHandle,
			  int		branch)
{
    void		*tmp;
    status_t		status;
    VuiAcqOrder_t 	AcqOrder;
    
    AcqOrder.VuiAcqOrder = NULL;
    if(VuiGetVfAcqOrder_(studyHandle,&AcqOrder) != E_NO_ERROR)
    {
	AcqOrder.SzAcqOrder = 0;
	AcqOrder.NumAcqOrder = 0;
	if(AcqOrder.VuiAcqOrder)
	{
	    free(AcqOrder.VuiAcqOrder);
	    AcqOrder.VuiAcqOrder = NULL;
	}
	return E_ERROR;
    }
    
    /* allocate memory for new branch */
    if(AcqOrder.SzAcqOrder == 0)
    {
	if((AcqOrder.VuiAcqOrder = (int *)malloc(sizeof(int32_t))) == NULL)
	{
	    return(E_ERROR);
	}
    }
    else
    {
	if((tmp =(int *)malloc ((AcqOrder.NumAcqOrder + 1)
				 * sizeof(int32_t))) == NULL)
	{
	    return(E_ERROR);
	}
	memcpy(tmp,AcqOrder.VuiAcqOrder,
	       AcqOrder.NumAcqOrder * sizeof(int32_t));
	
	free(AcqOrder.VuiAcqOrder);
	AcqOrder.VuiAcqOrder = (int *)tmp;
    }
    
    /* add it to list */
    AcqOrder.VuiAcqOrder[AcqOrder.NumAcqOrder] = branch;
    AcqOrder.NumAcqOrder += 1;
    AcqOrder.SzAcqOrder	= AcqOrder.NumAcqOrder;
    
    //DB_DO("+V1",vuiPrintAcqOrder(&AcqOrder));
    
    /* udpate the study file */
    status = VuiUpdateAcqOrder_(studyHandle,AcqOrder);
    
    AcqOrder.SzAcqOrder	= 0;
    AcqOrder.NumAcqOrder = 0;
    if(AcqOrder.VuiAcqOrder) free(AcqOrder.VuiAcqOrder);

    AcqOrder.VuiAcqOrder = NULL;
    
    return(status);
}

/**********************************************************************
 * Description: ???????????????????????????
 * Assumptions:
 * Side Effects:
 * Return Value:
 *		E_NO_ERROR = successfully read.
 *		E_ERROR	   = unable to allocate memory.
 **********************************************************************/
status_t CScrPadHandler::VuiGetVfAcqOrder_(VfDBHandle_t	vfHandle,
			  VuiAcqOrder_t *AcqOrder)
{
    int			n;
    VfFieldSpec_t	vf_req[2];
    int			num_found;
    int 		*tmpP;
    
    AcqOrder->SzAcqOrder  = 0;
    AcqOrder->NumAcqOrder = 0;
    AcqOrder->VuiAcqOrder = NULL;
    
    /* read from the study file */
    n = 0;
    VFF_SET_ARGS(vf_req[n],SVN_PLN_PROT_ACQ_ORDER,SVT_PLN_PROT_ACQ_ORDER,
		 0,0,VFO_VARIABLE_LEN | VFO_ALLOCATE);
    n++;
    
    if(VfGetFields(vfHandle,NULL,SVD_ST_STUDY,
		   vf_req,n,&num_found) != E_NO_ERROR)
    {
	return E_ERROR;
    }
    
    if(num_found)
    {
	AcqOrder->SzAcqOrder  = vf_req[0].size / sizeof(int32_t);
	AcqOrder->NumAcqOrder = AcqOrder->SzAcqOrder;
	tmpP = (int *)malloc(sizeof(int32_t)*(AcqOrder->NumAcqOrder));
	memcpy(tmpP,vf_req[0].buffer,sizeof(int32_t)*(AcqOrder->NumAcqOrder));
	AcqOrder->VuiAcqOrder = tmpP;
    }
    else
    {
	AcqOrder->SzAcqOrder  = 0;
	AcqOrder->NumAcqOrder = 0;
	AcqOrder->VuiAcqOrder = NULL;
    }
    
//     if(vf_req[0].buffer) //free(vf_req[0].buffer);
    
    return E_NO_ERROR;
}

/**********************************************************************
 * Description: Writes the Acquisition Order Array onto the study file.
 * Assumptions:
 * Side Effects:
 * Return Value:
 *		E_NO_ERROR = successfully done.
 *		E_ERROR    = unable to update field.
 **********************************************************************/
status_t CScrPadHandler::VuiUpdateAcqOrder_(VfDBHandle_t		studyHandle,
			   VuiAcqOrder_t	AcqOrder)
{
    int			n,num_put;
    time_t		time_stamp;
    status_t		status;
    VfDBHandle_t	vf_handle;
    VfFieldSpec_t	vf_req[2];
    
    n = 0;
    VFF_SET_ARGS(vf_req[n],
		 SVN_PLN_PROT_ACQ_ORDER,SVT_PLN_PROT_ACQ_ORDER,
		 sizeof(int32_t) * AcqOrder.NumAcqOrder,
		 AcqOrder.VuiAcqOrder,
		 (AcqOrder.NumAcqOrder == 0 ? VFO_DELETE : VFO_REPLACE));
    n++;
    
    vf_handle = studyHandle;
    
    //DB_DO("+V1",vuiPrintAcqOrder(&AcqOrder));
    
    /* update the study */
    status = VfPutFields(vf_handle,NULL,SVD_ST_STUDY,
			 vf_req,n,&num_put,&time_stamp);
    
    if(num_put != n || status != E_NO_ERROR) return E_ERROR;
    return E_NO_ERROR;
}
/////////////////////////         End of File      /////////////////////////////