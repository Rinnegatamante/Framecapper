#include <vitasdk.h>
#include <taihen.h>

//#define TARGET_20FPS // Uncomment this to target 20 FPS instead
//#define INJECT_VSYNC_WAIT // Uncomment this to inject a VSync wait

#ifdef TARGET_20FPS
#define VSYNC_INTERVAL (3)
#else
#define VSYNC_INTERVAL (2)
#endif	

static SceUID sceDisplayWaitVblankStartMulti_hook, sceDisplayWaitVblankStart_hook;
static SceUID sceDisplayWaitVblankStartMultiCB_hook, sceDisplayWaitVblankStartCB_hook;
static tai_hook_ref_t sceDisplayWaitVblankStartMulti_ref, sceDisplayWaitVblankStart_ref;
static tai_hook_ref_t sceDisplayWaitVblankStartMultiCB_ref, sceDisplayWaitVblankStartCB_ref;
static SceUID sceDisplaySetFrameBuf_hook;
static tai_hook_ref_t sceDisplaySetFrameBuf_ref;

int sceDisplayWaitVblankStartMulti_patched(unsigned int interval) {
	return TAI_CONTINUE(int, sceDisplayWaitVblankStartMulti_ref, VSYNC_INTERVAL);
}

int sceDisplayWaitVblankStart_patched() {
	return sceDisplayWaitVblankStartMulti(VSYNC_INTERVAL);
}

int sceDisplayWaitVblankStartMultiCB_patched(unsigned int interval) {
	return TAI_CONTINUE(int, sceDisplayWaitVblankStartMultiCB_ref, VSYNC_INTERVAL);
}

int sceDisplayWaitVblankStartCB_patched() {
	return sceDisplayWaitVblankStartMultiCB(VSYNC_INTERVAL);
}

int sceDisplaySetFrameBuf_patched(const SceDisplayFrameBuf *pParam, SceDisplaySetBufSync sync) {
	int ret = TAI_CONTINUE(int, sceDisplaySetFrameBuf_ref, pParam, sync);
	sceDisplayWaitVblankStartMulti(VSYNC_INTERVAL);
	return ret;
}

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args) {
	sceDisplayWaitVblankStartMulti_hook = taiHookFunctionImport(&sceDisplayWaitVblankStartMulti_ref,
						TAI_MAIN_MODULE,
						TAI_ANY_LIBRARY,
						0xDD0A13B8,
						sceDisplayWaitVblankStartMulti_patched);
						
	sceDisplayWaitVblankStart_hook = taiHookFunctionImport(&sceDisplayWaitVblankStart_ref,
						TAI_MAIN_MODULE,
						TAI_ANY_LIBRARY,
						0x5795E898,
						sceDisplayWaitVblankStart_patched);
						
	sceDisplayWaitVblankStartMultiCB_hook = taiHookFunctionImport(&sceDisplayWaitVblankStartMultiCB_ref,
						TAI_MAIN_MODULE,
						TAI_ANY_LIBRARY,
						0x05F27764,
						sceDisplayWaitVblankStartMultiCB_patched);
						
	sceDisplayWaitVblankStartCB_hook = taiHookFunctionImport(&sceDisplayWaitVblankStartCB_ref,
						TAI_MAIN_MODULE,
						TAI_ANY_LIBRARY,
						0x78B41B92,
						sceDisplayWaitVblankStartCB_patched);
#ifdef INJECT_VSYNC_WAIT						
	sceDisplaySetFrameBuf_hook = taiHookFunctionImport(&sceDisplaySetFrameBuf_ref,
						TAI_MAIN_MODULE,
						TAI_ANY_LIBRARY,
						0x7A410B64,
						sceDisplaySetFrameBuf_patched);
#endif				
	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) {
	taiHookRelease(sceDisplayWaitVblankStartMulti_hook, sceDisplayWaitVblankStartMulti_ref);
	taiHookRelease(sceDisplayWaitVblankStart_hook, sceDisplayWaitVblankStart_ref);
	taiHookRelease(sceDisplayWaitVblankStartMultiCB_hook, sceDisplayWaitVblankStartMultiCB_ref);
	taiHookRelease(sceDisplayWaitVblankStartCB_hook, sceDisplayWaitVblankStartCB_ref);
#ifdef INJECT_VSYNC_WAIT
	taiHookRelease(sceDisplaySetFrameBuf_hook, sceDisplaySetFrameBuf_ref);
#endif

	return SCE_KERNEL_STOP_SUCCESS;
	
}