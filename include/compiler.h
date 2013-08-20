#ifndef SLFREE_COMPILER_H
#define SLFREE_COMPILER_H

#ifdef __GNUG__

  #define CC_LIKELY(x) __builtin_expect(!!(x), 1)
  #define CC_UNLIKELY(x) __builtin_expect(!!(x), 0)
  
  #define CC_ALIGNED(x) __attribute__((aligned(x)))
  #define CC_PACKED __attribute__((packed))
  #define CC_ALWAYS_INLINE __attribute__((always_inline))
  #define CC_PURE_FUNCTION __attribute__((pure))
#endif

#endif
