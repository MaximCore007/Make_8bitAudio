/*
 ============================================================================
 Name        : create_8bit_music.c
 Author      : Maxim
 Version     :
 Copyright   : Your copyright notice
 Description : Create 8 bit music in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME_MAX_SIZE 15
/*
static unsigned int RIFF = 0x52494646;
static unsigned int WAVE = 0x57415645;
static unsigned int FMT  = 0x666D7420;
static unsigned int FMT_END  = 0x20;
*/
static unsigned int DATA = 0x64617461;
/*
struct hdr_file {
	struct wav_hdr {
		unsigned int chunk_id;	// Chunk ID
		unsigned int data_size; // Chunk Data Size
		unsigned int wave_id;
	};
	struct chank_fmt {
		unsigned int data_size;		// Chunk Data Size
		unsigned short cmp_code;	// compression code
		unsigned short num_ch;		// Number of channels
		unsigned int smpr;			// Sample rate
		unsigned int byteps;		// Average bytes per second
		unsigned short blk_align;	// Block align
		unsigned short bit_ps;		// Significant bits per sample
		unsigned short extra;		// Extra format bytes
		unsigned char end_hdr;
	};
	struct chank_data {
		unsigned int chunk_id;		// Chunk ID
		unsigned int data_size;
		unsigned char *data;
	};
};
*/
int main(int argc, char *argv[])
{
	puts("\nHello in program for extract audio data from WAV file with compression method: none (PCM data)");

//	int offset = atoi(*++argv);
//	int count = atoi(*++argv);
	const char *filename = *++argv;
	const char *outfilename = *++argv;

	FILE* f_in;

	f_in = fopen(filename, "rb");

	if (f_in == NULL) {
			printf("failed to open file %s.\n", filename);
			return -1;
	}
//	char *buffer = (char *)malloc(count);
	/*
	int seeked = fseek(f_in, offset, SEEK_SET);
	if (seeked != 0) {
			printf("failed seek to %d offset.\n", offset);
			fclose(f_in);
			return -2;
	}

	size_t readedBytes = fread(buffer, 1, 1, f_in);
	if (readedBytes != count) {
			printf("failed to read %d bytes", count);
			fclose(f_in);
			return -3;
	}
	fclose(f_in);
	*/

	char tmp[4] = {0};
	char data_found = 0;
	char buf_byte;
	unsigned int chank_code;
	unsigned int data_size = 0;
	unsigned int start_data = 0;
	puts("searching audio data...");

	for (int j = 0, i = 0; i < 1024; i++)
	{
		fread(buf_byte, 1, 1, f_in);
		tmp[j] =  buf_byte[i];
		chank_code = (int)(tmp[0] | (tmp[1] << 8) | (tmp[2] << 16) | (tmp[3] << 24));
		if (chank_code == DATA) {
			data_found = 1;
			for (int k = 0; k < 4; k++) {
				fread(data_size, 4, 1, f_in);
				tmp[k] = buffer[i + k];
			}
			start_data = i + 4;
			data_size = (int)(tmp[0] | (tmp[1] << 8) | (tmp[2] << 16) | (tmp[3] << 24));
			printf("audio data found. start position %d", start_data);
			printf("size data %d bytes", data_size);
			break;
		}
		j++;
		if (j > 3)
			j = 0;
	}
	if (data_found == 0) {
		puts("audio data not found :(");
		puts("close program");
		return 0;
	}

	FILE* f_out;
	f_out = fopen(outfilename, "w");

	for (int i = start_data; i < data_size; ++i) {
		fprintf(f_out, "0x%02x, ", (unsigned char)buffer[i - 1]);
		if (i % 16 == 0)
			fprintf(f_out, "\n");
	}
	fclose(f_out);

	return 0;
}
