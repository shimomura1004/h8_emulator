#include "elf_loader.h"

struct elf_header {
    struct {
        unsigned char magic[4];     // elf のマジックナンバ
        unsigned char klass;        // 32/64ビットの区別
        unsigned char format;       // エンディアン
        unsigned char version;      // elf フォーマットのバージョン
        unsigned char abi;          // OS の種別
        unsigned char abi_version;  // OS のバージョン
        unsigned char reserve[7];
    } id;
    int16_t type;                     // ファイルの種別
    int16_t arch;                     // CPU の種類
    int32_t version;                   // elf 形式のバージョン
    int32_t entry_point;               // 実行開始アドレス
    int32_t program_header_offset;     // プログラムヘッダテーブルの位置
    int32_t section_header_offset;     // セクションヘッダテーブルのいち
    int32_t flags;                     // 各種フラグ
    int16_t header_size;              // ELF ヘッダのサイズ
    int16_t program_header_size;      // プログラムヘッダのサイズ
    int16_t program_header_num;       // プログラムヘッダの数
    int16_t section_header_size;      // セクションヘッダのサイズ
    int16_t section_header_num;       // セクションヘッダの数
    int16_t section_name_index;       // セクション名を格納するセクションのインデックス
};

struct elf_program_header {
    int32_t type;                      // セグメントの種別
    int32_t offset;                    // ファイル中の位置
    int32_t virtual_addr;              // 論理アドレス(VA)
    int32_t physical_addr;             // 物理アドレス(PA)
    int32_t file_size;                 // ファイル中のサイズ
    int32_t memory_size;               // メモリ上でのサイズ
    int32_t flags;                     // 各種フラグ
    int32_t align;                     // アラインメント
};

#ifdef __BYTE_ORDER__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
static void swap_elf_header_endian(struct elf_header *header)
{
    header->type = __builtin_bswap16(header->type);
    header->arch = __builtin_bswap16(header->arch);
    header->version = __builtin_bswap32(header->version);
    header->entry_point = __builtin_bswap32(header->entry_point);
    header->program_header_offset = __builtin_bswap32(header->program_header_offset);
    header->section_header_offset = __builtin_bswap32(header->section_header_offset);
    header->flags = __builtin_bswap32(header->flags);
    header->header_size = __builtin_bswap16(header->header_size);
    header->program_header_size = __builtin_bswap16(header->program_header_size);
    header->program_header_num = __builtin_bswap16(header->program_header_num);
    header->section_header_size = __builtin_bswap16(header->section_header_size);
    header->section_header_num = __builtin_bswap16(header->section_header_num);
    header->section_name_index = __builtin_bswap16(header->section_name_index);
}

static void swap_elf_program_header_endian(struct elf_program_header *header)
{
    header->type = __builtin_bswap32(header->type);
    header->offset = __builtin_bswap32(header->offset);
    header->virtual_addr = __builtin_bswap32(header->virtual_addr);
    header->physical_addr = __builtin_bswap32(header->physical_addr);
    header->file_size = __builtin_bswap32(header->file_size);
    header->memory_size = __builtin_bswap32(header->memory_size);
    header->flags = __builtin_bswap32(header->flags);
    header->align = __builtin_bswap32(header->align);
}
#endif
#endif

static int elf_check(struct elf_header *header)
{
    if (header->id.magic[0] != '\x7f' || header->id.magic[1] != 'E' ||
        header->id.magic[2] != 'L' || header->id.magic[3] != 'F') {
        fprintf(stderr, "Invalid header: magic is %02x %02x %02x %02x\n",
                header->id.magic[0], header->id.magic[1],
                header->id.magic[2], header->id.magic[3]);
        return -1;
    }
    if (header->id.klass != 1) {
        // ELF32
        fprintf(stderr, "Invalid header: class is %02x\n", header->id.klass);
        return -1;
    }
    if (header->id.format != 2) {
        // ビッグエンディアン
        fprintf(stderr, "Invalid header: format is %02x\n", header->id.format);
        return -1;
    }
    if (header->id.version != 1) {
        // version 1
        fprintf(stderr, "Invalid header: version is %02x\n", header->id.version);
        return -1;
    }
    if (header->type != 2) {
        // 実行ファイル
        fprintf(stderr, "Invalid header: type is %04x\n", header->type);
        return -1;
    }
    if (header->version != 1) {
        // version 1
        fprintf(stderr, "Invalid header: version is %08x\n", header->version);
        return -1;
    }

    // アーキが H8/300(46) か H8/300H(47) であることを確認
    if ((header->arch != 46) && (header->arch != 47)) return -1;

    return 0;
}

