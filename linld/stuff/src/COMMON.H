// This file is distributed under GPL
//
// Common defs not belonging to CRTL

#define NL "\n"
#define VERSION_STR "0.97"

#if 0       // DEBUG
 #undef  NDEBUG
 #define DEBUG
 #define static
#else       // not DEBUG
 #define NDEBUG
 #undef  DEBUG
#endif

typedef unsigned long  u32; typedef signed long  s32;
typedef unsigned short u16; typedef signed short s16;
typedef unsigned char  u8;  typedef signed char  s8;

const u16 _16k = 16*1024u;
const u16 _32k = 32*1024u;
const u32 _64k = 64*1024ul;
const u32 _1m = 1024ul*1024ul;
const u32 _32m = 32*1024ul*1024ul;
const u32 _64m = 64*1024ul*1024ul;

const PAGE_BITS = 12;
const PAGE_SIZE = 1<<PAGE_BITS;
const PAGE_MASK = PAGE_SIZE-1;

// WARNING!
// BC 3.1 bug: seg() will generate incorrect code if inlined!
// replaced by _dirty_ hack for now
// u16 seg(const void far* p) { return FP_SEG(p); }
inline u16 seg(const void far*) { return _DS; }

inline u16 ofs(const void* p) { return FP_OFF(p); }

extern struct image_himem {
    int fd;
    u32 fallback;
    u32 size;
    u32 remaining;
    u32 buf;
    u32 *bufv;
    u32 chunk_size;
    u16 state;
    u16 fd2close;
} imgs[2];
#define pm (imgs[0])
#define initrd (imgs[1])
#define pm2initrd(p) (p+1)

extern u8 pm_low;

extern u32 far *initrd_desc;
#define cmdline		(cmdstr[0])
#define kernel_name	(cmdstr[1])
#define initrd_name	(cmdstr[2])
#define isofile		(cmdstr[3])
extern const char* cmdstr[4];

#define root_dev	(cmdnum[0])
#define vid_mode	(cmdnum[1])	// -3 = ask, -2 = Extended VGA, -1 = Normal VGA,  n = as "n" was pressed
#define topmem		(cmdnum[2])
#define base_himem	(cmdnum[3])
#define	skip_alloc	(* (char *) &cmdnum[4])
extern u32 cmdnum[5];

extern _fastcall void memcpy_image(struct image_himem *m);
extern _fastcall void storepage(u32 *dst);
#ifdef LARGE_IMAGES
extern _fastcall void reset_bufv(unsigned long *p);
extern "C" unsigned long* prev_bufv();
extern "C" unsigned long* next_bufv();
#define next(p)	 p = next_bufv()
#define prev(p)	 p = prev_bufv()
#else
#define reset_bufv(p)
#define next(p)	 ++p
#define prev(p)	 --p
#endif
extern char vcpi;
extern char version_string[];
// External asm helpers
#ifdef	WITH_XMM_ALLOC
extern _fastcall void xmm_alloc(struct image_himem *m);
#endif
extern "C" u32 memtop();
extern "C" void enable_a20_or_die();
extern "C" void vm2rm();
extern "C" void hook_int15_88();

// C++ helpers
#ifdef DEBUG
extern "C" void hang();
#endif

void load_image(struct image_himem *m);
extern _fastcall void open_image(struct image_himem *m, const char *name);
extern _fastcall int read_image(struct image_himem *m);
void load_kernel();
void load_initrd();
extern "C" void boot_kernel();
