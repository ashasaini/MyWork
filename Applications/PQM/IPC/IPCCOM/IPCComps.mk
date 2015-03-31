
IPCComps.dll: dlldata.obj IPCCom_p.obj IPCCom_i.obj
	link /dll /out:IPCComps.dll /def:IPCComps.def /entry:DllMain dlldata.obj IPCCom_p.obj IPCCom_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del IPCComps.dll
	@del IPCComps.lib
	@del IPCComps.exp
	@del dlldata.obj
	@del IPCCom_p.obj
	@del IPCCom_i.obj
