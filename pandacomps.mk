
pandacomps.dll: dlldata.obj pandacom_p.obj pandacom_i.obj
	link /dll /out:pandacomps.dll /def:pandacomps.def /entry:DllMain dlldata.obj pandacom_p.obj pandacom_i.obj \
		mtxih.lib mtx.lib mtxguid.lib \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
		ole32.lib advapi32.lib 

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		/MD \
		$<

clean:
	@del pandacomps.dll
	@del pandacomps.lib
	@del pandacomps.exp
	@del dlldata.obj
	@del pandacom_p.obj
	@del pandacom_i.obj
