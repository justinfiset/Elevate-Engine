#if defined(__clang__)
    #define EE_CLANG_DIAG_PUSH          _Pragma("clang diagnostic push")
    #define EE_CLANG_DIAG_POP           _Pragma("clang diagnostic pop")
    #define EE_CLANG_DIAG_IGNORE_OVERRIDE _Pragma("clang diagnostic ignored \"-Winconsistent-missing-override\"")
#else
    #define EE_CLANG_DIAG_PUSH
    #define EE_CLANG_DIAG_POP
    #define EE_CLANG_DIAG_IGNORE_OVERRIDE
#endif