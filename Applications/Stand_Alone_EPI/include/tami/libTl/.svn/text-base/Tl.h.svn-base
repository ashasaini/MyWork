/******************************************************************************
 *  File Name:		$Source: /mplus/cvsroot/Theta/develop/src.include/libTl/Tl.h,v $  ($Revision: 1.2 $)
 *  Last Modified:	$Date: 2010/02/17 02:32:30 $
 *  Revision Log:	$Log: Tl.h,v $
 *  Revision Log:	Revision 1.2  2010/02/17 02:32:30  karube
 *  Revision Log:	MCM0252-00020 Phase3 libWNP,Native
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.1.1  2010/01/06 06:45:42  ohba
 *  Revision Log:	no message
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.1.1.28.1.2.5  2003/04/24 02:59:15  nkatsu
 *  Revision Log:	*** empty log message ***
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.1.1.28.1.2.4  2003/04/22 08:54:37  nkatsu
 *  Revision Log:	*** empty log message ***
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.1.1.28.1.2.3  2003/03/31 07:25:19  ohba
 *  Revision Log:	from WIPRO MAR 31
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.1.1  2002/04/16 07:15:00  cvsadmin
 *  Revision Log:	Imported
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.1.1  2000/06/08 12:36:49  full
 *  Revision Log:	Change Include Path
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.1.1  2000/04/21 08:59:22  ohba
 *  Revision Log:	From Persia
 *  Revision Log:	
 * Revision 1.5  93/04/01  17:24:18  arkady
 * Added GetNames functions
 * 
 * Revision 1.4  92/09/29  22:23:46  lib
 * Converted to the new directory structure
 * 
 * Revision 1.3  92/09/11  18:34:22  hinkmond
 * Moved TlTitle function to libTm.
 * 
 * Revision 1.2  92/08/25  17:13:52  alex
 * Add TlTitle function to prototypes.
 * 
 * Revision 1.1  92/04/13  10:24:12  rickm
 * Initial revision
 * 
 *  
 *    File Overview:
 *	This is the include file for Toshiba library (Tl)
 *
 *****************************************************************************/
/* Revision History:
 * yy-mm-dd	modified-by	modifications-done
 * 92-11-09	hbw		Moved TlTitle function to libTm.
 * 91-10-24	arkady		Created
 * Sep.19.94	Naoyuki Furudate	Modified
 * End of History */


#ifndef tami_libTl_Tl_H__
#define tami_libTl_Tl_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>		/* for FILE in prototypes */

/******************************************************************************
 *
 * Function prototypes
 *
 *****************************************************************************/

/******************************************************************************
 * Convert a character string into NULL-terminated argv array of pointers
 * suitable for execv[p]() function calls.
 * Side Effect:	The string passed to this function is processed
 *		with strtok() and not usable after return from
 *		this function. Normaly this should not cause
 *		problems since one of the exec..() will be
 *		called right away.
 */

char **TlMakeArgv(char *);

/******************************************************************************
 * All of the above functions are similar to popen(command, "r").
 *
 * TlPopenBoth()	redirects both stdout and stderr to the pipe.
 * 			Function returns (FILE *) on success, NULL on failure
 * TlPopen2()		opens two pipes, one to stdout and one to stderr.
 *			Function returns 0 on success, -1 on failure.
 * TlPopenStdErr()	redirects stderr to the pipe.
 *			Function returns (FILE *) on success, NULL on failure
 * TlPclose()		MUST be used to close the pipe.
 *			Function returns exit status of the child process or
 *			-1 if stream is not associated with one of TlPopen...()
 *			functions.
 *			Use pstdout argument for TlPopen2()
 *
 * If whichExec=='p' execvp() is used (PATH is searched), else execv() is used.
 *
 * If startupDir != NULL a chdir(startupDir) is done after fork().
 *
 * TlPclose() MUST be used to close the pipe.
 */

FILE *TlPopenBoth(char *command, char whichExec, char *startupDir);
int TlPopen2(FILE **pstdout, FILE **pstderr, char *command, char whichExec,
	     char *startupDir);
FILE *TlPopenStdErr(char *command, char  whichExec, char *startupDir);
int TlPclose(FILE *);

/******************************************************************************
 * Based on exit status of a process (eg.: returned by wait()) determine if
 * status indicates coredump.
 * Function returnes T/F
 */

int TlIsCoredump(int);

