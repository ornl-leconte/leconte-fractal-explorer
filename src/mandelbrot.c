//



#include "mandelbrot.h"
#include "mandelbrot_render.h"


int world_size, world_rank;

char processor_name[MPI_MAX_PROCESSOR_NAME];
int processor_name_len;

fr_t fr;

#define M_EXIT(n) MPI_Finalize(); exit(0);

#define GETOPT_STOP ((char)-1)

#define IS_HEAD (world_rank == 0)
#define IS_COMPUTE (world_rank > 0)

#define compute_size (world_size - 1)
#define compute_rank (world_rank - 1)


void mandelbrot_show_help() {
    printf("Usage: mandelbrot [-h]\n");
    printf("  -h             show this help menu\n");
    printf("\n");
    printf("Questions? Issues? Please contact:\n");
    printf("<brownce@ornl.gov>\n");
}

// returns exit code, or -1 if we shouldn't exit
int parse_args(int argc, char ** argv) {
    char c;
    while ((c = getopt(argc, argv, "h")) != GETOPT_STOP) {
	switch (c) {
            case 'h':
		mandelbrot_show_help();
		return 0;
		break;
            case '?':
		printf("Unknown argument: -%c\n", optopt);
		return 1;
		break;
            default:
		printf("ERROR: unknown getopt return val\n");
		return 1;
                break;
	}
    }
    return -1;
}


int main(int argc, char ** argv) {
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);


    MPI_Get_processor_name(processor_name, &processor_name_len);

    int res = -100;

    if (IS_HEAD) {
        res = parse_args(argc, argv);
    }
    
    MPI_Bcast(&res, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (res >= 0) {
        M_EXIT(res);
    }

    if (compute_size <= 0) {
        printf("ERROR: please run with more than 1 thread (need at least 1 compute node!)\n");
        M_EXIT(1);
    }

    if (IS_HEAD) {
        printf("head node, name: %s, %d/%d\n", processor_name, world_rank + 1, world_size);
    } else {
        printf("compute node, name: %s, %d/%d (compute %d/%d)\n", processor_name, world_rank + 1, world_size, compute_rank + 1, compute_size);
    }


    if (IS_HEAD) {
        fr.cX = 0; fr.cY = 0;
        fr.Z = .5;
        fr.max_iter = 20;
        mandelbrot_render(&argc, argv);
    } else {

    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;

}

void start_render() {

}


void start_compute() {

}

