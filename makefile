CC = gcc
CFLAGS = -O3 -o
MDPC_SRCS = mdpc.c libs/matrix.c libs/polynome.c libs/md5.c 
ISD_SRCS =  isd.c libs/matrix.c libs_optimized/matrix_optimized.c

all: mdpc isd 

mdpc: $(MDPC_SRCS)
	$(CC) $(CFLAGS) mdpc $(MDPC_SRCS)

isd: $(ISD_SRCS)
	$(CC) $(CFLAGS) isd $(ISD_SRCS)


clean:
	rm -f mdpc isd
