#!/bin/sh
set -e
LC_ALL=C
export LC_ALL

die() { echo "gen_assets: $*" >&2; exit 1; }

[ -f install/index.html ]     || die "install/index.html not found"
[ -f app/cache.appcache ] || die "app/cache.appcache not found"
[ -f app/index.html ]         || die "app/index.html not found"

files=$(find install app -type f ! -path '*/.*' | sort)

bad=$(printf '%s\n' "$files" | grep -v '^[A-Za-z0-9._/-]*$' || true)
[ -z "$bad" ] || die "invalid filename (allowed characters: A-Z a-z 0-9 . _ - /): $bad"

build_id=$( { printf '%s\n' "$files"; cat $files; } | cksum | cut -d' ' -f1 )

entries=$(printf '%s\n' "$files" | while read -r f; do
  case "$f" in
    install/index.html) printf '/index.html\t%s\n' "$f" ;; 
    *)                  printf '/%s\t%s\n' "$f" "$f" ;;    
  esac
done | sort)

echo "/* Auto-generated (tools/gen_assets.sh) - Don't edit */"
echo "#define BUILD_ID \"$build_id\""

cat <<'EOF'
typedef struct {
    const char *url;
    const unsigned char *start, *end;
} asset_t;

EOF

printf '%s\n' "$entries" | awk -F'\t' '
BEGIN { print "__asm__(\".section .rodata\\n\"" }
{ n = NR - 1
  printf "    \".balign 16\\n.global asset_%d\\n.global asset_%d_end\\nasset_%d:\\n.incbin \\\"%s\\\"\\nasset_%d_end:\\n\"\n", n, n, n, $2, n }
END { print "    \".previous\\n\");\n" }
'

printf '%s\n' "$entries" | awk -F'\t' '
{ printf "extern const unsigned char asset_%d[], asset_%d_end[];\n", NR-1, NR-1 }
'

echo
echo "static const asset_t assets[] = {"
printf '%s\n' "$entries" | awk -F'\t' '
{ printf "    {\"%s\", asset_%d, asset_%d_end},\n", $1, NR-1, NR-1 }
'
echo "    {0, 0, 0}"
echo "};"