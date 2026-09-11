#ifdef EE_PLATFORM_WEB

#include <emscripten/atomic.h>
void ForceEmscriptenAtomics() {
    uint32_t val = 0;
    emscripten_atomic_add_u32(&val, 1);
    emscripten_atomic_load_u32(&val);
    emscripten_atomic_cas_u32(&val, 0, 1);
    emscripten_atomic_exchange_u32(&val, 1);
    emscripten_atomic_fence();
 }

#endif