/*
 * TlStrCpyCat()	concatinates an arbitrary number of strings.
 *			The first argument is a buffer to concatinate
 *			the strings into. If first argument is NULL -
 *			malloc buffer of sufficient size for result.
 *			Identical to strcat(strcat(strcpy(buf,s1),s2),s3)...
 * TlStrLenN()		Compute cumulative length of an arbitrary number of
 *			strings (null terminators not included).
 *			Identical to the sum of strlen()'s.
 * The argument lists to both functions must be terminated with a NULL argument.
 * Eg: TlStrCpyCat(buf, s1, s2, s3, s4, NULL)
 */

char *TlStrCpyCat(char *, char *, ...);
int TlStrLenN(char *, ...);

/******************************************************************************
 * TlBasename()		Extract basename from file name.
 *			Basename is part of the string following last '/'
 *			(or the beginning of the string).
 * 			If second argument (suffix) is not NULL and the
 *			filename ends with the suffix, the suffix is removed.
 * TlDirname()		Extract directory name from file name.
 *			Directory name is part of the string preceeding
 *			last '/'.
 *			If there is no '/',  "." is returned.
 * Both functions will ignore trailing slashes (except for "/" case).
 * Both functions return malloc()'ed strings. They will return NULL
 * if malloc fails. You are responsible for free()'ing them.
 * The behavior of these functions should be identical to
 * basename and dirname described in "man 1 dirname"
 */

char *TlBasename(char *, char *);
char *TlDirname(char *);

/******************************************************************************
 * 
 * Functions for getting names and associated data from a file.
 * The file format of the file is:
 *
 *	name ( access_list ) data
 *
 *	where
 *		name - 	the key field. It should not contain white
 *			spaces. A dictionary of names will be (I hope)
 *			maintained.
 *
 *		access_list  - a comma-separated list of names/tags
 *			interested in the name.
 *
 *		data - data assiciated with this name.
 *
 *	a line can be continued by ending it with a backslash.
 *	a line is a comment if it begins with '!' (except continuation lines)
 *
 *	Note that there are functions in libTm to retrieve labels (XmStrings)
 *	based on these names.
 *
 * The main function to access this is;
 */

int TlGetNamesRead(FILE *inputFile,			/* I: opened file */
		   const char *whoFor,		/* I: optional */
		   char ***retNamePtrArray,		/* O: required */
		   char ***retDataPtrArray,		/* O: optional */
		   int  *numFound			/* O: optinal */
		   );
/*
 * Where:
 *	inputFile 		is an opened input stream
 *	whoFor 			is an optional key to match an item in
 *				access list.
 *	retNamePtrArray		is returned and is a "pointer to an array of
 *				pointers to char" containing name(s).
 *				The array and the strings are malloc()d.
 *				The array is terminated with a NULL pointer.
 *	retDataPtrArray		similar to retNamePtrArray, but containes
 *				data. Optinal.
 *	numFound		number of elements allocated to the above two
 *				arrays. Optinal.
 *  return value:
 *	-1 malloc error
 *	 n number of names found (0 is a valid count).
 *
 * There is a number of wrappers for this function. Their return values are
 * the same except that -1 also returned if file could not be opened.
 * They are:
 */

/* open file, get names, close file */
int TlGetNames(const char *inputFileName,		/* I: file name */
	       const char *whoFor,			/* I: optional */
	       char ***retNamePtrArray,			/* O: required */
	       char ***retDataPtrArray,			/* O: optional */
	       int  *numFound				/* O: optinal */
	       );

/* use file name from environment, fallback to default.
 * for use by proCon, pqm, proEdit, graphicLocator
 */
int TlGetProEditorNames(const char *whoFor,
			char ***retPtrToName,
			char ***retPtrToData,
			int *numFound);
/*
 * Example:
 *	file "testFile":
 *		proedit (pqm,gl) proEdit -speed fast
 *		gl (pqm,proedit) display -mode gLocator
 *		prosee (pqm, gl) proSee -a -g
 *
 *	char **names;
 *	char **invocations;
 *	char **count;
 *
 * 1:
 *	TlGetNames( "testFile", "pqm", &names, &invocations, &count);
 * 
 *  names now points to an array of pointers:
 *	{ "proedit", "gl", "prosee" }
 *  invocations now points to an array of pointers:
 *	{ "proEdit -speed fast", "display -mode gLocator", "proSee -a -g" }
 *
 * 2:
 *	TlGetNames( "testFile", "gl", &names, &invocations, &count);
 *
 *  names now points to an array of pointers:
 *	{ "proedit", "prosee" }
 *  invocations now points to an array of pointers:
 *	{ "proEdit -speed fast", "proSee -a -g" }
 *	
 *
 * PS. There is a sample main() located in the source file TlGetNames.c
 */

#include <time.h>
struct tm *TlCalcAge(struct tm *,struct tm *);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* tami_libTl_Tl_H__ */
