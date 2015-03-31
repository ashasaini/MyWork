/*****************************************************************************
 *	コマンドラインテスト用ライブラリ
 *	command_line_test.h
 *	2001/12/20 y.morita	V0.01 P1.08 Initial Version.
 *****************************************************************************/
#ifndef __command_line_test_H__
#define __command_line_test_H__

#ifdef __cplusplus
extern "C" {
#endif

void	cmd_setup_handler(char *cmd, void (*handler)(), void *arg1, void *arg2);
char *	cmd_input(char *prompt);
void	cmd_wait();
void	cmd_quit();
void	cmd_help();

#ifdef __cplusplus
} // extern "C"
#endif

#endif	/* #ifndef __command_line_test_H__ */