static int elf_load_program(struct elf_header *header, unsigned char buf[], uint8_t* memory)
{
    int i;
    struct elf_program_header *phdr;

    // プログラムヘッダの個数分(セグメントの個数分)ループする
    for (i = 0; i < header->program_header_num; i++) {
        // プログラムヘッダを取得
        phdr = (struct elf_program_header *)
            ((char *)header + header->program_header_offset +
             header->program_header_size * i);

#ifdef __BYTE_ORDER__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        // ホストがリトルエンディアン環境の場合は変換
        swap_elf_program_header_endian(phdr);
#endif
#endif

        // ロード可能なセグメントかを確認
        if (phdr->type != 1)
            continue;

        printf("Loading segment %d\n", i);

        // 物理アドレス: 変数の初期値が格納されるアドレス
        // 論理アドレス: プログラムが実行時にアクセスするアドレス
        // プログラムが ROM に書き込まれる場合、実行時には書き換えられないので　物理 != 論理 になる
        // 物理アドレスに書いておいたデータを論理アドレスにコピーする作業はブートローダの役割

        // elf ヘッダの先頭からオフセット分だけずらした位置から、
        // 物理アドレスの場所に対象のセグメントをロードする
        for (int i = 0; i < phdr->file_size; i++) {
            memory[phdr->physical_addr + i] = (unsigned char)buf[phdr->offset + i];
        }

        // RAM 領域で、セグメントをコピーしたところから後ろをゼロクリアする
        // bss　領域などは、elf ファイル上は実体は不要だが、RAM 上には領域が必要
        // よって memory_size と file_size で差がでることがある
        // ここはゼロクリアする
        // おそらく elf ファイル上に実体がない領域はセグメントの後ろ側に集められる仕様になっている
        for (int i = 0; i < phdr->memory_size - phdr->file_size; i++) {
            memory[phdr->physical_addr + phdr->file_size + i] = 0;
        }

        // セグメントの情報を出力する
        printf("Offset        : 0x%08x\n", phdr->offset);
        printf("Virtual Addr  : 0x%08x\n", phdr->virtual_addr);
        printf("Physical Addr : 0x%08x\n", phdr->physical_addr);
        printf("File Size     : 0x%08x\n", phdr->file_size);
        printf("Memory Size   : 0x%08x\n", phdr->memory_size);
        printf("Flags         : 0x%08x\n", phdr->flags);
        printf("Align         : 0x%08x\n", phdr->align);
    }

    return 0;
}

uint32_t ElfLoader::load(uint8_t* memory, std::string filepath)
{
    // ヘッダ部分は実行時には不要、一時変数 buf に展開して使う
    // セグメントは実行時に必要、memory に書き込む

    FILE* fp = fopen(filepath.c_str(), "rb");
    if (fp == nullptr) {
        fprintf(stderr, "ELF file not found: %s\n", filepath.c_str());
        return 0;
    }

    fseek(fp, 0L, SEEK_END);
    long filesize = ftell(fp);
    fseek(fp, 0L, 0);
    unsigned char buf[filesize];

    int c;
    int i = 0;
    while ((c = fgetc(fp)) != EOF) {
        buf[i++] = c;
    }
    fclose(fp);

    // 読み込む ELF バイナリは H8 で読み込むのでビッグエンディアンで格納されている
    struct elf_header *header = (struct elf_header *)buf;
#ifdef __BYTE_ORDER__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    // ホストがリトルエンディアンの場合は変換
    swap_elf_header_endian(header);
#endif
#endif

    if (elf_check(header) < 0) {
        fprintf(stderr, "Unexpected ELF header\n");
        return 0;
    }
    
    if (elf_load_program(header, buf, memory) < 0) {
        fprintf(stderr, "Unexpected ELF content\n");
        return 0;
    }

    return (uint32_t)header->entry_point;
}
