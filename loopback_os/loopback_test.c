#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

// The Physical Address from Vivado Address Editor
#define PHY_ADDR 0x43C00000 
// Size of the address range (we only need a few bytes, but 4KB page is standard)
#define MEM_SIZE 4096

int main() {
    int dh;
    void *mapped_base;
    volatile unsigned int *reg_ptr; // Volatile is crucial for hardware registers!

    // 1. Open /dev/mem (Requires Root)
    dh = open("/dev/mem", O_RDWR | O_SYNC);
    if (dh == -1) {
        perror("Error opening /dev/mem. Are you root?");
        return -1;
    }

    // 2. Map Physical Memory to Virtual Memory
    mapped_base = mmap(0, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, dh, PHY_ADDR);
    if (mapped_base == (void *)-1) {
        perror("Error mapping memory");
        close(dh);
        return -1;
    }

    // 3. Create a pointer to our registers
    reg_ptr = (volatile unsigned int *)mapped_base;

    printf("--- FPGA Loopback Test ---\n");
    printf("Hardware Address: 0x%08x\n", PHY_ADDR);

    // 4. Write to Register 0 (Offset 0)
    unsigned int input_val = 42;
    printf("Writing %d to Register 0...\n", input_val);
    reg_ptr[0] = input_val;

    // 5. Read from Register 1 (Offset 4 bytes = index 1)
    // Recall: Our Verilog logic says Reg1 = Reg0 + 1
    unsigned int output_val = reg_ptr[1];
    printf("Read %d from Register 1.\n", output_val);

    // 6. Validation
    if (output_val == input_val + 1) {
        printf("\n[SUCCESS] Hardware added 1 correctly!\n");
    } else {
        printf("\n[FAIL] Expected %d, got %d. Is the bitstream loaded?\n", input_val + 1, output_val);
    }

    // Cleanup
    munmap(mapped_base, MEM_SIZE);
    close(dh);
    return 0;
}
