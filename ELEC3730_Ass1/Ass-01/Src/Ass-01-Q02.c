#include <stdio.h>
#include "Ass-01.h"
#include <inttypes.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>


// TODO
// data_p variable should be assigned with "sample" values

int16_t *samples = NULL;
char* data;
pcm_wavefile_header_t *header;
int read_pcm_wavefile(pcm_wavefile_header_t *header_p, char **data_p, char *filename)
{
	int fd;
	FILE *file;

	file = fopen(filename, "rb");

	if (!filename)
		errx(1, "Filename not specified");
	if ((fd = open(filename, O_RDONLY)) < 1)
		errx(1, "Error opening file");
	if (!header)
		header = (pcm_wavefile_header_t*)malloc(sizeof(pcm_wavefile_header_t));
	/* if (read(fd, header, sizeof(pcm_wavefile_header_t)) < sizeof(pcm_wavefile_header_t)) */
	fread(header, sizeof(pcm_wavefile_header_t), 1, file);
	/* errx(1, "File broken: header"); */
	if (strncmp(header->ChunkID, "RIFF", 4) ||
			strncmp(header->Format, "WAVE", 4))
		errx(1, "Not a wav file");
	if (header->AudioFormat != 1)
		errx(1, "Only PCM encoding supported");
	/* if (*samples) */
	/* { */
	/*     printf("her"); */
	/*     free(*samples);} */
	samples = (int16_t*)malloc(header->Subchunk2Size);//seg fault
	if (!samples)
		errx(1, "Error allocating memory");
	fread(samples, sizeof(char), header->Subchunk2Size, file);
	/* if (read(fd, samples, header->Subchunk2Size) < header->Subchunk2Size) */
	/* errx(1, "File broken: samples"); */
	close(fd);
}
int write_pcm_wavefile(pcm_wavefile_header_t *header_p, char *data, char *filename)
{
	printf("hello\n");
	int fd;
	FILE *file;
	file = fopen(filename, "wb");

	if (!filename)
		errx(1, "Filename not specified");
	if (!samples)
		errx(1, "Samples buffer not specified");
	if ((fd = creat(filename, 0666)) < 1)
		errx(1, "Error creating file");
	fwrite(header, sizeof(pcm_wavefile_header_t), 1, file);
	/* if (write(file, header, sizeof(pcm_wavefile_header_t)) < sizeof(pcm_wavefile_header_t)) */
	/* errx(1, "Error writing header"); */
	fwrite(samples, sizeof(char), header->Subchunk2Size, file);
	/* if (write(fd, samples, header->Subchunk2Size) < header->Subchunk2Size) */
	/*     errx(1, "Error writing samples"); */
	printf("ChunkID		= %c%c%c%c\n", header_p->ChunkID[0], header_p->ChunkID[1], header_p->ChunkID[2], header_p->ChunkID[3]);
	printf("ChunkSize	= %u\n", header_p->ChunkSize);
	printf("Format		= %c%c%c%c\n", header_p->Format[0], header_p->Format[1], header_p->Format[2], header_p->Format[3]);
	printf("Subchunk1SiE	= %u\n", header_p->Subchunk1Size);
	printf("AudioForma	= %u\n", header_p->AudioFormat);
	printf("NumChannels	= %u\n", header_p->NumChannels);
	printf("SampleRate	= %u\n", header_p->SampleRate);
	printf("ByteRate	= %u\n", header_p->ByteRate);
	printf("BlockAlign	= %u\n", header_p->BlockAlign);
	printf("BitsPerSample	= %u\n", header_p->BitsPerSample);
	printf("Subchunk2ID	= %c%c%c%c\n", header_p->Subchunk2ID[0], header_p->Subchunk2ID[1], header_p->Subchunk2ID[2], header_p->Subchunk2ID[3]);
	printf("Subchunk2Size	= %u\n", header_p->Subchunk2Size);
	close(fd);
}
int main()
{
	// test question here
	/* int16_t *samples = NULL; */
	printf("hello\n");
	read_pcm_wavefile(header, 0, "8k16bitpcm.wav");
	printf("No. of channels: %d\n",     header->NumChannels);
	printf("Sample rate:     %d\n",     header->SampleRate);
	printf("Bit rate:        %dkBitsPerSample\n", header->ByteRate*8 / 1000);
	printf("Bits per sample: %d\n\n",     header->BitsPerSample);
	/* printf("Sample 0:        %d\n", samples[0]); */
	/* printf("Sample 1:        %d\n", samples[1]); */
	// Modify the header values & samples before writing the new file
	write_pcm_wavefile(header, data, "track2.wav");
	free(header);
	free(samples);
	printf("finish");
}
