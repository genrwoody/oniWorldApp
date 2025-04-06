function ready() {
    Module.app_init = _app_init;
    Module.app_generate = _app_generate;
    Module.HEAP32 = HEAP32;
    Module.HEAPU8 = HEAPU8;
    Module.onRuntimeInitialized();
}
