#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>

/* Not technically required, but needed on some UNIX distributions */
#include <sys/types.h>
#include <sys/stat.h>

char * read_section64(int32_t fd, Elf64_Shdr sh)
{
	char* buff = malloc(sh.sh_size);
	if(!buff) {
		printf("%s:Failed to allocate %ldbytes\n",
				__func__, sh.sh_size);
	}

	assert(buff != NULL);
	assert(lseek(fd, (off_t)sh.sh_offset, SEEK_SET) == (off_t)sh.sh_offset);
	assert(read(fd, (void *)buff, sh.sh_size) == sh.sh_size);

	return buff;
}


void print_section_headers64(int32_t fd, Elf64_Ehdr eh, Elf64_Shdr sh_table[])
{
	uint32_t i;
	char* sh_str;	/* section-header string-table is also a section. */

	/* Read section-header string-table */
	printf("eh.e_shstrndx = 0x%x\n", eh.e_shstrndx);
	sh_str = read_section64(fd, sh_table[eh.e_shstrndx]);

	printf("========================================");
	printf("========================================\n");
	printf(" idx offset     load-addr  size       algn"
			" flags      type       section\n");
	printf("========================================");
	printf("========================================\n");

	for(i=0; i<eh.e_shnum; i++) {
		printf(" %03d ", i);
		printf("0x%08lx ", sh_table[i].sh_offset);
		printf("0x%08lx ", sh_table[i].sh_addr);
		printf("0x%08lx ", sh_table[i].sh_size);
		printf("%4ld ", sh_table[i].sh_addralign);
		printf("0x%08lx ", sh_table[i].sh_flags);
		printf("0x%08x ", sh_table[i].sh_type);
		printf("%s\t", (sh_str + sh_table[i].sh_name));
		printf("\n");
	}
	printf("========================================");
	printf("========================================\n");
	printf("\n");	/* end of section header table */
}

void read_elf_header64(int32_t fd, Elf64_Ehdr *elf_header)
{
	assert(elf_header != NULL);
	assert(lseek(fd, (off_t)0, SEEK_SET) == (off_t)0);
	assert(read(fd, (void *)elf_header, sizeof(Elf64_Ehdr)) == sizeof(Elf64_Ehdr));
}

void read_section_header_table64(int32_t fd, Elf64_Ehdr eh, Elf64_Shdr sh_table[])
{
	uint32_t i;

	assert(lseek(fd, (off_t)eh.e_shoff, SEEK_SET) == (off_t)eh.e_shoff);

	for(i=0; i<eh.e_shnum; i++) {
		assert(read(fd, (void *)&sh_table[i], eh.e_shentsize)
				== eh.e_shentsize);
	}

}


void print_symbol_table64(int32_t fd,
		Elf64_Ehdr eh,
		Elf64_Shdr sh_table[],
		uint32_t symbol_table)
{

	char *str_tbl;
	Elf64_Sym* sym_tbl;
	uint32_t i, symbol_count;

	sym_tbl = (Elf64_Sym*)read_section64(fd, sh_table[symbol_table]);

	/* Read linked string-table
	 * Section containing the string table having names of
	 * symbols of this section
	 */
	uint32_t str_tbl_ndx = sh_table[symbol_table].sh_link;
	printf("str_table_ndx = 0x%x\n", str_tbl_ndx);
	str_tbl = read_section64(fd, sh_table[str_tbl_ndx]);

	symbol_count = (sh_table[symbol_table].sh_size/sizeof(Elf64_Sym));
	printf("%d symbols\n", symbol_count);

	for(i=0; i< symbol_count; i++) {
		printf("0x%08lx ", sym_tbl[i].st_value);
		printf("0x%02x ", ELF32_ST_BIND(sym_tbl[i].st_info));
		printf("0x%02x ", ELF32_ST_TYPE(sym_tbl[i].st_info));
		printf("%s\n", (str_tbl + sym_tbl[i].st_name));
	}
}

void print_symbols64(int32_t fd, Elf64_Ehdr eh, Elf64_Shdr sh_table[])
{
	uint32_t i;

	for(i=0; i<eh.e_shnum; i++) {
		if ((sh_table[i].sh_type==SHT_SYMTAB)
				|| (sh_table[i].sh_type==SHT_DYNSYM)) {
			printf("\n[Section %03d]", i);
			print_symbol_table64(fd, eh, sh_table, i);
		}
	}
}

int main(int argc,char**argv)
{


    int32_t fd;
    Elf64_Ehdr eh64;	/* elf-header is fixed size */
    Elf64_Shdr* sh_tbl; /* section-header table is variable size */
    fd = open(argv[1], O_RDONLY|O_SYNC);
	if(fd<0) {
		printf("Error %d Unable to open %s\n", fd, argv[1]);
		return 0;
    }
    read_elf_header64(fd, &eh64);
    sh_tbl = malloc(eh64.e_shentsize * eh64.e_shnum);
		if(!sh_tbl) {
			printf("Failed to allocate %d bytes\n",
					(eh64.e_shentsize * eh64.e_shnum));
    }
    read_section_header_table64(fd, eh64, sh_tbl);
    print_section_headers64(fd, eh64, sh_tbl);
    return 0;
}