/* Auto-generated (tools/gen_assets.sh) - Don't edit */
#define BUILD_ID "3851323957"
typedef struct {
    const char *url;
    const unsigned char *start, *end;
} asset_t;

__asm__(".section .rodata\n"
    ".balign 16\n.global asset_0\n.global asset_0_end\nasset_0:\n.incbin \"app/cache.appcache\"\nasset_0_end:\n"
    ".balign 16\n.global asset_1\n.global asset_1_end\nasset_1:\n.incbin \"app/css/index.css\"\nasset_1_end:\n"
    ".balign 16\n.global asset_2\n.global asset_2_end\nasset_2:\n.incbin \"app/index.html\"\nasset_2_end:\n"
    ".balign 16\n.global asset_3\n.global asset_3_end\nasset_3:\n.incbin \"app/js/helper.js\"\nasset_3_end:\n"
    ".balign 16\n.global asset_4\n.global asset_4_end\nasset_4:\n.incbin \"app/payloads/sce-hell0-payload-1.elf\"\nasset_4_end:\n"
    ".balign 16\n.global asset_5\n.global asset_5_end\nasset_5:\n.incbin \"app/payloads/sce-hell0-payload-2.elf\"\nasset_5_end:\n"
    ".balign 16\n.global asset_6\n.global asset_6_end\nasset_6:\n.incbin \"app/payloads/sce-hell0-payload-3.elf\"\nasset_6_end:\n"
    ".balign 16\n.global asset_7\n.global asset_7_end\nasset_7:\n.incbin \"app/payloads/sce-hell0-payload-4.elf\"\nasset_7_end:\n"
    ".balign 16\n.global asset_8\n.global asset_8_end\nasset_8:\n.incbin \"app/payloads/sce-hell0-payload-5.elf\"\nasset_8_end:\n"
    ".balign 16\n.global asset_9\n.global asset_9_end\nasset_9:\n.incbin \"install/index.html\"\nasset_9_end:\n"
    ".balign 16\n.global asset_10\n.global asset_10_end\nasset_10:\n.incbin \"install/payloads/orchestrator.elf\"\nasset_10_end:\n"
    ".previous\n");

extern const unsigned char asset_0[], asset_0_end[];
extern const unsigned char asset_1[], asset_1_end[];
extern const unsigned char asset_2[], asset_2_end[];
extern const unsigned char asset_3[], asset_3_end[];
extern const unsigned char asset_4[], asset_4_end[];
extern const unsigned char asset_5[], asset_5_end[];
extern const unsigned char asset_6[], asset_6_end[];
extern const unsigned char asset_7[], asset_7_end[];
extern const unsigned char asset_8[], asset_8_end[];
extern const unsigned char asset_9[], asset_9_end[];
extern const unsigned char asset_10[], asset_10_end[];

static const asset_t assets[] = {
    {"/app/cache.appcache", asset_0, asset_0_end},
    {"/app/css/index.css", asset_1, asset_1_end},
    {"/app/index.html", asset_2, asset_2_end},
    {"/app/js/helper.js", asset_3, asset_3_end},
    {"/app/payloads/sce-hell0-payload-1.elf", asset_4, asset_4_end},
    {"/app/payloads/sce-hell0-payload-2.elf", asset_5, asset_5_end},
    {"/app/payloads/sce-hell0-payload-3.elf", asset_6, asset_6_end},
    {"/app/payloads/sce-hell0-payload-4.elf", asset_7, asset_7_end},
    {"/app/payloads/sce-hell0-payload-5.elf", asset_8, asset_8_end},
    {"/index.html", asset_9, asset_9_end},
    {"/install/payloads/orchestrator.elf", asset_10, asset_10_end},
    {0, 0, 0}
};
