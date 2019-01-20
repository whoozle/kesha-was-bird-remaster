int main() {
	unsigned char * dst = (unsigned char *)0x4000;
	unsigned n = 0x800;
	while(n--)
		*dst+= 0xaa;
	return 0;
}
